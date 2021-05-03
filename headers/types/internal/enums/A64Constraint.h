#ifndef A64_CONSTRAINT_H
#define A64_CONSTRAINT_H

#include <cstdint>

namespace arm_emu {

    enum class Constraint : std::uint32_t
    {
        NONE,
        UNKNOWN,
        UNDEF,
        NOP,
    };

} // namespace arm_emu

#endif // A64_CONSTRAINT_H
