#ifndef SHARED_OPERATIONS_FUNCTIONS_COMMON_HPP
#define SHARED_OPERATIONS_FUNCTIONS_COMMON_HPP

#include <cstdint>
#include <utilities.hpp>

namespace arm_emu {

    template < std::size_t N, std::size_t M >
    [[nodiscard]] inline static auto Replicate(std::bitset< M > x) noexcept {
        static_assert(N % M == 0);
        std::bitset< N * M > return_value;
        std::bitset< N * M > value(0);
        if (N * M > 0)
            value = x.to_ullong();
        if (N / M > 0) {
            for (std::size_t n = 0; n < static_cast< std::size_t >(N / M); n++) {
                return_value |= value;
                value <<= M;
            }
        }
        return return_value;
    }

    [[nodiscard]] inline static auto Replicate(std::uint64_t x, std::size_t currentSize, std::size_t targetSize) {
        if (targetSize % currentSize != 0)
            throw undefined_behaviour {};
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
    [[nodiscard]] inline static auto SignExtend(std::bitset< M > x) noexcept {
        static_assert(N >= M);
        auto             val          = Replicate< N - M >(std::bitset< 1 > { x[M - 1] });
        std::bitset< N > return_value = concate< N - M, M >(val, x);
        return return_value;
    }

    template < std::size_t N, std::size_t M >
    [[nodiscard]] inline static auto ZeroExtend(std::bitset< M > x) noexcept {
        static_assert(N >= M);
        return std::bitset< N >(x.to_ullong());
    }

    template < std::size_t N, std::size_t M >
    [[nodiscard]] inline static std::bitset< N > Extend(std::bitset< M > x, bool unsigned_) noexcept {
        if (unsigned_)
            return ZeroExtend< N, M >(x);
        else
            return SignExtend< N, M >(x);
    }

    template < std::size_t N >
    [[nodiscard]] inline static auto LSL_C(std::bitset< N > x, std::uint8_t shift) {
        if (shift < 0)
            throw undefined_behaviour {};
        std::bitset< N + 1 > extended_x { x.to_ullong() };
        extended_x <<= shift;
        std::bitset< N > result { extended_x.to_ullong() };
        auto             carry_out = extended_x[N];
        return std::make_pair(result, carry_out);
    }

    template < std::size_t N >
    [[nodiscard]] inline static auto LSL(std::bitset< N > x, std::uint8_t shift) {
        if (shift < 0)
            throw undefined_behaviour {};
        if (shift == 0)
            return x;
        else {
            auto [result, _] = LSL_C< N >(x, shift);
            return result;
        }
    }

    template < std::size_t N, std::uint8_t MaxShift >
    [[nodiscard]] inline static auto LSR_C(std::bitset< N > x, std::uint8_t shift) {
        if (shift < 0)
            throw undefined_behaviour {};
        auto extended_x = ZeroExtend< N + MaxShift >(x);
        auto carry_out  = extended_x[shift - 1];
        extended_x >>= shift;
        std::bitset< N > result { extended_x.to_ullong() };
        return std::make_pair(result, carry_out);
    }

    template < std::size_t N, std::uint8_t MaxShift >
    [[nodiscard]] inline static auto LSR(std::bitset< N > x, std::uint8_t shift) {
        if (shift < 0)
            throw undefined_behaviour {};
        if (shift == 0)
            return x;
        else {
            auto [result, _] = LSR_C< N, MaxShift >(x, shift);
            return result;
        }
    }

    template < std::size_t N, std::uint8_t MaxShift >
    [[nodiscard]] inline static auto ASR_C(std::bitset< N > x, std::uint8_t shift) {
        if (shift < 0)
            throw undefined_behaviour {};
        auto extended_x = SignExtend< N + MaxShift >(x);
        auto carry_out  = extended_x[shift - 1];
        extended_x >>= shift;
        std::bitset< N > result { extended_x.to_ullong() };
        return std::make_pair(result, carry_out);
    }

    template < std::size_t N, std::uint8_t MaxShift >
    [[nodiscard]] inline static auto ASR(std::bitset< N > x, std::uint8_t shift) {
        if (shift < 0)
            throw undefined_behaviour {};
        if (shift == 0)
            return x;
        else {
            auto [result, _] = ASR_C< N, MaxShift >(x, shift);
            return result;
        }
    }

    template < std::size_t N, std::uint8_t MaxShift >
    [[nodiscard]] inline static auto ROR_C(std::bitset< N > x, std::uint8_t shift) {
        if (shift < 0)
            throw undefined_behaviour {};

        auto m      = static_cast< std::uint8_t >(shift % N);
        auto result = LSR< N, MaxShift >(x, m);
        result |= LSL< N >(x, N - m);

        auto carry_out = result[N - 1];
        return std::make_pair(result, carry_out);
    }

    template < std::size_t N, std::uint8_t MaxShift >
    [[nodiscard]] inline static auto ROR(std::bitset< N > x, std::uint8_t shift) {
        if (shift < 0)
            throw undefined_behaviour {};
        if (shift == 0)
            return x;
        else {
            auto [result, _] = ROR_C< N, MaxShift >(x, shift);
            return result;
        }
    }

    [[nodiscard]] inline static auto ROR(std::uint64_t x, std::uint64_t currentSize, std::uint8_t shift) {
        if (shift < 0)
            throw undefined_behaviour {};
        if (shift == 0)
            return x;
        else {
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

    template < std::size_t N >
    [[nodiscard]] inline static std::int64_t HighestSetBit(std::bitset< N > x) noexcept {
        for (std::int64_t i = N - 1; i > -1; i--) {
            if (x[i] == true)
                return i;
        }
        return -1;
    }

    [[nodiscard]] inline static std::int64_t HighestSetBit(std::uint64_t x) {
        for (std::int64_t i = 63; i > -1; i--) {
            if (static_cast< bool >(x & (static_cast< std::uint64_t >(1) << i)) == true)
                return i;
        }
        throw undefined_behaviour {};
    }

    template < typename Type >
    [[nodiscard]] inline static std::int64_t RoundTowardsZero(Type x) {
        if (x == static_cast< Type >(0.0L)) {
            return static_cast< std::int64_t >(0);
        }

        if (x >= static_cast< Type >(0.0L)) {
            return std::floor(x);
        } else {
            return std::ceil(x);
        }
    }

} // namespace arm_emu

#endif // SHARED_OPERATIONS_FUNCTIONS_COMMON_HPP
