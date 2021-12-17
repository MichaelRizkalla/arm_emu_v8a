#if !defined(EXCEPTIONRECORD_H_INCLUDED_E8FD6058_C864_4970_A4B2_1B72E052FE78)
    #define EXCEPTIONRECORD_H_INCLUDED_E8FD6058_C864_4970_A4B2_1B72E052FE78

    #include <API/Api.h>
    #include <cstdint>

BEGIN_NAMESPACE

struct IProcessingUnit::ExceptionRecord {
    IProcessingUnit::Exception m_excepType; // Exception class
    std::uint32_t              m_syndrome;  // Syndrome record - 25bits
    std::uint64_t              m_vAddress;  // Virtual fault address  - 64bits
    bool                       m_ipaValid;  // Physical fault address for second stage faults is valid
    bool                       m_NS;        // Physical fault address for second stage faults is Non-secure or secure
    std::uint64_t              m_ipAddress; // Physical fault address for second stage faults - 52bits
};

END_NAMESPACE

#endif // !defined(EXCEPTIONRECORD_H_INCLUDED_E8FD6058_C864_4970_A4B2_1B72E052FE78)
