//
// Copyright (c) 2012 Daniel Lundin
//

#include "medida/reporting/abstract_polling_reporter.h"

#include <iostream>
#include <thread>
#include <atomic>

namespace medida {
namespace reporting {

class AbstractPollingReporter::Impl {
public:
    explicit Impl(AbstractPollingReporter &self);

    ~Impl();

    void shutdown();

    void start(Clock::duration period = std::chrono::seconds(5));

private:
    AbstractPollingReporter &self_;
    std::atomic<bool> running_;
    std::thread thread_;

    void loop(Clock::duration period);
};


AbstractPollingReporter::AbstractPollingReporter()
        : impl_(std::make_unique<AbstractPollingReporter::Impl>(*this)) {
}


AbstractPollingReporter::~AbstractPollingReporter() = default;


void AbstractPollingReporter::shutdown() {
    impl_->shutdown();
}


void AbstractPollingReporter::start(Clock::duration period) {
    impl_->start(period);
}


void AbstractPollingReporter::run() {
}


// === Implementation ===


AbstractPollingReporter::Impl::Impl(AbstractPollingReporter &self)
        : self_(self),
          running_{false} {
}


AbstractPollingReporter::Impl::~Impl() {
    shutdown();
}


void AbstractPollingReporter::Impl::shutdown() {
    if (running_) {
        running_ = false;
        thread_.join();
    }
}


void AbstractPollingReporter::Impl::start(Clock::duration period) {
    if (!running_) {
        running_ = true;
        thread_ = std::thread(&AbstractPollingReporter::Impl::loop, this, period);
    }
}


void AbstractPollingReporter::Impl::loop(Clock::duration period) {
    while (running_) {
        std::this_thread::sleep_for(period);
        self_.run();
    }
}


} // namespace reporting
} // namespace medida
