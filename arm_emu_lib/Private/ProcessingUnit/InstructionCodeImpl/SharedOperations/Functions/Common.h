
template < std::size_t N, std::size_t M >
[[nodiscard]] std::bitset< N * M > Replicate(std::bitset< M > x) const {
    static_assert(N % M == 0);
    std::bitset< N * M > return_value;
    std::bitset< N * M > value(0);
    if (N * M > 0) {
        value = x.to_ullong();
    }
    if (N / M > 0) {
        for (std::size_t n = 0; n < static_cast< std::size_t >(N / M); n++) {
            return_value |= value;
            value <<= M;
        }
    }
    return return_value;
}

[[nodiscard]] std::uint64_t Replicate(std::uint64_t x, std::size_t currentSize, std::size_t targetSize) const {
    if (targetSize % currentSize != 0) {
        throw undefined_behaviour {};
    }
    std::uint64_t return_value = 0;
    std::uint64_t value        = 0;
    if (targetSize * currentSize > 0) {
        value        = x;
        return_value = x;
    }
    for (std::size_t n = 1; n < static_cast< std::size_t >(targetSize / currentSize); n++) {
        value <<= currentSize;
        return_value |= value;
    }
    return return_value;
}

template < std::size_t N, std::size_t M >
[[nodiscard]] auto SignExtend(std::bitset< M > x) const {
    static_assert(N >= M);
    auto             val          = Replicate< N - M >(std::bitset< 1 > { x[M - 1] });
    std::bitset< N > return_value = concate< N - M, M >(val, x);
    return return_value;
}

template < std::size_t N, std::size_t M >
[[nodiscard]] auto ZeroExtend(std::bitset< M > x) const {
    static_assert(N >= M);
    return std::bitset< N >(x.to_ullong());
}

template < std::size_t N, std::size_t M >
[[nodiscard]] std::bitset< N > Extend(std::bitset< M > x, bool unsigned_) const {
    if (unsigned_)
        return ZeroExtend< N, M >(x);
    else
        return SignExtend< N, M >(x);
}

template < std::size_t N >
[[nodiscard]] std::int64_t HighestSetBit(std::bitset< N > x) const noexcept {
    for (std::int64_t i = N - 1; i > -1; i--) {
        if (x[i] == true) {
            return i;
        }
    }
    return -1;
}

[[nodiscard]] std::int64_t HighestSetBit(std::uint64_t x) const {
    for (std::int64_t i = 63; i > -1; i--) {
        if (static_cast< bool >(x & (static_cast< std::uint64_t >(1) << i)) == true)
            return i;
    }
    throw undefined_behaviour {};
}

template < std::size_t N >
[[nodiscard]] std::pair< std::bitset< N >, bool > LSL_C(std::bitset< N > x, std::uint8_t shift) const {
    if (shift < 0) {
        throw undefined_behaviour {};
    }
    std::bitset< N + 1 > extended_x { x.to_ullong() };
    extended_x <<= shift;
    std::bitset< N > result { extended_x.to_ullong() };
    auto             carry_out = extended_x[N];
    return std::make_pair(result, carry_out);
}

template < std::size_t N >
[[nodiscard]] std::bitset< N > LSL(std::bitset< N > x, std::uint8_t shift) const {
    if (shift < 0) {
        throw undefined_behaviour {};
    }
    if (shift == 0) {
        return x;
    } else {
        auto [result, _] = LSL_C< N >(x, shift);
        return result;
    }
}

template < std::size_t N >
[[nodiscard]] std::pair< std::bitset< N >, bool > LSR_C(std::bitset< N > x, std::uint8_t shift) const {
    if (shift < 0) {
        throw undefined_behaviour {};
    }

    Bitset extended_x { N + shift, x.to_ullong() };

    auto carry_out = extended_x[shift - 1];
    extended_x >>= shift;
    std::bitset< N > result { extended_x.ToULLong() };
    return std::make_pair(result, carry_out);
}

template < std::size_t N >
[[nodiscard]] std::bitset< N > LSR(std::bitset< N > x, std::uint8_t shift) const {
    if (shift < 0) {
        throw undefined_behaviour {};
    }
    if (shift == 0) {
        return x;
    } else {
        auto [result, _] = LSR_C< N >(x, shift);
        return result;
    }
}

template < std::size_t N >
[[nodiscard]] std::pair< std::bitset< N >, bool > ASR_C(std::bitset< N > x, std::uint8_t shift) const {
    if (shift < 0) {
        throw undefined_behaviour {};
    }

    Bitset extended_x { N + shift, x.to_ullong() };
    auto   carry_out = extended_x[shift - 1];
    extended_x >>= shift;
    std::bitset< N > result { extended_x.ToULLong() };
    return std::make_pair(result, carry_out);
}

template < std::size_t N >
[[nodiscard]] std::bitset< N > ASR(std::bitset< N > x, std::uint8_t shift) const {
    if (shift < 0) {
        throw undefined_behaviour {};
    }
    if (shift == 0) {
        return x;
    } else {
        auto [result, _] = ASR_C< N >(x, shift);
        return result;
    }
}

template < std::size_t N >
[[nodiscard]] std::pair< std::bitset< N >, bool > ROR_C(std::bitset< N > x, std::uint8_t shift) const {
    if (shift < 0) {
        throw undefined_behaviour {};
    }

    auto m      = static_cast< std::uint8_t >(shift % N);
    auto result = LSR< N >(x, m);
    result |= LSL< N >(x, N - m);

    auto carry_out = result[N - 1];
    return std::make_pair(result, carry_out);
}

template < std::size_t N >
[[nodiscard]] std::bitset< N > ROR(std::bitset< N > x, std::uint8_t shift) const {
    if (shift < 0) {
        throw undefined_behaviour {};
    }
    if (shift == 0) {
        return x;
    } else {
        auto [result, _] = ROR_C< N >(x, shift);
        return result;
    }
}

[[nodiscard]] std::uint64_t ROR(std::uint64_t x, std::uint64_t currentSize, std::uint8_t shift) const {
    if (shift < 0) {
        throw undefined_behaviour {};
    }
    if (shift == 0) {
        return x;
    } else {
        auto m   = static_cast< std::uint8_t >(shift % currentSize);
        auto LSR = x;
        LSR >>= m;
        auto result = LSR;
        auto LSL    = x;
        LSL <<= currentSize - m;
        result |= LSL;

        std::uint64_t return_value = 0;
        for (std::uint64_t i = 0; i < currentSize; i++) {
            return_value |= ((static_cast< std::uint64_t >(1) << i) & result);
        }

        return return_value;
    }
}

template < typename Type >
[[nodiscard]] std::int64_t RoundTowardsZero(Type x) const {
    if (x == static_cast< Type >(0.0L)) {
        return static_cast< std::int64_t >(0);
    }

    if (x >= static_cast< Type >(0.0L)) {
        return std::floor(x);
    } else {
        return std::ceil(x);
    }
}
