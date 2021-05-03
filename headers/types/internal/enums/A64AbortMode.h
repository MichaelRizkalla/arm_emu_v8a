#ifndef A64_ABORT_MODE_H
#define A64_ABORT_MODE_H

#include <cstdint>

namespace arm_emu {

    enum class AbortMode : std::uint32_t {
        USER       = 0b10000,
        FIQ        = 0b10001,
        IRQ        = 0b10010,
        SUPERVISOR = 0b10011,
        ABORT      = 0b10111,
        UNDEFINED  = 0b11011,
        SYSTEM     = 0b11111,

        RESERVED, // Illegal return event
    };

    enum class A64SelectedStackPointer : std::uint32_t {
        EL0t = 0b00000,
        EL1t = 0b00100,
        EL1h = 0b00101,

        EL2t = 0b01000,
        EL2h = 0b01001,

        EL3t = 0b01000,
        EL3h = 0b01101,
    };

} // namespace arm_emu

#endif // A64_ABORT_MODE_H
