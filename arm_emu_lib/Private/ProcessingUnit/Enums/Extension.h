#if !defined(EXTENSION_H_INCLUDED_CAE60F9D_2EA7_4109_85FC_F7DFD19663AE)
    #define EXTENSION_H_INCLUDED_CAE60F9D_2EA7_4109_85FC_F7DFD19663AE

    #include <API/Api.h>
    #include <cstdint>

BEGIN_NAMESPACE

enum class IProcessingUnit::Extension : std::uint32_t
{
    // Armv8p0
    Cryptographic,
    ReliabilityAvailabilityServiceability,
    EventMonitors,
    IVIPT,
    PCSampleBasedProfiling
};

END_NAMESPACE

#endif // !defined(EXTENSION_H_INCLUDED_CAE60F9D_2EA7_4109_85FC_F7DFD19663AE)
