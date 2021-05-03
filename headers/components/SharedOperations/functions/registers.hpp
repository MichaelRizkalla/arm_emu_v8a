#ifndef SHARED_OPERATIONS_FUNCTIONS_REGISTERS_HPP
#define SHARED_OPERATIONS_FUNCTIONS_REGISTERS_HPP

#include <cstdint>

namespace arm_emu {

    [[nodiscard]] inline static auto ThisInstrAddr(const std::uint64_t& PC) noexcept {
        /* assert N == 64 || (N == 32 && UsingAArch32()); */ // always true
        return PC;
    }

} // namespace arm_emu

#endif // SHARED_OPERATIONS_FUNCTIONS_REGISTERS_HPP
