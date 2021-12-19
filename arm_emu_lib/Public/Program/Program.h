#if !defined(PROGRAM_H_INCLUDED_ADB2A356_5D3D_4D20_93B4_83519D6E621D)
    #define PROGRAM_H_INCLUDED_ADB2A356_5D3D_4D20_93B4_83519D6E621D

    #include <Memory/IMemory.h>
    #include <Utility/UniqueRef.h>
    #include <cstdint>

namespace arm_emu {

    using ProgramSize = std::size_t;

    struct [[nodiscard]] Program : private std::pair< ProgramSize, UniqueRef< IMemory > > {
        Program(UniqueRef< IMemory > program, ProgramSize size) :
            std::pair< ProgramSize, UniqueRef< IMemory > >(size, std::move(program)) {
        }

        ProgramSize GetProgramSize() const noexcept {
            return first;
        }

        IMemory* const GetProgram() const noexcept {
            return second.get();
        }
    };

    ARMEMU_API Program GetTestProgram();

} // namespace arm_emu

#endif // !defined(PROGRAM_H_INCLUDED_ADB2A356_5D3D_4D20_93B4_83519D6E621D)
