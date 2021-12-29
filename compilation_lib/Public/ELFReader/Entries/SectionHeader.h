#if !defined(SECTIONHEADER_H_INCLUDED_5C4B0C8F_7C7A_48D8_8568_016B305D195B)
    #define SECTIONHEADER_H_INCLUDED_5C4B0C8F_7C7A_48D8_8568_016B305D195B

// Credits: https://github.com/lefticus/cpp_box/blob/master/include/cpp_box/elf_reader.hpp

    #include <API/Api.h>
    #include <ELFReader/Entries/BaseEntry.h>
    #include <ELFReader/Entries/RelocationEntry.h>
    #include <ELFReader/Entries/RelocationAddendEntry.h>
    #include <ELFReader/Entries/SymbolTableEntry.h>
    #include <cstdint>
    #include <string_view>

BEGIN_NAMESPACE

struct [[nodiscard]] SectionHeader final : public BaseEntry {

    enum class Entry : std::uint32_t
    {
        Name,
        Type,
        Flags,
        Address,
        Offset,
        Size,
        Link,
        Info,
        AddressAlign,
        EntrySize,
    };

    enum class EntryType : std::uint32_t
    {
        SHT_NULL          = 0x00,       // Section header table entry unused
        SHT_PROGBITS      = 0x01,       // Program data
        SHT_SYMTAB        = 0x02,       // Symbol table
        SHT_STRTAB        = 0x03,       // String table
        SHT_RELA          = 0x04,       // Relocation entries with addends
        SHT_HASH          = 0x05,       // Symbol hash table
        SHT_DYNAMIC       = 0x06,       // Dynamic linking information
        SHT_NOTE          = 0x07,       // Notes
        SHT_NOBITS        = 0x08,       // Program space with no data (bss)
        SHT_REL           = 0x09,       // Relocation entries, no addends
        SHT_SHLIB         = 0x0A,       // Reserved
        SHT_DYNSYM        = 0x0B,       // Dynamic linker symbol table
        SHT_INIT_ARRAY    = 0x0E,       // Array of constructors
        SHT_FINI_ARRAY    = 0x0F,       // Array of destructors
        SHT_PREINIT_ARRAY = 0x10,       // Array of pre-constructors
        SHT_GROUP         = 0x11,       // Section group
        SHT_SYMTAB_SHNDX  = 0x12,       // Extended section indices
        SHT_NUM           = 0x13,       // Number of defined types.
        SHT_LOOS          = 0x60000000, // Start OS-specific.
        Unknown
    };

    SectionHeader(bool is32Bits, bool isLittleEndian, std::basic_string_view< std::uint8_t > fullData,
                  std::basic_string_view< std::uint8_t > data);

    [[nodiscard]] std::size_t GetEntryOffset(std::uint32_t entry) const noexcept final;

    [[nodiscard]] std::size_t GetEntrySize(std::uint32_t entry) const noexcept final;

    [[nodiscard]] std::uint64_t Read(std::uint32_t entry) const noexcept final;

    [[nodiscard]] std::uint64_t ReadNameOffset() const noexcept;

    [[nodiscard]] std::string_view
        ReadNameFrom(const std::basic_string_view< std::uint8_t > stringTable) const noexcept;

    [[nodiscard]] EntryType ReadType() const noexcept;

    [[nodiscard]] std::uint64_t ReadOffset() const noexcept;

    [[nodiscard]] std::uint64_t ReadSize() const noexcept;

    [[nodiscard]] std::basic_string_view< std::uint8_t > GetSectionData() const noexcept;

    [[nodiscard]] std::size_t GetRelocationEntryCount() const noexcept;

    [[nodiscard]] RelocationEntry GetRelocationEntry(const std::size_t index) const noexcept;

    [[nodiscard]] BaseEntry::Iterable< RelocationEntry > GetRelocationEntries() const noexcept;

    [[nodiscard]] std::size_t GetRelocationAddendEntryCount() const noexcept;

    [[nodiscard]] RelocationAddendEntry GetRelocationAddendEntry(const std::size_t index) const noexcept;

    [[nodiscard]] BaseEntry::Iterable< RelocationAddendEntry > GetRelocationAddendEntries() const noexcept;

    [[nodiscard]] std::size_t GetSymbolTableEntryCount() const noexcept;

    [[nodiscard]] SymbolTableEntry GetSymbolTableEntry(const std::size_t index) const noexcept;

    [[nodiscard]] BaseEntry::Iterable< SymbolTableEntry > GetSymbolTableEntries() const noexcept;

  private:
    bool                                   m_is32Bits;
    bool                                   m_isLittleEndian;
    std::basic_string_view< std::uint8_t > m_fullData;
    std::basic_string_view< std::uint8_t > m_data;
};

END_NAMESPACE

#endif // !defined(SECTIONHEADER_H_INCLUDED_5C4B0C8F_7C7A_48D8_8568_016B305D195B)
