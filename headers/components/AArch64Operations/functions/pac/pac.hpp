#ifndef AARCH64_OPERATIONS_FUNCTIONS_PAC_PAC_HPP
#define AARCH64_OPERATIONS_FUNCTIONS_PAC_PAC_HPP

#include <SharedOperations/functions/system.hpp>

namespace arm_emu {

    [[nodiscard]] inline static auto HavePACExt() noexcept {
        return HasArchVersion(ArchVersion::ARMv8p3);
    }

} // namespace arm_emu

#endif // AARCH64_OPERATIONS_FUNCTIONS_PAC_PAC_HPP
