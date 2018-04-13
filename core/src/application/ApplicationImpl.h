#pragma once

// Copyright 2015 Stellar Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include "application/Application.h"
#include "application/Config.h"
#include "application/PersistentState.h"
#include "util/Timer.h"
#include "util/TmpDir.h"
#include "util/MetricResetter.h"

#include <thread>

namespace medida {
class Counter;

class Timer;
}

namespace vixal {
class TmpDirManager;

class LedgerManager;

class Herder;

class HerderPersistence;

class BucketManager;

class HistoryManager;

class ProcessManager;

class CommandHandler;

class Database;

class LoadGenerator;

class NtpSynchronizationChecker;

class ApplicationImpl : public Application {
public:
    ApplicationImpl(VirtualClock &clock, Config const &cfg);

    ~ApplicationImpl() override;

    void initialize() override;

    uint64_t timeNow() override;

    Config const &getConfig() override;

    State getState() const override;

    std::string getStateHuman() const override;

    bool isStopping() const override;

    VirtualClock &getClock() override;

    medida::MetricsRegistry &getMetrics() override;

    void syncOwnMetrics() override;

    void syncAllMetrics() override;

    void clearMetrics(std::string const &domain) override;

    TmpDirManager &getTmpDirManager() override;

    LedgerManager &getLedgerManager() override;

    BucketManager &getBucketManager() override;

    CatchupManager &getCatchupManager() override;

    HistoryArchiveManager &getHistoryArchiveManager() override;

    HistoryManager &getHistoryManager() override;

    Maintainer &getMaintainer() override;

    ProcessManager &getProcessManager() override;

    Herder &getHerder() override;

    HerderPersistence &getHerderPersistence() override;

    InvariantManager &getInvariantManager() override;

    OverlayManager &getOverlayManager() override;

    Database &getDatabase() const override;

    PersistentState &getPersistentState() override;

    CommandHandler &getCommandHandler() override;

    WorkManager &getWorkManager() override;

    BanManager &getBanManager() override;

    StatusManager &getStatusManager() override;

    asio::io_context &io_context() override;

    void newDB() override;

    void start() override;

    // Stops the worker io_service, which should cause the threads to exit once
    // they finish running any work-in-progress. If you want a more abrupt exit
    // than this, call exit() and hope for the best.
    void gracefulStop() override;

    // Wait-on and join all the threads this application started; should only
    // return when there is no more work to do or someone has force-stopped the
    // worker io_service. Application can be safely destroyed after this
    // returns.
    void joinAllThreads() override;

    bool manualClose() override;

    void generateLoad(bool isCreate, uint32_t nAccounts, uint32_t nTxs,
                      uint32_t txRate, uint32_t batchSize,
                      bool autoRate) override;

    LoadGenerator &getLoadGenerator() override;

    void checkDB() override;

    void applyCfgCommands() override;

    void reportCfgMetrics() override;

    Json::Value getJsonInfo() override;

    void reportInfo() override;

    Hash const &getNetworkID() const override;

protected:
    std::unique_ptr<LedgerManager> mLedgerManager;              // allow to change that for tests

    std::unique_ptr<Herder> mHerder; // allow to change that for tests

private:
    VirtualClock &mVirtualClock;
    Config mConfig;

    // NB: The io_context should come first, then the 'manager' sub-objects, then the threads. Do not reorder these
    // fields.
    //
    // The fields must be constructed in this order, because the subsystem objects need to be fully instantiated before
    // any workers acquire references to them.
    //
    // The fields must be destructed in the reverse order because all worker threads must be joined and destroyed
    // before we start tearing down subsystems.
    asio::io_context io_context_;
    asio::executor_work_guard<asio::io_context::executor_type> work_guard_;

    std::unique_ptr<Database> mDatabase;
    std::unique_ptr<TmpDirManager> mTmpDirManager;
    std::unique_ptr<OverlayManager> mOverlayManager;
    std::unique_ptr<BucketManager> mBucketManager;
    std::unique_ptr<CatchupManager> mCatchupManager;
    std::unique_ptr<HerderPersistence> mHerderPersistence;
    std::unique_ptr<HistoryArchiveManager> mHistoryArchiveManager;
    std::unique_ptr<HistoryManager> mHistoryManager;
    std::unique_ptr<InvariantManager> mInvariantManager;
    std::unique_ptr<Maintainer> mMaintainer;
    std::shared_ptr<ProcessManager> mProcessManager;
    std::unique_ptr<CommandHandler> mCommandHandler;
    std::shared_ptr<WorkManager> mWorkManager;
    std::unique_ptr<PersistentState> mPersistentState;
    std::unique_ptr<LoadGenerator> mLoadGenerator;
    std::unique_ptr<BanManager> mBanManager;
    std::shared_ptr<NtpSynchronizationChecker> mNtpSynchronizationChecker;
    std::unique_ptr<StatusManager> mStatusManager;

    std::vector<std::thread> mWorkerThreads;

    asio::signal_set mStopSignals;

    bool mStopping;

    VirtualTimer mStoppingTimer;

    std::unique_ptr<medida::MetricsRegistry> mMetrics;
    medida::Counter &mAppStateCurrent;
    medida::Timer &mAppStateChanges;
    VirtualClock::time_point mLastStateChange;
    VirtualClock::time_point mStartedOn;

    Hash mNetworkID;

    void shutdown();

    void runWorkerThread(unsigned i);

    void enableInvariantsFromConfig();

    virtual std::unique_ptr<Herder> createHerder();

    virtual std::unique_ptr<InvariantManager> createInvariantManager();

    virtual std::unique_ptr<OverlayManager> createOverlayManager();
};
}
