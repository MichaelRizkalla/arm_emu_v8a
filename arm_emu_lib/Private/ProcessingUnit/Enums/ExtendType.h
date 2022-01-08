#if !defined(EXTENDTYPE_H_INCLUDED_4CE49E2A_877D_4BF6_BDF1_5EA910EFF051)
    #define EXTENDTYPE_H_INCLUDED_4CE49E2A_877D_4BF6_BDF1_5EA910EFF051

    #include <API/Api.h>
    #include <cstdint>

BEGIN_NAMESPACE

enum class IProcessingUnit::ExtendType : std::uint32_t
{
    UXTB = 0b000,
    UXTH = 0b001,
    UXTW = 0b010,
    UXTX = 0b011,
    SXTB = 0b100,
    SXTH = 0b101,
    SXTW = 0b110,
    SXTX = 0b111,
};

END_NAMESPACE

#endif // !defined(EXTENDTYPE_H_INCLUDED_4CE49E2A_877D_4BF6_BDF1_5EA910EFF051)
