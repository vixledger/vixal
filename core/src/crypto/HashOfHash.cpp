#include "crypto/HashOfHash.h"

namespace std {

size_t
hash<vixal::uint256>::operator()(vixal::uint256 const &x) const noexcept {
    size_t res = x[0];
    res = (res << 8) | x[1];
    res = (res << 8) | x[2];
    res = (res << 8) | x[3];
    return res;
}
}
