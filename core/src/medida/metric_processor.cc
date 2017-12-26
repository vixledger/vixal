//
// Copyright (c) 2012 Daniel Lundin
//

#include "medida/metric_processor.h"

namespace medida {

MetricProcessor::~MetricProcessor() = default;

void MetricProcessor::process(Counter& counter) {
}


void MetricProcessor::process(Histogram& histogram) {
}


void MetricProcessor::process(Meter& meter) {
}


void MetricProcessor::process(Timer& timer) {
}


void MetricProcessor::process(MetricInterface& metric) {
}



} // namespace medida