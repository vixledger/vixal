#pragma once

// Copyright 2017 Stellar Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include "xdr/types.h"
#include <vector>

namespace vixal
{

struct SCPEnvelope;
struct SCPStatement;
struct VixalValue;

std::vector<Hash> getTxSetHashes(SCPEnvelope const& envelope);
std::vector<VixalValue> getVixalValues(SCPStatement const& envelope);
}
