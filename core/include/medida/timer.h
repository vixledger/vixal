//
// Copyright (c) 2012 Daniel Lundin
//

#ifndef MEDIDA_TIMER_H_
#define MEDIDA_TIMER_H_

#include <chrono>
#include <cstdint>
#include <functional>
#include <memory>

#include "medida/metered_interface.h"
#include "medida/metric_interface.h"
#include "medida/metric_processor.h"
#include "medida/sampling_interface.h"
#include "medida/summarizable_interface.h"
#include "medida/timer_context.h"

namespace medida {
using namespace std::literals;

class Timer : public MetricInterface, MeteredInterface, SamplingInterface, SummarizableInterface {
public:
    explicit Timer(std::chrono::nanoseconds duration_unit = 1ms,
                   std::chrono::nanoseconds rate_unit = 1s);

    ~Timer() override;

    void process(MetricProcessor &processor) override ;

    std::chrono::nanoseconds rate_unit() const override;

    std::string event_type() const override;

    std::uint64_t count() const override;

    double fifteen_minute_rate() override;

    double five_minute_rate() override;

    double one_minute_rate() override;

    double mean_rate() override;

    stats::Snapshot snapshot() const override;

    double max() const override;

    double min() const override;

    double mean() const override;

    double std_dev() const override;

    double sum() const override;

    std::chrono::nanoseconds duration_unit() const;

    void clear();

    void update(std::chrono::nanoseconds duration);

    TimerContext timeScope();

    void time(std::function<void()>);

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace medida

#endif // MEDIDA_TIMER_H_
