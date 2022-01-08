
#include <ELFReader/Entries/SectionHeader.h>
#include <cassert>

BEGIN_NAMESPACE

SectionHeader::SectionHeader(bool is32Bits, bool isLittleEndian,
                             std::basic_string_view< std::uint8_t > fullData, std::basic_string_view< std::uint8_t > data) :
    m_is32Bits(is32Bits), m_isLittleEndian(isLittleEndian), m_data(data), m_fullData(fullData) {
}

std::size_t SectionHeader::GetEntryOffset(std::uint32_t entry) const noexcept {
    Entry entryAsEnum = static_cast< Entry >(entry);

    switch (entryAsEnum) {
        case Entry::Name:
            return 0x00;
        case Entry::Type:
            return 0x04;
        case Entry::Flags:
            return 0x08;
        case Entry::Address:
            return m_is32Bits ? 0x0C : 0x10;
        case Entry::Offset:
            return m_is32Bits ? 0x10 : 0x18;
        case Entry::Size:
            return m_is32Bits ? 0x14 : 0x20;
        case Entry::Link:
            return m_is32Bits ? 0x18 : 0x28;
        case Entry::Info:
            return m_is32Bits ? 0x1C : 0x2C;
        case Entry::AddressAlign:
            return m_is32Bits ? 0x20 : 0x30;
        case Entry::EntrySize:
            return m_is32Bits ? 0x24 : 0x38;
    }

    assert(false && "Unreachable code path!");
    std::terminate();
}

std::size_t SectionHeader::GetEntrySize(std::uint32_t entry) const noexcept {
    Entry entryAsEnum = static_cast< Entry >(entry);

    switch (entryAsEnum) {
        case Entry::Name:
            return 4;
        case Entry::Type:
            return 4;
        case Entry::Flags:
            return m_is32Bits ? 4 : 8;
        case Entry::Address:
            return m_is32Bits ? 4 : 8;
        case Entry::Offset:
            return m_is32Bits ? 4 : 8;
        case Entry::Size:
            return m_is32Bits ? 4 : 8;
        case Entry::Link:
            return 4;
        case Entry::Info:
            return 4;
        case Entry::AddressAlign:
            return m_is32Bits ? 4 : 8;
        case Entry::EntrySize:
            return m_is32Bits ? 4 : 8;
    }

    assert(false && "Unreachable code path!");
    std::terminate();
}

std::uint64_t SectionHeader::Read(std::uint32_t entry) const noexcept {
    switch (GetEntrySize(entry)) {
        case 1:
            return BaseEntry::ReadFrom< 1 >(m_data, GetEntryOffset(entry) /*, isLittleEndian = true */);
        case 2:
            return BaseEntry::ReadFrom< 2 >(m_data, GetEntryOffset(entry), m_isLittleEndian);
        case 4:
            return BaseEntry::ReadFrom< 4 >(m_data, GetEntryOffset(entry), m_isLittleEndian);
        case 8:
            return BaseEntry::ReadFrom< 8 >(m_data, GetEntryOffset(entry), m_isLittleEndian);
    }

    assert(false && "Unreachable code path!");
    std::terminate();
}

std::uint64_t SectionHeader::ReadNameOffset() const noexcept {
    return Read(static_cast< std::underlying_type_t< Entry > >(Entry::Name));
}

std::string_view
    SectionHeader::ReadNameFrom(const std::basic_string_view< std::uint8_t > stringTable) const noexcept {
    const auto  nameOffset   = ReadNameOffset();
    const auto& nameStartIdx = stringTable.substr(nameOffset);
    const auto& name         = nameStartIdx.substr(0, nameStartIdx.find(static_cast< std::uint8_t >('\0')));

    return { static_cast< const char* >(static_cast< const void* >(name.data())), name.size() };
}

