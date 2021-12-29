#if !defined(WARNINGOPTIONS_H_INCLUDED_BA24B066_7AA4_48F0_A739_D4905648CDF8)
    #define WARNINGOPTIONS_H_INCLUDED_BA24B066_7AA4_48F0_A739_D4905648CDF8

    #include <API/Api.h>
    #include <cstdint>

namespace arm_emu {

    enum class WarningOptions : std::uint32_t
    {
        Wnone,
        Wall,
        Wextra,
        Wpendantic,
        Weverything,

        Werror,
    };

} // namespace arm_emu

#endif // !defined(WARNINGOPTIONS_H_INCLUDED_BA24B066_7AA4_48F0_A739_D4905648CDF8)
