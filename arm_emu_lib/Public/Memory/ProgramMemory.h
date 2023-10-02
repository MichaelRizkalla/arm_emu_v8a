#if !defined(PROGRAMMEMORY_H_INCLUDED_0734C686_BE75_44AA_89E3_917FE36E7F71)
    #define PROGRAMMEMORY_H_INCLUDED_0734C686_BE75_44AA_89E3_917FE36E7F71

    #include <API/Api.h>
    #include <Memory/IMemory.h>
    #include <Utility/UniqueRef.h>

namespace arm_emu {

      class [[nodiscard]] ARMEMU_API ProgramMemory final : public IMemory {
      private:
        static constexpr size_t      Default_memory_size = 1_KB;
        static constexpr const char* Default_name        = "ProgramMemory";

      public:
        ProgramMemory(std::string name = Default_name, Address addressableSize = Default_memory_size);

        ProgramMemory(ProgramMemory&&) noexcept;
        ProgramMemory& operator=(ProgramMemory&&) noexcept;
        virtual ~ProgramMemory();

        ProgramMemory(const ProgramMemory&) = delete;
        ProgramMemory& operator=(const ProgramMemory&) = delete;

        [[nodiscard]] DataUnit              Read(Address address) const noexcept final;
        [[nodiscard]] DataBlock< DataUnit > ReadBlock(Address start, std::uint64_t dataUnitCount) const final;

        void Write(Address address, DataUnit data) noexcept final;
        void WriteBlock(Address start, const DataBlock< DataUnit >& data) final;

        [[nodiscard]] Address Size() const noexcept final;

      private:
        [[nodiscard]] const MemoryWatcher& GetMemoryWatcher() const noexcept final;

        class Impl;
        UniqueRef< Impl > m_memory;

        template < class ImplDetail >
        [[nodiscard]] static UniqueRef< Impl > ConstructMemory(Address arg, ImplDetail detail);
    };

} // namespace arm_emu

#endif // !defined(PROGRAMMEMORY_H_INCLUDED_0734C686_BE75_44AA_89E3_917FE36E7F71)
