// Copyright 2015 Stellar Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include "historywork/GetHistoryArchiveStateWork.h"
#include "history/HistoryArchive.h"
#include "historywork/GetRemoteFileWork.h"
#include "ledger/LedgerManager.h"
#include "util/format.h"
#include "application/Application.h"
#include "util/Logging.h"
#include <medida/meter.h>
#include <medida/metrics_registry.h>

namespace vixal {

GetHistoryArchiveStateWork::GetHistoryArchiveStateWork(
        Application &app, AbstractWork &parent, std::string uniqueName,
        HistoryArchiveState &state, uint32_t seq,
        VirtualClock::duration const &initialDelay,
        std::shared_ptr<HistoryArchive const> archive, size_t maxRetries)
        : Work(app, parent, std::move(uniqueName), maxRetries), mState(state),
          mSeq(seq), mInitialDelay(initialDelay),
          mArchive(archive),
          mLocalFilename(archive ? HistoryArchiveState::localName(app, archive->getName())
                                 : app.getHistoryManager().localFilename(HistoryArchiveState::baseName())),
          mGetHistoryArchiveStateStart(app.getMetrics().newMeter(
                  {"history", "download-history-archive-state", "start"}, "event")),
          mGetHistoryArchiveStateSuccess(app.getMetrics().newMeter(
                  {"history", "download-history-archive-state", "success"}, "event")),
          mGetHistoryArchiveStateFailure(app.getMetrics().newMeter(
                  {"history", "download-history-archive-state", "failure"}, "event")) {
}

GetHistoryArchiveStateWork::~GetHistoryArchiveStateWork() {
    clearChildren();
}

std::string
GetHistoryArchiveStateWork::getStatus() const {
    if (getState() == WORK_FAILURE_RETRY) {
        auto eta = getRetryETA();
        return fmt::format("Awaiting checkpoint (ETA: {:d} seconds)", eta);
    }
    return Work::getStatus();
}

VirtualClock::duration
GetHistoryArchiveStateWork::getRetryDelay() const {
    if (mInitialDelay.count() != 0 && mRetries == 0) {
        return mInitialDelay;
    }
    return Work::getRetryDelay();
}

void
GetHistoryArchiveStateWork::onReset() {
    clearChildren();
    std::remove(mLocalFilename.c_str());
    addWork<GetRemoteFileWork>(mSeq == 0
                               ? HistoryArchiveState::wellKnownRemoteName()
                               : HistoryArchiveState::remoteName(mSeq),
                               mLocalFilename, mArchive, getMaxRetries());

    if (mSeq != 0 && mRetries == 0 && mInitialDelay.count() != 0) {
        // If this is our first reset (on addition) and we're fetching a
        // known snapshot, immediately initiate a timed retry, to avoid
        // cluttering the console with the initial-probe failure.
        setState(WORK_FAILURE_RETRY);
        scheduleRetry();
    } else {
        mGetHistoryArchiveStateStart.mark();
    }
}

void
GetHistoryArchiveStateWork::onRun() {
    try {
        mState.load(mLocalFilename);
        scheduleSuccess();
    }
    catch (std::runtime_error &e) {
        CLOG(ERROR, "History") << "error loading history state: " << e.what();
        scheduleFailure();
    }
}

Work::State
GetHistoryArchiveStateWork::onSuccess() {
    mGetHistoryArchiveStateSuccess.mark();
    return Work::onSuccess();
}

void
GetHistoryArchiveStateWork::onFailureRetry() {
    mGetHistoryArchiveStateFailure.mark();
    Work::onFailureRetry();
}

void
GetHistoryArchiveStateWork::onFailureRaise() {
    mGetHistoryArchiveStateFailure.mark();
    Work::onFailureRaise();
}
}
