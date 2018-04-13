// Copyright 2014 Stellar Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include "util/asio.h"
#include "../src/application/ApplicationImpl.h"
#include "application/Config.h"

#include "database/Database.h"
#include "catch.hpp"
#include "overlay/OverlayManager.h"
#include "../src/overlay/OverlayManagerImpl.h"
#include "test/TestAccount.h"
#include "test/TxTests.h"
#include "test/test.h"
#include "test/TestUtils.h"
#include "transactions/TransactionFrame.h"
#include "util/Timer.h"
#include <soci/soci.h>

using namespace vixal;
using namespace std;
using namespace soci;
using namespace txtest;

namespace vixal {

class PeerStub : public Peer {
public:
    int sent = 0;

    PeerStub(Application& app, PeerBareAddress const& addres)
            : Peer(app, WE_CALLED_REMOTE) {
        mPeerID = SecretKey::random().getPublicKey();
        mState = GOT_AUTH;
        mAddress = addres;
    }

    virtual PeerBareAddress
    makeAddress(int) const override {
        REQUIRE(false); // should not be called
        return {};
    }

    virtual void drop(bool) override {
    }

    virtual void
    sendMessage(xdr::msg_ptr &&xdrBytes) override {
        sent++;
    }
};

class OverlayManagerStub : public OverlayManagerImpl {
public:
    OverlayManagerStub(Application &app) : OverlayManagerImpl(app) {
    }

    virtual void
    connectTo(PeerRecord &pr) override {
        if (!getConnectedPeer(pr.getAddress())) {
            pr.backOff(mApp.getClock());
            pr.storePeerRecord(mApp.getDatabase());

            auto peerStub = std::make_shared<PeerStub>(mApp, pr.getAddress());
            addPendingPeer(peerStub);
            REQUIRE(acceptAuthenticatedPeer(peerStub));
        }
    }
};

class OverlayManagerTests {
    class ApplicationStub : public TestApplication {
    public:
        shared_ptr<OverlayManagerStub> mOverlayManager;

        ApplicationStub(VirtualClock &clock, Config const &cfg)
                : TestApplication(clock, cfg) {
        }

        virtual OverlayManagerStub &
        getOverlayManager() override {
            auto &overlay = ApplicationImpl::getOverlayManager();
            return static_cast<OverlayManagerStub &>(overlay);
        }

    private:
        virtual std::unique_ptr<OverlayManager>
        createOverlayManager() override {
            return std::make_unique<OverlayManagerStub>(*this);

        }
    };

protected:
    VirtualClock clock;
    std::shared_ptr<ApplicationStub> app;

    vector<string> fourPeers;
    vector<string> threePeers;

    OverlayManagerTests()
            : fourPeers(vector<string>{"127.0.0.1:2011", "127.0.0.1:2012", "127.0.0.1:2013", "127.0.0.1:2014"}),
              threePeers(vector<string> {"127.0.0.1:64000", "127.0.0.1:64001", "127.0.0.1:64002"}) {
        auto cfg = getTestConfig();
        cfg.TARGET_PEER_CONNECTIONS = 5;
        app = createTestApplication<ApplicationStub>(clock, cfg);
    }

    void
    test_addPeerList() {
        OverlayManagerStub &pm = app->getOverlayManager();

        pm.storePeerList(fourPeers, false, false);

        rowset<row> rs = app->getDatabase().getSession().prepare
                << "SELECT ip,port FROM peers ORDER BY nextattempt";
        vector<string> actual;
        for (auto it = rs.begin(); it != rs.end(); ++it)
            actual.push_back(it->get<string>(0) + ":" +
                             to_string(it->get<int>(1)));

        REQUIRE(actual == fourPeers);
    }

    vector<int>
    sentCounts(OverlayManagerImpl &pm) {
        vector<int> result;
        for (auto p : pm.mAuthenticatedPeers) {
            result.push_back(static_pointer_cast<PeerStub>(p.second)->sent);
        }
        return result;
    }

    void
    test_broadcast() {
        OverlayManagerStub &pm = app->getOverlayManager();

        pm.storePeerList(fourPeers, false, false);
        pm.storePeerList(threePeers, false, false);
        // connect to peers, respecting TARGET_PEER_CONNECTIONS
        pm.tick();
        REQUIRE(pm.mAuthenticatedPeers.size() == 5);
        auto a = TestAccount{*app, getAccount("a")};
        auto b = TestAccount{*app, getAccount("b")};
        auto c = TestAccount{*app, getAccount("c")};
        auto d = TestAccount{*app, getAccount("d")};

        VixalMessage AtoC = a.tx({payment(b, 10)})->toVixalMessage();
        auto i = 0;
        for (auto p : pm.mAuthenticatedPeers) {
            if (i++ == 2) {
                pm.recvFloodedMsg(AtoC, p.second);
            }
        }
        pm.broadcastMessage(AtoC, false);
        vector<int> expected{1, 1, 0, 1, 1};
        REQUIRE(sentCounts(pm) == expected);
        pm.broadcastMessage(AtoC, false);
        REQUIRE(sentCounts(pm) == expected);
        VixalMessage CtoD = c.tx({payment(d, 10)})->toVixalMessage();
        pm.broadcastMessage(CtoD, false);
        vector<int> expectedFinal{2, 2, 1, 2, 2};
        REQUIRE(sentCounts(pm) == expectedFinal);
    }
};

TEST_CASE_METHOD(OverlayManagerTests, "addPeerList() adds", "[overlay]") {
    test_addPeerList();
}

TEST_CASE_METHOD(OverlayManagerTests, "broadcast() broadcasts", "[overlay]") {
    test_broadcast();
}

}
