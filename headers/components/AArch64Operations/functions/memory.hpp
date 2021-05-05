#ifndef AARCH64_OPERATIONS_FUNCTIONS_MEMORY_HPP
#define AARCH64_OPERATIONS_FUNCTIONS_MEMORY_HPP

#include <SharedOperations/functions/memory.hpp>
#include <SharedOperations/functions/system.hpp>

namespace arm_emu {

    /// <summary>
    /// // Return the virtual address with tag bits removed for storing to the program counter.
    /// </summary>
    /// <param name="sp"></param>
    /// <param name="target"></param>
    /// <returns></returns>
    [[nodiscard]] inline static auto BranchAddr(SpecialRegisters* sp, const std::bitset< 64 >& target) noexcept {
        auto msbit = AddrTop(target, true, sp->CurrentEL.EL());

        if (msbit == 63)
            return target;

        std::bitset< 64 > vaddress { target };
        if ((sp->CurrentEL.EL() == EL0 || sp->CurrentEL.EL() == EL1 || IsInHost(sp)) && target[msbit] == true) {
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

} // namespace arm_emu

#endif // AARCH64_OPERATIONS_FUNCTIONS_MEMORY_HPP