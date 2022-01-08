
#include <ELFReader/Entries/FileHeader.h>
#include <Utility/File.h>
#include <cassert>

BEGIN_NAMESPACE

FileHeader::FileHeader(std::basic_string_view< std::uint8_t > data) : m_data(data) {
}

std::size_t FileHeader::GetEntryOffset(std::uint32_t entry) const noexcept {
    Entry entryAsEnum = static_cast< Entry >(entry);

    switch (entryAsEnum) {
        case Entry::EiMag:
            return 0x00;
        case Entry::EiMag0:
            return 0x00;
        case Entry::EiMag1:
            return 0x01;
        case Entry::EiMag2:
            return 0x02;
        case Entry::EiMag3:
            return 0x03;
        case Entry::EiClass:
            return 0x04;
        case Entry::EiData:
            return 0x05;
        case Entry::EiVersion:
            return 0x06;
        case Entry::EiOsABI:
            return 0x07;
        case Entry::EiABIVersion:
            return 0x08;
        case Entry::EiPad:
            return 0x09;
        case Entry::EType:
            return 0x10;
        case Entry::EMachine:
            return 0x12;
        case Entry::EVersion:
            return 0x14;
        case Entry::EEntry:
            return 0x18;
        case Entry::EProgramHeaderOffset:
            return Is32Bit() ? 0x1C : 0x20;
        case Entry::ESectionHeaderOffset:
            return Is32Bit() ? 0x20 : 0x28;
        case Entry::EFlags:
            return Is32Bit() ? 0x24 : 0x30;
        case Entry::EEHSize:
            return Is32Bit() ? 0x28 : 0x34;
        case Entry::EProgramHeaderEntrySize:
            return Is32Bit() ? 0x2A : 0x36;
        case Entry::EProgramHeaderNumEntries:
            return Is32Bit() ? 0x2C : 0x38;
        case Entry::ESectionHeaderEntrySize:
            return Is32Bit() ? 0x2E : 0x3A;
        case Entry::ESectionHeaderNumEntries:
            return Is32Bit() ? 0x30 : 0x3C;
        case Entry::ESectionHeaderStringTableIndex:
            return Is32Bit() ? 0x32 : 0x3E;
    }

    assert(false && "Unreachable code path!");
    std::terminate();
}

std::size_t FileHeader::GetEntrySize(std::uint32_t entry) const noexcept {
    Entry entryAsEnum = static_cast< Entry >(entry);

    switch (entryAsEnum) {
        case Entry::EiMag:
            return 4;
        case Entry::EiMag0:
            return 1;
        case Entry::EiMag1:
            return 1;
        case Entry::EiMag2:
            return 1;
        case Entry::EiMag3:
            return 1;
        case Entry::EiClass:
            return 1;
        case Entry::EiData:
            return 1;
        case Entry::EiVersion:
            return 1;
        case Entry::EiOsABI:
            return 1;
        case Entry::EiABIVersion:
            return 1;
        case Entry::EiPad:
            return 7;
        case Entry::EType:
            return 2;
        case Entry::EMachine:
            return 2;
        case Entry::EVersion:
            return 4;
        case Entry::EEntry:
            return Is32Bit() ? 4 : 8;
        case Entry::EProgramHeaderOffset:
            return Is32Bit() ? 4 : 8;
        case Entry::ESectionHeaderOffset:
            return Is32Bit() ? 4 : 8;
        case Entry::EFlags:
            return 4;
        case Entry::EEHSize:
            return 2;
        case Entry::EProgramHeaderEntrySize:
            return 2;
        case Entry::EProgramHeaderNumEntries:
            return 2;
        case Entry::ESectionHeaderEntrySize:
            return 2;
        case Entry::ESectionHeaderNumEntries:
            return 2;
        case Entry::ESectionHeaderStringTableIndex:
            return 2;
    }

    assert(false && "Unreachable code path!");
    std::terminate();
}

std::uint64_t FileHeader::Read(std::uint32_t entry) const noexcept {
    switch (GetEntrySize(entry)) {
        case 1:
            return BaseEntry::ReadFrom< 1 >(m_data, GetEntryOffset(entry) /*, isLittleEndian = true */);
        case 2:
            return BaseEntry::ReadFrom< 2 >(m_data, GetEntryOffset(entry), IsLittleEndian());
        case 4:
            return BaseEntry::ReadFrom< 4 >(m_data, GetEntryOffset(entry), IsLittleEndian());
        case 8:
            return BaseEntry::ReadFrom< 8 >(m_data, GetEntryOffset(entry), IsLittleEndian());
    }

    assert(false && "Unreachable code path!");
    std::terminate();
}

