#ifndef A64_ROUNDING_MODE_H
#define A64_ROUNDING_MODE_H

#include <cstdint>

namespace arm_emu {

    enum class RoundingMode : std::uint32_t
    {
        ROUND_TO_NEAREST        = 0b00,
        ROUND_TO_PLUS_INFINITY  = 0b01,
        ROUND_TO_MINUS_INFINITY = 0b10,
        ROUND_TO_ZERO           = 0b11,
    };

} // namespace arm_emu

#endif // A64_ROUNDING_MODE_H
