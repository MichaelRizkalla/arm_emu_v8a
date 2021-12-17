#if !defined(ACCESSTYPE_H_INCLUDED_2FC7B123_A9B4_47BF_AED0_19BB599D91A4)
    #define ACCESSTYPE_H_INCLUDED_2FC7B123_A9B4_47BF_AED0_19BB599D91A4

    #include <API/Api.h>
    #include <cstdint>

BEGIN_NAMESPACE

enum class IProcessingUnit::AccessType : std::uint32_t
{
    NORMAL,
    VEC, // Normal loads and stores
    STREAM,
    VECSTREAM, // Streaming loads and stores
    ATOMIC,
    ATOMICRW, // Atomic loads and stores
    ORDERED,
    ORDEREDRW,     // Load-Acquire and Store-Release
    ORDEREDATOMIC, // Load-Acquire and Store-Release with atomic access
    ORDEREDATOMICRW,
    LIMITEDORDERED, // Load-LOAcquire and Store-LORelease
    UNPRIV,         // Load and store unprivileged
    IFETCH,         // Instruction fetch
    PTW,            // Page table walk
    NONFAULT,       // Non-faulting loads
    CNOTFIRST,      // Contiguous FF load, not first element
    NV2REGISTER,    // MRS/MSR instruction used at EL1 and which is converted
                    // to a memory access that uses the EL2 translation regime
                    // Other operations
    DC,             // Data cache maintenance
    DC_UNPRIV,      // Data cache maintenance instruction used at EL0
    IC,             // Instruction cache maintenance
    DCZVA,          // DC ZVA instructions
    AT,             // Address translation
};

END_NAMESPACE

#endif // !defined(ACCESSTYPE_H_INCLUDED_2FC7B123_A9B4_47BF_AED0_19BB599D91A4)
