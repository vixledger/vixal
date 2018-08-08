// Copyright 2014 Stellar Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include "util/asio.h"
#include "transactions/CreateAccountOpFrame.h"
#include "database/Database.h"
#include "ledger/LedgerDelta.h"

#include "application/Application.h"
#include "medida/meter.h"
#include "medida/metrics_registry.h"
#include "util/XDROperators.h"

namespace vixal {

using namespace std;

CreateAccountOpFrame::CreateAccountOpFrame(Operation const &op,
                                           OperationResult &res,
                                           TransactionFrame &parentTx)
        : OperationFrame(op, res, parentTx), mCreateAccount(mOperation.body.createAccountOp()) {
}

bool
CreateAccountOpFrame::doApply(Application &app, LedgerDelta &delta,
                              LedgerManager &ledgerManager) {
    AccountFrame::pointer destAccount;

    Database &db = ledgerManager.getDatabase();

    destAccount = AccountFrame::loadAccount(delta, mCreateAccount.destination, db);

    if (!destAccount) {
        if (mCreateAccount.startingBalance < ledgerManager.getMinBalance(0)) {
            // not over the minBalance to make an account
            app.getMetrics().newMeter({"op-create-account", "failure", "low-reserve"}, "operation").mark();
            innerResult().code(CREATE_ACCOUNT_LOW_RESERVE);
            return false;
        } else {
            int64_t minBalance =
                    mSourceAccount->getMinimumBalance(ledgerManager);

            if (mSourceAccount->getAvailableBalance(ledgerManager) <
                mCreateAccount.startingBalance) { // they don't have enough to send
                app.getMetrics().newMeter({"op-create-account", "failure", "underfunded"}, "operation").mark();
                innerResult().code(CREATE_ACCOUNT_UNDERFUNDED);
                return false;
            }

            auto ok = mSourceAccount->addBalance(
                    -mCreateAccount.startingBalance, ledgerManager);
            assert(ok);

            mSourceAccount->storeChange(delta, db);

            destAccount = make_shared<AccountFrame>(mCreateAccount.destination);
            auto& acc = destAccount->getAccount();
            acc.seqNum = delta.getHeaderFrame().getStartingSequenceNumber();
            acc.balance = mCreateAccount.startingBalance;

            destAccount->storeAdd(delta, db);

            app.getMetrics().newMeter({"op-create-account", "success", "apply"}, "operation").mark();
            innerResult().code(CREATE_ACCOUNT_SUCCESS);
            return true;
        }
    } else {
        app.getMetrics().newMeter({"op-create-account", "failure", "already-exist"}, "operation").mark();
        innerResult().code(CREATE_ACCOUNT_ALREADY_EXIST);
        return false;
    }
}

bool
CreateAccountOpFrame::doCheckValid(Application &app) {
    if (mCreateAccount.startingBalance <= 0) {
        app.getMetrics().newMeter({"op-create-account", "invalid", "malformed-negative-balance"}, "operation").mark();
        innerResult().code(CREATE_ACCOUNT_MALFORMED);
        return false;
    }

    if (mCreateAccount.destination == getSourceID()) {
        app.getMetrics().newMeter({"op-create-account", "invalid",
                                   "malformed-destination-equals-source"},
                                  "operation").mark();
        innerResult().code(CREATE_ACCOUNT_MALFORMED);
        return false;
    }

    return true;
}
}
