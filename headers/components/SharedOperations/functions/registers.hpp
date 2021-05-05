#ifndef SHARED_OPERATIONS_FUNCTIONS_REGISTERS_HPP
#define SHARED_OPERATIONS_FUNCTIONS_REGISTERS_HPP

#include <AArch64Operations/functions/memory.hpp>
#include <cstdint>

namespace arm_emu {

    enum class BranchType : std::uint32_t
    {
        DIRCALL,   // Direct Branch with link
        INDCALL,   // Indirect Branch with link
        ERET,      // Exception return (indirect)
        DBGEXIT,   // Exit from Debug state
        RET,       // Indirect branch with function return hint
        DIR,       // Direct branch
        INDIR,     // Indirect branch
        EXCEPTION, // Exception entry
        RESET,     // Reset
        UNKNOWN,   // Other
    };

    /// <summary>
    /// Return address of the current instruction.
    /// </summary>
    /// <param name="PC"></param>
    /// <returns></returns>
    [[nodiscard]] inline static auto ThisInstrAddr(const std::uint64_t& PC) noexcept {
        /* assert N == 64 || (N == 32 && UsingAArch32()); */ // always true
        return PC;
    }

    /// <summary>
    /// Report the hint passed to BranchTo() and BranchToAddr(), for consideration when processing
    /// the next instruction.
    /// </summary>
    /// <param name="PC"></param>
    /// <param name="branch_type"></param>
    /// <returns></returns>
    inline static void Hint_Branch(std::uint64_t& PC, BranchType branch_type) noexcept {
        if (branch_type == BranchType::RET)
            PC = std::numeric_limits< std::uint64_t >::max();
    }

    /// <summary>
    /// Set program counter to a new address, with a branch type
    /// In AArch64 state the address might include a tag in the top eight bits.
    /// </summary>
    /// <param name="sp"></param>
    /// <param name="PC"></param>
    /// <param name="target"></param>
    /// <param name="branch_type"></param>
    /// <returns></returns>
    inline static void BranchTo(SpecialRegisters* sp, std::uint64_t& PC, const std::uint64_t& target, BranchType branch_type) noexcept {
        Hint_Branch(PC, branch_type);
        /*if (N == 32)
            if(UsingAArch32());
            PC = ZeroExtend(target);
        else
            assert N == 64 && !UsingAArch32();*/
        // Always using AArch64 now
        if (branch_type != BranchType::RET)
            PC = BranchAddr(sp, target).to_ullong();
        return;
    }

} // namespace arm_emu

#endif // SHARED_OPERATIONS_FUNCTIONS_REGISTERS_HPP
