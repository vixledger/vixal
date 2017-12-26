//
// Copyright (c) 2012 Daniel Lundin
//

#ifndef MEDIDA_EXP_DECAY_SAMPLE_H_
#define MEDIDA_EXP_DECAY_SAMPLE_H_

#include <cstdint>
#include <memory>

#include "medida/types.h"
#include "medida/stats/sample.h"

namespace medida {
namespace stats {

class ExpDecaySample : public Sample {
public:
    ExpDecaySample() = delete;

    ExpDecaySample(std::uint32_t reservoirSize, double alpha);

    ~ExpDecaySample() override;

    void clear() override;

    std::uint64_t size() const override;

    void update(std::int64_t value) override;

    virtual void update(std::int64_t value, Clock::time_point timestamp);

    Snapshot makeSnapshot() const override;

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace stats
} // namespace medida

#endif // MEDIDA_EXP_DECAY_SAMPLE_H_
