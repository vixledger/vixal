//
// Copyright (c) 2012 Daniel Lundin
//

#include "medida/counter.h"
#include <atomic>

namespace medida {

class Counter::Impl {
public:
    explicit Impl(Counter &self, std::size_t init = 0);

    ~Impl();

    void process(MetricProcessor &processor) {
        processor.process(self_);
    }

    std::size_t count() const;

    void set_count(std::size_t n);

    void inc(std::size_t n = 1);

    void dec(std::size_t n = 1);

    void clear();

private:
    Counter &self_;
    std::atomic<std::size_t> count_;
};


Counter::Counter(std::size_t init)
        : impl_(std::make_unique<Counter::Impl>(*this, init)) {
}


Counter::~Counter() = default;


void Counter::process(MetricProcessor &processor) {
    impl_->process(processor);
}


std::size_t Counter::count() const {
    return impl_->count();
}


void Counter::set_count(std::size_t n) {
    return impl_->set_count(n);
}


void Counter::inc(std::size_t n) {
    impl_->inc(n);
}


void Counter::dec(std::size_t n) {
    impl_->dec(n);
}


void Counter::clear() {
    impl_->clear();
}


// === Implementation ===


Counter::Impl::Impl(Counter &self, std::size_t init) :
        self_(self), count_{init} {
}


Counter::Impl::~Impl() = default;


std::size_t Counter::Impl::count() const {
    return count_.load();
}


void Counter::Impl::set_count(std::size_t n) {
    count_ = n;
}


void Counter::Impl::inc(std::size_t n) {
    count_ += n;
}


void Counter::Impl::dec(std::size_t n) {
    count_ -= n;
}


void Counter::Impl::clear() {
    set_count(0);
}




} // namespace medida
