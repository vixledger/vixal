//
// Copyright (c) 2012 Daniel Lundin
//

#ifndef MEDIDA_METER_H_
#define MEDIDA_METER_H_

#include <cstdint>
#include <memory>
#include <string>

#include "medida/stats/ewma.h"
#include "medida/metered_interface.h"
#include "medida/metric_interface.h"
#include "medida/metric_processor.h"
#include "medida/stats/sample.h"

namespace medida {

class Meter : public MetricInterface, MeteredInterface {
public:
    explicit Meter(std::string event_type, std::chrono::nanoseconds rate_unit = std::chrono::seconds(1));

    ~Meter() override;

    std::chrono::nanoseconds rate_unit() const override;

    std::string event_type() const override;

    std::uint64_t count() const override;

    double fifteen_minute_rate() override;

    double five_minute_rate() override;

    double one_minute_rate() override;

    double mean_rate() override;

    void mark(std::uint64_t n = 1);

    void process(MetricProcessor &processor) override;

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace medida

#endif // MEDIDA_METER_H_
