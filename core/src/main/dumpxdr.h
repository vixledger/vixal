#pragma once

// Copyright 2015 Stellar Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include "xdr/ledger-entries.h"
#include "xdr/ledger.h"
#include "xdr/scp.h"

namespace vixal {

std::string xdr_printer(const PublicKey &pk);

extern const char *signtxn_network_id;

void dumpxdr(std::string const &filename);

void printtxn(std::string const &filename, bool base64);

void signtxn(std::string const &filename, bool base64);

void priv2pub();
}
