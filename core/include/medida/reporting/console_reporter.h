//
// Copyright (c) 2012 Daniel Lundin
//

#ifndef MEDIDA_REPORTING_CONSOLE_REPORTER_H_
#define MEDIDA_REPORTING_CONSOLE_REPORTER_H_

#include <iostream>

#include "medida/metric_processor.h"
#include "medida/metrics_registry.h"
#include "medida/reporting/abstract_polling_reporter.h"

namespace medida {
namespace reporting {

class ConsoleReporter : public AbstractPollingReporter, public MetricProcessor {
public:
    explicit ConsoleReporter(MetricsRegistry &registry, std::ostream &out = std::cerr);

    ~ConsoleReporter() override;

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

#endif // MEDIDA_REPORTING_CONSOLE_REPORTER_H_
