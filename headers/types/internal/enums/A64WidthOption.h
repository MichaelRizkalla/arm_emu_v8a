#ifndef A64_WIDTH_OPTION_H
#define A64_WIDTH_OPTION_H

#include <cstdint>

namespace arm_emu {
    enum class WidthOption : std::uint32_t {
        W,

        X,
    };
}

#endif // A64_WIDTH_OPTION_H
