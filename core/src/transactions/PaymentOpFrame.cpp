// Copyright 2014 Stellar Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include "util/asio.h"

#include "transactions/PaymentOpFrame.h"
#include "transactions/PathPaymentOpFrame.h"

#include "ledger/LedgerDelta.h"

#include "database/Database.h"
#include "application/Application.h"

#include "util/Logging.h"

#include "medida/meter.h"
#include "medida/metrics_registry.h"
#include "util/XDROperators.h"

namespace vixal {

using namespace std;

PaymentOpFrame::PaymentOpFrame(Operation const &op, OperationResult &res,
                               TransactionFrame &parentTx)
        : OperationFrame(op, res, parentTx), mPayment(mOperation.body.paymentOp()) {
}

bool
PaymentOpFrame::doApply(Application &app, LedgerDelta &delta,
                        LedgerManager &ledgerManager) {
    // if sending to self XLM directly, just mark as success, else we need at
    // least to check trustlines
    // in ledger version 2 it would work for any asset type
    auto instantSuccess = app.getLedgerManager().getCurrentLedgerVersion() > 2
                          ? mPayment.destination == getSourceID() &&
                            mPayment.asset.type() == ASSET_TYPE_NATIVE
                          : mPayment.destination == getSourceID();
    if (instantSuccess) {
        app.getMetrics().newMeter({"op-payment", "success", "apply"}, "operation").mark();
        innerResult().code(PAYMENT_SUCCESS);
        return true;
    }

    // build a pathPaymentOp
    Operation op;
    op.sourceAccount = mOperation.sourceAccount;
    op.body.type(PATH_PAYMENT);
    PathPaymentOp &ppOp = op.body.pathPaymentOp();
    ppOp.sendAsset = mPayment.asset;
    ppOp.destAsset = mPayment.asset;

    ppOp.destAmount = mPayment.amount;
    ppOp.sendMax = mPayment.amount;

    ppOp.destination = mPayment.destination;

    OperationResult opRes;
    opRes.code(opINNER);
    opRes.tr().type(PATH_PAYMENT);
    PathPaymentOpFrame ppayment(op, opRes, mParentTx);
    ppayment.setSourceAccountPtr(mSourceAccount);

    if (!ppayment.doCheckValid(app) ||
        !ppayment.doApply(app, delta, ledgerManager)) {
        if (ppayment.getResultCode() != opINNER) {
            throw std::runtime_error("Unexpected error code from pathPayment");
        }
        PaymentResultCode res;

        switch (PathPaymentOpFrame::getInnerCode(ppayment.getResult())) {
            case PATH_PAYMENT_UNDERFUNDED:
                app.getMetrics().newMeter({"op-payment", "failure", "underfunded"}, "operation").mark();
                res = PAYMENT_UNDERFUNDED;
                break;
            case PATH_PAYMENT_SRC_NOT_AUTHORIZED:
                app.getMetrics().newMeter({"op-payment", "failure", "src-not-authorized"}, "operation").mark();
                res = PAYMENT_SRC_NOT_AUTHORIZED;
                break;
            case PATH_PAYMENT_SRC_NO_TRUST:
                app.getMetrics().newMeter({"op-payment", "failure", "src-no-trust"}, "operation").mark();
                res = PAYMENT_SRC_NO_TRUST;
                break;
            case PATH_PAYMENT_NO_DESTINATION:
                app.getMetrics().newMeter({"op-payment", "failure", "no-destination"}, "operation").mark();
                res = PAYMENT_NO_DESTINATION;
                break;
            case PATH_PAYMENT_NO_TRUST:
                app.getMetrics().newMeter({"op-payment", "failure", "no-trust"}, "operation").mark();
                res = PAYMENT_NO_TRUST;
                break;
            case PATH_PAYMENT_NOT_AUTHORIZED:
                app.getMetrics().newMeter({"op-payment", "failure", "not-authorized"}, "operation").mark();
                res = PAYMENT_NOT_AUTHORIZED;
                break;
            case PATH_PAYMENT_LINE_FULL:
                app.getMetrics().newMeter({"op-payment", "failure", "line-full"}, "operation").mark();
                res = PAYMENT_LINE_FULL;
                break;
            case PATH_PAYMENT_NO_ISSUER:
                app.getMetrics().newMeter({"op-payment", "failure", "no-issuer"}, "operation").mark();
                res = PAYMENT_NO_ISSUER;
                break;
            default:
                throw std::runtime_error("Unexpected error code from pathPayment");
        }
        innerResult().code(res);
        return false;
    }

    assert(PathPaymentOpFrame::getInnerCode(ppayment.getResult()) ==
           PATH_PAYMENT_SUCCESS);

    app.getMetrics().newMeter({"op-payment", "success", "apply"}, "operation").mark();
    innerResult().code(PAYMENT_SUCCESS);

    return true;
}

bool
PaymentOpFrame::doCheckValid(Application &app) {
    if (mPayment.amount <= 0) {
        app.getMetrics().newMeter({"op-payment", "invalid", "malformed-negative-amount"}, "operation").mark();
        innerResult().code(PAYMENT_MALFORMED);
        return false;
    }
    if (!isAssetValid(mPayment.asset)) {
        app.getMetrics().newMeter({"op-payment", "invalid", "malformed-invalid-asset"}, "operation").mark();
        innerResult().code(PAYMENT_MALFORMED);
        return false;
    }
    return true;
}
}
