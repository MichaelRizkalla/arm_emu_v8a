#if !defined(LOGSTATE_H_INCLUDED_557A80FE_4E96_4132_863A_53F3AD39C24C)
    #define LOGSTATE_H_INCLUDED_557A80FE_4E96_4132_863A_53F3AD39C24C

    #include <API/Api.h>
    #include <cstdint>

namespace arm_emu {

    enum class LogLevel : std::uint32_t
    {
        Trace,
        Normal,
        None,
    };

} // namespace arm_emu

#endif // !defined(LOGSTATE_H_INCLUDED_557A80FE_4E96_4132_863A_53F3AD39C24C)
