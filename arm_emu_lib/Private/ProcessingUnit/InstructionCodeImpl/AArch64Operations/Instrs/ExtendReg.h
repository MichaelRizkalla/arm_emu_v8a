
/// <summary>
/// Perform a register extension and shift
/// </summary>
/// <param name="gp_regs"></param>
/// <param name="reg"></param>
/// <param name="exttype"></param>
/// <param name="shift"></param>
/// <returns></returns>
template < std::size_t N >
[[nodiscard]] std::bitset< N > ExtendReg(const Bitset& reg, IProcessingUnit::ExtendType extType, std::uint8_t shift) {
    assert(shift >= 0 && shift <= 4);
    std::bitset< N > val;
    if constexpr (N == 32) {
        val = m_gpRegisters.W(reg);
    } else if constexpr (N == 64) {
        val = m_gpRegisters.X(reg);
    } else {
        assert(false && "Unexpectedly reached code path!");
        std::terminate();
    }

    bool         unsigned_ = false;
    std::uint8_t len       = 0;
    switch (extType) {
        case IProcessingUnit::ExtendType::SXTB:
            unsigned_ = false;
            len       = 8;
            break;
        case IProcessingUnit::ExtendType::SXTH:
            unsigned_ = false;
            len       = 16;
            break;
        case IProcessingUnit::ExtendType::SXTW:
            unsigned_ = false;
            len       = 32;
            break;
        case IProcessingUnit::ExtendType::SXTX:
            unsigned_ = false;
            len       = 64;
            break;
        case IProcessingUnit::ExtendType::UXTB:
            unsigned_ = true;
            len       = 8;
            break;
        case IProcessingUnit::ExtendType::UXTH:
            unsigned_ = true;
            len       = 16;
            break;
        case IProcessingUnit::ExtendType::UXTW:
            unsigned_ = true;
            len       = 32;
            break;
        case IProcessingUnit::ExtendType::UXTX:
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

/// <summary>
/// Decode a register extension option
/// </summary>
/// <typeparam name="Ty"></typeparam>
/// <param name="option"></param>
/// <returns></returns>
[[nodiscard]] IProcessingUnit::ExtendType DecodeRegExtend(const std::bitset< 3 >& option) noexcept {
    return static_cast< IProcessingUnit::ExtendType >(option.to_ulong());
}
