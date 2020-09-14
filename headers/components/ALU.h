#pragma once
#include <algorithm>
#include <cmath>

#include <A64Enums.h>
#include <AArch64.h>

namespace arm_emu {

struct ALU {

    template < std::size_t _TWidth >
    [[nodiscard]] inline static constexpr auto SP(SpecialRegisters* sp, std::bitset< _TWidth > value) {
        if (sp->SPSel.SP() == 0) {
            sp->SP_EL0.Set(value.to_ullong());
        } else {
            switch (sp->CurrentEL.EL()) {
                case ExceptionLevel::EL0:
                    sp->SP_EL0.Set(value.to_ullong());
                    break;
                case ExceptionLevel::EL1:
                    sp->SP_EL1.Set(value.to_ullong());
                    break;
                case ExceptionLevel::EL2:
                    sp->SP_EL2.Set(value.to_ullong());
                    break;
                case ExceptionLevel::EL3:
                    sp->SP_EL3.Set(value.to_ullong());
                    break;
                default:
                    throw undefined_behaviour {};
            }
        }
    }

    template < std::size_t _TWidth >
    [[nodiscard]] inline static constexpr std::bitset< _TWidth > SP(SpecialRegisters* sp) {
        static_assert((_TWidth == 8) || (_TWidth == 16) || (_TWidth == 32) || (_TWidth == 64));
        if (sp->SPSel.SP() == 0) {
            return sp->SP_EL0.Value();
        } else {
            switch (sp->CurrentEL.EL()) {
                case ExceptionLevel::EL0: {
                    return sp->SP_EL0.Value();
                } break;
                case ExceptionLevel::EL1: {
                    return sp->SP_EL1.Value();
                } break;
                case ExceptionLevel::EL2: {
                    return sp->SP_EL2.Value();
                } break;
                case ExceptionLevel::EL3: {
                    return sp->SP_EL3.Value();
                } break;
                default:
                    throw undefined_behaviour {};
            }
        }
    }

    template < std::size_t N, typename _Ty >
    [[nodiscard]] inline static constexpr auto AddWithCarry(std::bitset< N > x, std::bitset< N > y, _Ty c) {
        using _T  = std::uint64_t;
        using _ST = std::make_signed_t< _T >;

        _T               unsigned_sum = static_cast< _T >(x.to_ullong()) + static_cast< _T >(y.to_ullong()) + static_cast< _T >(c);
        _ST              signed_sum   = static_cast< _ST >(x.to_ullong()) + static_cast< _ST >(y.to_ullong()) + static_cast< _T >(c);
        std::bitset< N > result       = unsigned_sum;

        std::uint8_t nzcv = 0b0000'0000;
        nzcv |= static_cast< std::uint8_t >((result.to_ullong() >> (N - 1)) & 1) << 3;
        nzcv |= result.none() ? 1 << 2 : 0;
        nzcv |= static_cast< _T >(result.to_ullong()) == unsigned_sum ? 0 : 1 << 1;
        nzcv |= static_cast< _ST >(result.to_ullong()) == signed_sum ? 0 : 1 << 0;
        return std::make_pair(result, nzcv);
    }

    template < std::size_t N >
    [[nodiscard]] inline static constexpr std::int64_t HighestSetBit(std::bitset< N > x) {
        for (std::int64_t i = N - 1; i > -1; i--) {
            if (x[i] == true) return i;
        }
        return -1;
    }

    [[nodiscard]] inline static std::int64_t HighestSetBit(std::uint64_t x) {
        for (std::int64_t i = 63; i > -1; i--) {
            if (static_cast< bool >(x & (static_cast< std::uint64_t >(1) << i)) == true) return i;
        }
        throw undefined_behaviour {};
    }

