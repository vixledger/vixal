//
// Copyright (c) 2012 Daniel Lundin
//

#include "medida/meter.h"

#include <atomic>
#include <mutex>
#include <utility>

namespace medida {

static const auto kTickInterval = Clock::duration(std::chrono::seconds(5)).count();

class Meter::Impl {
public:
    explicit Impl(Meter &self, std::string event_type, std::chrono::nanoseconds rate_unit = std::chrono::seconds(1));

    ~Impl();

    std::chrono::nanoseconds rate_unit() const;

    std::string event_type() const;

    std::uint64_t count() const;

    double fifteen_minute_rate();

    double five_minute_rate();

    double one_minute_rate();

    double mean_rate();

    void mark(std::uint64_t n = 1);

    void process(MetricProcessor &processor) {
        processor.process(self_);
    }

private:
    Meter &self_;

    const std::string event_type_;
    const std::chrono::nanoseconds rate_unit_;
    std::atomic<std::uint64_t> count_;
    const Clock::time_point start_time_;
    std::atomic<std::int64_t> last_tick_;
    stats::EWMA m1_rate_;
    stats::EWMA m5_rate_;
    stats::EWMA m15_rate_;

    void tick();

    void tickIfNecessary();
};


Meter::Meter(std::string event_type, std::chrono::nanoseconds rate_unit)
        : impl_(std::make_unique<Meter::Impl>(*this, std::move(event_type), rate_unit)) {
}


Meter::~Meter() = default;


std::chrono::nanoseconds Meter::rate_unit() const {
    return impl_->rate_unit();
}


std::string Meter::event_type() const {
    return impl_->event_type();
}


std::uint64_t Meter::count() const {
    return impl_->count();
}


double Meter::fifteen_minute_rate() {
    return impl_->fifteen_minute_rate();
}


double Meter::five_minute_rate() {
    return impl_->five_minute_rate();
}


double Meter::one_minute_rate() {
    return impl_->one_minute_rate();
}


double Meter::mean_rate() {
    return impl_->mean_rate();
}


void Meter::mark(std::uint64_t n) {
    impl_->mark(n);
}


void Meter::process(MetricProcessor &processor) {
    impl_->process(processor); //processor.Process(*this);  // FIXME: pimpl?
}


// === Implementation ===


Meter::Impl::Impl(Meter &self, std::string event_type, std::chrono::nanoseconds rate_unit)
        : self_(self),
          event_type_(std::move(event_type)),
          rate_unit_(rate_unit), count_(0),
          start_time_(Clock::now()),
          last_tick_(std::chrono::duration_cast<std::chrono::nanoseconds>(start_time_.time_since_epoch()).count()),
          m1_rate_(stats::EWMA::oneMinuteEWMA()),
          m5_rate_(stats::EWMA::fiveMinuteEWMA()),
          m15_rate_(stats::EWMA::fifteenMinuteEWMA()) {
}


Meter::Impl::~Impl() = default;


std::chrono::nanoseconds Meter::Impl::rate_unit() const {
    return rate_unit_;
}


std::string Meter::Impl::event_type() const {
    return event_type_;
}


std::uint64_t Meter::Impl::count() const {
    return count_.load();
}


double Meter::Impl::fifteen_minute_rate() {
    tickIfNecessary();
    return m15_rate_.getRate();
}


double Meter::Impl::five_minute_rate() {
    tickIfNecessary();
    return m5_rate_.getRate();
}


double Meter::Impl::one_minute_rate() {
    tickIfNecessary();
    return m1_rate_.getRate();
}


double Meter::Impl::mean_rate() {
    double c = count_.load();
    if (c > 0) {
        std::chrono::nanoseconds elapsed = Clock::now() - start_time_;
        return c * rate_unit_.count() / elapsed.count();
    }
    return 0.0;
}


void Meter::Impl::mark(std::uint64_t n) {
    tickIfNecessary();
    count_ += n;
    m1_rate_.update(n);
    m5_rate_.update(n);
    m15_rate_.update(n);
}


void Meter::Impl::tick() {
    m1_rate_.tick();
    m5_rate_.tick();
    m15_rate_.tick();
}


void Meter::Impl::tickIfNecessary() {
    auto old_tick = last_tick_.load();
    auto new_tick = std::chrono::duration_cast<std::chrono::nanoseconds>(Clock::now().time_since_epoch()).count();
    auto age = new_tick - old_tick;
    if (age > kTickInterval) {
        last_tick_ = new_tick;
        auto required_ticks = age / kTickInterval;
        for (auto i = 0; i < required_ticks; i++) {
            tick();
        }
    }
}


} // namespace medida
