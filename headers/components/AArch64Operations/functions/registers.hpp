#ifndef AARCH64_OPERATIONS_FUNCTIONS_REGISTERS_HPP
#define AARCH64_OPERATIONS_FUNCTIONS_REGISTERS_HPP

namespace arm_emu {
    
    /// <summary>
    /// SP[] - assignment from
    /// Write to stack pointer from either a 32-bit or a 64-bit value
    /// </summary>
    /// <param name="sp"></param>
    /// <param name="value"></param>
    /// <returns></returns>
    template < std::size_t TWidth >
    [[nodiscard]] inline static auto SP(SpecialRegisters* sp, std::bitset< TWidth > value) {
        if (sp->SPSel.SP() == 0) {
            sp->SP_EL0.Set(value.to_ullong());
        } else {
            auto EL = sp->CurrentEL.EL();
            if (EL == EL0) {
                sp->SP_EL0.Set(value.to_ullong());
            } else if (EL == EL1) {
                sp->SP_EL1.Set(value.to_ullong());
            } else if (EL == EL2) {
                sp->SP_EL2.Set(value.to_ullong());
            } else if (EL == EL3) {
                sp->SP_EL3.Set(value.to_ullong());
            } else {
                throw undefined_behaviour {};
            }
        }
    }

    /// <summary>
    /// SP[] - non-assignment form
    /// Read stack pointer with implicit slice of 8, 16, 32 or 64 bits
    /// </summary>
    /// <param name="sp"></param>
    /// <returns></returns>
    template < std::size_t TWidth >
    [[nodiscard]] inline static std::bitset< TWidth > SP(SpecialRegisters* sp) {
        static_assert((TWidth == 8) || (TWidth == 16) || (TWidth == 32) || (TWidth == 64));
        if (sp->SPSel.SP() == 0) {
            return sp->SP_EL0.Value();
        } else {
            auto EL = sp->CurrentEL.EL();
            if (EL == EL0) {
                return sp->SP_EL0.Value();
            } else if (EL == EL1) {
                return sp->SP_EL1.Value();
            } else if (EL == EL2) {
                return sp->SP_EL2.Value();
            } else if (EL == EL3) {
                return sp->SP_EL3.Value();
            } else {
                throw undefined_behaviour {};
            }
        }
    }

} // namespace arm_emu

#endif // AARCH64_OPERATIONS_FUNCTIONS_REGISTERS_HPP