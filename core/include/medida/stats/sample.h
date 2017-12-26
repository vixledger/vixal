//
// Copyright (c) 2012 Daniel Lundin
//

#ifndef MEDIDA_SAMPLE_H_
#define MEDIDA_SAMPLE_H_

#include <cstddef>
#include <cstdint>

#include "medida/stats/snapshot.h"

namespace medida {
namespace stats {

class Sample {
public:
    virtual ~Sample() = default;;

    virtual void clear() = 0;

    virtual std::uint64_t size() const = 0;

    virtual void update(std::int64_t value) = 0;

    virtual Snapshot makeSnapshot() const = 0;
};

} // namespace stats
} // namespace medida

#endif // MEDIDA_SAMPLE_H_
