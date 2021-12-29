
#include <ELFReader/Entries/SymbolTableEntry.h>
#include <cassert>

BEGIN_NAMESPACE

SymbolTableEntry::SymbolTableEntry(bool is32Bits, bool isLittleEndian, std::basic_string_view< std::uint8_t > data) :
    m_is32Bits(is32Bits), m_isLittleEndian(isLittleEndian), m_data(data) {
}

std::size_t SymbolTableEntry::GetEntryOffset(std::uint32_t entry) const noexcept {
    Entry entryAsEnum = static_cast< Entry >(entry);

    switch (entryAsEnum) {
        case Entry::Name:
            return 0;
        case Entry::Value:
            return m_is32Bits ? 4 : 8;
        case Entry::Size:
            return m_is32Bits ? 8 : 16;
        case Entry::Info:
            return m_is32Bits ? 12 : 4;
        case Entry::Other:
            return m_is32Bits ? 13 : 5;
        case Entry::SectionHeaderTableIndex:
            return m_is32Bits ? 14 : 6;
    }

    assert(false && "Unreachable code path!");
    std::terminate();
}

std::size_t SymbolTableEntry::GetEntrySize(std::uint32_t entry) const noexcept {
    Entry entryAsEnum = static_cast< Entry >(entry);

    switch (entryAsEnum) {
        case Entry::Name:
            return 4;
        case Entry::Value:
            return m_is32Bits ? 4 : 8;
        case Entry::Size:
            return m_is32Bits ? 4 : 8;
        case Entry::Info:
            return 1;
        case Entry::Other:
            return 1;
        case Entry::SectionHeaderTableIndex:
            return 2;
    }

    assert(false && "Unreachable code path!");
    std::terminate();
}

std::uint64_t SymbolTableEntry::Read(std::uint32_t entry) const noexcept {
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

std::uint64_t SymbolTableEntry::ReadNameOffset() const noexcept {
    return Read(static_cast< std::underlying_type_t< Entry > >(Entry::Name));
}

[[nodiscard]] std::string_view
    SymbolTableEntry::ReadNameFrom(const std::basic_string_view< std::uint8_t > stringTable) const noexcept {
    const auto  nameOffset   = ReadNameOffset();
    const auto& nameStartIdx = stringTable.substr(nameOffset);
    const auto& name         = nameStartIdx.substr(0, nameStartIdx.find(static_cast< std::uint8_t >('\0')));

    return { static_cast< const char* >(static_cast< const void* >(name.data())), name.size() };
}

std::uint64_t SymbolTableEntry::ReadValue() const noexcept {
    return Read(static_cast< std::underlying_type_t< Entry > >(Entry::Value));
}

std::uint64_t SymbolTableEntry::ReadSize() const noexcept {
    return Read(static_cast< std::underlying_type_t< Entry > >(Entry::Size));
}

std::uint64_t SymbolTableEntry::ReadSectionHeaderTableIndex() const noexcept {
    return Read(static_cast< std::underlying_type_t< Entry > >(Entry::SectionHeaderTableIndex));
}

std::size_t SymbolTableEntry::GetSymbolTableEntrySize(const bool is32Bits) noexcept {
    if (is32Bits) {
        return 16;
    } else {
        return 24;
    }
}

END_NAMESPACE
