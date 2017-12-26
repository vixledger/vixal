//
// Copyright (c) 2012 Daniel Lundin
//

#include "medida/timer_context.h"

#include "medida/timer.h"
#include "medida/types.h"

namespace medida {

class TimerContext::Impl {
public:
    explicit Impl(Timer &timer);

    ~Impl();

    void reset();

    std::chrono::nanoseconds stop();

private:
    Clock::time_point start_time_;
    Timer &timer_;
    bool active_{};
};


TimerContext::TimerContext(TimerContext &&timer) noexcept
        : impl_{std::move(timer.impl_)} {
}

TimerContext::TimerContext(Timer &timer)
        : impl_(std::make_unique<TimerContext::Impl>(timer)) {
}


TimerContext::~TimerContext() = default;

void TimerContext::checkImpl() const {
    if (!impl_) {
        throw std::runtime_error("Access to moved TimerContext::impl_");
    }
}

void TimerContext::reset() {
    checkImpl();
    impl_->reset();
}


std::chrono::nanoseconds TimerContext::stop() {
    checkImpl();
    return impl_->stop();
}


// === Implementation ===


TimerContext::Impl::Impl(Timer &timer)
        : timer_{timer}, start_time_(Clock::now()),
          active_{true} {
}


TimerContext::Impl::~Impl() {
    stop();
}


void TimerContext::Impl::reset() {
    start_time_ = Clock::now();
    active_ = true;
}


std::chrono::nanoseconds TimerContext::Impl::stop() {
    if (active_) {
        auto dur = Clock::now() - start_time_;
        timer_.update(dur);
        active_ = false;
        return dur;
    }
    return std::chrono::nanoseconds(0);
}


} // namespace medida
