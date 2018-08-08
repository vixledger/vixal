// Copyright 2014 Stellar Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include "LedgerManagerImpl.h"
#include "ledger/DataFrame.h"
#include "ledger/OfferFrame.h"
#include "ledger/TrustFrame.h"
#include "bucket/BucketManager.h"
#include "crypto/Hex.h"
#include "crypto/KeyUtils.h"
#include "crypto/SHA.h"
#include "crypto/SecretKey.h"
#include "database/Database.h"
#include "herder/Herder.h"
#include "herder/HerderPersistence.h"
#include "herder/LedgerCloseData.h"
#include "herder/TxSetFrame.h"
#include "history/HistoryManager.h"
#include "invariant/InvariantDoesNotHold.h"
#include "invariant/InvariantManager.h"
#include "ledger/LedgerDelta.h"
#include "ledger/LedgerHeaderFrame.h"
#include "application/Application.h"
#include "application/Config.h"
#include "overlay/OverlayManager.h"

#include "util/Logging.h"
#include "util/XDROperators.h"
#include "util/format.h"

#include "medida/counter.h"
#include "medida/meter.h"
#include "medida/metrics_registry.h"
#include "medida/timer.h"
#include "xdrpp/printer.h"
#include "xdrpp/types.h"

#include <chrono>
#include <sstream>
#include <herder/Upgrades.h>

/**
The ledger module:
    1) gets the externalized tx set
    2) applies this set to the last closed ledger
    3) sends the changed entries to the BucketList
    4) saves the changed entries to SQL
    5) saves the ledger hash and header to SQL
    6) sends the new ledger hash and the tx set to the history
    7) sends the new ledger hash and header to the Herder


catching up to network:
    1) Wait for SCP to tell us what the network is on now
    2) Pull history log or static deltas from history archive
    3) Replay or force-apply deltas, depending on catchup mode

*/
using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;
using namespace std;

