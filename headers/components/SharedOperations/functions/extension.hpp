#ifndef SHARED_OPERATIONS_FUNCTIONS_EXTENSION_HPP
#define SHARED_OPERATIONS_FUNCTIONS_EXTENSION_HPP

#include "system.hpp"

namespace arm_emu {

    [[nodiscard]] inline static auto HaveVirtHostExt() noexcept {
        return HasArchVersion(ArchVersion::ARMv8p1);
    }

    [[nodiscard]] inline static auto HaveSecureEL2Ext() noexcept {
        return HasArchVersion(ArchVersion::ARMv8p4);
    }

    [[nodiscard]] inline static auto HaveBTIExt() noexcept {
        return HasArchVersion(ArchVersion::ARMv8p5);
    }

    [[nodiscard]] inline static auto HaveFlagManipulateExt() noexcept {
        return HasArchVersion(ArchVersion::ARMv8p4);
    }

} // namespace arm_emu

#endif // SHARED_OPERATIONS_FUNCTIONS_EXTENSION_HPP