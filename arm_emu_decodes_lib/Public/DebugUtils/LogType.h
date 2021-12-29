#if !defined(LOGTYPE_H_INCLUDED_D2D11EDF_2BC9_4B13_BCDF_F35719EB145A)
    #define LOGTYPE_H_INCLUDED_D2D11EDF_2BC9_4B13_BCDF_F35719EB145A

    #include <API/Api.h>
    #include <Utility/EnumBitfield.h>
    #include <cstdint>

namespace arm_emu {

    enum class LogType : std::uint32_t
    {
        None = 0x00000000,

        /* Log entities */
        Allocation   = 0x00000001,
        Deallocation = 0x00000002,
        Construction = 0x00000004,
        Destruction  = 0x00000008,

        /* Emulation-related log */
        MMU         = 0x00000010,
        Instruction = 0x00000020,

        /* Compiler related log */
        Compiler           = 0x00000100, // Track the compiler and its commands
        SymbolResolving    = 0x00000200,
        ParsingObjectFile  = 0x00000400, // Too verbose
        ParsingDisassembly = 0x00000800, // Too verbose
        CompileOutput      = 0x00001000,
        ELFSummary         = 0x00002000, // Too verbose

        Other          = 0x80000000,
        ObjectLifetime = Construction | Destruction,
        MemoryResource = Allocation | Deallocation,
        Compilation =
            Compiler | SymbolResolving | CompileOutput, /* | ParsingObjectFile | ParsingDisassembly | ELFSummary */
        All = 0xFFFFFFFF,
    };

} // namespace arm_emu

#endif // !defined(LOGTYPE_H_INCLUDED_D2D11EDF_2BC9_4B13_BCDF_F35719EB145A)
