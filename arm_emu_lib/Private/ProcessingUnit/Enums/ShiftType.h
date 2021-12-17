#if !defined(SHIFTTYPE_H_INCLUDED_E2883020_9A39_4E83_B60D_1B97905DB68C)
    #define SHIFTTYPE_H_INCLUDED_E2883020_9A39_4E83_B60D_1B97905DB68C

    #include <API/Api.h>
    #include <cstdint>

BEGIN_NAMESPACE

enum class IProcessingUnit::ShiftType : std::uint32_t
{
    LSL = 0b00,
    LSR = 0b01,
    ASR = 0b10,
    ROR = 0b11,
};

END_NAMESPACE

#endif // !defined(SHIFTTYPE_H_INCLUDED_E2883020_9A39_4E83_B60D_1B97905DB68C)