    template < std::size_t N, std::size_t M >
    [[nodiscard]] inline static constexpr auto Replicate(std::bitset< M > x) {
        static_assert(N % M == 0);
        std::bitset< N * M > return_value;
        std::bitset< N * M > value(0);
        if constexpr (N * M > 0) value = x.to_ullong();
        for (std::size_t n = 0; n < static_cast< std::size_t >(N / M); n++) {
            return_value |= value;
            value <<= M;
        }
        return return_value;
    }

    [[nodiscard]] inline static constexpr auto Replicate(std::uint64_t x, std::size_t currentSize, std::size_t targetSize) {
        if (targetSize % currentSize != 0) throw undefined_behaviour {};
        std::uint64_t return_value = 0;
        std::uint64_t value        = 0;
        if (targetSize * currentSize > 0) {
            value        = x;
            return_value = x;
        }
        for (std::size_t n = 1; n < static_cast< std::size_t >(targetSize / currentSize); n++) {
            value <<= currentSize;
            return_value |= value;
        }
        return return_value;
    }

    template < std::size_t N, std::size_t M >
    [[nodiscard]] inline static constexpr auto ZeroExtend(std::bitset< M > x) {
        static_assert(N >= M);
        return std::bitset< N >(x.to_ullong());
    }

    template < std::size_t N, std::size_t M >
    [[nodiscard]] inline static constexpr auto SignExtend(std::bitset< M > x) {
        static_assert(N >= M);
        auto             val          = Replicate< N - M >(std::bitset< 1 > { x[M - 1] });
        std::bitset< N > return_value = concate< N - M, M >(val, x);
        return return_value;
    }

    template < std::size_t N, std::size_t M >
    [[nodiscard]] inline static constexpr std::bitset< N > Extend(std::bitset< M > x, bool unsigned_) {
        if (unsigned_)
            return ZeroExtend< N, M >(x);
        else
            return SignExtend< N, M >(x);
    }

    template < typename _Ty >
    [[nodiscard]] inline static constexpr auto DecodeRegExtend(_Ty option) {
        return static_cast< ExtendType >(option);
    }

    template < std::size_t N >
    [[nodiscard]] inline static constexpr std::bitset< N > ExtendReg(const GPRegisters* gp_regs, std::uint8_t reg, ExtendType exttype, std::uint8_t shift) {
        std::bitset< N > val       = gp_regs->X(reg).to_ullong();
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
                throw undefined_behaviour {};
                break;
        }

        len = std::min(len, static_cast< std::uint8_t >(N - shift));

        std::bitset< N > mVal    = static_cast< std::size_t >(std::pow(2, len) - 1);
        auto             new_val = mVal & val;
        new_val <<= shift;
        return Extend< N, N >(new_val, unsigned_);
    }

    template < std::size_t N >
    [[nodiscard]] inline static constexpr auto LSL_C(std::bitset< N > x, std::uint8_t shift) {
        if (shift < 0) throw undefined_behaviour {};
        std::bitset< N + 1 > extended_x { x.to_ullong() };
        extended_x <<= shift;
        std::bitset< N > result { extended_x.to_ullong() };
        auto             carry_out = extended_x[N];
        return std::make_pair(result, carry_out);
    }

    template < std::size_t N >
    [[nodiscard]] inline static constexpr auto LSL(std::bitset< N > x, std::uint8_t shift) {
        if (shift < 0) throw undefined_behaviour {};
        if (shift == 0)
            return x;
        else {
            auto [result, _] = LSL_C< N >(x, shift);
            return result;
        }
    }

    template < std::size_t N, std::uint8_t MaxShift >
    [[nodiscard]] inline static constexpr auto LSR_C(std::bitset< N > x, std::uint8_t shift) {
        if (shift < 0) throw undefined_behaviour {};
        auto extended_x = ZeroExtend< N + MaxShift >(x);
        auto carry_out  = extended_x[shift - 1];
        extended_x >>= shift;
        std::bitset< N > result { extended_x.to_ullong() };
        return std::make_pair(result, carry_out);
    }

