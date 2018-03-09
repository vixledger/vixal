#pragma once

// Copyright 2014 Stellar Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include "xdr/ledger.h"
#include "xdr/ledger-entries.h"
#include "transactions/TransactionFrame.h"

namespace vixal {
class Application;

class TxSetFrame;

using TxSetFramePtr = std::shared_ptr<TxSetFrame>;

class TxSetFrame {
    bool mHashIsValid;
    Hash mHash;

    Hash mPreviousLedgerHash;

    bool
    checkOrTrim(Application &app,
                std::function<bool(TransactionFramePtr, SequenceNumber)> processInvalidTxLambda,
                std::function<bool(std::vector<TransactionFramePtr> const &)> processLastInvalidTxLambda);

public:
    std::vector<TransactionFramePtr> mTransactions;

    explicit TxSetFrame(Hash const &previousLedgerHash);

    TxSetFrame(TxSetFrame const &other) = default;

    // make it from the wire
    TxSetFrame(Hash const &networkID, TransactionSet const &xdrSet);

    // returns the hash of this tx set
    Hash getContentsHash();

    Hash &previousLedgerHash();

    Hash const &previousLedgerHash() const;

    void sortForHash();

    std::vector<TransactionFramePtr> sortForApply();

    bool checkValid(Application &app);

    void trimInvalid(Application &app,
                     std::vector<TransactionFramePtr> &trimmed);

    void surgePricingFilter(LedgerManager const &lm);

    void removeTx(TransactionFramePtr tx);

    void add(TransactionFramePtr tx) {
        mTransactions.push_back(tx);
        mHashIsValid = false;
    }

    size_t
    size() {
        return mTransactions.size();
    }

    void toXDR(TransactionSet &set);
};
} // namespace vixal
