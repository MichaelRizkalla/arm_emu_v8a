#if !defined(FILEHEADER_H_INCLUDED_49364CE6_78E8_4A86_9063_5DC7666D856A)
    #define FILEHEADER_H_INCLUDED_49364CE6_78E8_4A86_9063_5DC7666D856A

// Credits: https://github.com/lefticus/cpp_box/blob/master/include/cpp_box/elf_reader.hpp

    #include <ELFReader/Entries/BaseEntry.h>
    #include <ELFReader/Entries/RelocationEntry.h>
    #include <ELFReader/Entries/SectionHeader.h>
    #include <ELFReader/Entries/SymbolTableEntry.h>
    #include <assert.h>
    #include <cstdint>

BEGIN_NAMESPACE

struct [[nodiscard]] FileHeader final : public BaseEntry {

    enum class Entry : std::uint32_t
    {
        EiMag,
        EiMag0,
        EiMag1,
        EiMag2,
        EiMag3,
        EiClass,
        EiData,
        EiVersion,
        EiOsABI,
        EiABIVersion,
        EiPad,
        EType,
        EMachine,
        EVersion,
        EEntry,
        EProgramHeaderOffset,
        ESectionHeaderOffset,
        EFlags,
        EEHSize,
        EProgramHeaderEntrySize,
        EProgramHeaderNumEntries,
        ESectionHeaderEntrySize,
        ESectionHeaderNumEntries,
        ESectionHeaderStringTableIndex,
    };

    enum class Class : std::uint32_t
    {
        Bits32 = 1,
        Bits64 = 2,
        Unknown
    };

    enum class Data : std::uint32_t
    {
        LittleEndian = 1,
        BigEndian    = 2,
        Unknown
    };

    enum class OSABI : std::uint32_t
    {
        SystemV       = 0x00,
        HPUX          = 0x01,
        NetBSD        = 0x02,
        Linux         = 0x03,
        GNUHurd       = 0x04,
        Solaris       = 0x06,
        AIX           = 0x07,
        IRIX          = 0x08,
        FreeBSD       = 0x09,
        Tru64         = 0x0A,
        NovellModesto = 0x0B,
        OpenBSD       = 0x0C,
        OpenVMS       = 0x0D,
        NonStopKernel = 0x0E,
        AROS          = 0x0F,
        FenixOS       = 0x10,
        CloudABI      = 0x11,
        Unknown
    };

    enum class ObjectType : std::uint32_t
    {
        ETNone     = 0x00,
        ETRel      = 0x01,
        ETExec     = 0x02,
        ETDyn      = 0x03,
        ETCore     = 0x04,
        ETLowOs    = 0xFE00,
        ETHighOs   = 0xFEFF,
        ETLowProc  = 0xFF00,
        ETHighProc = 0xFFFF,
        Unknown
    };

    enum class Machine : std::uint32_t
    {
        Unknown = 0x00,
        SPARC   = 0x02,
        x86     = 0x03,
        MIPS    = 0x08,
        PowerPC = 0x14,
        S390    = 0x16,
        ARM     = 0x28,
        SuperH  = 0x2A,
        IA64    = 0x32,
        x86_64  = 0x3E,
        AArch64 = 0xB7,
        RiscV   = 0xF3
    };

    FileHeader(std::basic_string_view< std::uint8_t > data);

    [[nodiscard]] std::size_t GetEntryOffset(std::uint32_t entry) const noexcept final;

    [[nodiscard]] std::size_t GetEntrySize(std::uint32_t entry) const noexcept final;

    [[nodiscard]] std::uint64_t Read(std::uint32_t entry) const noexcept final;

    [[nodiscard]] bool IsELFFile() const noexcept;

    [[nodiscard]] Class GetBitClass() const noexcept;

    [[nodiscard]] Data GetMultibyteData() const noexcept;

    [[nodiscard]] bool Is32Bit() const noexcept;

    [[nodiscard]] bool IsLittleEndian() const noexcept;

    [[nodiscard]] std::uint64_t ReadELFVersion() const noexcept;
    [[nodiscard]] std::uint64_t ReadABIVersion() const noexcept;
    [[nodiscard]] std::uint64_t ReadVersion() const noexcept;
    [[nodiscard]] std::uint64_t ReadEntry() const noexcept;
    [[nodiscard]] std::uint64_t ReadProgramHeaderOffset() const noexcept;
    [[nodiscard]] std::uint64_t ReadSectionHeaderOffset() const noexcept;
    [[nodiscard]] std::uint64_t ReadFlags() const noexcept;
    [[nodiscard]] std::uint64_t ReadSize() const noexcept;
    [[nodiscard]] std::uint64_t ReadProgramHeaderSize() const noexcept;
    [[nodiscard]] std::uint64_t ReadProgramHeaderNumEntries() const noexcept;
    [[nodiscard]] std::uint64_t ReadSectionHeaderSize() const noexcept;
    [[nodiscard]] std::uint64_t ReadSectionHeaderNumEntries() const noexcept;
    [[nodiscard]] std::uint64_t ReadSectionHeaderStringTableIndex() const noexcept;
    [[nodiscard]] OSABI         ReadOsABI() const noexcept;
    [[nodiscard]] ObjectType    ReadObjectType() const noexcept;
    [[nodiscard]] Machine       ReadMachine() const noexcept;

    [[nodiscard]] SectionHeader                          GetSectionHeader(const std::size_t entry) const noexcept;
    [[nodiscard]] Iterable< SectionHeader >              GetSectionHeaders() const noexcept;
    [[nodiscard]] std::basic_string_view< std::uint8_t > GetSectionHeaderStringTable() const noexcept;
    [[nodiscard]] std::basic_string_view< std::uint8_t > GetStringTable() const noexcept;
    [[nodiscard]] SectionHeader                          GetSymbolTable() const noexcept;

  private:
    std::basic_string_view< std::uint8_t > m_data;
};

END_NAMESPACE

#endif // !defined(FILEHEADER_H_INCLUDED_49364CE6_78E8_4A86_9063_5DC7666D856A)
