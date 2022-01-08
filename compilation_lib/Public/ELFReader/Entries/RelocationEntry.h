#if !defined(RELOCATIONENTRY_H_INCLUDED_49974170_26DA_470F_B0E2_90AE9F9A890B)
    #define RELOCATIONENTRY_H_INCLUDED_49974170_26DA_470F_B0E2_90AE9F9A890B

// Credits: https://github.com/lefticus/cpp_box/blob/master/include/cpp_box/elf_reader.hpp

    #include <API/Api.h>
    #include <ELFReader/Entries/BaseEntry.h>
    #include <cstdint>
    #include <string_view>

namespace arm_emu {

    struct [[nodiscard]] ARMEMU_API RelocationEntry final : public BaseEntry {

        enum class Entry : std::uint32_t
        {
            FileOffset = 0,
            Info       = 1,
        };

        RelocationEntry(bool is32Bits, bool isLittleEndian, std::basic_string_view< std::uint8_t > data);

        [[nodiscard]] std::size_t GetEntryOffset(std::uint32_t entry) const noexcept final;

        [[nodiscard]] std::size_t GetEntrySize(std::uint32_t entry) const noexcept final;

        [[nodiscard]] std::uint64_t Read(std::uint32_t entry) const noexcept final;

        [[nodiscard]] std::uint64_t ReadFileOffset() const noexcept;

        [[nodiscard]] std::uint64_t ReadInfo() const noexcept;

        [[nodiscard]] std::uint64_t ReadSymbol() const noexcept;

        [[nodiscard]] std::uint64_t GetType() const noexcept;

        [[nodiscard]] static std::size_t GetRelocationEntrySize(const bool is32Bits) noexcept;

      private:
        bool                                   m_is32Bits;
        bool                                   m_isLittleEndian;
        std::basic_string_view< std::uint8_t > m_data;
    };

} // namespace arm_emu

#endif // !defined(RELOCATIONENTRY_H_INCLUDED_49974170_26DA_470F_B0E2_90AE9F9A890B)
