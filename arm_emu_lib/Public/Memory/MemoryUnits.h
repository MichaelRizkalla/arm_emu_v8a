#if !defined(MEMORYUNITS_H_INCLUDED_DD1DAA69_024A_42C5_8BAA_983F66E30A6A)
    #define MEMORYUNITS_H_INCLUDED_DD1DAA69_024A_42C5_8BAA_983F66E30A6A

    #include <API/Api.h>

namespace arm_emu {

    using Byte_t     = std::ratio< 1, 1 >;
    using KiloByte_t = std::ratio< 1024, 1 >;
    using MegaByte_t = std::ratio< 1024 * 1024, 1 >;
    using GigaByte_t = std::ratio< 1024 * 1024 * 1024, 1 >;

    inline constexpr Byte_t     Byte {};
    inline constexpr KiloByte_t KiloByte {};
    inline constexpr MegaByte_t MegaByte {};
    inline constexpr GigaByte_t GigaByte {};

} // namespace arm_emu

#endif // !defined(MEMORYUNITS_H_INCLUDED_DD1DAA69_024A_42C5_8BAA_983F66E30A6A)