    template < std::size_t N, std::uint8_t MaxShift >
    [[nodiscard]] inline static constexpr auto LSR(std::bitset< N > x, std::uint8_t shift) {
        if (shift < 0) throw undefined_behaviour {};
        if (shift == 0)
            return x;
        else {
            auto [result, _] = LSR_C< N, MaxShift >(x, shift);
            return result;
        }
    }

    template < std::size_t N, std::uint8_t MaxShift >
    [[nodiscard]] inline static constexpr auto ASR_C(std::bitset< N > x, std::uint8_t shift) {
        if (shift < 0) throw undefined_behaviour {};
        auto extended_x = SignExtend< N + MaxShift >(x);
        auto carry_out  = extended_x[shift - 1];
        extended_x >>= shift;
        std::bitset< N > result { extended_x.to_ullong() };
        return std::make_pair(result, carry_out);
    }

    template < std::size_t N, std::uint8_t MaxShift >
    [[nodiscard]] inline static constexpr auto ASR(std::bitset< N > x, std::uint8_t shift) {
        if (shift < 0) throw undefined_behaviour {};
        if (shift == 0)
            return x;
        else {
            auto [result, _] = ASR_C< N, MaxShift >(x, shift);
            return result;
        }
    }

    template < std::size_t N, std::uint8_t MaxShift >
    [[nodiscard]] inline static constexpr auto ROR_C(std::bitset< N > x, std::uint8_t shift) {
        if (shift < 0) throw undefined_behaviour {};

        auto m      = static_cast< std::uint8_t >(shift % N);
        auto result = LSR< N, MaxShift >(x, m);
        result |= LSL< N >(x, N - m);

        auto carry_out = result[N - 1];
        return std::make_pair(result, carry_out);
    }

    template < std::size_t N, std::uint8_t MaxShift >
    [[nodiscard]] inline static constexpr auto ROR(std::bitset< N > x, std::uint8_t shift) {
        if (shift < 0) throw undefined_behaviour {};
        if (shift == 0)
            return x;
        else {
            auto [result, _] = ROR_C< N, MaxShift >(x, shift);
            return result;
        }
    }

    [[nodiscard]] inline static constexpr auto ROR(std::uint64_t x, std::uint64_t currentSize, std::uint8_t shift) {
        if (shift < 0) throw undefined_behaviour {};
        if (shift == 0)
            return x;
        else {
            auto m   = static_cast< std::uint8_t >(shift % currentSize);
            auto LSR = x;
            LSR >>= m;
            auto result = LSR;
            auto LSL    = x;
            LSL <<= currentSize - m;
            result |= LSL;

            std::uint64_t return_value = 0;
            for (std::uint64_t i = 0; i < currentSize; i++) { return_value |= ((static_cast< std::uint64_t >(1) << i) & result); }

            return return_value;
        }
    }

