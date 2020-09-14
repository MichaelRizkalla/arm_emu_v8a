#pragma once
#include <algorithm>
#include <cmath>

#include <A64Enums.h>

namespace arm_emu {

struct AArch64 {

    inline static constexpr auto ARMv8p0 = 0ull;
    inline static constexpr auto ARMv8p1 = 1ull;

    // Will be used when implementing FEAT_MTE
    /*static constexpr std::bitset<4> AllocationTagFromAddress(std::bitset<64> tagged_address) {
        std::bitset<4> return_value;
        return_value[0] = tagged_address[56];
        return_value[1] = tagged_address[57];
        return_value[2] = tagged_address[58];
        return_value[3] = tagged_address[59];
        return return_value;
    }*/

    inline static constexpr auto HasArchVersion(std::size_t ver) { return ver == ARMv8p0; }

    [[nodiscard]] inline static constexpr bool HaveVirtHostExt() { return HasArchVersion(ARMv8p1); }

    [[nodiscard]] inline static constexpr bool ELIsInHost(ExceptionLevel el) {
        (void)el;
        return false;
        // Always false as HaveVirtHostExt() = false;
        /* ((IsSecureEL2Enabled() || !IsSecureBelowEL3()) && HaveVirtHostExt() && !ELUsingAArch32(EL2) && HCR_EL2.E2H == '1' &&
                (el == EL2 || (el == EL0 && HCR_EL2.TGE == '1')));*/
    }

    [[nodiscard]] inline static constexpr bool IsInHost(SpecialRegisters* sp) {
        return ELIsInHost(sp->CurrentEL.EL());
    }

    [[nodiscard]] inline static constexpr auto S1TranslationRegime(ExceptionLevel el) {
        if (el != ExceptionLevel::EL0) return el;
        // Always false ... not using AArch32
        /*else if HaveEL(EL3) && ELUsingAArch32(EL3) && SCR.NS == '0' then
            return ExceptionLevel::EL3;*/
        else if (HaveVirtHostExt() && ELIsInHost(el))
            return ExceptionLevel::EL2;
        else
            return ExceptionLevel::EL1;
    }

    [[nodiscard]] inline static constexpr bool EffectiveTBI(std::bitset< 64 > address, bool IsInstr, ExceptionLevel el) {
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

    [[nodiscard]] inline static constexpr auto AddrTop(const std::bitset< 64 >& address, bool IsInstr, ExceptionLevel el) {
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

    [[nodiscard]] inline static constexpr auto BranchAddr(SpecialRegisters* sp, const std::uint64_t& target) {
        std::bitset< 64 > vaddress { target };

        auto msbit = AddrTop(vaddress, true, sp->CurrentEL.EL());

        if (msbit == 63)
            return vaddress;
        else if ((sp->CurrentEL.EL() == ExceptionLevel::EL0 || sp->CurrentEL.EL() == ExceptionLevel::EL1 || IsInHost(sp)) && vaddress[msbit] == true) {
            bool value = vaddress[msbit];
            for (auto i = msbit + 1; i < 64; i++) { vaddress[i] = value; }
            return vaddress;
        } else {
            for (auto i = msbit + 1; i < 64; i++) { vaddress[i] = false; }
            return vaddress;
        }
    }

    AArch64()               = delete;
    AArch64(const AArch64&) = delete;
    AArch64(AArch64&&)      = delete;
    AArch64& operator=(const AArch64&) = delete;
    AArch64& operator=(AArch64&&) = delete;
    ~AArch64()                    = delete;
};

} // namespace arm_emu