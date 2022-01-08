
/// <summary>
/// Return 'true' iff COND cuurently holds
/// </summary>
/// <param name="cond"></param>
/// <param name="nzcv"></param>
/// <returns>bool</returns>
[[nodiscard]] bool ConditionHolds(const Bitset& cond, const std::bitset< 4 >& nzcv) noexcept {
    assert(cond.Size() == 4);

    constexpr size_t N = 3;
    constexpr size_t Z = 2;
    constexpr size_t C = 1;
    constexpr size_t V = 0;

    bool result = false;

    std::string cond_s = cond.ToString();
    std::string mCond  = cond_s.substr(0, 3);
    auto        mCond0 = cond_s[3];
    if (mCond == "000") {
        result = (nzcv[Z] == true);
    } else if (mCond == "001") {
        result = (nzcv[C] == true);
    } else if (mCond == "010") {
        result = (nzcv[N] == true);
    } else if (mCond == "011") {
        result = (nzcv[V] == true);
    } else if (mCond == "100") {
        result = (nzcv[C] == true && nzcv[Z] == false);
    } else if (mCond == "101") {
        result = (nzcv[N] == nzcv[V]);
    } else if (mCond == "110") {
        result = (nzcv[N] == nzcv[V] && nzcv[Z] == false);
    } else if (mCond == "111") {
        result = (nzcv[Z] == true);
    }

    if (mCond0 == '1' && cond.ToULong() != 15) {
        return !result;
    }
    return result;
}

/// <summary>
/// </summary>
/// <param name="el"></param>
/// <returns></returns>
[[nodiscard]] bool ELIsInHost() const noexcept { // TODO
    return false;
    // Always false as HaveVirtHostExt() = false;
    /* ((IsSecureEL2Enabled() || !IsSecureBelowEL3()) && HaveVirtHostExt() && !ELUsingAArch32(EL2) && HCR_EL2.E2H == '1'
       && (el == EL2 || (el == EL0 && HCR_EL2.TGE == '1')));*/
}

/// <summary>
/// </summary>
/// <param name="sp"></param>
/// <returns></returns>
[[nodiscard]] bool IsInHost() const noexcept {
    return ELIsInHost();
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
[[nodiscard]] bool IsSecuredBelowEL3() {
    if (HaveEL(static_cast< std::underlying_type_t< IProcessingUnit::ExceptionLevel > >(
            IProcessingUnit::ExceptionLevel::EL3))) {
        // return SCR_GEN[].NS == '0';
        throw not_implemented_feature {};
    } else if (HaveEL(static_cast< std::underlying_type_t< IProcessingUnit::ExceptionLevel > >(
                   IProcessingUnit::ExceptionLevel::EL2)) &&
               !HaveSecureEL2Ext()) {
        return false;
    } else {
        return true;
    }
}

/// <summary>
/// Returns TRUE if current Exception level is in Secure state.
/// </summary>
/// <param name="sp"></param>
/// <returns></returns>
[[nodiscard]] bool IsSecure() {
    if (HaveEL(static_cast< std::underlying_type_t< IProcessingUnit::ExceptionLevel > >(
            IProcessingUnit::ExceptionLevel::EL3)) &&
        /* !UsingAArch32() && */ EL.to_ulong() ==
            static_cast< std::underlying_type_t< IProcessingUnit::ExceptionLevel > >(
                IProcessingUnit::ExceptionLevel::EL3))
        return true;
    /*else if (SystemSettings::IsImplemented<Features, Features::EL3>() && UsingAArch32() && PSTATE.M == M32_Monitor)
        return true; */
    return IsSecuredBelowEL3();
}

[[noreturn]] void Unreachable() noexcept {
    assert(false);
    abort();
}
