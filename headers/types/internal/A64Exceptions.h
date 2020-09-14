#pragma once
#include <cstdint>
#include <limits>
#include <type_traits>

namespace arm_emu {

	enum class ExceptionLevel : std::uint32_t {
		EL0 = 0b00,
		EL1 = 0b01,
		EL2 = 0b10,
		EL3 = 0b11,

		ARM_EMU_COUNT,

		Undefined = std::numeric_limits< std::underlying_type_t< ExceptionLevel > >::max(),
	};

}