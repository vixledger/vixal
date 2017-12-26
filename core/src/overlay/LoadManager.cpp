// Copyright 2015 Stellar Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include <crypto/SecretKey.h>
#include "overlay/LoadManager.h"
#include "util/format.h"
#include "application/Application.h"
#include "overlay/OverlayManager.h"
#include "util/Logging.h"
#include "util/types.h"

namespace vixal {

using xdr::operator<;

LoadManager::LoadManager() : mPeerCosts(128) {
}

std::string
byteMag(uint64_t bytes) {
    static char const *sz[7] = {"B", "KiB", "MiB", "GiB", "TiB", "PiB", "EiB"};
    for (auto i = 6; i >= 0; --i) {
        uint64_t mag = i * 10u;
        if (bytes >= (1ULL << mag)) {
            return fmt::format("{:>d}{:s}", bytes >> mag, sz[i]);
        }
    }
    return "0";
}

std::string
timeMag(uint64_t nanos) {

    static char const *sz[4] = {"ns", "us", "ms", "s"};
    uint64_t mag = 1000000000;
    for (auto i = 3; i >= 0; --i) {
        if (nanos >= mag) {
            return fmt::format("{:>d}{:s}", nanos / mag, sz[i]);
        }
        mag /= 1000;
    }
    return "0";
}

void
LoadManager::reportLoads(std::map<NodeID, Peer::pointer> const& peers,
                         Application &app) {
    CLOG(INFO, "Overlay") << "";
    CLOG(INFO, "Overlay") << "Cumulative peer-load costs:";
    CLOG(INFO, "Overlay") << "------------------------------------------------------";
    CLOG(INFO, "Overlay") << fmt::format("{:>10s} {:>10s} {:>10s} {:>10s} {:>10s}", "peer", "time", "send",
                                         "recv", "query");
    for (auto const &peer : peers) {
        auto cost = getPeerCosts(peer.first);
        CLOG(INFO, "Overlay") << fmt::format("{:>10s} {:>10s} {:>10s} {:>10s} {:>10d}",
                                             app.getConfig().toShortString(peer.first),
                                             timeMag(static_cast<uint64_t>(cost->mTimeSpent.one_minute_rate())),
                                             byteMag(static_cast<uint64_t>(cost->mBytesSend.one_minute_rate())),
                                             byteMag(static_cast<uint64_t>(cost->mBytesRecv.one_minute_rate())),
                                             cost->mSQLQueries.count());
    }
    CLOG(INFO, "Overlay") << "";
}

LoadManager::~LoadManager() = default;

void
LoadManager::maybeShedExcessLoad(Application &app) {
    uint32_t minIdle = app.getConfig().MINIMUM_IDLE_PERCENT;
    uint32_t idleClock = app.getClock().recentIdleCrankPercent();
    uint32_t idleDb = app.getDatabase().recentIdleDbPercent();

    if ((idleClock < minIdle) || (idleDb < minIdle)) {
        CLOG(WARNING, "Overlay") << "";
        CLOG(WARNING, "Overlay") << "System appears to be overloaded";
        CLOG(WARNING, "Overlay") << "Idle minimum " << minIdle << "% vs. "
                                 << "clock " << idleClock << "%, "
                                 << "DB " << idleDb << "%";
        CLOG(WARNING, "Overlay") << "";

        auto peers = app.getOverlayManager().getAuthenticatedPeers();

        reportLoads(peers, app);

        // Look for the worst-behaved of the current peers and kick them out.
        std::shared_ptr<Peer> victim;
        std::shared_ptr<LoadManager::PeerCosts> victimCost;
        for (const auto &peer : peers) {
            auto peerCost = getPeerCosts(peer.first);
            if (!victim || victimCost->isLessThan(peerCost)) {
                victim = peer.second;
                victimCost = peerCost;
            }
        }

        if (victim) {
            CLOG(WARNING, "Overlay") << "Disconnecting suspected culprit "
                                     << app.getConfig().toShortString(victim->getPeerID());

            app.getMetrics().newMeter({"overlay", "drop", "load-shed"}, "drop").mark();

            victim->drop();

            app.getClock().resetIdleCrankPercent();
        }
    }
}

LoadManager::PeerCosts::PeerCosts()
        : mTimeSpent("nanoseconds"), mBytesSend("byte"), mBytesRecv("byte"), mSQLQueries("query") {
}

bool
LoadManager::PeerCosts::isLessThan(
        std::shared_ptr<LoadManager::PeerCosts> other) {
    double ownRates[4] = {
            mTimeSpent.one_minute_rate(), mBytesSend.one_minute_rate(),
            mBytesRecv.one_minute_rate(), static_cast<double>(mSQLQueries.count())};
    double otherRates[4] = {other->mTimeSpent.one_minute_rate(),
                            other->mBytesSend.one_minute_rate(),
                            other->mBytesRecv.one_minute_rate(),
                            static_cast<double>(other->mSQLQueries.count())};
    return std::lexicographical_compare(ownRates, ownRates + 4, otherRates,
                                        otherRates + 4);
}

std::shared_ptr<LoadManager::PeerCosts>
LoadManager::getPeerCosts(NodeID const &node) {
    if (mPeerCosts.exists(node)) {
        return mPeerCosts.get(node);
    }
    auto p = std::make_shared<LoadManager::PeerCosts>();
    mPeerCosts.put(node, p);
    return p;
}

LoadManager::PeerContext::PeerContext(Application &app, NodeID const &node)
        : mApp(app), mNode(node), mWorkStart(app.getClock().now()),
          mBytesSendStart(Peer::getByteWriteMeter(app).count()), mBytesRecvStart(Peer::getByteReadMeter(app).count()),
          mSQLQueriesStart(app.getDatabase().getQueryMeter().count()) {
}

LoadManager::PeerContext::~PeerContext() {
    if (!isZero(mNode.ed25519())) {
        auto pc = mApp.getOverlayManager().getLoadManager().getPeerCosts(mNode);
        auto time = std::chrono::duration_cast<std::chrono::nanoseconds>(
                mApp.getClock().now() - mWorkStart);
        auto send = Peer::getByteWriteMeter(mApp).count() - mBytesSendStart;
        auto recv = Peer::getByteReadMeter(mApp).count() - mBytesRecvStart;
        auto query =
                (mApp.getDatabase().getQueryMeter().count() - mSQLQueriesStart);
        if (Logging::logTrace("Overlay"))
            CLOG(TRACE, "Overlay")
                    << "Debiting peer " << mApp.getConfig().toShortString(mNode)
                    << " time:" << timeMag(static_cast<uint64_t>(time.count()))
                    << " send:" << byteMag(send) << " recv:" << byteMag(recv)
                    << " query:" << query;
        pc->mTimeSpent.mark(static_cast<uint64_t>(time.count()));
        pc->mBytesSend.mark(send);
        pc->mBytesRecv.mark(recv);
        pc->mSQLQueries.mark(query);
    }
}
}
