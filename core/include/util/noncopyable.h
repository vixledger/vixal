#pragma once

// Copyright 2015 Stellar Development Foundation and contributors. Licensed
// under the Apache License, Version 2.0. See the COPYING file at the root
// of this distribution or at http://www.apache.org/licenses/LICENSE-2.0

namespace vixal {

struct noncopyable {
    noncopyable() = default;

    noncopyable(noncopyable const &) = delete;

    noncopyable &operator=(noncopyable const &) = delete;
};

struct nonmovable {
    nonmovable() = default;

    nonmovable(nonmovable &&) = delete;

    nonmovable &operator=(nonmovable &&) = delete;
};

struct nonmovableorcopyable : private noncopyable, nonmovable {
    nonmovableorcopyable() = default;
};
}