bool FileHeader::IsELFFile() const noexcept {
    return FileUtility::IsELFFile(m_data.data());
}

FileHeader::Class FileHeader::GetBitClass() const noexcept {
    return static_cast< Class >(Read(static_cast< std::underlying_type_t< Entry > >(Entry::EiClass)));
}

FileHeader::Data FileHeader::GetMultibyteData() const noexcept {
    return static_cast< Data >(Read(static_cast< std::underlying_type_t< Entry > >(Entry::EiData)));
}

bool FileHeader::Is32Bit() const noexcept {
    switch (GetBitClass()) {
        case Class::Bits32:
            return true;
        case Class::Bits64:
            return false;
        case Class::Unknown:
            abort();
    }

    std::terminate();
}

bool FileHeader::IsLittleEndian() const noexcept {
    switch (GetMultibyteData()) {
        case Data::LittleEndian:
            return true;
        case Data::BigEndian:
            return false;
        case Data::Unknown:
            abort();
    }

    std::terminate();
}

std::uint64_t FileHeader::ReadELFVersion() const noexcept {
    return Read(static_cast< std::underlying_type_t< Entry > >(Entry::EiVersion));
}

std::uint64_t FileHeader::ReadABIVersion() const noexcept {
    return Read(static_cast< std::underlying_type_t< Entry > >(Entry::EiABIVersion));
}

std::uint64_t FileHeader::ReadVersion() const noexcept {
    return Read(static_cast< std::underlying_type_t< Entry > >(Entry::EVersion));
}

std::uint64_t FileHeader::ReadEntry() const noexcept {
    return Read(static_cast< std::underlying_type_t< Entry > >(Entry::EEntry));
}

std::uint64_t FileHeader::ReadProgramHeaderOffset() const noexcept {
    return Read(static_cast< std::underlying_type_t< Entry > >(Entry::EProgramHeaderOffset));
}

std::uint64_t FileHeader::ReadSectionHeaderOffset() const noexcept {
    return Read(static_cast< std::underlying_type_t< Entry > >(Entry::ESectionHeaderOffset));
}

std::uint64_t FileHeader::ReadFlags() const noexcept {
    return Read(static_cast< std::underlying_type_t< Entry > >(Entry::EFlags));
}

std::uint64_t FileHeader::ReadSize() const noexcept {
    return Read(static_cast< std::underlying_type_t< Entry > >(Entry::EEHSize));
}

std::uint64_t FileHeader::ReadProgramHeaderSize() const noexcept {
    return Read(static_cast< std::underlying_type_t< Entry > >(Entry::EProgramHeaderEntrySize));
}

std::uint64_t FileHeader::ReadProgramHeaderNumEntries() const noexcept {
    return Read(static_cast< std::underlying_type_t< Entry > >(Entry::EProgramHeaderNumEntries));
}

std::uint64_t FileHeader::ReadSectionHeaderSize() const noexcept {
    return Read(static_cast< std::underlying_type_t< Entry > >(Entry::ESectionHeaderEntrySize));
}

std::uint64_t FileHeader::ReadSectionHeaderNumEntries() const noexcept {
    return Read(static_cast< std::underlying_type_t< Entry > >(Entry::ESectionHeaderNumEntries));
}

std::uint64_t FileHeader::ReadSectionHeaderStringTableIndex() const noexcept {
    return Read(static_cast< std::underlying_type_t< Entry > >(Entry::ESectionHeaderStringTableIndex));
}

