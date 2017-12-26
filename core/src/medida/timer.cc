//
// Copyright (c) 2012 Daniel Lundin
//

#include <utility>

#include "medida/timer.h"

#include "medida/histogram.h"
#include "medida/meter.h"

#include "medida/timer_context.h"

namespace medida {

class Timer::Impl {
public:
    explicit Impl(Timer &self, std::chrono::nanoseconds duration_unit = std::chrono::milliseconds(1),
                  std::chrono::nanoseconds rate_unit = std::chrono::seconds(1));

    ~Impl();

    void process(MetricProcessor &processor);

    std::chrono::nanoseconds rate_unit() const;

    std::string event_type() const;

    std::uint64_t count() const;

    double fifteen_minute_rate();

    double five_minute_rate();

    double one_minute_rate();

    double mean_rate();

    stats::Snapshot getSnapshot() const;

    double max() const;

    double min() const;

    double mean() const;

    double std_dev() const;

    double sum() const;

    std::chrono::nanoseconds duration_unit() const;

    void clear();

    void update(std::chrono::nanoseconds duration);

    TimerContext timeScope();

    void time(std::function<void()>);

private:
    Timer &self_;
    const std::chrono::nanoseconds duration_unit_;
    const std::int64_t duration_unit_nanos_;
    const std::chrono::nanoseconds rate_unit_;
    Meter meter_;
    Histogram histogram_;
};


Timer::Timer(std::chrono::nanoseconds duration_unit, std::chrono::nanoseconds rate_unit)
        : impl_{new Timer::Impl{*this, duration_unit, rate_unit}} {
}


Timer::~Timer() = default;


std::chrono::nanoseconds Timer::duration_unit() const {
    return impl_->duration_unit();
}


std::chrono::nanoseconds Timer::rate_unit() const {
    return impl_->rate_unit();
}


std::uint64_t Timer::count() const {
    return impl_->count();
}


double Timer::min() const {
    return impl_->min();
}


double Timer::max() const {
    return impl_->max();
}


double Timer::mean() const {
    return impl_->mean();
}


double Timer::std_dev() const {
    return impl_->std_dev();
}


double Timer::sum() const {
    return impl_->sum();
}


std::string Timer::event_type() const {
    return impl_->event_type();
}


double Timer::fifteen_minute_rate() {
    return impl_->fifteen_minute_rate();
}


double Timer::five_minute_rate() {
    return impl_->five_minute_rate();
}


double Timer::one_minute_rate() {
    return impl_->one_minute_rate();
}


double Timer::mean_rate() {
    return impl_->mean_rate();
}


void Timer::process(MetricProcessor &processor) {
    impl_->process(processor);
    // processor.process(*this);  // FIXME: pimpl?
}


void Timer::clear() {
    impl_->clear();
}


void Timer::update(std::chrono::nanoseconds duration) {
    impl_->update(duration);
}


stats::Snapshot Timer::snapshot() const {
    return impl_->getSnapshot();
}


TimerContext Timer::timeScope() {
    return impl_->timeScope();
}


void Timer::time(std::function<void()> func) {
    impl_->time(std::move(func));
}


// === Implementation ===


Timer::Impl::Impl(Timer &self, std::chrono::nanoseconds duration_unit, std::chrono::nanoseconds rate_unit)
        : self_(self),
          duration_unit_{duration_unit},
          duration_unit_nanos_{duration_unit.count()},
          rate_unit_{rate_unit},
          meter_{"calls", rate_unit},
          histogram_{SamplingInterface::kBiased} {
}


Timer::Impl::~Impl() = default;


std::chrono::nanoseconds Timer::Impl::duration_unit() const {
    return duration_unit_;
}


std::chrono::nanoseconds Timer::Impl::rate_unit() const {
    return rate_unit_;
}


std::uint64_t Timer::Impl::count() const {
    return histogram_.count();
}


double Timer::Impl::min() const {
    return histogram_.min() / duration_unit_nanos_;
}


double Timer::Impl::max() const {
    return histogram_.max() / duration_unit_nanos_;
}


double Timer::Impl::mean() const {
    return histogram_.mean() / duration_unit_nanos_;
}


double Timer::Impl::std_dev() const {
    return histogram_.std_dev() / duration_unit_nanos_;
}


double Timer::Impl::sum() const {
    return histogram_.sum() / duration_unit_nanos_;
}


std::string Timer::Impl::event_type() const {
    return meter_.event_type();
}


double Timer::Impl::fifteen_minute_rate() {
    return meter_.fifteen_minute_rate();
}


double Timer::Impl::five_minute_rate() {
    return meter_.five_minute_rate();
}


double Timer::Impl::one_minute_rate() {
    return meter_.one_minute_rate();
}


double Timer::Impl::mean_rate() {
    return meter_.mean_rate();
}


void Timer::Impl::process(MetricProcessor &processor) {
    processor.process(self_);
}


void Timer::Impl::clear() {
    histogram_.clear();
}


void Timer::Impl::update(std::chrono::nanoseconds duration) {
    auto count = duration.count();
    if (count >= 0) {
        histogram_.update(count);
        meter_.mark();
    }
}


stats::Snapshot Timer::Impl::getSnapshot() const {
    auto values = histogram_.snapshot().getValues();
    std::vector<double> converted;
    converted.reserve(values.size());
    for (auto &v : values) {
        converted.push_back(v / (double) duration_unit_nanos_);
    }
    return {converted};
}


void Timer::Impl::time(std::function<void()> func) {
    auto t = self_.timeScope();
    func();
}


TimerContext Timer::Impl::timeScope() {
    return {self_};
}


} // namespace medida