SectionHeader::EntryType SectionHeader::ReadType() const noexcept {
    const auto entryTypeVal =
        static_cast< EntryType >(Read(static_cast< std::underlying_type_t< Entry > >(Entry::Type)));

    switch (entryTypeVal) {
        case EntryType::SHT_NULL:
            [[fallthrough]];
        case EntryType::SHT_PROGBITS:
            [[fallthrough]];
        case EntryType::SHT_SYMTAB:
            [[fallthrough]];
        case EntryType::SHT_STRTAB:
            [[fallthrough]];
        case EntryType::SHT_RELA:
            [[fallthrough]];
        case EntryType::SHT_HASH:
            [[fallthrough]];
        case EntryType::SHT_DYNAMIC:
            [[fallthrough]];
        case EntryType::SHT_NOTE:
            [[fallthrough]];
        case EntryType::SHT_NOBITS:
            [[fallthrough]];
        case EntryType::SHT_REL:
            [[fallthrough]];
        case EntryType::SHT_SHLIB:
            [[fallthrough]];
        case EntryType::SHT_DYNSYM:
            [[fallthrough]];
        case EntryType::SHT_INIT_ARRAY:
            [[fallthrough]];
        case EntryType::SHT_FINI_ARRAY:
            [[fallthrough]];
        case EntryType::SHT_PREINIT_ARRAY:
            [[fallthrough]];
        case EntryType::SHT_GROUP:
            [[fallthrough]];
        case EntryType::SHT_SYMTAB_SHNDX:
            [[fallthrough]];
        case EntryType::SHT_NUM:
            [[fallthrough]];
        case EntryType::SHT_LOOS:
            [[fallthrough]];
        case EntryType::Unknown:
            return entryTypeVal;
    }

    return EntryType::Unknown;
}

std::uint64_t SectionHeader::ReadOffset() const noexcept {
    return Read(static_cast< std::underlying_type_t< Entry > >(Entry::Offset));
}

std::uint64_t SectionHeader::ReadSize() const noexcept {
    return Read(static_cast< std::underlying_type_t< Entry > >(Entry::Size));
}

std::basic_string_view< std::uint8_t > SectionHeader::GetSectionData() const noexcept {
    return m_fullData.substr(ReadOffset(), ReadSize());
}

std::size_t SectionHeader::GetRelocationEntryCount() const noexcept {
    if (ReadType() == EntryType::SHT_REL) {
        return ReadSize() / RelocationEntry::GetRelocationEntrySize(m_is32Bits);
    } else {
        return 0;
    }
}

RelocationEntry SectionHeader::GetRelocationEntry(const std::size_t index) const noexcept {
    const auto relocationEntrySize = RelocationEntry::GetRelocationEntrySize(m_is32Bits);
    return RelocationEntry { m_is32Bits, m_isLittleEndian,
                             GetSectionData().substr(relocationEntrySize * index, relocationEntrySize) };
}

BaseEntry::Iterable< RelocationEntry > SectionHeader::GetRelocationEntries() const noexcept {
    return Iterable< RelocationEntry > {
        [this](const std::size_t idx) -> RelocationEntry { return GetRelocationEntry(idx); }, GetRelocationEntryCount()
    };
}

std::size_t SectionHeader::GetRelocationAddendEntryCount() const noexcept {
    if (ReadType() == EntryType::SHT_RELA) {
        return ReadSize() / RelocationAddendEntry::GetRelocationAddendEntrySize(m_is32Bits);
    } else {
        return 0;
    }
}

RelocationAddendEntry SectionHeader::GetRelocationAddendEntry(const std::size_t index) const noexcept {
    const auto relocationAddendEntrySize = RelocationAddendEntry::GetRelocationAddendEntrySize(m_is32Bits);
    return RelocationAddendEntry { m_is32Bits, m_isLittleEndian,
                                   GetSectionData().substr(relocationAddendEntrySize * index,
                                                           relocationAddendEntrySize) };
}

BaseEntry::Iterable< RelocationAddendEntry > SectionHeader::GetRelocationAddendEntries() const noexcept {
    return Iterable< RelocationAddendEntry > { [this](const std::size_t idx) -> RelocationAddendEntry {
                                                  return GetRelocationAddendEntry(idx);
                                              },
                                               GetRelocationAddendEntryCount()
    };
}

std::size_t SectionHeader::GetSymbolTableEntryCount() const noexcept {
    if (ReadType() == EntryType::SHT_SYMTAB) {
        return ReadSize() / SymbolTableEntry::GetSymbolTableEntrySize(m_is32Bits);
    } else {
        return 0;
    }
}

SymbolTableEntry SectionHeader::GetSymbolTableEntry(const std::size_t index) const noexcept {
    const auto symbolTableEntrySize = SymbolTableEntry::GetSymbolTableEntrySize(m_is32Bits);
    return SymbolTableEntry { m_is32Bits, m_isLittleEndian,
                              GetSectionData().substr(symbolTableEntrySize * index, symbolTableEntrySize) };
}

BaseEntry::Iterable< SymbolTableEntry > SectionHeader::GetSymbolTableEntries() const noexcept {
    return Iterable< SymbolTableEntry > { [this](const std::size_t idx) { return GetSymbolTableEntry(idx); },
                                          GetSymbolTableEntryCount() };
}

END_NAMESPACE
