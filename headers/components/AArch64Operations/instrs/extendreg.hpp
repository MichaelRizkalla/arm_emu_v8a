#ifndef AARCH64_OPERATIONS_INSTRS_EXTENDREG_HPP
#define AARCH64_OPERATIONS_INSTRS_EXTENDREG_HPP

#include <Registers.h>
#include <SharedOperations/functions/system.hpp>
#include <SharedOperations/functions/common.hpp>
#include <bitset>
#include <cassert>
#include <cstdint>
#include <utility>

namespace arm_emu {

    enum class ExtendType : std::uint32_t
    {
        UXTB = 0b000,
        UXTH = 0b001,
        UXTW = 0b010,
        UXTX = 0b011,
        SXTB = 0b100,
        SXTH = 0b101,
        SXTW = 0b110,
        SXTX = 0b111,
    };

    template < std::size_t N >
    [[nodiscard]] inline static std::bitset< N > ExtendReg(const GPRegisters* gp_regs, std::uint8_t reg, ExtendType exttype, std::uint8_t shift) {
        assert(shift >= 0 && shift <= 4);
        std::bitset< N > val;
        if constexpr (N == 32) {
            val = gp_regs->W(reg);
        } else {
            val = gp_regs->X(reg);
        }
        bool             unsigned_ = false;
        std::uint8_t     len       = 0;
        switch (exttype) {
            case ExtendType::SXTB:
                unsigned_ = false;
                len       = 8;
                break;
            case ExtendType::SXTH:
                unsigned_ = false;
                len       = 16;
                break;
            case ExtendType::SXTW:
                unsigned_ = false;
                len       = 32;
                break;
            case ExtendType::SXTX:
                unsigned_ = false;
                len       = 64;
                break;
            case ExtendType::UXTB:
                unsigned_ = true;
                len       = 8;
                break;
            case ExtendType::UXTH:
                unsigned_ = true;
                len       = 16;
                break;
            case ExtendType::UXTW:
                unsigned_ = true;
                len       = 32;
                break;
            case ExtendType::UXTX:
                unsigned_ = true;
                len       = 64;
                break;
            default:
                Unreachable();
                break;
        }

        len = std::min(len, static_cast< std::uint8_t >(N - shift));

        std::bitset< N > mVal    = static_cast< std::size_t >(std::pow(2, len) - 1);
        auto             new_val = mVal & val;
        new_val <<= shift;
        return Extend< N, N >(new_val, unsigned_);
    }

} // namespace arm_emu

#endif // AARCH64_OPERATIONS_INSTRS_EXTENDREG_HPP
