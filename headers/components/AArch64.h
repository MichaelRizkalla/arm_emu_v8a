#pragma once
#include <algorithm>
#include <cmath>

#include "AArch64Operations/AArch64Ops.hpp"
#include "SharedOperations/SharedOps.hpp"
#include <A64Enums.h>

namespace arm_emu {

    struct AArch64 {

        // Will be used when implementing FEAT_MTE
        /*static std::bitset<4> AllocationTagFromAddress(std::bitset<64> tagged_address) {
            std::bitset<4> return_value;
            return_value[0] = tagged_address[56];
            return_value[1] = tagged_address[57];
            return_value[2] = tagged_address[58];
            return_value[3] = tagged_address[59];
            return return_value;
        }*/

        [[nodiscard]] inline static auto ELIsInHost(std::bitset< 2 > el) noexcept {
            (void)el;
            return false;
            // Always false as HaveVirtHostExt() = false;
            /* ((IsSecureEL2Enabled() || !IsSecureBelowEL3()) && HaveVirtHostExt() && !ELUsingAArch32(EL2) && HCR_EL2.E2H == '1' &&
                    (el == EL2 || (el == EL0 && HCR_EL2.TGE == '1')));*/
        }

        [[nodiscard]] inline static auto IsInHost(const SpecialRegisters* const sp) noexcept {
            return ELIsInHost(sp->CurrentEL.EL());
        }

        [[nodiscard]] inline static auto S1TranslationRegime(std::bitset< 2 > el) noexcept {
            if (el != EL0)
                return el;
            // Always false ... not using AArch32
            /*else if HaveEL(EL3) && ELUsingAArch32(EL3) && SCR.NS == '0' then
                return ExceptionLevel::EL3;*/
            else if (HaveVirtHostExt() && ELIsInHost(el))
                return EL2;
            else
                return EL1;
        }

        [[nodiscard]] inline static auto EffectiveTBI(std::bitset< 64 > address, bool IsInstr, std::bitset< 2 > el) noexcept {
            (void)el;
            (void)IsInstr;
            (void)address;
            /*auto regime = S1TranslationRegime(el);

            bool tbi, tbid;*/
            /*switch (regime) {
                case ExceptionLevel::EL1: {
                    tbi = address[55] == true ? TCR_EL1.TBI1() : TCR_EL1.TBI0();
                    if (HavePACExt()) tbid = address[55] == true ? TCR_EL1.TBID1() : TCR_EL1.TBID0();
                } break;
                case ExceptionLevel::EL2: {
                    if (HaveVirtHostExt() && ELIsInHost(el)) tbi = address[55] == true ? TCR_EL2.TBI1() : TCR_EL2.TBI0();
                    if (HavePACExt())
                        tbid = address[55] == true ? TCR_EL2.TBID1() : TCR_EL2.TBID0();
                    else
                        tbi = TCR_EL2.TBI();
                    if (HavePACExt()) tbid = TCR_EL2.TBID();
                } break;
                case ExceptionLevel::EL3: {
                    tbi = TCR_EL3.TBI();
                    if (HavePACExt()) tbid = TCR_EL3.TBID();
                } break;
            }*/
            /*return (tbi == true && (!HavePACExt() || tbid == '0' || !IsInstr) ? true : false); */
            return false;
        }

        [[nodiscard]] inline static auto AddrTop(const std::bitset< 64 >& address, bool IsInstr, std::bitset< 2 > el) noexcept {
            /*auto regime = S1TranslationRegime(el);*/
            /*if ELUsingAArch32(regime) then
                // AArch32 translation regime.
                return 31;
            else*/
            // Always AArch64
            if (EffectiveTBI(address, IsInstr, el) == true) {
                return 55;
            } else {
                return 63;
            }
        }

        [[nodiscard]] inline static auto BranchAddr(SpecialRegisters* sp, const std::uint64_t& target) noexcept {
            std::bitset< 64 > vaddress { target };

            auto msbit = AddrTop(vaddress, true, sp->CurrentEL.EL());

            if (msbit == 63)
                return vaddress;
            else if ((sp->CurrentEL.EL() == EL0 || sp->CurrentEL.EL() == EL1 || IsInHost(sp)) && vaddress[msbit] == true) {
                bool value = vaddress[msbit];
                for (auto i = msbit + 1; i < 64; i++) {
                    vaddress[i] = value;
                }
                return vaddress;
            } else {
                for (auto i = msbit + 1; i < 64; i++) {
                    vaddress[i] = false;
                }
                return vaddress;
            }
        }

        [[nodiscard]] inline static auto IsSecuredBelowEL3(const SpecialRegisters* const sp) {
            if (HaveEL(EL3))
                // return SCR_GEN[].NS == '0';
                throw not_implemented_feature {};
            else if (HaveEL(EL2) && !HaveSecureEL2Ext())
                return false;
            else
                return true;
        }

        [[nodiscard]] inline static auto IsSecure(const SpecialRegisters* const sp) {
            if (HaveEL(EL3) && /* !UsingAArch32() && */ sp->CurrentEL.EL() == EL3)
                return true;
            /*else if (SystemSettings::IsImplemented<Features, Features::EL3>() && UsingAArch32() && PSTATE.M == M32_Monitor)
                return true; */
            return IsSecuredBelowEL3(sp);
        }

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

        inline static auto SysInstr(std::uint8_t op0, std::uint8_t op1, std::uint8_t crn, std::uint8_t crm, std::uint8_t op2, std::bitset< 64 > val) noexcept {
            // TO BE Implemented, check P.807
            return;
        }

        AArch64()               = delete;
        AArch64(const AArch64&) = delete;
        AArch64(AArch64&&)      = delete;
        AArch64& operator=(const AArch64&) = delete;
        AArch64& operator=(AArch64&&) = delete;
        ~AArch64()                    = delete;
    };

} // namespace arm_emu