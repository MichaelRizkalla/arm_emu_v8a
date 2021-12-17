#if !defined(LOGTYPE_H_INCLUDED_D2D11EDF_2BC9_4B13_BCDF_F35719EB145A)
    #define LOGTYPE_H_INCLUDED_D2D11EDF_2BC9_4B13_BCDF_F35719EB145A

    #include <API/Api.h>
    #include <cstdint>

namespace arm_emu {

    enum class LogType : std::uint32_t
    {
        None         = 0x00000000,
        Allocation   = 0x00000001,
        Deallocation = 0x00000002,
        Construction = 0x00000004,
        Destruction  = 0x00000008,
        Other        = 0x00000010,

        ObjectLifetime = Construction | Destruction,
        Memory         = Allocation | Deallocation,

        All = 0xFFFFFFFF,
    };

} // namespace arm_emu

#endif // !defined(LOGTYPE_H_INCLUDED_D2D11EDF_2BC9_4B13_BCDF_F35719EB145A)
