#ifndef AARCH64_OPERATIONS_EXCEPTIONS_EXCEPTIONS_HPP
#define AARCH64_OPERATIONS_EXCEPTIONS_EXCEPTIONS_HPP

#include <SharedOperations/exceptions/exceptions.hpp>
#include <SharedOperations/functions/system.hpp>
#include <cstdint>

namespace arm_emu {

    inline static void TakeException(SpecialRegisters* sp, std::bitset< 2 > target_el, ExceptionRecord exception, std::uint64_t preferred_exception_return,
                                               std::uint32_t vect_offset) {
        if (HaveEL(target_el) && (target_el.to_ulong() > sp->CurrentEL.EL().to_ulong()))
            throw undefined_behaviour {};

        // Take Exception ...
        // TODO
    }

    inline static void SoftwareBreakpoint(SpecialRegisters* sp, const std::uint64_t& PC, std::uint16_t immediate) {
        bool route_to_el2 = (sp->CurrentEL.EL() == EL0 || sp->CurrentEL.EL() == EL1) && false /*EL2Enabled()*/
                            && false /*(HCR_EL2.TGE == '1' || MDCR_EL2.TDE == '1')*/;

        std::uint64_t preferred_exception_return = ThisInstrAddr(PC);
        auto          vect_offset                = 0x0;

        auto exception     = ExceptionSyndrome(Exception::SoftwareBreakpoint);
        exception.syndrome = immediate;

        if (sp->CurrentEL.EL().to_ulong() > EL1.to_ulong())
            TakeException(sp, sp->CurrentEL.EL(), exception, preferred_exception_return, vect_offset);
        else if (route_to_el2)
            TakeException(sp, EL2, exception, preferred_exception_return, vect_offset);
        else
            TakeException(sp, EL1, exception, preferred_exception_return, vect_offset);
    }

} // namespace arm_emu

#endif // AARCH64_OPERATIONS_EXCEPTIONS_EXCEPTIONS_HPP
