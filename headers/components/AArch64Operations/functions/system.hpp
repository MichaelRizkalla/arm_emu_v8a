#ifndef AARCH64_OPERATIONS_FUNCTIONS_SYSTEM_HPP
#define AARCH64_OPERATIONS_FUNCTIONS_SYSTEM_HPP

#include <cstdint>

namespace arm_emu {
	
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
    inline static auto SysInstr(SystemRegisters* sys_registers, std::uint8_t op0, std::uint8_t op1, std::uint8_t crn, std::uint8_t crm, std::uint8_t op2, std::bitset< 64 > val) {
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
    inline static auto SysRegRead(SystemRegisters* sys_registers, std::uint8_t op0, std::uint8_t op1, std::uint8_t crn, std::uint8_t crm, std::uint8_t op2) {
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
    inline static auto SysRegWrite(SystemRegisters* sys_registers, std::uint8_t op0, std::uint8_t op1, std::uint8_t crn, std::uint8_t crm, std::uint8_t op2,
                                  std::bitset< 64 > val) {
        throw not_implemented_feature {};
        return;
    }

}

#endif // AARCH64_OPERATIONS_FUNCTIONS_SYSTEM_HPP