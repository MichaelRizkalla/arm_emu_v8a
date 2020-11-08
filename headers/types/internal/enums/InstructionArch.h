#pragma once
#include <cstdint>

namespace arm_emu {

	enum class InstructionArch : std::uint32_t {
		ARM32,

		ARM64,
	};
}