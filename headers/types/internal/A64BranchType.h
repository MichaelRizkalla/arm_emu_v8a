#pragma once
#include <cstdint>

namespace arm_emu {

	enum class BranchType : std::uint32_t {
		DIRCALL, // Direct Branch with link
		INDCALL, // Indirect Branch with link
		ERET, // Exception return (indirect)
		DBGEXIT, // Exit from Debug state
		RET, // Indirect branch with function return hint
		DIR, // Direct branch
		INDIR, // Indirect branch
		EXCEPTION, // Exception entry
		RESET, // Reset
		UNKNOWN, // Other 
	};

}