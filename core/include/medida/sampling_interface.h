//
// Copyright (c) 2012 Daniel Lundin
//

#ifndef MEDIDA_SAMPLING_INTERFACE_H_
#define MEDIDA_SAMPLING_INTERFACE_H_

#include "medida/stats/snapshot.h"

namespace medida {

class SamplingInterface {
public:
    enum SampleType {
        kUniform, kBiased
    };

    virtual ~SamplingInterface() = default;;

    virtual stats::Snapshot snapshot() const = 0;
};

} // namespace medida

#endif // MEDIDA_SAMPLING_INTERFACE_H_
