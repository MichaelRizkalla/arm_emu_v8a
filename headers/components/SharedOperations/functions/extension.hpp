#ifndef SHARED_OPERATIONS_FUNCTIONS_EXTENSION_HPP
#define SHARED_OPERATIONS_FUNCTIONS_EXTENSION_HPP

#include <cstdint>

namespace arm_emu {

    enum class ArchVersion : std::uint32_t
    {
        ARMv8p0 = 0,
        ARMv8p1 = 1,
        ARMv8p2 = 2,
        ARMv8p3 = 3,
        ARMv8p4 = 4,
        ARMv8p5 = 5,
        ARMv8p6 = 6,
        ARMv8p7 = 7,
    };

    /// <summary>
    /// Returns TRUE if the implemented architecture includes the extensions defined in the specified
    /// architecture version.
    /// </summary>
    /// <param name="ver"></param>
    /// <returns></returns>
    [[nodiscard]] inline constexpr static auto HasArchVersion(ArchVersion ver) noexcept {
        return static_cast< bool >(ver == ArchVersion::ARMv8p0);
    }

    [[nodiscard]] inline constexpr static auto HaveVirtHostExt() noexcept {
        return HasArchVersion(ArchVersion::ARMv8p1);
    }

    [[nodiscard]] inline constexpr static auto HaveSecureEL2Ext() noexcept {
        return HasArchVersion(ArchVersion::ARMv8p4);
    }

    [[nodiscard]] inline constexpr static auto HaveBTIExt() noexcept {
        return HasArchVersion(ArchVersion::ARMv8p5);
    }

    [[nodiscard]] inline constexpr static auto HaveFlagManipulateExt() noexcept {
        return HasArchVersion(ArchVersion::ARMv8p4);
    }

} // namespace arm_emu

#endif // SHARED_OPERATIONS_FUNCTIONS_EXTENSION_HPP