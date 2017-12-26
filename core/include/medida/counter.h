//
// Copyright (c) 2012 Daniel Lundin
//

#ifndef MEDIDA_COUNTER_H_
#define MEDIDA_COUNTER_H_

#include <cstdint>
#include <memory>

#include "medida/metric_interface.h"

namespace medida {

class Counter : public MetricInterface {
public:
    explicit Counter(std::size_t init = 0);

    ~Counter() override;

    void process(MetricProcessor &processor) override;

    std::size_t count() const;

    void set_count(std::size_t n);

    void inc(std::size_t n = 1);

    void dec(std::size_t n = 1);

    void clear();

private:
    class Impl;

    std::unique_ptr<Impl> impl_;
};

} // namespace medida

#endif // MEDIDA_COUNTER_H_
