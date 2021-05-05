#ifndef SHARED_OPERATIONS_TRANSLATION_TRANSLATION_HPP
#define SHARED_OPERATIONS_TRANSLATION_TRANSLATION_HPP

#include <SharedOperations/functions/extension.hpp>
#include <SharedOperations/functions/system.hpp>
#include <bitset>

namespace arm_emu {

    [[nodiscard]] inline static auto S1TranslationRegime(std::bitset< 2 > el) noexcept {
        if (el != EL0)
            return el;
        // Always false ... not using AArch32
        /*else if HaveEL(EL3) && ELUsingAArch32(EL3) && SCR.NS == '0' then
            return ExceptionLevel::EL3;*/
        else if (HaveVirtHostExt() && ELIsInHost(el))
            return EL2;
        else
            return EL1;
    }

} // namespace arm_emu

#endif // SHARED_OPERATIONS_TRANSLATION_TRANSLATION_HPP