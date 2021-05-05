#ifndef AARCH64_OPERATIONS_FUNCTIONS_SYSTEM_HPP
#define AARCH64_OPERATIONS_FUNCTIONS_SYSTEM_HPP

namespace arm_emu {
	
    /// <summary>
    /// Execute a system instruction with write (source operand).
    /// </summary>
    /// <param name="op0"></param>
    /// <param name="op1"></param>
    /// <param name="crn"></param>
    /// <param name="crm"></param>
    /// <param name="op2"></param>
    /// <param name="val"></param>
    /// <returns></returns>
    inline static auto SysInstr(std::uint8_t op0, std::uint8_t op1, std::uint8_t crn, std::uint8_t crm, std::uint8_t op2, std::bitset< 64 > val) noexcept {
        // TO BE Implemented, check P.807
        return;
    }

}

#endif // AARCH64_OPERATIONS_FUNCTIONS_SYSTEM_HPP