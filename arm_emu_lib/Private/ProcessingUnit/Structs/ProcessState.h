#if !defined(PROCESSSTATE_H_INCLUDED_1B2DEC33_C58F_49AC_944B_A8CB19FC5A34)
    #define PROCESSSTATE_H_INCLUDED_1B2DEC33_C58F_49AC_944B_A8CB19FC5A34

    #include <API/Api.h>
    #include <Utility/Bitset.h>
    #include <cstdint>

BEGIN_NAMESPACE

struct [[nodiscard]] IProcessingUnit::ProcessState {
    std::bitset< 1 > N { 0 };
    std::bitset< 1 > Z { 0 };
    std::bitset< 1 > C { 0 };
    std::bitset< 1 > V { 0 };
    std::bitset< 1 > D { 0 };
    std::bitset< 1 > A { 0 };
    std::bitset< 1 > I { 0 };
    std::bitset< 1 > F { 0 };
    std::bitset< 1 > PAN { 0 };
    std::bitset< 1 > UAO { 0 };
    std::bitset< 1 > DIT { 0 };
    std::bitset< 1 > TCO { 0 };
    std::bitset< 2 > BTYPE { 0 };
    std::bitset< 1 > SS { 0 };
    std::bitset< 1 > IL { 0 };
    std::bitset< 2 > EL { 0 };
    std::bitset< 1 > mRW { 0 };
    std::bitset< 1 > SP { 0 };
    std::bitset< 1 > Q { 0 };
    std::bitset< 4 > GE { 0 };
    std::bitset< 1 > SSBS { 0 };
    std::bitset< 8 > IT { 0 };
    std::bitset< 1 > J { 0 };
    std::bitset< 1 > T { 0 };
    std::bitset< 1 > E { 0 };
    std::bitset< 5 > M { 5 };
};

END_NAMESPACE

#endif // !defined(PROCESSSTATE_H_INCLUDED_1B2DEC33_C58F_49AC_944B_A8CB19FC5A34)
