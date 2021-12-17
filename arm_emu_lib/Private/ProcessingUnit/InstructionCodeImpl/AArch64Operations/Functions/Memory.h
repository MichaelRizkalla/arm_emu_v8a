
/// <summary>
/// // Return the virtual address with tag bits removed for storing to the program counter.
/// </summary>
/// <param name="sp"></param>
/// <param name="target"></param>
/// <returns></returns>
[[nodiscard]] auto BranchAddr(const std::bitset< 64 >& target) noexcept {
    auto msbit = AddrTop(target, true);

    if (msbit == 63)
        return target;

    std::bitset< 64 > vaddress { target };
    auto              ELLevel = static_cast< IProcessingUnit::ExceptionLevel >(EL.to_ulong());
    assert(static_cast< std::underlying_type_t< IProcessingUnit::ExceptionLevel > >(ELLevel) >= 0 &&
           static_cast< std::underlying_type_t< IProcessingUnit::ExceptionLevel > >(ELLevel) < 4);

    if ((ELLevel == IProcessingUnit::ExceptionLevel::EL0 || ELLevel == IProcessingUnit::ExceptionLevel::EL1 ||
         IsInHost()) &&
        target[msbit] == true) {
        bool value = vaddress[msbit];
        for (auto i = msbit + 1; i < 64; i++) {
            vaddress[i] = value;
        }
        return vaddress;
    } else {
        for (auto i = msbit + 1; i < 64; i++) {
            vaddress[i] = false;
        }
        return vaddress;
    }
}
