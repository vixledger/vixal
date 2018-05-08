// Copyright 2017 Stellar Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include "herder/HerderSCPDriver.h"
#include "ledger/LedgerManager.h"
#include "crypto/Hex.h"
#include "crypto/SHA.h"
#include "HerderImpl.h"
#include "herder/LedgerCloseData.h"
#include "application/Application.h"
#include "util/Logging.h"
#include <xdrpp/marshal.h>
#include <util/format.h>

namespace vixal {

HerderSCPDriver::SCPMetrics::SCPMetrics(Application &app)
        : mEnvelopeSign(app.getMetrics().newMeter({"scp", "envelope", "sign"}, "envelope")),
          mEnvelopeValidSig(app.getMetrics().newMeter({"scp", "envelope", "validsig"}, "envelope")),
          mEnvelopeInvalidSig(app.getMetrics().newMeter({"scp", "envelope", "invalidsig"}, "envelope")),
          mValueValid(app.getMetrics().newMeter({"scp", "value", "valid"}, "value")),
          mValueInvalid(app.getMetrics().newMeter({"scp", "value", "invalid"}, "value")),
          mValueExternalize(app.getMetrics().newMeter({"scp", "value", "externalize"}, "value")),
          mQuorumHeard(app.getMetrics().newMeter({"scp", "quorum", "heard"}, "quorum")),
          mNominatingValue(app.getMetrics().newMeter({"scp", "value", "nominating"}, "value")),
          mUpdatedCandidate(app.getMetrics().newMeter({"scp", "value", "candidate"}, "value")),
          mStartBallotProtocol(app.getMetrics().newMeter({"scp", "ballot", "started"}, "ballot")),
          mAcceptedBallotPrepared(app.getMetrics().newMeter({"scp", "ballot", "accepted-prepared"}, "ballot")),
          mConfirmedBallotPrepared(app.getMetrics().newMeter({"scp", "ballot", "confirmed-prepared"}, "ballot")),
          mAcceptedCommit(app.getMetrics().newMeter({"scp", "ballot", "accepted-commit"}, "ballot")),
          mHerderStateCurrent(app.getMetrics().newCounter({"herder", "state", "current"})),
          mHerderStateChanges(app.getMetrics().newTimer({"herder", "state", "changes"})),
          mNominateToPrepare(app.getMetrics().newTimer({"scp", "timing", "nominated"})),
          mPrepareToExternalize(app.getMetrics().newTimer({"scp", "timing", "externalized"})) {
}

HerderSCPDriver::HerderSCPDriver(Application &app, HerderImpl &herder,
                                 Upgrades const &upgrades,
                                 PendingEnvelopes &pendingEnvelopes)
        : mApp{app}, mHerder{herder},
          mLedgerManager{mApp.getLedgerManager()},
          mUpgrades{upgrades},
          mPendingEnvelopes{pendingEnvelopes},
          mSCP(*this, mApp.getConfig().NODE_SEED.getPublicKey(),
               mApp.getConfig().NODE_IS_VALIDATOR, mApp.getConfig().QUORUM_SET),
          mSCPMetrics{mApp},
          mLastStateChange{mApp.getClock().now()} {
}

HerderSCPDriver::~HerderSCPDriver() = default;

void
HerderSCPDriver::stateChanged() {
    mSCPMetrics.mHerderStateCurrent.set_count(static_cast<size_t>(getState()));
    auto now = mApp.getClock().now();
    mSCPMetrics.mHerderStateChanges.update(now - mLastStateChange);
    mLastStateChange = now;
    mApp.syncOwnMetrics();
}

void
HerderSCPDriver::bootstrap() {
    stateChanged();
    clearSCPExecutionEvents();
}

void
HerderSCPDriver::lostSync() {
    stateChanged();

    // transfer ownership to mHerderSCPDriver.lastTrackingSCP()
    mLastTrackingSCP.reset(mTrackingSCP.release());
}

Herder::State
HerderSCPDriver::getState() const {
    return mTrackingSCP && mLastTrackingSCP ? Herder::HERDER_TRACKING_STATE
                                            : Herder::HERDER_SYNCING_STATE;
}

void
HerderSCPDriver::syncMetrics() {
    auto c = mSCPMetrics.mHerderStateCurrent.count();
    auto n = static_cast<int64_t>(getState());
    if (c != n) {
        mSCPMetrics.mHerderStateCurrent.set_count(n);
    }
}

void
HerderSCPDriver::restoreSCPState(uint64_t index, VixalValue const &value) {
    mTrackingSCP = std::make_unique<ConsensusData>(index, value);
}

// envelope handling

void
HerderSCPDriver::signEnvelope(SCPEnvelope &envelope) {
    mSCPMetrics.mEnvelopeSign.mark();
    envelope.signature = mApp.getConfig().NODE_SEED.sign(xdr::xdr_to_opaque(
            mApp.getNetworkID(), ENVELOPE_TYPE_SCP, envelope.statement));
}

bool
HerderSCPDriver::verifyEnvelope(SCPEnvelope const &envelope) {
    auto b = PubKeyUtils::verifySig(
            envelope.statement.nodeID, envelope.signature,
            xdr::xdr_to_opaque(mApp.getNetworkID(), ENVELOPE_TYPE_SCP,
                               envelope.statement));
    if (b) {
        mSCPMetrics.mEnvelopeValidSig.mark();
    } else {
        mSCPMetrics.mEnvelopeInvalidSig.mark();
    }

    return b;
}

void
HerderSCPDriver::emitEnvelope(SCPEnvelope const &envelope) {
    mHerder.emitEnvelope(envelope);
}

// value validation

bool
HerderSCPDriver::isSlotCompatibleWithCurrentState(uint64_t slotIndex) const {
    bool res = false;
    if (mLedgerManager.isSynced()) {
        auto const &lcl = mLedgerManager.getLastClosedLedgerHeader();
        res = (slotIndex == (lcl.header.ledgerSeq + 1));
    }

    return res;
}

SCPDriver::ValidationLevel
HerderSCPDriver::validateValueHelper(uint64_t slotIndex,
                                     VixalValue const &b) const {
    uint64_t lastCloseTime;

    bool compat = isSlotCompatibleWithCurrentState(slotIndex);

    if (compat) {
        lastCloseTime = mLedgerManager.getLastClosedLedgerHeader()
                .header.scpValue.closeTime;
    } else {
        if (!mTrackingSCP) {
            // if we're not tracking, there is not much more we can do to
            // validate
            return SCPDriver::kMaybeValidValue;
        }

        // Check slotIndex.
        if (nextConsensusLedgerIndex() > slotIndex) {
            // we already moved on from this slot
            // still send it through for emitting the final messages
            return SCPDriver::kMaybeValidValue;
        }
        if (nextConsensusLedgerIndex() < slotIndex) {
            // this is probably a bug as "tracking" means we're processing
            // messages only for smaller slots
            CLOG(ERROR, "Herder")
                    << "HerderSCPDriver::validateValue"
                    << " i: " << slotIndex
                    << " processing a future message while tracking";

            return SCPDriver::kInvalidValue;
        }
        lastCloseTime = mTrackingSCP->mConsensusValue.closeTime;
    }

    // Check closeTime (not too old)
    if (b.closeTime <= lastCloseTime) {
        return SCPDriver::kInvalidValue;
    }

    // Check closeTime (not too far in future)
    uint64_t timeNow = mApp.timeNow();
    if (b.closeTime > timeNow + Herder::MAX_TIME_SLIP_SECONDS.count()) {
        return SCPDriver::kInvalidValue;
    }

    if (!compat) {
        // this is as far as we can go if we don't have the state
        return SCPDriver::kMaybeValidValue;
    }

    Hash const &txSetHash = b.txSetHash;

    // we are fully synced up

    TxSetFramePtr txSet = mPendingEnvelopes.getTxSet(txSetHash);

    SCPDriver::ValidationLevel res;

    if (!txSet) {
        CLOG(ERROR, "Herder") << "HerderSCPDriver::validateValue"
                              << " i: " << slotIndex << " txSet not found?";

        res = SCPDriver::kInvalidValue;
    } else if (!txSet->checkValid(mApp)) {
        if (Logging::logDebug("Herder"))
            CLOG(DEBUG, "Herder") << "HerderSCPDriver::validateValue"
                                  << " i: " << slotIndex << " Invalid txSet:"
                                  << " " << hexAbbrev(txSet->getContentsHash());
        res = SCPDriver::kInvalidValue;
    } else {
        if (Logging::logDebug("Herder"))
            CLOG(DEBUG, "Herder")
                    << "HerderSCPDriver::validateValue"
                    << " i: " << slotIndex
                    << " txSet: " << hexAbbrev(txSet->getContentsHash()) << " OK";
        res = SCPDriver::kFullyValidatedValue;
    }
    return res;
}

SCPDriver::ValidationLevel
HerderSCPDriver::validateValue(uint64_t slotIndex, Value const &value,
                               bool nomination) {
    VixalValue b;
    try {
        xdr::xdr_from_opaque(value, b);
    }
    catch (...) {
        mSCPMetrics.mValueInvalid.mark();
        return SCPDriver::kInvalidValue;
    }

    SCPDriver::ValidationLevel res = validateValueHelper(slotIndex, b);
    if (res != SCPDriver::kInvalidValue) {
        auto const &lcl = mLedgerManager.getLastClosedLedgerHeader();

        LedgerUpgradeType lastUpgradeType = LEDGER_UPGRADE_VERSION;
        // check upgrades
        for (size_t i = 0; i < b.upgrades.size(); i++) {
            LedgerUpgradeType thisUpgradeType;
            if (!mUpgrades.isValid(b.upgrades[i], thisUpgradeType, nomination,
                                   mApp.getConfig(), lcl.header)) {
                CLOG(TRACE, "Herder")
                        << "HerderSCPDriver::validateValue invalid step at index "
                        << i;
                res = SCPDriver::kInvalidValue;
            }
            if (i != 0 && (lastUpgradeType >= thisUpgradeType)) {
                CLOG(TRACE, "Herder")
                        << "HerderSCPDriver::validateValue out of "
                           "order upgrade step at index "
                        << i;
                res = SCPDriver::kInvalidValue;
            }

            lastUpgradeType = thisUpgradeType;
        }
    }

    if (res) {
        mSCPMetrics.mValueValid.mark();
    } else {
        mSCPMetrics.mValueInvalid.mark();
    }
    return res;
}

Value
HerderSCPDriver::extractValidValue(uint64_t slotIndex, Value const &value) {
    VixalValue b;
    try {
        xdr::xdr_from_opaque(value, b);
    }
    catch (...) {
        return Value();
    }
    Value res;
    if (validateValueHelper(slotIndex, b) == SCPDriver::kFullyValidatedValue) {
        auto const &lcl = mLedgerManager.getLastClosedLedgerHeader();

        // remove the upgrade steps we don't like
        LedgerUpgradeType thisUpgradeType;
        for (auto it = b.upgrades.begin(); it != b.upgrades.end();) {

            if (!mUpgrades.isValid(*it, thisUpgradeType, true, mApp.getConfig(),
                                   lcl.header)) {
                it = b.upgrades.erase(it);
            } else {
                it++;
            }
        }

        res = xdr::xdr_to_opaque(b);
    }

    return res;
}

// value marshaling

std::string
HerderSCPDriver::toShortString(PublicKey const &pk) const {
    return mApp.getConfig().toShortString(pk);
}

std::string
HerderSCPDriver::getValueString(Value const &v) const {
    VixalValue b;
    if (v.empty()) {
        return "[:empty:]";
    }

    try {
        xdr::xdr_from_opaque(v, b);

        return stellarValueToString(b);
    }
    catch (...) {
        return "[:invalid:]";
    }
}

// timer handling

void
HerderSCPDriver::setupTimer(uint64_t slotIndex, int timerID,
                            std::chrono::milliseconds timeout,
                            std::function<void()> cb) {
    // don't setup timers for old slots
    if (slotIndex <= mApp.getHerder().getCurrentLedgerSeq()) {
        mSCPTimers.erase(slotIndex);
        return;
    }

    auto &slotTimers = mSCPTimers[slotIndex];

    auto it = slotTimers.find(timerID);
    if (it == slotTimers.end()) {
        it = slotTimers.emplace(timerID, std::make_unique<VirtualTimer>(mApp.getClock())).first;
    }
    auto &timer = *it->second;
    timer.cancel();
    if (cb) {
        timer.expires_after(timeout);
        timer.async_wait(cb, &VirtualTimer::onFailureNoop);
    }
}

// core SCP

Value
HerderSCPDriver::combineCandidates(uint64_t slotIndex, std::set<Value> const &candidates) {
    Hash h;

    VixalValue comp(h, 0, emptyUpgradeSteps, 0);

    std::map<LedgerUpgradeType, LedgerUpgrade> upgrades;

    std::set<TransactionFramePtr> aggSet;

    auto const &lcl = mLedgerManager.getLastClosedLedgerHeader();

    Hash candidatesHash;

    std::vector<VixalValue> candidateValues;

    for (auto const &c : candidates) {
        candidateValues.emplace_back();
        VixalValue &sv = candidateValues.back();

        xdr::xdr_from_opaque(c, sv);
        candidatesHash ^= sha256(c);

        // max closeTime
        if (comp.closeTime < sv.closeTime) {
            comp.closeTime = sv.closeTime;
        }
        for (auto const &upgrade : sv.upgrades) {
            LedgerUpgrade lupgrade;
            xdr::xdr_from_opaque(upgrade, lupgrade);
            auto it = upgrades.find(lupgrade.type());
            if (it == upgrades.end()) {
                upgrades.emplace(std::make_pair(lupgrade.type(), lupgrade));
            } else {
                LedgerUpgrade &clUpgrade = it->second;
                switch (lupgrade.type()) {
                    case LEDGER_UPGRADE_VERSION:
                        // pick the highest version
                        clUpgrade.newLedgerVersion() = std::max(clUpgrade.newLedgerVersion(),
                                                                lupgrade.newLedgerVersion());
                        break;
                    case LEDGER_UPGRADE_BASE_FEE:
                        // take the max fee
                        clUpgrade.newBaseFee() = std::max(clUpgrade.newBaseFee(), lupgrade.newBaseFee());
                        break;
                    case LEDGER_UPGRADE_MAX_TX_SET_SIZE:
                        // take the max tx set size
                        clUpgrade.newMaxTxSetSize() = std::max(clUpgrade.newMaxTxSetSize(), lupgrade.newMaxTxSetSize());
                        break;
                    case LEDGER_UPGRADE_BASE_RESERVE:
                        // take the max base reserve
                        clUpgrade.newBaseReserve() = std::max(clUpgrade.newBaseReserve(), lupgrade.newBaseReserve());
                    default:
                        // should never get there with values that are not valid
                        throw std::runtime_error("invalid upgrade step");
                }
            }
        }
    }

    // take the txSet with the highest number of transactions,
    // highest xored hash that we have
    TxSetFramePtr bestTxSet;
    {
        Hash highest;
        TxSetFramePtr highestTxSet;
        for (auto const &sv : candidateValues) {
            TxSetFramePtr cTxSet = mPendingEnvelopes.getTxSet(sv.txSetHash);

            if (cTxSet && cTxSet->previousLedgerHash() == lcl.hash) {
                if (!highestTxSet || (cTxSet->mTransactions.size() >
                                      highestTxSet->mTransactions.size()) ||
                    ((cTxSet->mTransactions.size() ==
                      highestTxSet->mTransactions.size()) &&
                     lessThanXored(highest, sv.txSetHash, candidatesHash))) {
                    highestTxSet = cTxSet;
                    highest = sv.txSetHash;
                }
            }
        }
        // make a copy as we're about to modify it and we don't want to mess
        // with the txSet cache
        bestTxSet = std::make_shared<TxSetFrame>(*highestTxSet);
    }

    for (auto const &upgrade : upgrades) {
        Value v(xdr::xdr_to_opaque(upgrade.second));
        comp.upgrades.emplace_back(v.begin(), v.end());
    }

    std::vector<TransactionFramePtr> removed;

    // just to be sure
    bestTxSet->trimInvalid(mApp, removed);
    comp.txSetHash = bestTxSet->getContentsHash();

    if (removed.size() != 0) {
        CLOG(WARNING, "Herder") << "Candidate set had " << removed.size() << " invalid transactions";

        // post to avoid triggering SCP handling code recursively
        asio::post(mApp.getClock().io_context(), [this, bestTxSet]() {
            mPendingEnvelopes.recvTxSet(bestTxSet->getContentsHash(),
                                        bestTxSet);
        });
    }

    return xdr::xdr_to_opaque(comp);
}

void
HerderSCPDriver::valueExternalized(uint64_t slotIndex, Value const &value) {
    mSCPMetrics.mValueExternalize.mark();

    auto it = mSCPTimers.begin(); // cancel all timers below this slot
    while (it != mSCPTimers.end() && it->first <= slotIndex) {
        it = mSCPTimers.erase(it);
    }

    if (slotIndex <= mApp.getHerder().getCurrentLedgerSeq()) {
        // externalize may trigger on older slots:
        //  * when the current instance starts up
        //  * when getting back in sync (a gap potentially opened)
        // in both cases it's safe to just ignore those as we're already
        // tracking a more recent state
        CLOG(DEBUG, "Herder") << "Ignoring old ledger externalize " << slotIndex;
        return;
    }

    VixalValue b;
    try {
        xdr::xdr_from_opaque(value, b);
    }
    catch (...) {
        // This may not be possible as all messages are validated and should
        // therefore contain a valid VixalValue.
        CLOG(ERROR, "Herder") << "HerderSCPDriver::valueExternalized"
                              << " Externalized VixalValue malformed";
        // no point in continuing as 'b' contains garbage at this point
        abort();
    }

    // log information from older ledger to increase the chances that
    // all messages made it
    if (slotIndex > 2) {
        logQuorumInformation(slotIndex - 2);
    }

    if (!mCurrentValue.empty()) {
        // stop nomination
        // this may or may not be the ledger that is currently externalizing
        // in both cases, we want to stop nomination as:
        // either we're closing the current ledger (typical case)
        // or we're going to trigger catchup from history
        mSCP.stopNomination(mLedgerSeqNominating);
        mCurrentValue.clear();
    }

    if (!mTrackingSCP) {
        stateChanged();
    }

    mTrackingSCP = std::make_unique<ConsensusData>(slotIndex, b);

    if (!mLastTrackingSCP) {
        mLastTrackingSCP = std::make_unique<ConsensusData>(*mTrackingSCP);
    }

    mHerder.valueExternalized(slotIndex, b);
}

void
HerderSCPDriver::logQuorumInformation(uint64_t index) {
    std::string res;
    auto v = mApp.getHerder().getJsonQuorumInfo(mSCP.getLocalNodeID(), true, index);
    auto slots = v.get("slots", "");
    if (!slots.empty()) {
        std::string indexs = std::to_string(static_cast<uint32>(index));
        auto i = slots.get(indexs, "");
        if (!i.empty()) {
            Json::StreamWriterBuilder swb;
            swb["indentation"] = "";
            CLOG(INFO, "Herder") << "Quorum information for " << index << " : " << Json::writeString(swb, i);
        }
    }
}

void
HerderSCPDriver::nominate(uint64_t slotIndex, VixalValue const &value,
                          TxSetFramePtr proposedSet,
                          VixalValue const &previousValue) {
    mCurrentValue = xdr::xdr_to_opaque(value);
    mLedgerSeqNominating = static_cast<uint32_t>(slotIndex);

    auto valueHash = sha256(xdr::xdr_to_opaque(mCurrentValue));
    CLOG(DEBUG, "Herder") << "HerderSCPDriver::triggerNextLedger"
                          << " txSet.size: "
                          << proposedSet->mTransactions.size()
                          << " previousLedgerHash: "
                          << hexAbbrev(proposedSet->previousLedgerHash())
                          << " value: " << hexAbbrev(valueHash)
                          << " slot: " << slotIndex;

    auto prevValue = xdr::xdr_to_opaque(previousValue);
    mSCP.nominate(slotIndex, mCurrentValue, prevValue);
}

SCPQuorumSetPtr
HerderSCPDriver::getQSet(Hash const &qSetHash) {
    return mPendingEnvelopes.getQSet(qSetHash);
}

void
HerderSCPDriver::ballotDidHearFromQuorum(uint64_t, SCPBallot const &) {
    mSCPMetrics.mQuorumHeard.mark();
}

void
HerderSCPDriver::nominatingValue(uint64_t slotIndex, Value const &value) {
    if (Logging::logDebug("Herder"))
        CLOG(DEBUG, "Herder") << "nominatingValue i:" << slotIndex
                              << " v: " << getValueString(value);

    if (!value.empty()) {
        mSCPMetrics.mNominatingValue.mark();
    }
}

void
HerderSCPDriver::updatedCandidateValue(uint64_t slotIndex, Value const &value) {
    mSCPMetrics.mUpdatedCandidate.mark();
}

void
HerderSCPDriver::startedBallotProtocol(uint64_t slotIndex,
                                       SCPBallot const &ballot) {
    recordSCPEvent(slotIndex, false);
    mSCPMetrics.mStartBallotProtocol.mark();
}

void
HerderSCPDriver::acceptedBallotPrepared(uint64_t slotIndex,
                                        SCPBallot const &ballot) {
    mSCPMetrics.mAcceptedBallotPrepared.mark();
}

void
HerderSCPDriver::confirmedBallotPrepared(uint64_t slotIndex,
                                         SCPBallot const &ballot) {
    mSCPMetrics.mConfirmedBallotPrepared.mark();
}

void
HerderSCPDriver::acceptedCommit(uint64_t slotIndex, SCPBallot const &ballot) {
    mSCPMetrics.mAcceptedCommit.mark();
}

optional<VirtualClock::time_point>
HerderSCPDriver::getPrepareStart(uint64_t slotIndex) {
    optional<VirtualClock::time_point> res;
    auto it = mSCPExecutionTimes.find(slotIndex);
    if (it != mSCPExecutionTimes.end()) {
        res = it->second.mPrepareStart;
    }
    return res;
}

void
HerderSCPDriver::recordSCPEvent(uint64_t slotIndex, bool isNomination) {
    auto &timing = mSCPExecutionTimes[slotIndex];
    VirtualClock::time_point start = mApp.getClock().now();
    if (isNomination) {
        timing.mNominationStart =
                make_optional<VirtualClock::time_point>(start);
    } else {
        timing.mPrepareStart = make_optional<VirtualClock::time_point>(start);
    }
}

void
HerderSCPDriver::recordSCPExecutionMetrics(uint64_t slotIndex) {
    auto externalizeStart = mApp.getClock().now();

    // Use threshold of 0 in case of a single node
    auto &qset = mApp.getConfig().QUORUM_SET;
    auto isSingleNode = qset.innerSets.size() == 0 &&
                        qset.validators.size() == 1 &&
                        qset.validators[0] == getSCP().getLocalNodeID();
    auto threshold = isSingleNode ? std::chrono::nanoseconds::zero()
                                  : Herder::TIMERS_THRESHOLD_NANOSEC;

    auto SCPTimingIt = mSCPExecutionTimes.find(slotIndex);
    if (SCPTimingIt == mSCPExecutionTimes.end()) {
        return;
    }

    auto &SCPTiming = SCPTimingIt->second;

    auto recordTiming = [&](VirtualClock::time_point start,
                            VirtualClock::time_point end, medida::Timer &timer,
                            std::string const &logStr) {
        auto delta =
                std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
        CLOG(DEBUG, "Herder") << fmt::format("{} delta for slot {} is {} ns.",
                                             logStr, slotIndex, delta.count());
        if (delta >= threshold) {
            timer.update(delta);
        }
    };

    // Compute nomination time
    if (SCPTiming.mNominationStart && SCPTiming.mPrepareStart) {
        recordTiming(*SCPTiming.mNominationStart, *SCPTiming.mPrepareStart,
                     mSCPMetrics.mNominateToPrepare, "Nominate");
    }

    // Compute prepare time
    if (SCPTiming.mPrepareStart) {
        recordTiming(*SCPTiming.mPrepareStart, externalizeStart,
                     mSCPMetrics.mPrepareToExternalize, "Prepare");
    }

    // Clean up timings map
    auto it = mSCPExecutionTimes.begin();
    while (it != mSCPExecutionTimes.end() && it->first < slotIndex) {
        it = mSCPExecutionTimes.erase(it);
    }
}

void
HerderSCPDriver::clearSCPExecutionEvents() {
    mSCPExecutionTimes.clear();
}
}
