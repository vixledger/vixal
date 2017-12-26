//
// Copyright (c) 2012 Daniel Lundin
//

#include "medida/metrics_registry.h"

#include <algorithm>
#include <mutex>
#include <thread>
#include <utility>

#include "medida/metric_name.h"

namespace medida {

class MetricsRegistry::Impl {
public:
    Impl();

    ~Impl();

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
    std::map<MetricName, std::shared_ptr<MetricInterface>> metrics_;
    mutable std::mutex mutex_;

    template<typename T, typename... Args>
    T &newMetric(const MetricName &name, Args... args);
};


MetricsRegistry::MetricsRegistry()
        : impl_(std::make_unique<MetricsRegistry::Impl>()) {
}


MetricsRegistry::~MetricsRegistry() = default;


Counter &
MetricsRegistry::newCounter(const MetricName &name, std::int64_t init_value) {
    return impl_->newCounter(name, init_value);
}


Histogram &
MetricsRegistry::newHistogram(const MetricName &name,
                                         SamplingInterface::SampleType sample_type) {
    return impl_->newHistogram(name, sample_type);
}


Meter &
MetricsRegistry::newMeter(const MetricName &name, std::string event_type,
                                 Clock::duration rate_unit) {
    return impl_->newMeter(name, std::move(event_type), rate_unit);
}


Timer &
MetricsRegistry::newTimer(const MetricName &name, std::chrono::nanoseconds duration_unit,
                                 std::chrono::nanoseconds rate_unit) {
    return impl_->newTimer(name, duration_unit, rate_unit);
}


std::map<MetricName, std::shared_ptr<MetricInterface>>
MetricsRegistry::getAllMetrics() const {
    return impl_->getAllMetrics();
}


// === Implementation ===


MetricsRegistry::Impl::Impl() = default;


MetricsRegistry::Impl::~Impl() = default;


Counter &MetricsRegistry::Impl::newCounter(const MetricName &name, std::int64_t init_value) {
    return newMetric<Counter>(name, init_value);
}


Histogram &MetricsRegistry::Impl::newHistogram(const MetricName &name,
                                               SamplingInterface::SampleType sample_type) {
    return newMetric<Histogram>(name, sample_type);
}


Meter &MetricsRegistry::Impl::newMeter(const MetricName &name, std::string event_type,
                                       Clock::duration rate_unit) {
    return newMetric<Meter>(name, std::move(event_type), rate_unit);
}


Timer &MetricsRegistry::Impl::newTimer(const MetricName &name, std::chrono::nanoseconds duration_unit,
                                       std::chrono::nanoseconds rate_unit) {
    return newMetric<Timer>(name, duration_unit, rate_unit);
}


template<typename MetricType, typename... Args>
MetricType &MetricsRegistry::Impl::newMetric(const MetricName &name, Args... args) {
    std::lock_guard<std::mutex> lock{mutex_};
    if (metrics_.find(name) == std::end(metrics_)) {
        // GCC 4.6: Bug 44436 emplace* not implemented. Use ::reset instead.
        // metrics_[name].reset(new MetricType(args...));
        metrics_[name] = std::make_shared<MetricType>(args...);
    } else {
    }
    return dynamic_cast<MetricType &>(*metrics_[name]);
}

std::map<MetricName, std::shared_ptr<MetricInterface>>
MetricsRegistry::Impl::getAllMetrics() const {
    return {metrics_};
}


} // namespace medida