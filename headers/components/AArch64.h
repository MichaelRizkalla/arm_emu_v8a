#pragma once
#include <algorithm>
#include <cmath>

#include <A64Enums.h>

namespace arm_emu {

struct AArch64 {

    inline static constexpr auto ARMv8p0 = 0ull;
    inline static constexpr auto ARMv8p1 = 1ull;
    inline static constexpr auto ARMv8p2 = 2ull;
    inline static constexpr auto ARMv8p3 = 3ull;
    inline static constexpr auto ARMv8p4 = 4ull;
    inline static constexpr auto ARMv8p5 = 5ull;
    inline static constexpr auto ARMv8p6 = 6ull;

    // Will be used when implementing FEAT_MTE
    /*static constexpr std::bitset<4> AllocationTagFromAddress(std::bitset<64> tagged_address) {
        std::bitset<4> return_value;
        return_value[0] = tagged_address[56];
        return_value[1] = tagged_address[57];
        return_value[2] = tagged_address[58];
        return_value[3] = tagged_address[59];
        return return_value;
    }*/

    [[nodiscard]] inline static constexpr auto ExceptionSyndrome(Exception exceptype) {
        ExceptionRecord r;

        r.exceptype = exceptype;

        r.syndrome  = std::uint32_t { 0 };
        r.vaddress  = std::uint64_t { 0 };
        r.ipavalid  = false;
        r.NS        = false;
        r.ipaddress = std::uint64_t { 0 };

        return r;
    }

    inline static constexpr auto HasArchVersion(std::size_t ver) { return ver == ARMv8p0; }

    [[nodiscard]] inline static constexpr bool HaveVirtHostExt() { return HasArchVersion(ARMv8p1); }

    [[nodiscard]] inline static constexpr bool HaveSecureEL2Ext() { return HasArchVersion(ARMv8p4); }

    [[nodiscard]] inline static constexpr bool HavePACExt() { return HasArchVersion(ARMv8p3); }

    [[nodiscard]] inline static constexpr bool HaveBTIExt() { return HasArchVersion(ARMv8p5); }

    [[nodiscard]] inline static constexpr bool HaveFlagManipulateExt() { return HasArchVersion(ARMv8p4); }

    [[nodiscard]] inline static constexpr auto HaveEL(ExceptionLevel el) {
        if (el == ExceptionLevel::EL0 || el == ExceptionLevel::EL1) return true;
        if (el == ExceptionLevel::EL2) throw not_implemented_feature {};
        return SystemSettings::IsImplemented< Features, Features::EL3 >();
    }

    [[nodiscard]] inline static constexpr bool ELIsInHost(ExceptionLevel el) {
        (void)el;
        return false;
        // Always false as HaveVirtHostExt() = false;
        /* ((IsSecureEL2Enabled() || !IsSecureBelowEL3()) && HaveVirtHostExt() && !ELUsingAArch32(EL2) && HCR_EL2.E2H == '1' &&
                (el == EL2 || (el == EL0 && HCR_EL2.TGE == '1')));*/
    }

    [[nodiscard]] inline static constexpr bool IsInHost(const SpecialRegisters* const sp) { return ELIsInHost(sp->CurrentEL.EL()); }

    [[nodiscard]] inline static constexpr auto ThisInstrAddr(const std::uint64_t& PC) {
        /* assert N == 64 || (N == 32 && UsingAArch32()); */ // always true
        return PC;
    }

    inline static constexpr void TakeException(SpecialRegisters* sp, ExceptionLevel target_el, ExceptionRecord exception, std::uint64_t preferred_exception_return,
                                               std::uint32_t vect_offset) {
        if (HaveEL(target_el) && (target_el > sp->CurrentEL.EL())) throw undefined_behaviour {};

        // Take Exception ...
        // TODO
    }

    inline static constexpr void SoftwareBreakpoint(SpecialRegisters* sp, const std::uint64_t& PC, std::uint16_t immediate) {
        bool route_to_el2 = (sp->CurrentEL.EL() == ExceptionLevel::EL0 || sp->CurrentEL.EL() == ExceptionLevel::EL1) && false /*EL2Enabled()*/
                            && false /*(HCR_EL2.TGE == '1' || MDCR_EL2.TDE == '1')*/;

        std::uint64_t preferred_exception_return = ThisInstrAddr(PC);
        auto          vect_offset                = 0x0;

        auto exception     = ExceptionSyndrome(Exception::SoftwareBreakpoint);
        exception.syndrome = immediate;

        if (sp->CurrentEL.EL() > ExceptionLevel::EL1)
            TakeException(sp, sp->CurrentEL.EL(), exception, preferred_exception_return, vect_offset);
        else if (route_to_el2)
            TakeException(sp, ExceptionLevel::EL2, exception, preferred_exception_return, vect_offset);
        else
            TakeException(sp, ExceptionLevel::EL1, exception, preferred_exception_return, vect_offset);
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

    [[nodiscard]] inline static constexpr auto IsSecuredBelowEL3(const SpecialRegisters* const sp) {
        if (HaveEL(ExceptionLevel::EL3))
            // return SCR_GEN[].NS == '0';
            throw not_implemented_feature {};
        else if (HaveEL(ExceptionLevel::EL2) && !HaveSecureEL2Ext())
            return false;
        else
            return true;
    }

    [[nodiscard]] inline static constexpr auto IsSecure(const SpecialRegisters* const sp) {
        if (HaveEL(ExceptionLevel::EL3) && /* !UsingAArch32() && */ sp->CurrentEL.EL() == ExceptionLevel::EL3) return true;
        /*else if (SystemSettings::IsImplemented<Features, Features::EL3>() && UsingAArch32() && PSTATE.M == M32_Monitor)
            return true; */
        return IsSecuredBelowEL3(sp);
    }

    inline static constexpr void CheckSystemAccess(const SpecialRegisters* const sp, std::uint8_t op0, std::uint8_t op1, std::uint8_t crn, std::uint8_t crm,
                                                   std::uint8_t op2, std::uint8_t rt, bool read) {
        auto           need_secure = false;
        ExceptionLevel min_EL { ExceptionLevel::Undefined };
        switch (op1) {
            case 0:
            case 1:
                min_EL = ExceptionLevel::EL1;
                break;
            case 2:
                min_EL = ExceptionLevel::EL2;
                break;
            case 3:
                min_EL = ExceptionLevel::EL0;
                break;
            case 4:
                min_EL = ExceptionLevel::EL2;
                break;
            case 5:
                if (!HaveVirtHostExt()) throw undefined_behaviour {};
                min_EL = ExceptionLevel::EL2;
                break;
            case 6:
                min_EL = ExceptionLevel::EL3;
                break;
            case 7:
                min_EL      = ExceptionLevel::EL1;
                need_secure = true;
                break;
            default:
                throw undefined_instruction {};
        }

        if (static_cast< std::underlying_type_t< ExceptionLevel > >(sp->CurrentEL.EL()) < static_cast< std::underlying_type_t< ExceptionLevel > >(min_EL))
            throw undefined_behaviour {};
        else if (need_secure && !IsSecure(sp))
            throw undefined_behaviour {};
    }

    inline static constexpr auto SysInstr(std::uint8_t op0, std::uint8_t op1, std::uint8_t crn, std::uint8_t crm, std::uint8_t op2, std::bitset< 64 > val) {
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