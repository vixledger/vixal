#pragma once

#include <xdr/ledger-entries.h>
#include <functional>

namespace vixal {

using LedgerEntryProcessor = std::function<void(LedgerEntry const &)>;

}
