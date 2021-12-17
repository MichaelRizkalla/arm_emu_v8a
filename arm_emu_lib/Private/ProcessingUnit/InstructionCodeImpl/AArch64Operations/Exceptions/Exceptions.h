void TakeException(std::bitset< 2 > target_el, IProcessingUnit::ExceptionRecord exception,
                   std::uint64_t preferred_exception_return, std::uint32_t vect_offset) {
    if (HaveEL(target_el) && (target_el.to_ulong() > EL.to_ulong())) {
        throw undefined_behaviour {};
    }

    // Take Exception ...
    // TODO
}

void SoftwareBreakpoint(std::uint16_t immediate) {
    // TODO
    auto excepLevel = static_cast< IProcessingUnit::ExceptionLevel >(EL.to_ulong());
    bool route_to_el2 =
        (excepLevel == IProcessingUnit::ExceptionLevel::EL0 || excepLevel == IProcessingUnit::ExceptionLevel::EL1) &&
        false /*EL2Enabled()*/
        && false /*(HCR_EL2.TGE == '1' || MDCR_EL2.TDE == '1')*/;

    std::uint64_t preferred_exception_return = ThisInstrAddr();
    auto          vect_offset                = 0x0;

    auto exception       = ExceptionSyndrome(IProcessingUnit::Exception::SoftwareBreakpoint);
    exception.m_syndrome = immediate;

    if (excepLevel > IProcessingUnit::ExceptionLevel::EL1) {
        TakeException(EL, exception, preferred_exception_return, vect_offset);
    } else if (route_to_el2) {
        TakeException(static_cast< std::underlying_type_t< IProcessingUnit::ExceptionLevel > >(
                          IProcessingUnit::ExceptionLevel::EL2),
                      exception, preferred_exception_return, vect_offset);
    } else {
        TakeException(static_cast< std::underlying_type_t< IProcessingUnit::ExceptionLevel > >(
                          IProcessingUnit::ExceptionLevel::EL1),
                      exception, preferred_exception_return, vect_offset);
    }
}
