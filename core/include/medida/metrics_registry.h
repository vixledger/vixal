//
// Copyright (c) 2012 Daniel Lundin
//

#ifndef MEDIDA_METRICS_REGISTRY_H_
#define MEDIDA_METRICS_REGISTRY_H_

#include <cstdint>
#include <map>
#include <memory>
#include <string>

#include "medida/counter.h"
#include "medida/histogram.h"
#include "medida/meter.h"
#include "medida/metric_interface.h"
#include "medida/metric_name.h"
#include "medida/metric_processor.h"
#include "medida/timer.h"

namespace medida {

class MetricsRegistry {
public:
    MetricsRegistry();

    ~MetricsRegistry();

    Counter &newCounter(const MetricName &name, std::int64_t init_value = 0);

    Histogram &newHistogram(const MetricName &name,
                            SamplingInterface::SampleType sample_type = SamplingInterface::kUniform);

    Meter &newMeter(const MetricName &name, std::string event_type,
                    Clock::duration rate_unit = std::chrono::seconds(1));

    Timer &newTimer(const MetricName &name,
                    std::chrono::nanoseconds duration_unit = std::chrono::milliseconds(1),
                    std::chrono::nanoseconds rate_unit = std::chrono::seconds(1));

    std::map<MetricName, std::shared_ptr<MetricInterface>> getAllMetrics() const;

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace medida

#endif // MEDIDA_METRICS_REGISTRY_H_