    template < std::size_t N, std::uint8_t MaxShift >
    [[nodiscard]] inline static constexpr std::bitset< N > ShiftReg(const GPRegisters* gp_regs, std::uint8_t reg, ShiftType shifttype, std::uint8_t amount) {
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

    template < typename _Ty >
    [[nodiscard]] inline static constexpr auto getOption(_Ty in) {
        if (in == 0b111 || in == 0b011) { return WidthOption::X; }
        return WidthOption::W;
    }

    [[nodiscard]] inline static constexpr auto DecodeShift(std::uint8_t op) {
        switch (op) {
            case 0b00:
            case 0b01:
            case 0b10:
            case 0b11:
                return static_cast< ShiftType >(op);
                break;
            default:
                throw undefined_behaviour {};
        }
    }

    template < std::size_t M >
    [[nodiscard]] inline static constexpr auto DecodeBitMasks(bool immN, std::bitset< 6 > imms, std::bitset< 6 > immr, bool immediate) {
        std::bitset< M > tmask, wmask;
        std::bitset< 6 > levels;

        auto NOTimms = imms;
        NOTimms.flip();
        const auto len = HighestSetBit(concate< 1, 6 >(immN, NOTimms));
        if (len < 1) throw undefined_behaviour {};

        std::uint8_t levels_ = 0;
        std::int64_t loc     = len - 1;
        for (auto i = loc; i >= 0; i--) { levels_ |= (static_cast< std::uint8_t >(1) << i); }
        levels = levels_;
        if (immediate && ((imms & levels) == levels)) throw undefined_behaviour {};

        auto S    = static_cast< std::uint32_t >((imms & levels).to_ulong());
        auto R    = static_cast< std::uint32_t >((immr & levels).to_ulong());
        auto diff = S - R;

        auto          esize = (static_cast< std::int64_t >(1) << len);
        (void)esize; // suppress compiler warnings for now
        std::uint32_t mask  = 0;
        for (auto i = 0; i < len - 1; i++) { mask |= static_cast< std::uint32_t >(1) << i; }
        std::uint32_t d = mask & diff;

        std::uint64_t welem = 0, telem = 0;

        for (int64_t i = S; i >= 0; i--) { welem |= (static_cast< std::uint64_t >(1) << i); }
        for (int64_t i = d; i >= 0; i--) { telem |= (static_cast< std::uint64_t >(1) << i); }

        auto tmp = ROR(welem, HighestSetBit(welem), static_cast< std::uint8_t >(R));
        wmask    = Replicate(tmp, HighestSetBit(tmp) + 1, M);
        tmask    = Replicate(telem, HighestSetBit(telem) + 1, M);
        return std::make_pair(wmask, tmask);
    }

    [[nodiscard]] inline static bool ConditionHold(std::bitset< 4 >&& cond, NZCVRegister* nzcv) {
        bool result = false;

        std::string cond_s = cond.to_string();
        std::string mCond  = cond_s.substr(0, 3);
        auto        mCond0 = cond_s[3];
        if (mCond == "000")
            result = (nzcv->Z() == true);
        else if (mCond == "001")
            result = (nzcv->C() == true);
        else if (mCond == "010")
            result = (nzcv->N() == true);
        else if (mCond == "011")
            result = (nzcv->V() == true);
        else if (mCond == "100")
            result = (nzcv->C() == true && nzcv->Z() == false);
        else if (mCond == "101")
            result = (nzcv->N() == nzcv->V());
        else if (mCond == "110")
            result = (nzcv->N() == nzcv->V() && nzcv->Z() == false);
        else if (mCond == "111")
            result = (nzcv->Z() == true);

        if (mCond0 == '1' && cond != 15) { return !result; }
        return result;
    }

    inline static constexpr void Hint_Branch(std::uint64_t& PC, BranchType branch_type) {
        if (branch_type == BranchType::RET) PC = std::numeric_limits< std::uint64_t >::max();
    }

    inline static constexpr void BranchTo(SpecialRegisters* sp, std::uint64_t& PC, const std::uint64_t& target, BranchType branch_type) {
        Hint_Branch(PC, branch_type);
        /*if (N == 32)
            if(UsingAArch32());
            PC = ZeroExtend(target);
        else
            assert N == 64 && !UsingAArch32();*/
        // Always using AArch64 now
        if (branch_type != BranchType::RET) PC = AArch64::BranchAddr(sp, target).to_ullong();
        return;
    }

    ALU()           = delete;
    ALU(const ALU&) = delete;
    ALU(ALU&&)      = delete;
    ALU& operator=(const ALU&) = delete;
    ALU& operator=(ALU&&) = delete;
    ~ALU()                = delete;

    inline static constexpr std::uint64_t LOG2_TAG_GRANULE = 4;
    inline static constexpr std::uint64_t TAG_GRANULE      = 1 << LOG2_TAG_GRANULE;
};

} // namespace arm_emu