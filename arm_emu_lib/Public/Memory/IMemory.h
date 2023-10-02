#if !defined(IMEMORY_H_INCLUDED_FCF65B63_594B_42BF_BF51_CC8D00442364)
    #define IMEMORY_H_INCLUDED_FCF65B63_594B_42BF_BF51_CC8D00442364

    #include <API/Api.h>
    #include <DebugUtils/Object.h>
    #include <Memory/DataUnit.h>
    #include <Memory/MemoryUnits.h>
    #include <cassert>
    #include <concepts>
    #include <cstdint>
    #include <ratio>
    #include <vector>

namespace arm_emu {

    class MemoryWatcher;

    class [[nodiscard]] IMemory : public Object {
      public:
        using Address  = std::uint64_t;
        using DataUnit = std::uint32_t;

        static constexpr DataUnit Empty_data_unit = DataUnit { 0 };

        IMemory(std::string name = "IMemory") : Object(std::move(name)) {
        }
        IMemory(IMemory&&)            = default;
        IMemory& operator=(IMemory&&) = default;
        virtual ~IMemory()            = default;

        IMemory(const IMemory&)            = delete;
        IMemory& operator=(const IMemory&) = delete;

        [[nodiscard]] virtual DataUnit              Read(Address address) const noexcept                         = 0;
        [[nodiscard]] virtual DataBlock< DataUnit > ReadBlock(Address start, std::uint64_t dataUnitCount) const  = 0;
        virtual void                                Write(Address address, DataUnit data)                        = 0;
        virtual void                                WriteBlock(Address start, const DataBlock< DataUnit >& data) = 0;
        [[nodiscard]] virtual Address               Size() const noexcept                                        = 0;
        [[nodiscard]] virtual const MemoryWatcher&  GetMemoryWatcher() const noexcept                            = 0;
    };

    namespace literals {
        [[nodiscard]] constexpr IMemory::Address operator""_B(unsigned long long value) noexcept {
            return static_cast< std::uint64_t >((static_cast< double >(value) / sizeof(IMemory::DataUnit)) *
                                                Byte_t::num / Byte_t::den);
        }
        [[nodiscard]] constexpr IMemory::Address operator""_KB(unsigned long long value) noexcept {
            return static_cast< std::uint64_t >((static_cast< double >(value) / sizeof(IMemory::DataUnit)) *
                                                KiloByte_t::num / KiloByte_t::den);
        }
        [[nodiscard]] constexpr IMemory::Address operator""_MB(unsigned long long value) noexcept {
            return static_cast< std::uint64_t >((static_cast< double >(value) / sizeof(IMemory::DataUnit)) *
                                                MegaByte_t::num / MegaByte_t::den);
        }
        [[nodiscard]] constexpr IMemory::Address operator""_GB(unsigned long long value) noexcept {
            return static_cast< std::uint64_t >((static_cast< double >(value) / sizeof(IMemory::DataUnit)) *
                                                GigaByte_t::num / GigaByte_t::den);
        }
    } // namespace literals

    using namespace literals;

} // namespace arm_emu

#endif // !defined(IMEMORY_H_INCLUDED_FCF65B63_594B_42BF_BF51_CC8D00442364)
