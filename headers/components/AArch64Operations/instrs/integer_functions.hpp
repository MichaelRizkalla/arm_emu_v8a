#ifndef AARCH64_OPERATIONS_INSTRS_INTEGER_FUNCTIONS_HPP
#define AARCH64_OPERATIONS_INSTRS_INTEGER_FUNCTIONS_HPP

#include <bitset>
#include <cstdint>
#include <SharedOperations/functions/common.hpp>

namespace arm_emu {

    enum class ShiftType : std::uint32_t
    {
        LSL = 0b00,
        LSR = 0b01,
        ASR = 0b10,
        ROR = 0b11,
    };

    [[nodiscard]] inline static auto DecodeShift(std::bitset< 2 > op) {
        std::uint8_t op_ = static_cast< std::uint8_t >(op.to_ulong());
        switch (op_) {
            case 0b00:
            case 0b01:
            case 0b10:
            case 0b11:
                return static_cast< ShiftType >(op_);
                break;
            default:
                throw undefined_behaviour {};
        }
    }

    template < std::size_t N, std::uint8_t MaxShift >
    [[nodiscard]] inline static std::bitset< N > ShiftReg(const GPRegisters* gp_regs, std::uint8_t reg, ShiftType shifttype, std::uint8_t amount) {
        std::bitset< N > result = gp_regs->X(reg).to_ullong();

        switch (shifttype) {
            case ShiftType::LSL:
                result = LSL< N >(result, amount);
                break;
            case ShiftType::LSR:
                result = LSR< N, MaxShift >(result, amount);
                break;
            case ShiftType::ASR:
                result = ASR< N, MaxShift >(result, amount);
                break;
            case ShiftType::ROR:
                result = ROR< N, MaxShift >(result, amount);
                break;
        }
        return result;
    }

    // Decode AArch64 bitfield and logical immediate masks which use a similar encoding structure
    template < std::size_t M >
    [[nodiscard]] inline static auto DecodeBitMasks(bool immN, std::bitset< 6 > imms, std::bitset< 6 > immr, bool immediate) {
        std::bitset< M > tmask, wmask;
        std::bitset< 6 > levels;

        // Compute log2 of element size
        // 2^len must be in range [2, M]
        auto NOTimms = imms;
        NOTimms.flip();
        const auto len = HighestSetBit(concate< 1, 6 >(immN, NOTimms));
        if (len < 1)
            throw undefined_behaviour {};
        assert(M >= (1 << len));

        // Determine S, R and S - R parameters
        std::uint8_t levels_ = 0;
        std::int64_t loc     = len - 1;
        for (auto i = loc; i >= 0; i--) {
            levels_ |= (static_cast< std::uint8_t >(1) << i);
        }
        levels = levels_;
        // For logical immediates an all-ones value of S is reserved
        // since it would generate a useless all-ones result (many times)
        if (immediate && ((imms & levels) == levels))
            throw undefined_behaviour {};

        auto S    = static_cast< std::uint32_t >((imms & levels).to_ulong());
        auto R    = static_cast< std::uint32_t >((immr & levels).to_ulong());
        auto diff = S - R; // 6-bit subtract with borrow

        auto esize = (static_cast< std::int64_t >(1) << len);
        (void)esize; // suppress compiler warnings for now
        std::uint32_t mask = 0;
        for (auto i = 0; i < len - 1; i++) {
            mask |= static_cast< std::uint32_t >(1) << i;
        }
        std::uint32_t d = mask & diff;

        std::uint64_t welem = 0, telem = 0;

        for (int64_t i = S; i >= 0; i--) { // ZeroExtend(Ones(S+1), esize) -- zeros on the rhs
            welem |= (static_cast< std::uint64_t >(1) << i);
        }
        for (int64_t i = d; i >= 0; i--) {
            telem |= (static_cast< std::uint64_t >(1) << i);
        }

        auto tmp = ROR(welem, HighestSetBit(welem), static_cast< std::uint8_t >(R));
        wmask    = Replicate(tmp, HighestSetBit(tmp) + 1, M);
        tmask    = Replicate(telem, HighestSetBit(telem) + 1, M);
        return std::make_pair(wmask, tmask);
    }

} // namespace arm_emu

#endif // AARCH64_OPERATIONS_INSTRS_INTEGER_FUNCTIONS_HPP
