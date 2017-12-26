//
// Copyright (c) 2012 Daniel Lundin
//

#ifndef MEDIDA_REPORTING_COLLECTD_REPORTER_H_
#define MEDIDA_REPORTING_COLLECTD_REPORTER_H_

#include <cstdint>
#include <memory>
#include <string>

#include "medida/metrics_registry.h"
#include "medida/metric_processor.h"
#include "medida/reporting/abstract_polling_reporter.h"

namespace medida {
namespace reporting {


class CollectdReporter : public AbstractPollingReporter, public MetricProcessor {
public:
    CollectdReporter(MetricsRegistry &registry, const std::string &hostname = "127.0.0.1", std::uint16_t port = 25826);

    ~CollectdReporter() override;

    void run() override;

    void process(Counter &counter) override;

    void process(Meter &meter) override;

    void process(Histogram &histogram) override;

    void process(Timer &timer) override;

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};


} // namespace reporting
} // namespace medida

#endif // MEDIDA_REPORTING_COLLECTD_REPORTER_H_
