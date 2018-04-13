// Copyright 2018 Stellar Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

#include "util/MetricResetter.h"

namespace vixal {
void
MetricResetter::process(medida::Counter &counter) {
    counter.clear();
}

void
MetricResetter::process(medida::Meter& meter) {
    // Meter has no "clear" method
}

void
MetricResetter::process(medida::Histogram& histogram) {
    histogram.clear();
}

void
MetricResetter::process(medida::Timer& timer) {
    timer.clear();
}
}

