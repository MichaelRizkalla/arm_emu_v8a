/// <summary>
/// Return TRUE if Exception level 'el' is supported
/// </summary>
/// <param name="el"></param>
/// <returns></returns>
[[nodiscard]] bool HaveEL(std::bitset< 2 > el) {
    auto excepLevel = static_cast< IProcessingUnit::ExceptionLevel >(el.to_ulong());
    if (excepLevel == IProcessingUnit::ExceptionLevel::EL0 || excepLevel == IProcessingUnit::ExceptionLevel::EL1) {
        return true;
    }
    if (excepLevel == IProcessingUnit::ExceptionLevel::EL2) {
        return supportedExceptionLevels.find(IProcessingUnit::ExceptionLevel::EL2) != supportedExceptionLevels.end();
    }
    return supportedExceptionLevels.find(IProcessingUnit::ExceptionLevel::EL3) != supportedExceptionLevels.end();
}

void AArch64CheckSystemAccess(std::uint8_t op0, std::uint8_t op1, std::uint8_t crn, std::uint8_t crm, std::uint8_t op2,
                              std::uint8_t rt, bool read) {
    auto                            need_secure = false;
    IProcessingUnit::ExceptionLevel min_EL;
    switch (op1) {
        case 0:
        case 1:
            min_EL = IProcessingUnit::ExceptionLevel::EL1;
            break;
        case 2:
            min_EL = IProcessingUnit::ExceptionLevel::EL2;
            break;
        case 3:
            min_EL = IProcessingUnit::ExceptionLevel::EL0;
            break;
        case 4:
            min_EL = IProcessingUnit::ExceptionLevel::EL2;
            break;
        case 5:
            if (!HaveVirtHostExt())
                throw undefined_behaviour {};
            min_EL = IProcessingUnit::ExceptionLevel::EL2;
            break;
        case 6:
            min_EL = IProcessingUnit::ExceptionLevel::EL3;
            break;
        case 7:
            min_EL      = IProcessingUnit::ExceptionLevel::EL1;
            need_secure = true;
            break;
        default:
            throw undefined_instruction {};
    }

    if (EL.to_ulong() < static_cast< std::underlying_type_t< IProcessingUnit::ExceptionLevel > >(min_EL)) {
        throw undefined_behaviour {};
    } else if (need_secure && !IsSecure()) {
        throw undefined_behaviour {};
    }
}

/// <summary>
/// Execute a system instruction with write (source operand).
/// </summary>
/// <param name="sys_registers"></param>
/// <param name="op0"></param>
/// <param name="op1"></param>
/// <param name="crn"></param>
/// <param name="crm"></param>
/// <param name="op2"></param>
/// <param name="val"></param>
/// <returns></returns>
void SysInstr(std::uint8_t op0, std::uint8_t op1, std::uint8_t crn, std::uint8_t crm, std::uint8_t op2,
              std::bitset< 64 > val) {
    throw not_implemented_feature {};
    return;
}

/// <summary>
/// Read from a system register and return the contents of the register.
/// </summary>
/// <param name="sys_registers"></param>
/// <param name="op0"></param>
/// <param name="op1"></param>
/// <param name="crn"></param>
/// <param name="crm"></param>
/// <param name="op2"></param>
/// <returns></returns>
std::bitset< 64 > SysRegRead(std::uint8_t op0, std::uint8_t op1, std::uint8_t crn, std::uint8_t crm, std::uint8_t op2) {
    throw not_implemented_feature {};
    return std::bitset< 64 > {};
}

/// <summary>
/// Read from a system register and return the contents of the register.
/// </summary>
/// <param name="sys_registers"></param>
/// <param name="op0"></param>
/// <param name="op1"></param>
/// <param name="crn"></param>
/// <param name="crm"></param>
/// <param name="op2"></param>
/// <param name="val"></param>
/// <returns></returns>
void SysRegWrite(std::uint8_t op0, std::uint8_t op1, std::uint8_t crn, std::uint8_t crm, std::uint8_t op2,
                 std::bitset< 64 > val) {
    throw not_implemented_feature {};
    return;
}
