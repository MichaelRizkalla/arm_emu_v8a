#pragma once
#include <algorithm>
#include <cmath>

#include <A64Enums.h>
#include <AArch64.h>
#include "AArch64Operations/AArch64Ops.hpp"
#include "SharedOperations/SharedOps.hpp"

namespace arm_emu {

    struct ALU {

        template < std::size_t TWidth >
        [[nodiscard]] inline static auto SP(SpecialRegisters* sp, std::bitset< TWidth > value) {
            if (sp->SPSel.SP() == 0) {
                sp->SP_EL0.Set(value.to_ullong());
            } else {
                auto EL = sp->CurrentEL.EL();
                if (EL == EL0) {
                    sp->SP_EL0.Set(value.to_ullong());
                } else if (EL == EL1) {
                    sp->SP_EL1.Set(value.to_ullong());
                } else if (EL == EL2) {
                    sp->SP_EL2.Set(value.to_ullong());
                } else if (EL == EL3) {
                    sp->SP_EL3.Set(value.to_ullong());
                } else {
                    throw undefined_behaviour {};
                }
            }
        }

        template < std::size_t TWidth >
        [[nodiscard]] inline static std::bitset< TWidth > SP(SpecialRegisters* sp) {
            static_assert((TWidth == 8) || (TWidth == 16) || (TWidth == 32) || (TWidth == 64));
            if (sp->SPSel.SP() == 0) {
                return sp->SP_EL0.Value();
            } else {
                auto EL = sp->CurrentEL.EL();
                if (EL == EL0) {
                    return sp->SP_EL0.Value();
                } else if (EL == EL1) {
                    return sp->SP_EL1.Value();
                } else if (EL == EL2) {
                    return sp->SP_EL2.Value();
                } else if (EL == EL3) {
                    return sp->SP_EL3.Value();
                } else {
                    throw undefined_behaviour {};
                }
            }
        }

        template < typename Ty >
        [[nodiscard]] inline static auto DecodeRegExtend(Ty option) noexcept {
            return static_cast< ExtendType >(option);
        }

        template < typename Ty >
        [[nodiscard]] inline static auto getOption(Ty in) noexcept {
            if (in == 0b111 || in == 0b011) {
                return WidthOption::X;
            }
            return WidthOption::W;
        }

        [[nodiscard]] inline static bool ConditionHold(std::bitset< 4 >&& cond, NZCVRegister* nzcv) noexcept {
            bool result = false;

            std::string cond_s = cond.to_string();
            std::string mCond  = cond_s.substr(0, 3);
            auto        mCond0 = cond_s[3];
            if (mCond == "000")
                result = (nzcv->Z() == true);
            else if (mCond == "001")
                result = (nzcv->C() == true);
            else if (mCond == "010")
                result = (nzcv->N() == true);
            else if (mCond == "011")
                result = (nzcv->V() == true);
            else if (mCond == "100")
                result = (nzcv->C() == true && nzcv->Z() == false);
            else if (mCond == "101")
                result = (nzcv->N() == nzcv->V());
            else if (mCond == "110")
                result = (nzcv->N() == nzcv->V() && nzcv->Z() == false);
            else if (mCond == "111")
                result = (nzcv->Z() == true);

            if (mCond0 == '1' && cond != 15) {
                return !result;
            }
            return result;
        }

        inline static void Hint_Branch(std::uint64_t& PC, BranchType branch_type) noexcept {
            if (branch_type == BranchType::RET)
                PC = std::numeric_limits< std::uint64_t >::max();
        }

        inline static void BranchTo(SpecialRegisters* sp, std::uint64_t& PC, const std::uint64_t& target, BranchType branch_type) noexcept {
            Hint_Branch(PC, branch_type);
            /*if (N == 32)
                if(UsingAArch32());
                PC = ZeroExtend(target);
            else
                assert N == 64 && !UsingAArch32();*/
            // Always using AArch64 now
            if (branch_type != BranchType::RET)
                PC = AArch64::BranchAddr(sp, target).to_ullong();
            return;
        }

        ALU()           = delete;
        ALU(const ALU&) = delete;
        ALU(ALU&&)      = delete;
        ALU& operator=(const ALU&) = delete;
        ALU& operator=(ALU&&) = delete;
        ~ALU()                = delete;
    };

} // namespace arm_emu