#if !defined(IMEMORY_H_INCLUDED_FCF65B63_594B_42BF_BF51_CC8D00442364)
    #define IMEMORY_H_INCLUDED_FCF65B63_594B_42BF_BF51_CC8D00442364

    #include <API/Api.h>
    #include <DebugUtils/Object.h>
    #include <cassert>
    #include <concepts>
    #include <cstdint>
    #include <ratio>
    #include <vector>

namespace arm_emu {

    using Byte_t     = std::ratio< 1, 1 >;
    using KiloByte_t = std::ratio< 1024, 1 >;
    using MegaByte_t = std::ratio< 1024 * 1024, 1 >;
    using GigaByte_t = std::ratio< 1024 * 1024 * 1024, 1 >;

    inline constexpr Byte_t     Byte {};
    inline constexpr KiloByte_t KiloByte {};
    inline constexpr MegaByte_t MegaByte {};
    inline constexpr GigaByte_t GigaByte {};

    template < class Unit >
    struct [[nodiscard]] DataBlock {
        constexpr DataBlock() = default;
        constexpr DataBlock(size_t size, Unit initVal) : m_data(std::pmr::vector< Unit >(size, initVal)) {
        }
        constexpr DataBlock(std::initializer_list< Unit > lst) : m_data(std::move(lst)) {
        }
        constexpr DataBlock(size_t size, const Unit* const ptr) : m_data(std::pmr::vector< Unit >(size)) {
            std::copy(ptr, ptr + size, m_data.data());
        }

        [[nodiscard]] constexpr decltype(auto) At(size_t loc) const {
            assert(loc < m_data.size());
            return m_data.at(loc);
        }

        [[nodiscard]] constexpr decltype(auto) At(size_t loc) {
            assert(loc < m_data.size());
            return m_data.at(loc);
        }

        [[nodiscard]] constexpr decltype(auto) Get() const noexcept {
            return m_data;
        }

        [[nodiscard]] constexpr decltype(auto) Get() noexcept {
            return m_data;
        }

        [[nodiscard]] constexpr decltype(auto) Data() const noexcept {
            return m_data.data();
        }

        [[nodiscard]] constexpr decltype(auto) Data() noexcept {
            return m_data.data();
        }

        [[nodiscard]] constexpr decltype(auto) Begin() const noexcept {
            return m_data.begin();
        }

        [[nodiscard]] constexpr decltype(auto) Begin() noexcept {
            return m_data.begin();
        }

        [[nodiscard]] constexpr decltype(auto) End() const noexcept {
            return m_data.end();
        }

        [[nodiscard]] constexpr decltype(auto) End() noexcept {
            return m_data.end();
        }

        [[nodiscard]] constexpr size_t Size() const noexcept {
            return m_data.size();
        }

      private:
        std::pmr::vector< Unit > m_data {};
    };

    template < class Unit >
    [[nodiscard]] constexpr decltype(auto) begin(DataBlock< Unit >& block) noexcept {
        return block.Begin();
    }

    template < class Unit >
    [[nodiscard]] constexpr decltype(auto) begin(const DataBlock< Unit >& block) noexcept {
        return block.Begin();
    }

    template < class Unit >
    [[nodiscard]] constexpr decltype(auto) end(DataBlock< Unit >& block) noexcept {
        return block.End();
    }

    template < class Unit >
    [[nodiscard]] constexpr decltype(auto) end(const DataBlock< Unit >& block) noexcept {
        return block.End();
    }

    class MemoryWatcher;

    class [[nodiscard]] IMemory : public Object {
      public:
        using Address  = std::uint64_t;
        using DataUnit = std::uint32_t;

        static constexpr DataUnit Empty_data_unit = DataUnit { 0 };

        IMemory(std::string name = "IMemory") : Object(std::move(name)) {
        }
        IMemory(IMemory&&) = default;
        IMemory& operator=(IMemory&&) = default;
        virtual ~IMemory()            = default;

        IMemory(const IMemory&) = delete;
        IMemory& operator=(const IMemory&) = delete;

        [[nodiscard]] virtual DataUnit              Read(Address address) noexcept                               = 0;
        [[nodiscard]] virtual DataBlock< DataUnit > ReadBlock(Address start, std::uint64_t dataUnitCount)        = 0;
        [[nodiscard]] virtual void                  Write(Address address, DataUnit data)                        = 0;
        [[nodiscard]] virtual void                  WriteBlock(Address start, const DataBlock< DataUnit >& data) = 0;
        [[nodiscard]] virtual Address               Size() const noexcept                                        = 0;
        [[nodiscard]] virtual const MemoryWatcher&  GetMemoryWatcher() const noexcept                            = 0;
    };

    namespace literals {
        [[nodiscard]] constexpr IMemory::Address operator""_B(std::uint64_t value) noexcept {
            return static_cast< std::uint64_t >((static_cast< double >(value) / sizeof(IMemory::DataUnit)) *
                                                Byte_t::num / Byte_t::den);
        }
        [[nodiscard]] constexpr IMemory::Address operator""_KB(std::uint64_t value) noexcept {
            return static_cast< std::uint64_t >((static_cast< double >(value) / sizeof(IMemory::DataUnit)) *
                                                KiloByte_t::num / KiloByte_t::den);
        }
        [[nodiscard]] constexpr IMemory::Address operator""_MB(std::uint64_t value) noexcept {
            return static_cast< std::uint64_t >((static_cast< double >(value) / sizeof(IMemory::DataUnit)) *
                                                MegaByte_t::num / MegaByte_t::den);
        }
        [[nodiscard]] constexpr IMemory::Address operator""_GB(std::uint64_t value) noexcept {
            return static_cast< std::uint64_t >((static_cast< double >(value) / sizeof(IMemory::DataUnit)) *
                                                GigaByte_t::num / GigaByte_t::den);
        }
    } // namespace literals

    using namespace literals;

} // namespace arm_emu

#endif // !defined(IMEMORY_H_INCLUDED_FCF65B63_594B_42BF_BF51_CC8D00442364)
