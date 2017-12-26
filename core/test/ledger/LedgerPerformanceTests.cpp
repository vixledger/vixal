// Copyright 2014 Stellar Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include "bucket/BucketManager.h"
#include "database/Database.h"
#include "herder/LedgerCloseData.h"
#include "ledger/LedgerHeaderFrame.h"
#include "catch.hpp"
#include "application/Application.h"
#include "simulation/Simulation.h"
#include "test/test.h"
#include "util/Math.h"

using namespace vixal;
using namespace std;
using namespace soci;
using namespace medida;
using namespace nonstd;


namespace vixal {
class LedgerPerformanceTests : public Simulation {
public:
    size_t nAccounts = 10;

    Application::pointer mApp;

    explicit LedgerPerformanceTests(Hash const &networkID)
            : Simulation(Simulation::OVER_LOOPBACK, networkID) {
    }

    void
    resizeAccounts(size_t n) {
        loadAccount(*mAccounts.front());
        mAccounts.resize(n);
    }

    optional<TxInfo>
    ensureAccountIsLoadedCreated(size_t i) {
        if (!mAccounts[i]) {
            auto newAccount = createAccount(i);
            mAccounts[i] = newAccount;
            if (!loadAccount(*newAccount)) {
                newAccount->mSeq =
                        LedgerHeaderFrame(mApp->getLedgerManager().getCurrentLedgerHeader())
                                .getStartingSequenceNumber();
                return make_optional<TxInfo>(newAccount->creationTransaction());
            }
        }
        return nullopt;
    }

    vector<TxInfo>
    createRandomTransaction_uniformLoadingCreating() {
        auto from = pickRandomAccount(mAccounts.at(0), 0);
        auto to = pickRandomAccount(from, 0);
        vector<optional<TxInfo>> txs;

        txs.push_back(ensureAccountIsLoadedCreated(from->mId));
        txs.push_back(ensureAccountIsLoadedCreated(to->mId));

        int64_t amount = static_cast<int64_t>(
                rand_fraction() * min(static_cast<int64_t>(1000),
                                      (from->mBalance - mMinBalance) / 3));

        txs.push_back(make_optional<TxInfo>(createTransferNativeTransaction(from, to, amount)));

        vector<TxInfo> result;
        for (auto tx : txs) {
            if (tx) {
                result.push_back(*tx);
            }
        }
        return result;
    }

    vector<TxInfo>
    createRandomTransactions_uniformLoadingCreating(size_t n) {
        vector<TxInfo> result;
        for (size_t i = 0; i < n; i++) {
            auto newTxs = createRandomTransaction_uniformLoadingCreating();
            std::copy(newTxs.begin(), newTxs.end(), std::back_inserter(result));
        }
        return result;
    }

    static pair<vector<TxInfo>, vector<TxInfo>>
    partitionCreationTransaction(vector<TxInfo> txs) {
        vector<Simulation::TxInfo> creationTxs;
        vector<Simulation::TxInfo> otherTxs;
        for (auto &tx : txs) {
            if (tx.mFrom->mId == 0) {
                creationTxs.push_back(tx);
            } else {
                otherTxs.push_back(tx);
            }
        }
        return pair<vector<TxInfo>, vector<TxInfo>>(creationTxs, otherTxs);
    }

    void
    closeLedger(vector<Simulation::TxInfo> txs) {
        auto baseFee = mApp->getConfig().TESTING_UPGRADE_DESIRED_FEE;
        LoadGenerator::TxMetrics txm(mApp->getMetrics());
        TxSetFramePtr txSet = make_shared<TxSetFrame>(
                mApp->getLedgerManager().getLastClosedLedgerHeader().hash);
        for (auto &tx : txs) {
            std::vector<TransactionFramePtr> txfs;
            tx.toTransactionFrames(*mApp, txfs, txm);
            for (auto f : txfs)
                txSet->add(f);
            tx.recordExecution(baseFee);
        }

        VixalValue sv(txSet->getContentsHash(),
                        VirtualClock::to_time_t(mApp->getClock().now()), emptyUpgradeSteps, 0);
        LedgerCloseData ledgerData(mApp->getLedgerManager().getLedgerNum(),
                                   txSet, sv);

        mApp->getLedgerManager().closeLedger(ledgerData);
    }
};
}

