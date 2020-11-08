#pragma once
#include <cstdint>

namespace arm_emu {
	enum class ExtendType : std::uint32_t {
		UXTB = 0b000,
		UXTH = 0b001,
		UXTW = 0b010,
		UXTX = 0b011,
		SXTB = 0b100,
		SXTH = 0b101,
		SXTW = 0b110,
		SXTX = 0b111,
	};
}