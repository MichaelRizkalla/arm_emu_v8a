
#include <ELFReader/Entries/RelocationEntry.h>
#include <cassert>

// Credits: https://github.com/lefticus/cpp_box/blob/master/include/cpp_box/elf_reader.hpp

BEGIN_NAMESPACE

RelocationEntry::RelocationEntry(bool is32Bits, bool isLittleEndian, std::basic_string_view< std::uint8_t > data) :
    m_is32Bits(is32Bits), m_isLittleEndian(isLittleEndian), m_data(data) {
}

std::size_t RelocationEntry::GetEntryOffset(std::uint32_t entry) const noexcept {
    Entry entryAsEnum = static_cast< Entry >(entry);

    switch (entryAsEnum) {
        case Entry::FileOffset:
            return 0;
        case Entry::Info:
            return m_is32Bits ? 4 : 8;
    }

    assert(false && "Unreachable code path!");
    std::terminate();
}

std::size_t RelocationEntry::GetEntrySize(std::uint32_t entry) const noexcept {
    Entry entryAsEnum = static_cast< Entry >(entry);

    switch (entryAsEnum) {
        case Entry::FileOffset:
            return m_is32Bits ? 4 : 8;
        case Entry::Info:
            return m_is32Bits ? 4 : 8;
    }

    assert(false && "Unreachable code path!");
    std::terminate();
}

std::uint64_t RelocationEntry::Read(std::uint32_t entry) const noexcept {
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

std::uint64_t RelocationEntry::ReadFileOffset() const noexcept {
    return Read(static_cast< std::underlying_type_t< Entry > >(Entry::FileOffset));
}

std::uint64_t RelocationEntry::ReadInfo() const noexcept {
    return Read(static_cast< std::underlying_type_t< Entry > >(Entry::Info));
}

std::uint64_t RelocationEntry::ReadSymbol() const noexcept {
    if (m_is32Bits) {
        return ReadInfo() >> 8;
    } else {
        return ReadInfo() >> 32;
    }
}

std::uint64_t RelocationEntry::GetType() const noexcept {
    if (m_is32Bits) {
        return ReadInfo() & 0xFF;
    } else {
        return ReadInfo() & 0xFFFFFFFF;
    }
}

std::size_t RelocationEntry::GetRelocationEntrySize(const bool is32Bits) noexcept {
    if (is32Bits) {
        return 8;
    } else {
        return 16;
    }
}

END_NAMESPACE
