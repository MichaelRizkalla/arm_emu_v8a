#if !defined(CONDITIONCODE_H_INCLUDED_5583991B_26A3_4EBA_BA82_4F360C4BFF48)
    #define CONDITIONCODE_H_INCLUDED_5583991B_26A3_4EBA_BA82_4F360C4BFF48
// Condition Code based on C1.2.4 Section

    #include <API/Api.h>
    #include <cstdint>

BEGIN_NAMESPACE

/// <summary>
/// Condition code of A64 ISA for integers and floating-point
/// </summary>
/// <remarks>
/// floating-point unordered means at least one NaN operand
/// </rema
enum class IProcessingUnit::ConditionCode : std::uint32_t
{
    EQ = 0b0000, // Z == 1				INT(equal) / FP(equal)
    NE = 0b0001, // Z == 0				INT(not equal)						/ FP(not
                 // equal or unordered)
    CS = 0b0010, // C == 1				INT(carry set)						/ FP(greater than,
                 // equal or unordered)
    HS = 0b0010,
    CC = 0b0011, // C == 0				INT(carry clear)					/
                 // FP(less than)
    LO = 0b0011,
    MI = 0b0100, // N == 1				INT(minus, negative)				/ FP(less than)
    PL = 0b0101, // N == 0				INT(plus, positive or zero)			/ FP(greater than,
                 // equal or unordered)
    VS = 0b0110, // V == 1				INT(overflow)						/
                 // FP(unordered)
    VC = 0b0111, // V == 0				INT(no overflow)					/
                 // FP(ordered)
    HI = 0b1000, // C == 1 && Z == 0	INT(unsigned higher)				/ FP(greater than or unordered)
    LS = 0b1001, // C == 0 || Z == 1	INT(unsigned lower or same)			/ FP(less than or equal)
    GE = 0b1010, // N == V				INT(signed greater than or equal)	/ FP(greater than or
                 // equal)
    LT = 0b1011, // N != V				INT(signed less than)				/ FP(less than
                 // or unordered)
    GT = 0b1100, // Z == 0 && N == V	INT(signed greater than)			/ FP(greater than)
    LE = 0b1101, // Z == 1 || N == V	INT(signed less than or equal)		/ FP(less than, equal or unordered)
    AL = 0b1110, // Any				INT(Always)							/
                 // FP(Always)
    NV = 0b1111  // Reserved to provide valid disassembly, similar behaviour to AL
};

END_NAMESPACE

#endif // !defined(CONDITIONCODE_H_INCLUDED_5583991B_26A3_4EBA_BA82_4F360C4BFF48)
