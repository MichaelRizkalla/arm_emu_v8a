#if !defined(PROGRAM_H_INCLUDED_ADB2A356_5D3D_4D20_93B4_83519D6E621D)
    #define PROGRAM_H_INCLUDED_ADB2A356_5D3D_4D20_93B4_83519D6E621D

    #include <Memory/IMemory.h>
    #include <Utility/UniqueRef.h>
    #include <cstdint>

namespace arm_emu {

    using ProgramSize = std::size_t;

    struct [[nodiscard]] Program : private std::pair< ProgramSize, UniqueRef< IMemory > > {
        constexpr Program() : std::pair< ProgramSize, UniqueRef< IMemory > >(0, nullptr) {
        }

        Program(UniqueRef< IMemory > program, ProgramSize size) :
            std::pair< ProgramSize, UniqueRef< IMemory > >(size, std::move(program)) {
        }

        ProgramSize GetProgramSize() const noexcept {
            return first;
        }

        IMemory* const GetProgram() const noexcept {
            return second.get();
        }

        void Reset(UniqueRef< IMemory > program, ProgramSize size) noexcept {
            assert(((program && size != 0) || (!program && size == 0)) &&
                   "Either reset to empty program or add proper program size!");
            first  = size;
            second = std::move(program);
        }

        bool Empty() const noexcept {
            return !static_cast< bool >(second);
        }
    };

    ARMEMU_API Program GetTestProgram(size_t idx);

} // namespace arm_emu

#endif // !defined(PROGRAM_H_INCLUDED_ADB2A356_5D3D_4D20_93B4_83519D6E621D)
