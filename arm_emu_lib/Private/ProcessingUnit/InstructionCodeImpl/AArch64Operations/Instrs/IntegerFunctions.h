
/// <summary>
/// Decode AArch64 bitfield and logical immediate masks which use a similar encoding structure
/// </summary>
/// <param name="immN"></param>
/// <param name="imms"></param>
/// <param name="immr"></param>
/// <param name="immediate"></param>
/// <returns></returns>
template < std::size_t M >
[[nodiscard]] auto DecodeBitMasks(bool immN, const Bitset& imms, const Bitset& immr, bool immediate) {
    assert(imms.Size() == 6 && immr.Size() == 6);

    std::bitset< M > tmask, wmask;
    Bitset           levels { 6, 0 };

    // Compute log2 of element size
    // 2^len must be in range [2, M]
    auto NOTimms = imms;
    NOTimms.Flip();
    const auto len = HighestSetBit(concate< 1, 6 >(immN, NOTimms.ToULong()));
    if (len < 1) {
        throw undefined_behaviour {};
    }
    assert(M >= (1 << std::bit_cast< std::uint64_t >(len)));

    // Determine S, R and S - R parameters
    std::uint8_t levels_ = 0;
    std::int64_t loc     = len - 1;
    for (auto i = loc; i >= 0; i--) {
        levels_ |= (static_cast< std::uint8_t >(1) << i);
    }
    levels = levels_;
    // For logical immediates an all-ones value of S is reserved
    // since it would generate a useless all-ones result (many times)
    if (immediate && ((imms & levels) == levels)) {
        throw undefined_behaviour {};
    }

    auto S    = static_cast< std::uint32_t >((imms & levels).ToULong());
    auto R    = static_cast< std::uint32_t >((immr & levels).ToULong());
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

[[nodiscard]] IProcessingUnit::ShiftType DecodeShift(const Bitset& op) {
    assert(op.Size() == 2);

    std::uint8_t op_ = static_cast< std::uint8_t >(op.ToULong());
    switch (op_) {
        case 0b00:
        case 0b01:
        case 0b10:
        case 0b11:
            return static_cast< IProcessingUnit::ShiftType >(op_);
            break;
        default:
            throw undefined_behaviour {};
    }
}

/// <summary>
/// Perform shift of a register operand
/// </summary>
/// <param name="gp_regs"></param>
/// <param name="reg"></param>
/// <param name="shifttype"></param>
/// <param name="amount"></param>
/// <returns></returns>
template < std::size_t N >
[[nodiscard]] std::bitset< N > ShiftReg(const Bitset& reg, IProcessingUnit::ShiftType shifttype, std::uint8_t amount) {
    std::bitset< N > result = m_gpRegisters.X(reg).to_ullong();

    switch (shifttype) {
        case IProcessingUnit::ShiftType::LSL:
            result = LSL< N >(result, amount);
            break;
        case IProcessingUnit::ShiftType::LSR:
            result = LSR< N >(result, amount);
            break;
        case IProcessingUnit::ShiftType::ASR:
            result = ASR< N >(result, amount);
            break;
        case IProcessingUnit::ShiftType::ROR:
            result = ROR< N >(result, amount);
            break;
    }
    if constexpr (N == 64) {
        return result;
    } else {
        return std::bitset< N > { result.to_ullong() };
    }
}
