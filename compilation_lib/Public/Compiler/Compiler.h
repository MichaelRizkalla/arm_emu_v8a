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

BEGIN_NAMESPACE

struct Location {
    std::string           m_disassembly;
    std::filesystem::path m_filename;
    std::uint64_t         m_lineNumber;
    std::string           m_section;
    std::string           m_functionName;
};

struct CompilationResult {
    std::string m_source;
    std::string m_assembly;

    ELFFile m_ELFFile;

    std::pmr::unordered_map< std::uint32_t, Location > m_locations;
};

class [[nodicard]] Compiler : public Object {
    static constexpr const char* Default_name          = "Compiler";
    static constexpr const char* Obj_dump_name         = "llvm-objdump.exe";
    static constexpr const char* Default_compiler_name = "clang++";

  public:
    static std::optional< Compiler > FindCompiler(std::vector< std::filesystem::path > searchPaths) noexcept;

    Compiler(std::filesystem::path compilerPath);
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

  private:
    Compiler(std::filesystem::path compilerPath, bool);

    std::array< std::uint32_t, 3 > m_compilerOptions;
    std::filesystem::path          m_compiler;
};

END_NAMESPACE

#endif // !defined(COMPILER_H_INCLUDED_3F2F3F80_04DE_423C_811A_B55CAFFC6B59)
