#ifndef INSTRUCTION_ARCH_H
#define INSTRUCTION_ARCH_H

#include <cstdint>

namespace arm_emu {

	enum class InstructionArch : std::uint32_t {
		ARM32,

		ARM64,
	};
}

#endif // INSTRUCTION_ARCH_H
