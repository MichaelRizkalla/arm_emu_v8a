// Based on C4.1.1 A64 instruction set encoding

#pragma once
#include <utilities.hpp>

namespace arm_emu {

enum class ReservedGroupA64 : std::uint32_t {
  // Reserved
  UDP,

  ARM_EMU_COUNT,

  Undefined = std::numeric_limits< std::underlying_type_t< ReservedGroupA64 > >::max(),
};

static Table< ReservedGroupA64, enum_size_v< ReservedGroupA64 > > ReservedGroupA64Table { { {
    { 0b1111'1111'1111'1111'0000'0000'0000'0000, 0b0000'0000'0000'0000'0000'0000'0000'0000, ReservedGroupA64::UDP },
    } } };

} // namespace arm_emu