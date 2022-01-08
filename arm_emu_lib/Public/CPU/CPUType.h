#if !defined(CPUTYPE_H_INCLUDED_5889E05C_14A4_47FA_B7F0_FAACCC4D3E5D)
    #define CPUTYPE_H_INCLUDED_5889E05C_14A4_47FA_B7F0_FAACCC4D3E5D

    #include <API/Api.h>
    #include <cstdint>

namespace arm_emu {

    /// @brief CPU Type to be created
    enum class CPUType : std::uint8_t
    {
        A64,

        A32, /* Unavailable */

        T32, /* Unavailable */
    };

} // namespace arm_emu

#endif // !defined(CPUTYPE_H_INCLUDED_5889E05C_14A4_47FA_B7F0_FAACCC4D3E5D)
