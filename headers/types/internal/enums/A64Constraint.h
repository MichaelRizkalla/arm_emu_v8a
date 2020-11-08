#pragma once
#include <cstdint>

namespace arm_emu {

	enum class Constraint : std::uint32_t {
		NONE, 
		UNKNOWN,
		UNDEF,
		NOP,
	};

}