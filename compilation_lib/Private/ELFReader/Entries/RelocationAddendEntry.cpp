
#include <ELFReader/Entries/RelocationAddendEntry.h>
#include <cassert>

BEGIN_NAMESPACE

RelocationAddendEntry::RelocationAddendEntry(bool is32Bits, bool isLittleEndian,
                                       std::basic_string_view< std::uint8_t > data) :
    m_is32Bits(is32Bits), m_isLittleEndian(isLittleEndian), m_data(data) {
}

std::size_t RelocationAddendEntry::GetEntryOffset(std::uint32_t entry) const noexcept {
    Entry entryAsEnum = static_cast< Entry >(entry);

    switch (entryAsEnum) {
        case Entry::FileOffset:
            return 0;
        case Entry::Info:
            return m_is32Bits ? 4 : 8;
        case Entry::Addend:
            return m_is32Bits ? 8 : 16;
    }

    assert(false && "Unreachable code path!");
    std::terminate();
}

std::size_t RelocationAddendEntry::GetEntrySize(std::uint32_t entry) const noexcept {
    Entry entryAsEnum = static_cast< Entry >(entry);

    switch (entryAsEnum) {
        case Entry::FileOffset:
            return m_is32Bits ? 4 : 8;
        case Entry::Info:
            return m_is32Bits ? 4 : 8;
        case Entry::Addend:
            return m_is32Bits ? 4 : 8;
    }

    assert(false && "Unreachable code path!");
    std::terminate();
}

std::uint64_t RelocationAddendEntry::Read(std::uint32_t entry) const noexcept {
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

std::uint64_t RelocationAddendEntry::ReadFileOffset() const noexcept {
    return Read(static_cast< std::underlying_type_t< Entry > >(Entry::FileOffset));
}

std::uint64_t RelocationAddendEntry::ReadInfo() const noexcept {
    return Read(static_cast< std::underlying_type_t< Entry > >(Entry::Info));
}

std::uint64_t RelocationAddendEntry::ReadAddend() const noexcept {
    return Read(static_cast< std::underlying_type_t< Entry > >(Entry::Addend));
}

std::uint64_t RelocationAddendEntry::ReadSymbol() const noexcept {
    if (m_is32Bits) {
        return ReadInfo() >> 8;
    } else {
        return ReadInfo() >> 32;
    }
}

std::uint64_t RelocationAddendEntry::GetType() const noexcept {
    if (m_is32Bits) {
        return ReadInfo() & 0xFF;
    } else {
        return ReadInfo() & 0xFFFFFFFF;
    }
}

std::size_t RelocationAddendEntry::GetRelocationAddendEntrySize(const bool is32Bits) noexcept {
    if (is32Bits) {
        return 12;
    } else {
        return 24;
    }
}

END_NAMESPACE