namespace vixal {

const uint32_t LedgerManager::GENESIS_LEDGER_SEQ = 1;
const uint32_t LedgerManager::GENESIS_LEDGER_VERSION = 0;
const uint32_t LedgerManager::GENESIS_LEDGER_BASE_FEE = 100;
const uint32_t LedgerManager::GENESIS_LEDGER_BASE_RESERVE = 100000000;
const uint32_t LedgerManager::GENESIS_LEDGER_MAX_TX_SIZE = 100;
const int64_t LedgerManager::GENESIS_LEDGER_TOTAL_COINS = 1000000000000000000;

std::unique_ptr<LedgerManager>
LedgerManager::create(Application &app) {
    return std::make_unique<LedgerManagerImpl>(app);
}

std::string
LedgerManager::ledgerAbbrev(uint32_t seq, uint256 const &hash) {
    std::ostringstream oss;
    oss << "[seq=" << seq << ", hash=" << hexAbbrev(hash) << "]";
    return oss.str();
}

std::string
LedgerManager::ledgerAbbrev(LedgerHeader const &header, uint256 const &hash) {
    return ledgerAbbrev(header.ledgerSeq, hash);
}

std::string
LedgerManager::ledgerAbbrev(LedgerHeaderFrame::pointer p) {
    if (!p) {
        return "[empty]";
    }
    return ledgerAbbrev(p->mHeader, p->getHash());
}

std::string
LedgerManager::ledgerAbbrev(LedgerHeaderHistoryEntry he) {
    return ledgerAbbrev(he.header, he.hash);
}

LedgerManagerImpl::LedgerManagerImpl(Application &app)
        : mApp(app),
          mTransactionApply(app.getMetrics().newTimer({"ledger", "transaction", "apply"})),
          mTransactionCount(app.getMetrics().newHistogram({"ledger", "transaction", "count"})),
          mLedgerClose(app.getMetrics().newTimer({"ledger", "ledger", "close"})),
          mLedgerAgeClosed(app.getMetrics().newTimer({"ledger", "age", "closed"})),
          mLedgerAge(app.getMetrics().newCounter({"ledger", "age", "current-seconds"})),
          mLedgerStateCurrent(app.getMetrics().newCounter({"ledger", "state", "current"})),
          mLedgerStateChanges(app.getMetrics().newTimer({"ledger", "state", "changes"})),
          mLastClose(mApp.getClock().now()),
          mLastStateChange(mApp.getClock().now()),
          mSyncingLedgersSize(app.getMetrics().newCounter({"ledger", "memory", "syncing-ledgers"})),
          mState(LM_BOOTING_STATE) {
}

void
LedgerManagerImpl::bootstrap() {
    setState(LM_SYNCED_STATE);
}

void
LedgerManagerImpl::setState(State s) {
    if (s != getState()) {
        std::string oldState = getStateHuman();
        mState = s;
        mLedgerStateCurrent.set_count(static_cast<size_t>(s));
        auto now = mApp.getClock().now();
        mLedgerStateChanges.update(now - mLastStateChange);
        mLastStateChange = now;
        mApp.syncOwnMetrics();
        CLOG(INFO, "Ledger") << "Changing state " << oldState << " -> " << getStateHuman();
        if (mState != LM_CATCHING_UP_STATE) {
            mCatchupState = CatchupState::NONE;
            mApp.getCatchupManager().logAndUpdateCatchupStatus(true);
        }
    }
}

void
LedgerManagerImpl::setCatchupState(CatchupState s) {
    mCatchupState = s;
    setState(LM_CATCHING_UP_STATE);
}

LedgerManager::State
LedgerManagerImpl::getState() const {
    return mState;
}

LedgerManager::CatchupState
LedgerManagerImpl::getCatchupState() const {
    return mCatchupState;
}


std::string
LedgerManagerImpl::getStateHuman() const {
    static const char *stateStrings[LM_NUM_STATE] = {
            "LM_BOOTING_STATE", "LM_SYNCED_STATE", "LM_CATCHING_UP_STATE"};
    return std::string(stateStrings[getState()]);
}

LedgerHeader
LedgerManager::genesisLedger() {
    LedgerHeader result;
    // all fields are initialized by default to 0
    // set the ones that are not 0
    result.ledgerVersion = GENESIS_LEDGER_VERSION;
    result.baseFee = GENESIS_LEDGER_BASE_FEE;
    result.baseReserve = GENESIS_LEDGER_BASE_RESERVE;
    result.maxTxSetSize = GENESIS_LEDGER_MAX_TX_SIZE;
    result.totalCoins = GENESIS_LEDGER_TOTAL_COINS;
    result.ledgerSeq = GENESIS_LEDGER_SEQ;
    return result;
}

void
LedgerManagerImpl::startNewLedger(LedgerHeader genesisLedger) {
    DBTimeExcluder qtExclude(mApp);
    auto ledgerTime = mLedgerClose.timeScope();
    SecretKey skey = SecretKey::fromSeed(mApp.getNetworkID());

    AccountFrame masterAccount(skey.getPublicKey());


    masterAccount.getAccount().balance = genesisLedger.totalCoins;

    LedgerDelta delta(genesisLedger, getDatabase());
    masterAccount.storeAdd(delta, this->getDatabase());

    delta.commit();

    mCurrentLedger = make_shared<LedgerHeaderFrame>(genesisLedger);
    CLOG(INFO, "Ledger") << "Established genesis ledger, closing";
    CLOG(INFO, "Ledger") << "Root account seed: " << skey.getStrKeySeed().value;
    ledgerClosed(delta);
}

void
LedgerManagerImpl::startNewLedger() {
    auto ledger = genesisLedger();
    auto const &cfg = mApp.getConfig();
    if (cfg.USE_CONFIG_FOR_GENESIS) {

        ledger.ledgerVersion = cfg.LEDGER_PROTOCOL_VERSION;
        ledger.baseFee = cfg.TESTING_UPGRADE_DESIRED_FEE;
        ledger.baseReserve = cfg.TESTING_UPGRADE_RESERVE;
        ledger.maxTxSetSize = cfg.TESTING_UPGRADE_MAX_TX_PER_LEDGER;
    }
    startNewLedger(std::move(ledger));
}

void
LedgerManagerImpl::loadLastKnownLedger(
        function<void(asio::error_code const &ec)> handler) {
    DBTimeExcluder qtExclude(mApp);
    auto ledgerTime = mLedgerClose.timeScope();

    string lastLedger = mApp.getPersistentState().getState(PersistentState::kLastClosedLedger);

    if (lastLedger.empty()) {
        throw std::runtime_error("No ledger in the DB");
    } else {
        LOG(INFO) << "Loading last known ledger";
        Hash lastLedgerHash = hexToBin256(lastLedger);

        mCurrentLedger =
                LedgerHeaderFrame::loadByHash(lastLedgerHash, getDatabase());
        if (!mCurrentLedger) {
            throw std::runtime_error("Could not load ledger from database");
        }

        if (handler) {
            string hasString = mApp.getPersistentState().getState(
                    PersistentState::kHistoryArchiveState);
            HistoryArchiveState has;
            has.fromString(hasString);

            auto continuation = [this, handler,
                    has](asio::error_code const &ec) {
                if (ec) {
                    handler(ec);
                } else {
                    mApp.getBucketManager().assumeState(has);

                    CLOG(INFO, "Ledger") << "Loaded last known ledger: "
                                         << ledgerAbbrev(mCurrentLedger);

                    advanceLedgerPointers();
                    handler(ec);
                }
            };

            auto missing =
                    mApp.getBucketManager().checkForMissingBucketsFiles(has);
            auto pubmissing = mApp.getHistoryManager()
                    .getMissingBucketsReferencedByPublishQueue();
            missing.insert(missing.end(), pubmissing.begin(), pubmissing.end());
            if (!missing.empty()) {
                CLOG(WARNING, "Ledger")
                        << "Some buckets are missing in '"
                        << mApp.getBucketManager().getBucketDir() << "'.";
                CLOG(WARNING, "Ledger")
                        << "Attempting to recover from the history store.";
                mApp.getHistoryManager().downloadMissingBuckets(has,
                                                                continuation);
            } else {
                continuation(asio::error_code());
            }
        } else {
            advanceLedgerPointers();
        }
    }
}

Database &
LedgerManagerImpl::getDatabase() {
    return mApp.getDatabase();
}

uint32_t
LedgerManagerImpl::getTxFee() const {
    return mCurrentLedger->mHeader.baseFee;
}

uint32_t
LedgerManagerImpl::getMaxTxSetSize() const {
    return mCurrentLedger->mHeader.maxTxSetSize;
}

int64_t
LedgerManagerImpl::getMinBalance(uint32_t ownerCount) const {
    auto &lh = mCurrentLedger->mHeader;
    if (lh.ledgerVersion <= 8) {
        return (2 + ownerCount) * mCurrentLedger->mHeader.baseReserve;
    } else {
        return (2 + ownerCount) * int64_t(mCurrentLedger->mHeader.baseReserve);
    };
}

uint32_t
LedgerManagerImpl::getLedgerNum() const {
    return mCurrentLedger->mHeader.ledgerSeq;
}

uint64_t
LedgerManagerImpl::getCloseTime() const {
    return mCurrentLedger->mHeader.scpValue.closeTime;
}

LedgerHeader const &
LedgerManagerImpl::getCurrentLedgerHeader() const {
    return mCurrentLedger->mHeader;
}

LedgerHeader &
LedgerManagerImpl::getCurrentLedgerHeader() {
    return mCurrentLedger->mHeader;
}

uint32_t
LedgerManagerImpl::getCurrentLedgerVersion() const {
    return getCurrentLedgerHeader().ledgerVersion;
}

LedgerHeaderHistoryEntry const &
LedgerManagerImpl::getLastClosedLedgerHeader() const {
    return mLastClosedLedger;
}

uint32_t
LedgerManagerImpl::getLastClosedLedgerNum() const {
    return mLastClosedLedger.header.ledgerSeq;
}

uint32_t
getCatchupCount(Application &app) {
    return app.getConfig().CATCHUP_COMPLETE
           ? std::numeric_limits<uint32_t>::max()
           : app.getConfig().CATCHUP_RECENT;
}

// called by txherder
void
LedgerManagerImpl::valueExternalized(LedgerCloseData const &ledgerData) {
    CLOG(INFO, "Ledger")
            << "Got consensus: "
            << "[seq=" << ledgerData.getLedgerSeq()
            << ", prev=" << hexAbbrev(ledgerData.getTxSet()->previousLedgerHash())
            << ", tx_count=" << ledgerData.getTxSet()->size()
            << ", sv: " << stellarValueToString(ledgerData.getValue()) << "]";

    auto st = getState();
    switch (st) {
        case LedgerManager::LM_BOOTING_STATE:
        case LedgerManager::LM_SYNCED_STATE: {
            switch (closeLedgerIf(ledgerData)) {
                case CloseLedgerIfResult::CLOSED: {
                    setState(LM_SYNCED_STATE);
                    break;
                }
                case CloseLedgerIfResult::TOO_OLD:
                    // nothing to do
                    break;
                case CloseLedgerIfResult::TOO_NEW: {
                    initializeCatchup(ledgerData);
                    break;
                }
            }
            break;
        }
        case LedgerManager::LM_CATCHING_UP_STATE: {
            switch (mCatchupState) {
                case CatchupState::WAITING_FOR_CLOSING_LEDGER: {
                    finalizeCatchup(ledgerData);
                    break;
                }
                default: {
                    continueCatchup(ledgerData);
                    break;
                }
            }
            break;
        }
        default: {
            assert(false);
        }
    }
}

LedgerManagerImpl::CloseLedgerIfResult
LedgerManagerImpl::closeLedgerIf(LedgerCloseData const &ledgerData) {
    if (mLastClosedLedger.header.ledgerSeq + 1 == ledgerData.getLedgerSeq()) {
        if (mLastClosedLedger.hash ==
            ledgerData.getTxSet()->previousLedgerHash()) {
            closeLedger(ledgerData);
            CLOG(INFO, "Ledger")
                    << "Closed ledger: " << ledgerAbbrev(mLastClosedLedger);
            return CloseLedgerIfResult::CLOSED;
        } else {
            CLOG(FATAL, "Ledger") << "Network consensus for ledger "
                                  << mLastClosedLedger.header.ledgerSeq
                                  << " changed; this should never happen";
            throw std::runtime_error("Network consensus inconsistency");
        }
    } else if (ledgerData.getLedgerSeq() <= mLastClosedLedger.header.ledgerSeq) {
        CLOG(INFO, "Ledger")
                << "Skipping close ledger: local state is "
                << mLastClosedLedger.header.ledgerSeq << ", more recent than "
                << ledgerData.getLedgerSeq();
        return CloseLedgerIfResult::TOO_OLD;
    } else {
        // Out of sync, buffer what we just heard and start catchup.
        CLOG(INFO, "Ledger")
                << "Lost sync, local LCL is " << mLastClosedLedger.header.ledgerSeq
                << ", network closed ledger " << ledgerData.getLedgerSeq();
        return CloseLedgerIfResult::TOO_NEW;
    }
}

void
LedgerManagerImpl::initializeCatchup(LedgerCloseData const &ledgerData) {
    assert(mState != LM_CATCHING_UP_STATE);
    assert(mCatchupState == CatchupState::NONE);
    assert(mSyncingLedgers.empty());

    setState(LM_CATCHING_UP_STATE);
    mCatchupTriggerLedger = mApp.getHistoryManager().nextCheckpointLedger(
            ledgerData.getLedgerSeq()) +
                            1;
    setCatchupState(CatchupState::WAITING_FOR_TRIGGER_LEDGER);
    addToSyncingLedgers(ledgerData);
    startCatchupIf(ledgerData.getLedgerSeq());
}

void
LedgerManagerImpl::continueCatchup(LedgerCloseData const &ledgerData) {
    assert(mState == LM_CATCHING_UP_STATE);

    addToSyncingLedgers(ledgerData);
    startCatchupIf(ledgerData.getLedgerSeq());
}

void
LedgerManagerImpl::finalizeCatchup(LedgerCloseData const &ledgerData) {
    assert(mState == LM_CATCHING_UP_STATE);
    assert(mCatchupState == CatchupState::WAITING_FOR_CLOSING_LEDGER);
    assert(mSyncingLedgers.empty());

    switch (closeLedgerIf(ledgerData)) {
        case CloseLedgerIfResult::CLOSED: {
            CLOG(INFO, "Ledger") << "Catchup final ledger closed: "
                                 << ledgerAbbrev(mLastClosedLedger);
            setState(LM_SYNCED_STATE);
        }
            break;
        case CloseLedgerIfResult::TOO_OLD:
            // nothing to do
            break;
        case CloseLedgerIfResult::TOO_NEW: {
            setState(LM_BOOTING_STATE);
            initializeCatchup(ledgerData);
        }
            break;
    }
}

void
LedgerManagerImpl::addToSyncingLedgers(LedgerCloseData const &ledgerData) {
    switch (mSyncingLedgers.add(ledgerData)) {
        case SyncingLedgerChainAddResult::CONTIGUOUS:
            // Normal close while catching up
            CLOG(INFO, "Ledger")
                    << "Close of ledger " << ledgerData.getLedgerSeq() << " buffered";
            mSyncingLedgersSize.set_count(mSyncingLedgers.size());
            return;
        case SyncingLedgerChainAddResult::TOO_OLD:
            CLOG(INFO, "Ledger")
                    << "Skipping close ledger: latest known is "
                    << mSyncingLedgers.back().getLedgerSeq() << ", more recent than "
                    << ledgerData.getLedgerSeq();
            return;
        case SyncingLedgerChainAddResult::TOO_NEW:
            // Out-of-order close while catching up; timeout / network failure?
            CLOG(WARNING, "Ledger")
                    << "Out-of-order close during catchup, buffered to "
                    << mSyncingLedgers.back().getLedgerSeq() << " but network closed "
                    << ledgerData.getLedgerSeq();

            CLOG(WARNING, "Ledger")
                    << "this round of catchup will fail and restart.";
            return;
        default:
            assert(false);
    }
}

void
LedgerManagerImpl::startCatchupIf(uint32_t lastReceivedLedgerSeq) {
    assert(mSyncingLedgers.size() > 0);
    assert(mCatchupState != CatchupState::NONE);

    auto contiguous =
            lastReceivedLedgerSeq == mSyncingLedgers.back().getLedgerSeq();
    if (mCatchupState != CatchupState::WAITING_FOR_TRIGGER_LEDGER) {
        mApp.getCatchupManager().logAndUpdateCatchupStatus(contiguous);
        return;
    }

    if (lastReceivedLedgerSeq >= mCatchupTriggerLedger) {
        setCatchupState(CatchupState::APPLYING_HISTORY);

        auto message = fmt::format("Starting catchup after ensuring checkpoint "
                                   "ledger {} was closed on network",
                                   mCatchupTriggerLedger);
        mApp.getCatchupManager().logAndUpdateCatchupStatus(contiguous, message);

        // catchup just before first buffered ledger that way we will have a way
        // to verify history consistency - compare previousLedgerHash of
        // buffered ledger with last one downloaded from history
        auto firstBufferedLedgerSeq = mSyncingLedgers.front().getLedgerSeq();
        startCatchup({firstBufferedLedgerSeq - 1, getCatchupCount(mApp)},
                     false);
    } else {
        auto eta = (mCatchupTriggerLedger - lastReceivedLedgerSeq) *
                   mApp.getConfig().getExpectedLedgerCloseTime();
        auto message = fmt::format(
                "Waiting for trigger ledger: {}/{}, ETA: {}s",
                lastReceivedLedgerSeq, mCatchupTriggerLedger, eta.count());
        mApp.getCatchupManager().logAndUpdateCatchupStatus(contiguous, message);
    }
}


void
LedgerManagerImpl::startCatchup(CatchupConfiguration configuration,
                                bool manualCatchup) {
    auto lastClosedLedger = getLastClosedLedgerNum();
    if ((configuration.toLedger() != CatchupConfiguration::CURRENT) &&
        (configuration.toLedger() <= lastClosedLedger)) {
        throw std::invalid_argument("Target ledger is not newer than LCL");
    }

    setCatchupState(CatchupState::APPLYING_HISTORY);

    mApp.getCatchupManager().catchupHistory(
            configuration, manualCatchup,
            std::bind(&LedgerManagerImpl::historyCaughtup, this, _1, _2, _3));
}

HistoryManager::LedgerVerificationStatus
LedgerManagerImpl::verifyCatchupCandidate(
        LedgerHeaderHistoryEntry const &candidate, bool manualCatchup) const {
    assert(mCatchupState == CatchupState::APPLYING_HISTORY);

    if (manualCatchup) {
        assert(mSyncingLedgers.empty());
        CLOG(WARNING, "History")
                << "Accepting unknown-hash ledger due to manual catchup";
        return HistoryManager::VERIFY_STATUS_OK;
    }

    assert(!mSyncingLedgers.empty());
    assert(mSyncingLedgers.front().getLedgerSeq() ==
           candidate.header.ledgerSeq + 1);

    // asserts dont work in release builds
    if (mSyncingLedgers.empty()) {
        return HistoryManager::VERIFY_STATUS_ERR_BAD_HASH;
    }

    auto &firstSyncing = mSyncingLedgers.front();
    if (firstSyncing.getLedgerSeq() == candidate.header.ledgerSeq + 1 &&
        firstSyncing.getTxSet()->previousLedgerHash() == candidate.hash) {
        return HistoryManager::VERIFY_STATUS_OK;
    } else {
        return HistoryManager::VERIFY_STATUS_ERR_BAD_HASH;
    }
}

void
LedgerManagerImpl::historyCaughtup(asio::error_code const &ec,
                                   CatchupWork::ProgressState progressState,
                                   LedgerHeaderHistoryEntry const &lastClosed) {
    assert(mCatchupState == CatchupState::APPLYING_HISTORY);

    if (ec) {
        CLOG(ERROR, "Ledger") << "Error catching up: " << ec.message();
        CLOG(ERROR, "Ledger") << "Catchup will restart at next close.";
        setState(LM_BOOTING_STATE);
        mApp.getCatchupManager().historyCaughtup();
    } else {
        switch (progressState) {
            case CatchupWork::ProgressState::APPLIED_BUCKETS: {
                mCurrentLedger = make_shared<LedgerHeaderFrame>(lastClosed.header);
                storeCurrentLedger();

                mLastClosedLedger = lastClosed;
                mCurrentLedger = make_shared<LedgerHeaderFrame>(lastClosed);
                return;
            }
            case CatchupWork::ProgressState::APPLIED_TRANSACTIONS: {
                // In this case we should actually have been caught-up during the
                // replay process and, if judged successful, our LCL should be the
                // one provided as well.
                assert(lastClosed.hash == mLastClosedLedger.hash);
                assert(lastClosed.header == mLastClosedLedger.header);
                return;
            }
            case CatchupWork::ProgressState::FINISHED: {
                break;
            }
            default: {
                assert(false);
            }
        }

        CLOG(INFO, "Ledger") << "Caught up to LCL from history: "
                             << ledgerAbbrev(mLastClosedLedger);
        mApp.getCatchupManager().historyCaughtup();

        // Now replay remaining txs from buffered local network history.
        for (auto const &lcd : mSyncingLedgers) {
            assert(lcd.getLedgerSeq() ==
                   mLastClosedLedger.header.ledgerSeq + 1);
            CLOG(INFO, "Ledger")
                    << "Replaying buffered ledger-close: "
                    << "[seq=" << lcd.getLedgerSeq()
                    << ", prev=" << hexAbbrev(lcd.getTxSet()->previousLedgerHash())
                    << ", tx_count=" << lcd.getTxSet()->size()
                    << ", sv: " << stellarValueToString(lcd.getValue()) << "]";
            closeLedger(lcd);
        }

        CLOG(INFO, "Ledger") << "Caught up to LCL including recent network activity: "
                             << ledgerAbbrev(mLastClosedLedger)
                             << "; waiting for closing ledger";
        setCatchupState(CatchupState::WAITING_FOR_CLOSING_LEDGER);
    }

    // Either way, we're done processing the ledgers backlog
    mSyncingLedgers = {};
    mSyncingLedgersSize.set_count(mSyncingLedgers.size());
}

uint64_t
LedgerManagerImpl::secondsSinceLastLedgerClose() const {
    uint64_t ct = getLastClosedLedgerHeader().header.scpValue.closeTime;
    uint64_t now = mApp.timeNow();
    return (now > ct) ? (now - ct) : 0;
}

void
LedgerManagerImpl::syncMetrics() {
    auto n = static_cast<int64_t>(getState());
    auto c = mLedgerStateCurrent.count();
    if (n != c) {
        mLedgerStateCurrent.set_count(n);
        auto now = mApp.getClock().now();
        mLedgerStateChanges.update(now - mLastStateChange);
        mLastStateChange = now;
    }
    mLedgerAge.set_count(secondsSinceLastLedgerClose());
    mApp.syncOwnMetrics();
}

/*
    This is the main method that closes the current ledger based on
the close context that was computed by SCP or by the historical module
during replays.

*/
void
LedgerManagerImpl::closeLedger(LedgerCloseData const &ledgerData) {
    DBTimeExcluder qtExclude(mApp);
    CLOG(DEBUG, "Ledger") << "starting closeLedger() on ledgerSeq="
                          << mCurrentLedger->mHeader.ledgerSeq;

    auto now = mApp.getClock().now();
    mLedgerAgeClosed.update(now - mLastClose);
    mLastClose = now;
    mLedgerAge.set_count(0);

    // If we do not support ledger version, we can't apply that ledger, fail!
    if (mCurrentLedger->mHeader.ledgerVersion >
        Config::CURRENT_LEDGER_PROTOCOL_VERSION) {
        CLOG(ERROR, "Ledger") << "Unknown ledger version: "
                              << mCurrentLedger->mHeader.ledgerVersion;
        throw std::runtime_error(
                fmt::format("cannot apply ledger with not supported version: {}",
                            mCurrentLedger->mHeader.ledgerVersion));
    }

    if (ledgerData.getTxSet()->previousLedgerHash() != getLastClosedLedgerHeader().hash) {
        CLOG(ERROR, "Ledger")
                << "TxSet mismatch: LCD wants "
                << ledgerAbbrev(ledgerData.getLedgerSeq() - 1,
                                ledgerData.getTxSet()->previousLedgerHash())
                << ", LCL is " << ledgerAbbrev(getLastClosedLedgerHeader());

        CLOG(ERROR, "Ledger")
                << "Full LCL: " << xdr::xdr_to_string(getLastClosedLedgerHeader());

        throw std::runtime_error("txset mismatch");
    }

    if (ledgerData.getTxSet()->getContentsHash() != ledgerData.getValue().txSetHash) {
        throw std::runtime_error("corrupt transaction set");
    }

    soci::transaction txscope(getDatabase().getSession());

    auto ledgerTime = mLedgerClose.timeScope();

    auto const &sv = ledgerData.getValue();
    mCurrentLedger->mHeader.scpValue = sv;

    LedgerDelta ledgerDelta(mCurrentLedger->mHeader, getDatabase());

    // the transaction set that was agreed upon by consensus
    // was sorted by hash; we reorder it so that transactions are
    // sorted such that sequence numbers are respected
    vector<TransactionFramePtr> txs = ledgerData.getTxSet()->sortForApply();

    // first, charge fees
    processFeesSeqNums(txs, ledgerDelta);

    TransactionResultSet txResultSet;
    txResultSet.results.reserve(txs.size());

    applyTransactions(txs, ledgerDelta, txResultSet);

    ledgerDelta.getHeader().txSetResultHash =
            sha256(xdr::xdr_to_opaque(txResultSet));

    // apply any upgrades that were decided during consensus
    // this must be done after applying transactions as the txset
    // was validated before upgrades
    LedgerHeader headerBeforeUpgrades = getCurrentLedgerHeader();
    for (size_t i = 0; i < sv.upgrades.size(); i++) {
        LedgerUpgrade lupgrade;
        try {
            xdr::xdr_from_opaque(sv.upgrades[i], lupgrade);
        }
        catch (xdr::xdr_runtime_error &e) {
            CLOG(FATAL, "Ledger") << "Unknown upgrade step at index " << i;
            throw;
        }

        LedgerHeader previousHeader = getCurrentLedgerHeader();
        try {
            soci::transaction upgradeScope(getDatabase().getSession());
            LedgerDelta upgradeDelta(ledgerDelta);
            Upgrades::applyTo(lupgrade, *this, upgradeDelta);
            // Note: Index from 1 rather than 0 to match the behavior of
            // storeTransaction and storeTransactionFee.
            Upgrades::storeUpgradeHistory(*this, lupgrade,
                                          upgradeDelta.getChanges(),
                                          static_cast<int>(i + 1));
            upgradeDelta.commit();
            upgradeScope.commit();
        }
        catch (std::runtime_error &e) {
            CLOG(ERROR, "Ledger") << "Exception during upgrade: " << e.what();
            getCurrentLedgerHeader() = previousHeader;
        }
        catch (...) {
            CLOG(ERROR, "Ledger") << "Unknown exception during upgrade";
            getCurrentLedgerHeader() = previousHeader;
        }

    }

    // It is required to rollback the current LedgerHeader in order to satisfy
    // the consistency checks enforced by LedgerDelta::commit.
    getCurrentLedgerHeader() = headerBeforeUpgrades;

    ledgerDelta.commit();
    ledgerClosed(ledgerDelta);

    // The next 4 steps happen in a relatively non-obvious, subtle order.
    // This is unfortunate and it would be nice if we could make it not
    // be so subtle, but for the time being this is where we are.
    //
    // 1. Queue any history-checkpoint to the database, _within_ the current
    //    transaction. This way if there's a crash after commit and before
    //    we've published successfully, we'll re-publish on restart.
    //
    // 2. Commit the current transaction.
    //
    // 3. Start any queued checkpoint publishing, _after_ the commit so that
    //    it takes its snapshot of history-rows from the committed state, but
    //    _before_ we GC any buckets (because this is the step where the
    //    bucket refcounts are incremented for the duration of the publish).
    //
    // 4. GC unreferenced buckets. Only do this once publishes are in progress.

    // step 1
    auto &hm = mApp.getHistoryManager();
    hm.maybeQueueHistoryCheckpoint();

    // step 2
    mApp.getDatabase().clearPreparedStatementCache();
    txscope.commit();

    // step 3
    hm.publishQueuedHistory();
    hm.logAndUpdatePublishStatus();

    // step 4
    mApp.getBucketManager().forgetUnreferencedBuckets();
}

void
LedgerManagerImpl::deleteOldEntries(Database &db, uint32_t ledgerSeq, uint32_t count) {
    soci::transaction txscope(db.getSession());
    db.clearPreparedStatementCache();
    LedgerHeaderFrame::deleteOldEntries(db, ledgerSeq, count);
    TransactionFrame::deleteOldEntries(db, ledgerSeq, count);
    HerderPersistence::deleteOldEntries(db, ledgerSeq, count);
    Upgrades::deleteOldEntries(db, ledgerSeq, count);
    db.clearPreparedStatementCache();
    txscope.commit();
}

void
LedgerManagerImpl::checkDbState() {
    std::unordered_map<AccountID, AccountFrame::pointer> aData =
            AccountFrame::checkDB(getDatabase());
    std::unordered_map<AccountID, std::vector<TrustFrame::pointer>> trustLines;
    trustLines = TrustFrame::loadAllLines(getDatabase());
    std::unordered_map<AccountID, std::vector<OfferFrame::pointer>> offers;
    offers = OfferFrame::loadAllOffers(getDatabase());
    std::unordered_map<AccountID, std::vector<DataFrame::pointer>> datas;
    datas = DataFrame::loadAllData(getDatabase());

    for (auto &i : aData) {
        auto const &a = i.second->getAccount();

        // checks the number of sub entries found in the database
        size_t actualSubEntries = a.signers.size();
        auto itTL = trustLines.find(i.first);
        if (itTL != trustLines.end()) {
            actualSubEntries += itTL->second.size();
        }
        auto itOffers = offers.find(i.first);
        if (itOffers != offers.end()) {
            actualSubEntries += itOffers->second.size();
        }

        auto itDatas = datas.find(i.first);
        if (itDatas != datas.end()) {
            actualSubEntries += itDatas->second.size();
        }

        if (a.numSubEntries != (uint32) actualSubEntries) {
            throw std::runtime_error(
                    fmt::format("Mismatch in number of subentries for account {}: "
                                "account says {} but found {}",
                                KeyUtils::toStrKey(i.first), a.numSubEntries,
                                actualSubEntries));
        }
    }
    for (auto &tl : trustLines) {
        if (aData.find(tl.first) == aData.end()) {
            throw std::runtime_error(
                    fmt::format("Unexpected trust line found for account {}",
                                KeyUtils::toStrKey(tl.first)));
        }
    }
    for (auto &of : offers) {
        if (aData.find(of.first) == aData.end()) {
            throw std::runtime_error(
                    fmt::format("Unexpected offer found for account {}",
                                KeyUtils::toStrKey(of.first)));
        }
    }
}

void
LedgerManagerImpl::advanceLedgerPointers() {
    CLOG(DEBUG, "Ledger") << "Advancing LCL: "
                          << ledgerAbbrev(mLastClosedLedger) << " -> "
                          << ledgerAbbrev(mCurrentLedger);

    mLastClosedLedger.hash = mCurrentLedger->getHash();
    mLastClosedLedger.header = mCurrentLedger->mHeader;

    mCurrentLedger = make_shared<LedgerHeaderFrame>(mLastClosedLedger);
    CLOG(DEBUG, "Ledger") << "New current ledger: seq="
                          << mCurrentLedger->mHeader.ledgerSeq;
}

void
LedgerManagerImpl::processFeesSeqNums(std::vector<TransactionFramePtr> &txs,
                                      LedgerDelta &delta) {
    CLOG(DEBUG, "Ledger") << "processing fees and sequence numbers";
    int index = 0;
    try {
        soci::transaction sqlTx(mApp.getDatabase().getSession());
        for (auto tx : txs) {
            LedgerDelta thisTxDelta(delta);
            tx->processFeeSeqNum(thisTxDelta, *this);
            tx->storeTransactionFee(*this, thisTxDelta.getChanges(), ++index);
            thisTxDelta.commit();
        }
        sqlTx.commit();
    }
    catch (std::exception &e) {
        CLOG(FATAL, "Ledger") << "processFeesSeqNums error @ " << index << " : "
                              << e.what();
        throw;
    }
}

void
LedgerManagerImpl::applyTransactions(std::vector<TransactionFramePtr> &txs,
                                     LedgerDelta &ledgerDelta,
                                     TransactionResultSet &txResultSet) {
    CLOG(DEBUG, "Tx") << "applyTransactions: ledger = "
                      << mCurrentLedger->mHeader.ledgerSeq;
    int index = 0;

    // Record tx count
    auto numTxs = txs.size();
    if (numTxs > 0) {
        mTransactionCount.update(static_cast<int64_t>(numTxs));
    }

    for (auto tx : txs) {
        auto txTime = mTransactionApply.timeScope();
        LedgerDelta delta(ledgerDelta);
        TransactionMeta tm(1);
        try {
            CLOG(DEBUG, "Tx")
                    << " tx#" << index << " = " << hexAbbrev(tx->getFullHash())
                    << " txseq=" << tx->getSeqNum() << " (@ "
                    << mApp.getConfig().toShortString(tx->getSourceID()) << ")";
            tx->apply(ledgerDelta, tm.v1(), mApp);
        }
        catch (InvariantDoesNotHold &e) {
            throw e;
        }
        catch (std::runtime_error &e) {
            CLOG(ERROR, "Ledger") << "Exception during tx->apply: " << e.what();
            tx->getResult().result.code(txINTERNAL_ERROR);
        }
        catch (...) {
            CLOG(ERROR, "Ledger") << "Unknown exception during tx->apply";
            tx->getResult().result.code(txINTERNAL_ERROR);
        }
        tx->storeTransaction(*this, tm, ++index, txResultSet);
    }
}

void
LedgerManagerImpl::storeCurrentLedger() {
    mCurrentLedger->storeInsert(*this);

    mApp.getPersistentState().setState(PersistentState::kLastClosedLedger,
                                       binToHex(mCurrentLedger->getHash()));

    // Store the current HAS in the database; this is really just to checkpoint
    // the bucketlist so we can survive a restart and re-attach to the buckets.
    HistoryArchiveState has(mCurrentLedger->mHeader.ledgerSeq,
                            mApp.getBucketManager().getBucketList());

    // We almost always want to try to resolve completed merges to single
    // buckets, as it makes restarts less fragile: fewer saved/restored shadows,
    // fewer buckets for the user to accidentally delete from their buckets
    // dir. But we support the option of not-doing so, only for the sake of
    // testing. Note: this is nonblocking in any case.
    if (!mApp.getConfig().ARTIFICIALLY_PESSIMIZE_MERGES_FOR_TESTING) {
        has.resolveAnyReadyFutures();
    }

    mApp.getPersistentState().setState(PersistentState::kHistoryArchiveState,
                                       has.toString());
}

void
LedgerManagerImpl::ledgerClosed(LedgerDelta const &delta) {
    delta.markMeters(mApp);
    mApp.getBucketManager().addBatch(mApp, mCurrentLedger->mHeader.ledgerSeq,
                                     delta.getLiveEntries(),
                                     delta.getDeadEntries());

    mApp.getBucketManager().snapshotLedger(mCurrentLedger->mHeader);
    storeCurrentLedger();
    advanceLedgerPointers();
}

}
