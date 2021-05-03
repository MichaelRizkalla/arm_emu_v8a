#ifndef A64_FEATURES_H
#define A64_FEATURES_H

#include <cstdint>

namespace arm_emu {
    enum class Features : std::uint32_t
    {
        FEAT_DIT,
        EL3,
        FEAT_FP16,
        FEAT_PAN,
        FEAT_SSBS,
        FEAT_MTE,
        FEAT_UAO,
        FEAT_BTI
    };
} // namespace arm_emu

#endif // A64_FEATURES_H
