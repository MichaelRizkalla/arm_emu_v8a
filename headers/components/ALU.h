#pragma once
#include <algorithm>
#include <cmath>

#include <A64Enums.h>
#include <AArch64.h>
#include "AArch64Operations/AArch64Ops.hpp"
#include "SharedOperations/SharedOps.hpp"

namespace arm_emu {

    struct ALU {

        template < typename Ty >
        [[nodiscard]] inline static auto getOption(Ty in) noexcept {
            if (in == 0b111 || in == 0b011) {
                return WidthOption::X;
            }
            return WidthOption::W;
        }

        ALU()           = delete;
        ALU(const ALU&) = delete;
        ALU(ALU&&)      = delete;
        ALU& operator=(const ALU&) = delete;
        ALU& operator=(ALU&&) = delete;
        ~ALU()                = delete;
    };

} // namespace arm_emu