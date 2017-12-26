#pragma once

// Copyright 2017 Stellar Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include "catchup/CatchupWork.h"
#include "xdr/transaction.h"
#include "xdrpp/printer.h"
#include "application/Application.h"
#include "history/HistoryTestsUtils.h"
#include <catch.hpp>

namespace vixal {

struct OfferState;
}

namespace Catch {

template<>
struct StringMaker<vixal::ClaimOfferAtom> {
    static std::string convert(vixal::ClaimOfferAtom const &coa);
};

template<>
struct StringMaker<vixal::Hash> {
    static std::string convert(vixal::Hash const &coa);
};

template<>
struct StringMaker<vixal::OfferEntry> {
    static std::string convert(vixal::OfferEntry const &coa);
};

template<>
struct StringMaker<vixal::OfferState> {
    static std::string convert(vixal::OfferState const &coa);
};

template<>
struct StringMaker<vixal::TransactionResult> {
    static std::string convert(vixal::TransactionResult const &coa);
};

template<>
struct StringMaker<vixal::CatchupRange> {
    static std::string convert(vixal::CatchupRange const &coa);
};

template<>
struct StringMaker<vixal::historytestutils::CatchupPerformedWork> {
    static std::string convert(vixal::historytestutils::CatchupPerformedWork const &coa);
};

}

