//
// Copyright (c) 2012 Daniel Lundin
//

#ifndef MEDIDA_HISTOGRAM_H_
#define MEDIDA_HISTOGRAM_H_

#include <cstdint>
#include <memory>

#include "medida/metric_interface.h"
#include "medida/sampling_interface.h"
#include "medida/summarizable_interface.h"
#include "medida/stats/sample.h"

namespace medida {

class Histogram : public MetricInterface, SamplingInterface, SummarizableInterface {
public:
    explicit Histogram(SampleType sample_type = kUniform);

    ~Histogram() override;

    stats::Snapshot snapshot() const override ;

    double sum() const override;

    double max() const override;

    double min() const override ;

    double mean() const override ;

    double std_dev() const override ;

    void update(std::int64_t value);

    std::uint64_t count() const;

    double variance() const;

    void process(MetricProcessor &processor) override ;

    void clear();

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace medida

#endif // MEDIDA_HISTOGRAM_H_
