#ifndef SHARED_OPERATIONS_FUNCTIONS_SYSTEM_HPP
#define SHARED_OPERATIONS_FUNCTIONS_SYSTEM_HPP

#include <A64SystemSettings.h>
#include <SharedOperations/functions/extension.hpp>
#include <bitset>
#include <cassert>
#include <cstdint>

namespace arm_emu {

    static std::bitset< 2 > EL3 = 0b11;
    static std::bitset< 2 > EL2 = 0b10;
    static std::bitset< 2 > EL1 = 0b01;
    static std::bitset< 2 > EL0 = 0b00;

    [[noreturn]] void Unreachable() noexcept {
        assert(false);
        abort();
    }

    /// <summary>
    /// Return TRUE if Exception level 'el' is supported
    /// </summary>
    /// <param name="el"></param>
    /// <returns></returns>
    [[nodiscard]] inline static auto HaveEL(std::bitset< 2 > el) {
        if (el == EL0 || el == EL1)
            return true;
        if (el == EL2)
            throw not_implemented_feature {};
        return SystemSettings::IsImplemented< Features, Features::EL3 >();
    }

    /// <summary>
    /// Return 'true' iff COND cuurently holds
    /// </summary>
    /// <param name="cond"></param>
    /// <param name="nzcv"></param>
    /// <returns>bool</returns>
    [[nodiscard]] inline static bool ConditionHolds(std::bitset< 4 >&& cond, NZCVRegister* nzcv) noexcept {
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

    /// <summary>
    /// </summary>
    /// <param name="el"></param>
    /// <returns></returns>
    [[nodiscard]] inline static auto ELIsInHost(std::bitset< 2 > el) noexcept {
        (void)el;
        return false;
        // Always false as HaveVirtHostExt() = false;
        /* ((IsSecureEL2Enabled() || !IsSecureBelowEL3()) && HaveVirtHostExt() && !ELUsingAArch32(EL2) && HCR_EL2.E2H == '1' &&
                (el == EL2 || (el == EL0 && HCR_EL2.TGE == '1')));*/
    }

    /// <summary>
    /// </summary>
    /// <param name="sp"></param>
    /// <returns></returns>
    [[nodiscard]] inline static auto IsInHost(const SpecialRegisters* const sp) noexcept {
        return ELIsInHost(sp->CurrentEL.EL());
    }

    /// <summary>
    /// Return TRUE if an Exception level below EL3 is in Secure state
    /// or would be following an exception return to that level.
    ///
    /// Differs from IsSecure in that it ignores the current EL or Mode
    /// in considering security state.
    /// That is, if at AArch64 EL3 or in AArch32 Monitor mode, whether an
    /// exception return would pass to Secure or Non-secure state.
    /// </summary>
    /// <param name="sp"></param>
    /// <returns></returns>
    [[nodiscard]] inline static auto IsSecuredBelowEL3(const SpecialRegisters* const sp) {
        if (HaveEL(EL3))
            // return SCR_GEN[].NS == '0';
            throw not_implemented_feature {};
        else if (HaveEL(EL2) && !HaveSecureEL2Ext())
            return false;
        else
            return true;
    }

    /// <summary>
    /// Returns TRUE if current Exception level is in Secure state.
    /// </summary>
    /// <param name="sp"></param>
    /// <returns></returns>
    [[nodiscard]] inline static auto IsSecure(const SpecialRegisters* const sp) {
        if (HaveEL(EL3) && /* !UsingAArch32() && */ sp->CurrentEL.EL() == EL3)
            return true;
        /*else if (SystemSettings::IsImplemented<Features, Features::EL3>() && UsingAArch32() && PSTATE.M == M32_Monitor)
            return true; */
        return IsSecuredBelowEL3(sp);
    }

} // namespace arm_emu

#endif // SHARED_OPERATIONS_FUNCTIONS_SYSTEM_HPP
