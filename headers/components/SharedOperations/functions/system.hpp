#ifndef SHARED_OPERATIONS_FUNCTIONS_SYSTEM_HPP
#define SHARED_OPERATIONS_FUNCTIONS_SYSTEM_HPP

#include <A64SystemSettings.h>
#include <bitset>
#include <cassert>
#include <cstdint>

namespace arm_emu {

    static std::bitset< 2 > EL3 = 0b11;
    static std::bitset< 2 > EL2 = 0b10;
    static std::bitset< 2 > EL1 = 0b01;
    static std::bitset< 2 > EL0 = 0b00;

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

    [[noreturn]] void Unreachable() noexcept {
        assert(false);
        abort();
    }

    [[nodiscard]] inline static auto HasArchVersion(ArchVersion ver) noexcept {
        return static_cast< bool >(ver == ArchVersion::ARMv8p0);
    }

    [[nodiscard]] inline static auto HaveEL(std::bitset< 2 > el) {
        if (el == EL0 || el == EL1)
            return true;
        if (el == EL2)
            throw not_implemented_feature {};
        return SystemSettings::IsImplemented< Features, Features::EL3 >();
    }

} // namespace arm_emu

#endif // SHARED_OPERATIONS_FUNCTIONS_SYSTEM_HPP
