#pragma once
#include <cstdint>

namespace arm_emu {

	enum class ShiftType : std::uint32_t {
		LSL = 0b00,
		LSR = 0b01,
		ASR = 0b10,
		ROR = 0b11,
	};

}