Application::pointer
newLoadTestApp1(VirtualClock &clock) {
    Config cfg =
#ifdef USE_POSTGRES
            !force_sqlite ? getTestConfig(0, Config::TESTDB_POSTGRESQL) :
#endif
            getTestConfig(0, Config::TESTDB_ON_DISK_SQLITE);
    cfg.RUN_STANDALONE = false;
    cfg.TESTING_UPGRADE_MAX_TX_PER_LEDGER = 10000;
    Application::pointer appPtr = Application::create(clock, cfg);
    appPtr->start();
    // force maxTxSetSize to avoid throwing txSets on the floor during the first
    // ledger close
    appPtr->getLedgerManager().getCurrentLedgerHeader().maxTxSetSize = cfg.TESTING_UPGRADE_MAX_TX_PER_LEDGER;
    return appPtr;
}

TEST_CASE("Auto-calibrated single node load test 1"
                  "", "[performance][hide]") {
    VirtualClock clock(VirtualClock::REAL_TIME);
    auto appPtr = newLoadTestApp1(clock);
    appPtr->generateLoad(100000, 100000, 10, true);
    auto &io = clock.io_context();
    auto work = asio::make_work_guard(io);
    auto &complete = appPtr->getMetrics().newMeter({"loadgen", "run", "complete"}, "run");
    while (!io.stopped() && complete.count() == 0) {
        clock.crank();
    }
}

TEST_CASE("ledger performance test", "[performance][hide]") {


    auto nAccounts = 100000;
    auto nLedgers = 1 /* weeks */ * 1 * 24 * 60 * 60 / 500 /* seconds between ledgers */;
    auto nTransactionsPerLedger = 3u;

    auto cfg = getTestConfig(1);

    Hash networkID = sha256(cfg.NETWORK_PASSPHRASE);
    LedgerPerformanceTests sim(networkID);


    SIMULATION_CREATE_NODE(10);

    SCPQuorumSet qSet0;
    qSet0.threshold = 1;
    qSet0.validators.push_back(v10NodeID);

    cfg.DATABASE = SecretValue{"postgresql://host=localhost dbname=performance_test "
                                       "user=test password=test"};
    cfg.BUCKET_DIR_PATH = "performance-test.db.buckets";
    cfg.MANUAL_CLOSE = true;
    sim.addNode(v10SecretKey, qSet0, &cfg);
    sim.mApp = sim.getNodes().front();

    sim.startAllNodes();



    Timer &ledgerTimer = sim.mApp->getMetrics().newTimer({"performance-test", "ledger", "close"});
    Timer &mergeTimer = sim.mApp->getBucketManager().getMergeTimer();
    // use uint64_t for iAccounts to prevent overflows
    for (uint64_t iAccounts = 10000; iAccounts <= nAccounts; iAccounts *= 10) {
        ledgerTimer.clear();
        mergeTimer.clear();

        LOG(INFO) << "Performance test with " << iAccounts << " accounts, starting";
        sim.resizeAccounts(iAccounts);

        for (auto iLedgers = 0; iLedgers < nLedgers; iLedgers++) {
            auto txs = sim.createRandomTransactions_uniformLoadingCreating(nTransactionsPerLedger);

            auto scope = ledgerTimer.timeScope();

            auto createTxs_otherTxs = LedgerPerformanceTests::partitionCreationTransaction(txs);
            if (!createTxs_otherTxs.first.empty()) {
                sim.closeLedger(createTxs_otherTxs.first);
            }
            sim.closeLedger(createTxs_otherTxs.second);

            while (sim.crankAllNodes() > 0);

            if (iLedgers % 100 == 0 && iLedgers != 0) {
                cout << ".";
                cout.flush();
            }

            if (iLedgers % 1000 == 0 && iLedgers != 0) {
                LOG(INFO) << endl
                          << "Performance test with " << iAccounts << " accounts after " << iLedgers << " ledgers";
                LOG(INFO) << endl << sim.metricsSummary("performance-test");
                LOG(INFO) << endl << sim.metricsSummary("bucket");
            }
        }
        LOG(INFO) << "Performance test with " << iAccounts << " accounts, done";
        LOG(INFO) << endl << sim.metricsSummary("performance-test");
        LOG(INFO) << endl << sim.metricsSummary("bucket");
        LOG(INFO) << "done";
    }
}
