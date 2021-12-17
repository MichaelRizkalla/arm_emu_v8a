#if !defined(PROGRAM_H_INCLUDED_ADB2A356_5D3D_4D20_93B4_83519D6E621D)
    #define PROGRAM_H_INCLUDED_ADB2A356_5D3D_4D20_93B4_83519D6E621D

    #include <Memory/IMemory.h>
    #include <Utility/UniqueRef.h>
    #include <cstdint>

namespace arm_emu {

    using ProgramSize = std::size_t;
    using Program     = std::pair< ProgramSize, IMemory* >;

    ARMEMU_API std::size_t GetLostMem() noexcept;

    ARMEMU_API UniqueRef< IMemory > GetProgram();

} // namespace arm_emu

#endif // !defined(PROGRAM_H_INCLUDED_ADB2A356_5D3D_4D20_93B4_83519D6E621D)
