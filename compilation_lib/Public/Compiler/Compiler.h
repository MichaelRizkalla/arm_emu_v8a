#if !defined(COMPILER_H_INCLUDED_3F2F3F80_04DE_423C_811A_B55CAFFC6B59)
    #define COMPILER_H_INCLUDED_3F2F3F80_04DE_423C_811A_B55CAFFC6B59

    #include <API/Api.h>
    #include <Compiler/Options/CppStandard.h>
    #include <Compiler/Options/OptimizationLevel.h>
    #include <Compiler/Options/WarningOptions.h>
    #include <DebugUtils/Object.h>
    #include <ELFReader/ELFFile.h>
    #include <array>
    #include <filesystem>
    #include <optional>
    #include <unordered_map>
    #include <vector>

namespace arm_emu {

    struct Location {
        std::string           m_disassembly;
        std::filesystem::path m_filename;
        std::uint64_t         m_lineNumber;
        std::string           m_section;
        std::string           m_functionName;
    };

    struct CompilationResult {
        using ObjectData = std::pmr::vector< std::uint8_t >;

        std::string m_source;
        std::string m_assembly;

        ELFFile m_ELFFile;

        std::pmr::unordered_map< std::uint32_t, Location > m_locations;
    };

    enum class ClangVersion : std::uint32_t
    {
        Version10 = 10,
        Version11 = 11,
        Version12 = 12,
        Version13 = 13,
    };

    class [[nodiscard]] ARMEMU_API Compiler : public Object {
        static constexpr const char* Default_name = "Compiler";
        static constexpr const char* Obj_dump_name =
    #ifdef ARMEMU_OS_WINDOWS
            "llvm-objdump.exe";
    #else
            "llvm-objdump-";
    #endif

      public:
        static std::optional< Compiler > FindCompiler(std::pmr::vector< std::filesystem::path > searchPaths) noexcept;

        Compiler(std::filesystem::path compilerPath, ClangVersion version);
        Compiler(Compiler&&) noexcept;
        Compiler& operator=(Compiler&&) noexcept;
        ~Compiler();

        Compiler(const Compiler&) = delete;
        Compiler& operator=(const Compiler&) = delete;

        void SetOption(CppStandard cppStandard) noexcept;
        void SetOption(OptimizationLevel optimizationLevel) noexcept;
        void SetOption(WarningOptions warningOptions) noexcept;

        std::optional< CompilationResult > Compile(const std::filesystem::path&                     sourceFilePath,
                                                   const std::pmr::vector< std::filesystem::path >& libraries);

        std::filesystem::path GetPath() const noexcept;

      private:
        Compiler(std::filesystem::path compilerPath, ClangVersion, bool);

        std::array< std::uint32_t, 3 > m_compilerOptions;
        std::filesystem::path          m_compiler;
        ClangVersion                   m_version;
    };

} // namespace arm_emu

#endif // !defined(COMPILER_H_INCLUDED_3F2F3F80_04DE_423C_811A_B55CAFFC6B59)
