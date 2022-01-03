#if !defined(PROGRAM_H_INCLUDED_ADB2A356_5D3D_4D20_93B4_83519D6E621D)
    #define PROGRAM_H_INCLUDED_ADB2A356_5D3D_4D20_93B4_83519D6E621D

    #include <Memory/IMemory.h>
    #include <Utility/UniqueRef.h>
    #include <cstdint>

namespace arm_emu {

    struct [[nodiscard]] Program : private std::pair< IMemory::Address, UniqueRef< IMemory > > {
        using EntryPoint = IMemory::Address;

        constexpr Program() : std::pair< EntryPoint, UniqueRef< IMemory > >(0, nullptr) {
        }

        Program(UniqueRef< IMemory > program, EntryPoint entry) :
            std::pair< EntryPoint, UniqueRef< IMemory > >(entry, std::move(program)) {
        }

        EntryPoint GetEntryPoint() const noexcept {
            return first;
        }

        IMemory* const GetProgram() const noexcept {
            return second.get();
        }

        void Reset(UniqueRef< IMemory > program, EntryPoint entry) noexcept {
            first  = entry;
            second = std::move(program);
        }

        bool Empty() const noexcept {
            return !static_cast< bool >(second);
        }
    };

    namespace test {
        ARMEMU_API Program GetSampleProgram(size_t idx);
    }

} // namespace arm_emu

#endif // !defined(PROGRAM_H_INCLUDED_ADB2A356_5D3D_4D20_93B4_83519D6E621D)
