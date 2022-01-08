#if !defined(ABORTMODE_H_INCLUDED_9029BD16_245A_47D7_AA23_5614F5AFEABA)
    #define ABORTMODE_H_INCLUDED_9029BD16_245A_47D7_AA23_5614F5AFEABA

    #include <API/Api.h>
    #include <cstdint>

BEGIN_NAMESPACE

enum class IProcessingUnit::AbortMode : std::uint32_t
{
    USER       = 0b10000,
    FIQ        = 0b10001,
    IRQ        = 0b10010,
    SUPERVISOR = 0b10011,
    ABORT      = 0b10111,
    UNDEFINED  = 0b11011,
    SYSTEM     = 0b11111,

    RESERVED, // Illegal return event
};

END_NAMESPACE

#endif // !defined(ABORTMODE_H_INCLUDED_9029BD16_245A_47D7_AA23_5614F5AFEABA)
