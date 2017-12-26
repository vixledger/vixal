//
// Copyright (c) 2012 Daniel Lundin
//

#include "medida/stats/uniform_sample.h"

#include <algorithm>
#include <atomic>
#include <mutex>
#include <random>
#include <vector>

namespace medida {
namespace stats {

class UniformSample::Impl {
public:
    explicit Impl(std::uint32_t reservoirSize);

    ~Impl();

    void clear();

    std::uint64_t size() const;

    void update(std::int64_t value);

    Snapshot makeSnapshot() const;

private:
    std::atomic<std::uint64_t> count_;
    std::vector<std::int64_t> values_;
    mutable std::mt19937_64 rng_;
    mutable std::mutex mutex_;
};


UniformSample::UniformSample(std::uint32_t reservoirSize)
        : impl_(std::make_unique<Impl>(reservoirSize)) {
}


UniformSample::~UniformSample() = default;


void UniformSample::clear() {
    impl_->clear();
}


std::uint64_t UniformSample::size() const {
    return impl_->size();
}


void UniformSample::update(std::int64_t value) {
    impl_->update(value);
}


Snapshot UniformSample::makeSnapshot() const {
    return impl_->makeSnapshot();
}


// === Implementation ===


UniformSample::Impl::Impl(std::uint32_t reservoirSize)
        : count_{},
          values_(reservoirSize), // FIXME: Explicit and non-uniform
          rng_{std::random_device()()},
          mutex_{} {
    clear();
}


UniformSample::Impl::~Impl() = default;


void UniformSample::Impl::clear() {
    std::lock_guard<std::mutex> lock{mutex_};
    for (auto &v : values_) {
        v = 0;
    }
    count_ = 0;
}


std::uint64_t UniformSample::Impl::size() const {
    std::uint64_t size = values_.size();
    std::uint64_t count = count_.load();
    return std::min(count, size);
}


void UniformSample::Impl::update(std::int64_t value) {
    auto count = ++count_;
    std::lock_guard<std::mutex> lock{mutex_};
    auto size = values_.size();
    if (count < size) {
        values_[count - 1] = value;
    } else {
        std::uniform_int_distribution<uint64_t> uniform(0, count - 1);
        auto rand = uniform(rng_);
        if (rand < size) {
            values_[rand] = value;
        }
    }
}


Snapshot UniformSample::Impl::makeSnapshot() const {
    std::uint64_t size = values_.size();
    std::uint64_t count = count_.load();
    std::lock_guard<std::mutex> lock{mutex_};
    auto begin = std::begin(values_);
    return Snapshot {{begin, begin + std::min(count, size)}};
}


} // namespace stats
} // namespace medida
