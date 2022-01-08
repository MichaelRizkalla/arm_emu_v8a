#if !defined(WARNINGLEVEL_H_INCLUDED_991AF3E6_4B66_4B60_B500_6C2467431919)
    #define WARNINGLEVEL_H_INCLUDED_991AF3E6_4B66_4B60_B500_6C2467431919

    #include <API/Api.h>
    #include <cstdint>

namespace arm_emu {

    enum class OptimizationLevel : std::uint32_t
    {
        O0,
        O1,
        O2,
        O3,
    };

} // namespace arm_emu

#endif // !defined(WARNINGLEVEL_H_INCLUDED_991AF3E6_4B66_4B60_B500_6C2467431919)
