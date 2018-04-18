// Copyright 2017 Stellar Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include "test/TestPrinter.h"
#include "test/TestMarket.h"
#include <util/format.h>

namespace Catch {

std::string
StringMaker<vixal::OfferState>::convert(vixal::OfferState const &os) {
    return fmt::format(
            "selling: {}, buying: {}, price: {}, amount: {}, type: {}",
            xdr::xdr_to_string(os.selling), xdr::xdr_to_string(os.buying),
            xdr::xdr_to_string(os.price), os.amount,
            os.type == vixal::OfferType::PASSIVE ? "passive" : "active");
}

std::string
StringMaker<vixal::CatchupRange>::convert(vixal::CatchupRange const &cr) {
    return fmt::format("[{}..{}], applyBuckets: {}", cr.first.first(),
                       cr.first.last(), cr.second);
}

std::string
StringMaker<vixal::historytestutils::CatchupPerformedWork>::convert(
        vixal::historytestutils::CatchupPerformedWork const &cm) {
    return fmt::format("{}, {}, {}, {}, {}, {}, {}, {}",
                       cm.mHistoryArchiveStatesDownloaded,
                       cm.mLedgersDownloaded, cm.mLedgersVerified,
                       cm.mLedgerChainsVerificationFailed,
                       cm.mBucketsDownloaded, cm.mBucketsApplied,
                       cm.mTransactionsDownloaded, cm.mTransactionsApplied);
}

}
