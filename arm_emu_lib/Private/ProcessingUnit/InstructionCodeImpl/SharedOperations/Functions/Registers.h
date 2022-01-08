/// <summary>
/// Report the hint passed to BranchTo() and BranchToAddr(), for consideration when processing
/// the next instruction.
/// </summary>
/// <param name="PC"></param>
/// <param name="branch_type"></param>
/// <returns></returns>
void Hint_Branch(IProcessingUnit::BranchType branchType) noexcept {
    if (branchType == IProcessingUnit::BranchType::Ret) {
        PC() = std::numeric_limits< std::uint64_t >::max();
    }
}

/// <summary>
/// Set program counter to a new address, with a branch type
/// In AArch64 state the address might include a tag in the top eight bits.
/// </summary>
/// <param name="sp"></param>
/// <param name="PC"></param>
/// <param name="target"></param>
/// <param name="branch_type"></param>
/// <returns></returns>
void BranchTo(const std::uint64_t& target, IProcessingUnit::BranchType branchType) noexcept {
    Hint_Branch(branchType);
    // TODO
    /*if (N == 32)
        if(UsingAArch32());
        PC = ZeroExtend(target);
    else
        assert N == 64 && !UsingAArch32();*/
    // Always using AArch64 now
    if (branchType != IProcessingUnit::BranchType::Ret)
        PC() = BranchAddr(target).to_ullong();
    return;
}

    /// <summary>
/// Return address of the current instruction.
/// </summary>
/// <param name="PC"></param>
/// <returns></returns>
[[nodiscard]] std::uint64_t ThisInstrAddr() noexcept {
    // TODO
    /* assert N == 64 || (N == 32 && UsingAArch32()); */ // always true
    return PC();
}
