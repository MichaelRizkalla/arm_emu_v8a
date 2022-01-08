#if !defined(ELFREADER_H_INCLUDED_AE36A694_9B9C_4D57_B333_1F1AF3C35D53)
    #define ELFREADER_H_INCLUDED_AE36A694_9B9C_4D57_B333_1F1AF3C35D53

    #include <API/Api.h>
    #include <DebugUtils/Object.h>
    #include <ELFReader/ELFFile.h>
    #include <filesystem>
    #include <memory>

BEGIN_NAMESPACE

struct ARMEMU_API ELFReader {
    STATIC_CLASS(ELFReader)

    /// @brief Consumes objectFileData and return an ELFFile
    /// @param objectFileData object file to parse
    /// @return ELFFile object with parsed object file
    static ELFFile ParseObjectFile(const std::filesystem::path&                   objectFilePath,
                                   std::unique_ptr< std::vector< std::uint8_t > > objectFileData, Object& debugObject);

  private:
    static void ResolveSymbols(std::vector< std::uint8_t >& data, const FileHeader& fileHeader, Object& debugObject);
};

END_NAMESPACE

#endif // !defined(ELFREADER_H_INCLUDED_AE36A694_9B9C_4D57_B333_1F1AF3C35D53)
