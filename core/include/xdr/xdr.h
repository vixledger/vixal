#pragma once

#include <xdr/ledger-entries.h>
#include <xdr/ledger.h>
#include <xdr/overlay.h>
#include <xdr/transaction.h>
#include <xdr/types.h>
#include <functional>

namespace vixal {

using LedgerEntryProcessor = std::function< void(LedgerEntry const & )>;

}
