// Copyright 2015 Stellar Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include "historywork/WriteSnapshotWork.h"
#include "database/Database.h"
#include "history/StateSnapshot.h"
#include "historywork/Progress.h"
#include "ledger/LedgerHeaderFrame.h"
#include "application/Application.h"
#include "util/XDRStream.h"

namespace vixal {

WriteSnapshotWork::WriteSnapshotWork(Application &app, AbstractWork &parent,
                                     std::shared_ptr<StateSnapshot> snapshot)
        : Work(app, parent, "write-snapshot", Work::RETRY_A_LOT), mSnapshot(snapshot) {
}

WriteSnapshotWork::~WriteSnapshotWork() {
    clearChildren();
}

void
WriteSnapshotWork::onStart() {
    auto handler = callComplete();
    auto snap = mSnapshot;
    auto work = [handler, snap]() {
        asio::error_code ec;
        if (!snap->writeHistoryBlocks()) {
            ec = std::make_error_code(std::errc::io_error);
        }
        asio::post(snap->mApp.getClock().io_context(), [handler, ec]() { handler(ec); });
    };

    // Throw the work over to a worker thread if we can use DB pools,
    // otherwise run on main thread.
    if (mApp.getDatabase().canUsePool()) {
        asio::post(mApp.io_context(), work);
    } else {
        work();
    }
}

void
WriteSnapshotWork::onRun() {
    // Do nothing: we spawned the writer in onStart().
}
}
