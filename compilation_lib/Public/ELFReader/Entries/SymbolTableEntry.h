#if !defined(SYMBOLTABLEENTRY_H_INCLUDED_B1CECB9E_25D0_4B4E_BC9D_07AAA9B08D08)
    #define SYMBOLTABLEENTRY_H_INCLUDED_B1CECB9E_25D0_4B4E_BC9D_07AAA9B08D08

// Credits: https://github.com/lefticus/cpp_box/blob/master/include/cpp_box/elf_reader.hpp

    #include <API/Api.h>
    #include <ELFReader/Entries/BaseEntry.h>
    #include <cstdint>
    #include <string_view>

BEGIN_NAMESPACE

struct [[nodiscard]] SymbolTableEntry final : public BaseEntry {

    enum class Entry : std::uint32_t
    {
        Name,
        Value,
        Size,
        Info,
        Other,
        SectionHeaderTableIndex,
    };

    SymbolTableEntry(bool is32Bits, bool isLittleEndian, std::basic_string_view< std::uint8_t > data);

    [[nodiscard]] std::size_t GetEntryOffset(std::uint32_t entry) const noexcept final;

    [[nodiscard]] std::size_t GetEntrySize(std::uint32_t entry) const noexcept final;

    [[nodiscard]] std::uint64_t Read(std::uint32_t entry) const noexcept final;

    [[nodiscard]] std::uint64_t ReadNameOffset() const noexcept;

    [[nodiscard]] std::string_view
        ReadNameFrom(const std::basic_string_view< std::uint8_t > stringTable) const noexcept;

    [[nodiscard]] std::uint64_t ReadValue() const noexcept;

    [[nodiscard]] std::uint64_t ReadSize() const noexcept;

    [[nodiscard]] std::uint64_t ReadSectionHeaderTableIndex() const noexcept;

    [[nodiscard]] static std::size_t GetSymbolTableEntrySize(const bool is32Bits) noexcept;

  private:
    bool                                   m_is32Bits;
    bool                                   m_isLittleEndian;
    std::basic_string_view< std::uint8_t > m_data;
};

END_NAMESPACE

#endif // !defined(SYMBOLTABLEENTRY_H_INCLUDED_B1CECB9E_25D0_4B4E_BC9D_07AAA9B08D08)
