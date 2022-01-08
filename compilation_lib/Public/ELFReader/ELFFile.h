#if !defined(ELFFILE_H_INCLUDED_45C179ED_0D11_4502_8DB5_9C325E1B3F74)
    #define ELFFILE_H_INCLUDED_45C179ED_0D11_4502_8DB5_9C325E1B3F74

    #include <API/Api.h>
    #include <ELFReader/Entries/FileHeader.h>
    #include <map>
    #include <memory>
    #include <memory_resource>

namespace arm_emu {

    struct ELFFile {
        std::unique_ptr< std::vector< std::uint8_t > > m_objectFileData;
        FileHeader                                     m_fileHeader { { nullptr, 0 } };
        std::map< std::string, std::uint64_t >         m_sectionOffsets;
        std::uint64_t                                  m_entryPointOffset;

        bool m_validBinary;
    };

} // namespace arm_emu

#endif // !defined(ELFFILE_H_INCLUDED_45C179ED_0D11_4502_8DB5_9C325E1B3F74)
