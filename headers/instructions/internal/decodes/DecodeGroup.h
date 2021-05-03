// Based on C4.1 A64 instruction set encoding

#pragma once
#include <utilities.hpp>

namespace arm_emu {

enum class DecodeGroupA64 : std::uint32_t {
    Reserved,

    ScalableVectorExtension,

    DataProcessingImmediate,

    Branch_Exception_System,

    Load_Store,

    DataProcessingRegister,

    DataProcessing_ScalarFloatingPoint_AdvancedSIMD,

    ARM_EMU_COUNT,

    Undefined = std::numeric_limits< std::underlying_type_t< DecodeGroupA64 > >::max(),
};

struct DecodeGroupA64Entry {
    std::uint8_t   code;
    std::uint8_t   expected;
    DecodeGroupA64 type;
};

static std::array< DecodeGroupA64Entry, 7 > DecodeGroupA64Table { {
    { 0b0000'1111, 0b0000'0000, DecodeGroupA64::Reserved },
    { 0b0000'1111, 0b0000'0010, DecodeGroupA64::ScalableVectorExtension },
    { 0b0000'1110, 0b0000'1000, DecodeGroupA64::DataProcessingImmediate },
    { 0b0000'1110, 0b0000'1010, DecodeGroupA64::Branch_Exception_System },
    { 0b0000'0111, 0b0000'0101, DecodeGroupA64::DataProcessingRegister },
    { 0b0000'0111, 0b0000'0111, DecodeGroupA64::DataProcessing_ScalarFloatingPoint_AdvancedSIMD },
    { 0b0000'0101, 0b0000'0100, DecodeGroupA64::Load_Store },
} };
} // namespace arm_emu