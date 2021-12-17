[[nodiscard]] IProcessingUnit::ExceptionRecord ExceptionSyndrome(IProcessingUnit::Exception excepType) noexcept {
    IProcessingUnit::ExceptionRecord r;

    r.m_excepType = excepType;

    r.m_syndrome  = std::uint32_t { 0 };
    r.m_vAddress  = std::uint64_t { 0 };
    r.m_ipaValid  = false;
    r.m_NS        = false;
    r.m_ipAddress = std::uint64_t { 0 };

    return r;
}
