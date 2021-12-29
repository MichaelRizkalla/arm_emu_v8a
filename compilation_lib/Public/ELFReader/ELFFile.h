#if !defined(ELFFILE_H_INCLUDED_45C179ED_0D11_4502_8DB5_9C325E1B3F74)
    #define ELFFILE_H_INCLUDED_45C179ED_0D11_4502_8DB5_9C325E1B3F74

    #include <API/Api.h>
    #include <ELFReader/Entries/FileHeader.h>
    #include <Utility/UniqueRef.h>
    #include <map>

BEGIN_NAMESPACE

struct ELFFile {
    UniqueRef< std::pmr::vector< std::uint8_t > > m_objectFileData;
    FileHeader                                    m_fileHeader;
    std::pmr::map< std::string, std::uint64_t >   m_sectionOffsets;
    std::uint64_t                                 m_entryPointOffset;

    bool m_validBinary;
};

END_NAMESPACE

#endif // !defined(ELFFILE_H_INCLUDED_45C179ED_0D11_4502_8DB5_9C325E1B3F74)
