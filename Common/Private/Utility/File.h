#if !defined(FILE_H_INCLUDED_B31B6D24_88CA_4D65_8FA9_D51888934A1A)
    #define FILE_H_INCLUDED_B31B6D24_88CA_4D65_8FA9_D51888934A1A

    #include <API/Api.h>
    #include <cstdint>
    #include <filesystem>
    #include <fstream>
    #include <vector>

BEGIN_NAMESPACE

struct FileUtility {
    STATIC_CLASS(FileUtility)

    static std::vector< std::uint8_t > Read(const std::filesystem::path& path) {
        if (std::ifstream is { path, std::ios::binary }; is.good()) {
            const auto fileSize = is.seekg(0, std::ios_base::end).tellg();
            is.seekg(0);
            std::vector< char > data {};
            data.resize(static_cast< std::size_t >(fileSize));
            is.read(data.data(), fileSize);
            return { data.begin(), data.end() };
        } else {
            return {};
        }
    }

    template < class Type >
    static void WriteBinary(const std::filesystem::path& path, std::basic_string_view< Type > data) {
        std::ofstream os { path, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc };

        os.write(reinterpret_cast< const char* >(data.data()),
                 static_cast< std::streamsize >(data.size() * sizeof(Type) / sizeof(char)));
    }

    static bool IsELFFile(const uint8_t* data) noexcept {
        const auto ELFMagicNumber = { 0x7F, 0x45, 0x4C, 0x46 };
        for (auto& number : ELFMagicNumber) {
            if (number != *data)
                return false;
            ++data;
        }
        return true;
    }
};

END_NAMESPACE

#endif // !defined(FILE_H_INCLUDED_B31B6D24_88CA_4D65_8FA9_D51888934A1A)
