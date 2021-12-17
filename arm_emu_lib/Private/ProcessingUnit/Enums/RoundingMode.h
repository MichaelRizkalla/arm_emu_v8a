#if !defined(ROUNDINGMODE_H_INCLUDED_49BD34E4_EA7E_4121_A86F_DF4FDBE71348)
    #define ROUNDINGMODE_H_INCLUDED_49BD34E4_EA7E_4121_A86F_DF4FDBE71348

    #include <API/Api.h>
    #include <cstdint>

BEGIN_NAMESPACE

enum class IProcessingUnit::RoundingMode : std::uint32_t
{
    RoundToNearest       = 0b00,
    RoundToPlusInfinity  = 0b01,
    RoundToMinusInfinity = 0b10,
    RoundToZero          = 0b11,
};

END_NAMESPACE

#endif // !defined(ROUNDINGMODE_H_INCLUDED_49BD34E4_EA7E_4121_A86F_DF4FDBE71348)
