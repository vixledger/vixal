﻿#include "util/asio.h"
#include "herder/LedgerCloseData.h"
#include "herder/Upgrades.h"
#include "crypto/Hex.h"
#include "application/Application.h"
#include "util/Logging.h"
#include "util/XDROperators.h"

#include <overlay/OverlayManager.h>
#include <xdrpp/marshal.h>

using namespace std;

namespace vixal {

LedgerCloseData::LedgerCloseData(uint32_t ledgerSeq, TxSetFramePtr txSet,
                                 VixalValue const &v)
        : mLedgerSeq(ledgerSeq), mTxSet(txSet), mValue(v) {
    Value x;
    Value y(x.begin(), x.end());

    assert(txSet->getContentsHash() == mValue.txSetHash);
}

std::string
stellarValueToString(VixalValue const &sv) {
    std::stringstream res;

    res << "[ "
        << " txH: " << hexAbbrev(sv.txSetHash) << ", ct: " << sv.closeTime
        << ", upgrades: [";
    for (auto const &upgrade : sv.upgrades) {
        if (upgrade.empty()) {
            // should not happen as this is not valid
            res << "<empty>";
        } else {
            try {
                LedgerUpgrade lupgrade;
                xdr::xdr_from_opaque(upgrade, lupgrade);
                res << Upgrades::toString(lupgrade);
            }
            catch (std::exception &) {
                res << "<unknown>";
            }
        }
        res << ", ";
    }
    res << " ] ]";

    return res.str();
}
}
