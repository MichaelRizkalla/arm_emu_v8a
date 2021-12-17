
#include <Utility/Bitset.h>

BEGIN_NAMESPACE

void swap(Bitset& lhs, Bitset& rhs) noexcept {
    lhs.swap(rhs);
}

Bitset operator&(const Bitset& lhs, const Bitset& rhs) {
    return Bitset { lhs } &= rhs;
}

Bitset operator|(const Bitset& lhs, const Bitset& rhs) {
    return Bitset { lhs } |= rhs;
}

Bitset operator^(const Bitset& lhs, const Bitset& rhs) {
    return Bitset { lhs } ^= rhs;
}

Bitset Concat(Bitset lhs, Bitset rhs) {
    return lhs.Concat(rhs);
}

END_NAMESPACE
