#pragma once
#include <algorithm>
#include <cmath>

#include "AArch64Operations/AArch64Ops.hpp"
#include "SharedOperations/SharedOps.hpp"
#include <A64Enums.h>

namespace arm_emu {

    struct AArch64 {

        inline static void CheckSystemAccess(const SpecialRegisters* const sp, std::uint8_t op0, std::uint8_t op1, std::uint8_t crn, std::uint8_t crm, std::uint8_t op2,
                                             std::uint8_t rt, bool read) {
            auto             need_secure = false;
            std::bitset< 2 > min_EL { 0b00 };
            switch (op1) {
                case 0:
                case 1:
                    min_EL = EL1;
                    break;
                case 2:
                    min_EL = EL2;
                    break;
                case 3:
                    min_EL = EL0;
                    break;
                case 4:
                    min_EL = EL2;
                    break;
                case 5:
                    if (!HaveVirtHostExt())
                        throw undefined_behaviour {};
                    min_EL = EL2;
                    break;
                case 6:
                    min_EL = EL3;
                    break;
                case 7:
                    min_EL      = EL1;
                    need_secure = true;
                    break;
                default:
                    throw undefined_instruction {};
            }

            if (sp->CurrentEL.EL().to_ulong() < min_EL.to_ulong())
                throw undefined_behaviour {};
            else if (need_secure && !IsSecure(sp))
                throw undefined_behaviour {};
        }

        AArch64()               = delete;
        AArch64(const AArch64&) = delete;
        AArch64(AArch64&&)      = delete;
        AArch64& operator=(const AArch64&) = delete;
        AArch64& operator=(AArch64&&) = delete;
        ~AArch64()                    = delete;
    };

} // namespace arm_emu