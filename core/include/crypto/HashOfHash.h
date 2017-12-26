#pragma once

#include <xdr/types.h>

namespace std {
template<>
struct hash<vixal::uint256> {
    size_t operator()(vixal::uint256 const &x) const noexcept;
};
}
