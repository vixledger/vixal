#pragma once

#include <cereal/cereal.hpp>
#include "optional.hpp"

namespace vixal {
using nonstd::optional;
using nonstd::make_optional;
using nonstd::nullopt;
}

namespace cereal {
template<class Archive, class T>
void
save(Archive &ar, vixal::optional<T> const &opt) {
    ar(make_nvp("has", !!opt));
    if (opt) {
        ar(make_nvp("val", *opt));
    }
};

template<class Archive, class T>
void
load(Archive &ar, vixal::optional<T> &o) {
    bool isSet;
    o.reset();
    ar(make_nvp("has", isSet));
    if (isSet) {
        T v;
        ar(make_nvp("val", v));
        o = nonstd::make_optional<T>(v);
    }
};
}