FileHeader::OSABI FileHeader::ReadOsABI() const noexcept {
    const auto entryType = static_cast< OSABI >(Read(static_cast< std::underlying_type_t< Entry > >(Entry::EiOsABI)));
    switch (entryType) {
        case OSABI::SystemV:
            [[fallthrough]];
        case OSABI::HPUX:
            [[fallthrough]];
        case OSABI::NetBSD:
            [[fallthrough]];
        case OSABI::Linux:
            [[fallthrough]];
        case OSABI::GNUHurd:
            [[fallthrough]];
        case OSABI::Solaris:
            [[fallthrough]];
        case OSABI::AIX:
            [[fallthrough]];
        case OSABI::IRIX:
            [[fallthrough]];
        case OSABI::FreeBSD:
            [[fallthrough]];
        case OSABI::Tru64:
            [[fallthrough]];
        case OSABI::NovellModesto:
            [[fallthrough]];
        case OSABI::OpenBSD:
            [[fallthrough]];
        case OSABI::OpenVMS:
            [[fallthrough]];
        case OSABI::NonStopKernel:
            [[fallthrough]];
        case OSABI::AROS:
            [[fallthrough]];
        case OSABI::FenixOS:
            [[fallthrough]];
        case OSABI::CloudABI:
            [[fallthrough]];
        case OSABI::Unknown:
            return entryType;
    }

    return OSABI::Unknown;
}

FileHeader::ObjectType FileHeader::ReadObjectType() const noexcept {
    const auto entryType =
        static_cast< ObjectType >(Read(static_cast< std::underlying_type_t< Entry > >(Entry::EType)));
    switch (entryType) {
        case ObjectType::ETNone:
            [[fallthrough]];
        case ObjectType::ETRel:
            [[fallthrough]];
        case ObjectType::ETExec:
            [[fallthrough]];
        case ObjectType::ETDyn:
            [[fallthrough]];
        case ObjectType::ETCore:
            [[fallthrough]];
        case ObjectType::ETLowOs:
            [[fallthrough]];
        case ObjectType::ETHighOs:
            [[fallthrough]];
        case ObjectType::ETLowProc:
            [[fallthrough]];
        case ObjectType::ETHighProc:
            [[fallthrough]];
        case ObjectType::Unknown:
            return entryType;
    }

    return ObjectType::Unknown;
}

FileHeader::Machine FileHeader::ReadMachine() const noexcept {
    const auto entryType =
        static_cast< Machine >(Read(static_cast< std::underlying_type_t< Entry > >(Entry::EMachine)));
    switch (entryType) {
        case Machine::Unknown:
            [[fallthrough]];
        case Machine::SPARC:
            [[fallthrough]];
        case Machine::x86:
            [[fallthrough]];
        case Machine::MIPS:
            [[fallthrough]];
        case Machine::PowerPC:
            [[fallthrough]];
        case Machine::S390:
            [[fallthrough]];
        case Machine::ARM:
            [[fallthrough]];
        case Machine::SuperH:
            [[fallthrough]];
        case Machine::IA64:
            [[fallthrough]];
        case Machine::x86_64:
            [[fallthrough]];
        case Machine::AArch64:
            [[fallthrough]];
        case Machine::RiscV:
            return entryType;
    }

    return Machine::Unknown;
}

SectionHeader FileHeader::GetSectionHeader(const std::size_t entry) const noexcept {
    assert(entry < ReadSectionHeaderNumEntries());
    return { Is32Bit(), IsLittleEndian(), m_data,
             m_data.substr(ReadSectionHeaderOffset() + ReadSectionHeaderSize() * entry) };
}

BaseEntry::Iterable< SectionHeader > FileHeader::GetSectionHeaders() const noexcept {
    return Iterable< SectionHeader > { [this](const std::size_t idx) { return GetSectionHeader(idx); },
                                       ReadSectionHeaderNumEntries() };
}

std::basic_string_view< std::uint8_t > FileHeader::GetSectionHeaderStringTable() const noexcept {
    const auto stringHeader { GetSectionHeader(ReadSectionHeaderStringTableIndex()) };
    return stringHeader.GetSectionData();
}

std::basic_string_view< std::uint8_t > FileHeader::GetStringTable() const noexcept {
    const auto &sectionHeaderTable = GetSectionHeaderStringTable();

    for (const auto &sectionHeader : GetSectionHeaders()) {
        const char str[] = ".strtab";
        if (sectionHeader.ReadNameFrom(sectionHeaderTable) == std::string_view { str }) {
            return sectionHeader.GetSectionData();
        }
    }

    assert(false && "Unreachable code path!");
    std::terminate();
}

SectionHeader FileHeader::GetSymbolTable() const noexcept {
    for (const auto &sectionHeader : GetSectionHeaders()) {
        if (sectionHeader.GetSymbolTableEntryCount() != 0) {
            return sectionHeader;
        }
    }

    assert(false && "Unreachable code path!");
    std::terminate();
}

END_NAMESPACE
