
#include <Compiler/Compiler.h>
#include <DebugUtils/Log.h>
#include <ELFReader/ELFReader.h>
#include <Utility/StreamableEnum.h>
#include <Utility/System.h>
#include <Utility/Utilities.h>
#include <memory_resource>
#include <regex>

BEGIN_NAMESPACE

namespace {
    static inline constexpr std::size_t optimizationLevelLocation = 0;
    static inline constexpr std::size_t cppStandardLocation       = 1;
    static inline constexpr std::size_t warningOptionsLocation    = 2;

    namespace {

        using namespace std::literals::string_view_literals;

        static constexpr SmallConstMap< ClangVersion, std::string_view, 6 > clangVersionStringMap { { {
            { ClangVersion::Version10, "10"sv },
            { ClangVersion::Version11, "11"sv },
            { ClangVersion::Version12, "12"sv },
            { ClangVersion::Version13, "13"sv },
        } } };

    } // namespace

} // namespace

template <>
std::string_view Enum::ToChar(ClangVersion enumValue) {
    return clangVersionStringMap.At(enumValue);
}

template <>
std::ostream& operator<<(std::ostream& os, ClangVersion enumValue) {
    os << Enum::ToChar(enumValue);
    return os;
}

std::optional< Compiler > Compiler::FindCompiler(std::pmr::vector< std::filesystem::path > searchPaths) noexcept {
    for (const auto& path : searchPaths) {
        if (std::error_code errorCode {};
            std::filesystem::is_regular_file(path, errorCode) || std::filesystem::is_symlink(path, errorCode)) {
            if (const auto [result, out, err] = System::Call(fmt::format(R"({} --version)", path.string()));
                out.find("clang") != std::string::npos) {

                auto version      = out.substr(out.find("clang version ") + 14, 2);
                auto clangVersion = static_cast< ClangVersion >(std::stoi(version));

                return Compiler { path, clangVersion, true };
            }
        }
    }
    return std::nullopt;
}

Compiler::Compiler(std::filesystem::path compilerPath, ClangVersion version) :
    m_compiler(compilerPath), m_compilerOptions({ 0, 0, 0 }), m_version(version), Object(Default_name) {
}

Compiler::Compiler(std::filesystem::path compilerPath, ClangVersion version, bool) :
    m_compiler(compilerPath), m_compilerOptions({ 0, 0, 0 }), m_version(version), Object(Default_name) {
    Log(LogType::Compiler, "Found clang-{} compiler at path: {}", Enum::ToChar(m_version), m_compiler.string());
}

Compiler::Compiler(Compiler&&) noexcept = default;

Compiler& Compiler::operator=(Compiler&&) noexcept = default;

Compiler::~Compiler() = default;

void Compiler::SetOption(CppStandard cppStandard) noexcept {
    m_compilerOptions[cppStandardLocation] = static_cast< std::underlying_type_t< CppStandard > >(cppStandard);
}

void Compiler::SetOption(OptimizationLevel optimizationLevel) noexcept {
    m_compilerOptions[optimizationLevelLocation] =
        static_cast< std::underlying_type_t< OptimizationLevel > >(optimizationLevel);
}

void Compiler::SetOption(WarningOptions warningOptions) noexcept {
    m_compilerOptions[warningOptionsLocation] = static_cast< std::underlying_type_t< WarningOptions > >(warningOptions);
}

// Credits: https://github.com/lefticus/cpp_box/blob/master/lib/compiler.cpp
std::optional< CompilationResult > Compiler::Compile(const std::filesystem::path&                     sourceFilePath,
                                                     const std::pmr::vector< std::filesystem::path >& libraries) {
    ScopedDirectory dir {};
    Log(LogType::None, "dir");
    const auto cppFile         = dir.Path() / "src.cpp";
    const auto asmFile         = dir.Path() / "src.s";
    const auto objFile         = dir.Path() / "src.o";
    const auto disassemblyFile = dir.Path() / "src.dis";

    auto makeString = [](auto&& data) { return std::string { data.begin(), data.end() }; };
    auto sourceFile = makeString(FileUtility::Read(sourceFilePath));

    if (std::ofstream os(cppFile); os.good()) {
        os.write(sourceFile.data(), static_cast< std::streamsize >(sourceFile.size()));
        os.flush();
    }

    std::string includeLibrariesCommand { "" };
    for (const auto& libraryPath : libraries) {
        includeLibrariesCommand.append(fmt::format(R"(-I"{}" )", libraryPath.string()));
    }

    const auto buildCommand = fmt::format(
        R"({} -std={} "{}" -c -o "{}" -{} -g -{} -save-temps=obj --target=aarch64-none-elf -march=armv8-a -mfpu=vfp -mfloat-abi=hard -nostdinc {} -D__ELF__ -D_LIBCPP_HAS_NO_THREADS)",
        m_compiler.string(), Enum::ToChar(static_cast< CppStandard >(m_compilerOptions[cppStandardLocation])),
        cppFile.string(), objFile.string(),
        Enum::ToChar(static_cast< OptimizationLevel >(m_compilerOptions[optimizationLevelLocation])),
        Enum::ToChar(static_cast< WarningOptions >(m_compilerOptions[warningOptionsLocation])),
        includeLibrariesCommand);
    Log(LogType::Compiler, "Executing compile commands: '{}'", buildCommand);

    const auto [result, output, error] = System::Call(buildCommand);
    Log(LogType::CompileOutput, "Compilation finished with return code: '{}', stdout: '{}', stderr: '{}'", result,
        output, error);
    if (result != 0) {
        return std::nullopt;
    }
    const auto assembly   = FileUtility::Read(asmFile);
    auto       objectData = allocate_unique< std::pmr::vector< std::uint8_t > >(
        std::pmr::polymorphic_allocator< std::pmr::vector< std::uint8_t > > {}, FileUtility::Read(objFile));
    auto elfFile = ELFReader::ParseObjectFile(objFile, std::move(objectData), *this);

    std::string objDumpEntity { Obj_dump_name };
#ifndef ARMEMU_OS_WINDOWS
    objDumpEntity.insert(13, Enum::ToChar(m_version).data());
#endif // !ARMEMU_OS_WINDOWS

    const auto disassembleCommand =
        fmt::format(R"({} --disassemble --demangle --line-numbers --full-leading-addr --source "{}")",
                    (m_compiler.parent_path() / objDumpEntity.c_str()).string(), objFile.string());
    Log(LogType::Compiler, "Executing disassemble commands: '{}'", disassembleCommand);

    const auto [disassemblyResult, disassembly, disassemblyError] = System::Call(disassembleCommand);
    Log(LogType::CompileOutput, "Disassembly finished with return code: '{}'", disassemblyResult);
    if (disassemblyResult != 0) {
        return std::nullopt;
    }

    const std::regex strip_attributes { R"(\n\s+\..*)", std::regex::ECMAScript };

    const auto parseDisassembly = [this](const std::string& file, const auto& sectionOffsets) {
        const std::regex readDisassembly { R"(\s+([0-9a-f]+):\s+(..)\s+(..)\s+(..)\s+(..)\s+\t(.*))" };
        const std::regex readSectionName { R"(^Disassembly of section (.*):$)" };
        const std::regex readFunctionName { R"(^(.*:)$)" };
        const std::regex readLineNumber { R"(^; (.*):([0-9]+)$)" };
        const std::regex readSourceCode { R"(^; (.*)$)" };

        std::stringstream ss { file };

        std::pmr::unordered_map< std::uint32_t, Location > memoryLocations;

        std::string   currentFunctionName {};
        std::string   currentSection {};
        std::uint32_t currentOffset {};
        std::string   currentFileName {};
        std::uint64_t currentLineNumber {};
        std::string   currentSourceText {};

        for (std::string line; std::getline(ss, line);) {
            std::smatch results;
            if (std::regex_match(line, results, readDisassembly)) {
                Log(LogType::ParsingDisassembly, "Parsed disassembly line: (offset: '{}') '{}' '{}' '{}' '{}' '{}'",
                    results.str(1), results.str(2), results.str(3), results.str(4), results.str(5), results.str(6));

                const auto b1    = static_cast< std::uint32_t >(std::stoi(results.str(2), nullptr, 16));
                const auto b2    = static_cast< std::uint32_t >(std::stoi(results.str(3), nullptr, 16));
                const auto b3    = static_cast< std::uint32_t >(std::stoi(results.str(4), nullptr, 16));
                const auto b4    = static_cast< std::uint32_t >(std::stoi(results.str(5), nullptr, 16));
                const auto value = (b4 << 24) | (b3 << 16) | (b2 << 8) | b1;

                currentOffset = static_cast< std::uint32_t >(std::stoi(results.str(1), nullptr, 16));

                Log(LogType::ParsingDisassembly, "Disassembly: '{:08x}', '{}'", value, results.str(6));
                memoryLocations[static_cast< std::uint32_t >(currentOffset + sectionOffsets.at(currentSection))] =
                    Location { results.str(6), currentFileName, currentLineNumber, currentSection,
                               currentFunctionName };

            } else if (std::regex_match(line, results, readSectionName)) {
                Log(LogType::ParsingDisassembly, "Entering binary section: '{}'", results.str(1));
                currentSection = results.str(1);
            } else if (std::regex_match(line, results, readLineNumber)) {
                Log(LogType::ParsingDisassembly, "Entering line: '{}':'{}'", results.str(1), results.str(2));
                currentFileName   = results.str(1);
                currentLineNumber = std::stoi(results.str(2));
            } else if (std::regex_match(line, results, readSourceCode)) {
                Log(LogType::ParsingDisassembly, "Source line: '{}'", results.str(1));
                currentSourceText = results.str(1);
            } else if (std::regex_match(line, results, readFunctionName)) {
                Log(LogType::ParsingDisassembly, "Entering function: '{}'", results.str(1));
                currentFunctionName = results.str(1);
            }
        }
        return memoryLocations;
    };

    std::pmr::map< std::string, std::uint64_t > sectionOffsets = elfFile.m_sectionOffsets;

    if ((Log::GetLogType() & LogType::ELFSummary) == LogType::ELFSummary) {
        std::stringstream ss {};

        const auto& fileHeader = elfFile.m_fileHeader;
        ss << "ELFSummary: \n";
        ss << "\tis_elf_file: " << fileHeader.IsELFFile() << '\n';
        ss << "\tprogram_header_num_entries: " << fileHeader.ReadProgramHeaderNumEntries() << '\n';
        ss << "\tsection_header_num_entries: " << fileHeader.ReadSectionHeaderNumEntries() << '\n';
        ss << "\tsection_header_string_table_index: " << fileHeader.ReadSectionHeaderStringTableIndex() << '\n';

        const auto  stringHeader = fileHeader.GetSectionHeader(fileHeader.ReadSectionHeaderStringTableIndex());
        const auto& sectionHeaderStringTable = fileHeader.GetSectionHeaderStringTable();

        ss << "\tstring_table_offset: " << stringHeader.ReadOffset() << '\n';
        ss << "\tstring_table_name_offset: " << stringHeader.ReadNameOffset() << '\n';
        ss << "\tstring_table_name: " << stringHeader.ReadNameFrom(sectionHeaderStringTable) << '\n';
        ss << "\tstring_table_size: " << stringHeader.ReadSize() << '\n';

        ss << "\tIterating Tables\n";
        const auto& stringTable = fileHeader.GetStringTable();

        for (const auto& header : fileHeader.GetSectionHeaders()) {
            ss << "\t\ttable name: " << header.ReadNameFrom(sectionHeaderStringTable)
               << " offset: " << header.ReadOffset() << " size: " << header.ReadSize()
               << " type: " << static_cast< int >(header.ReadType())
               << " num symbol entries: " << header.GetSymbolTableEntryCount() << '\n';

            for (const auto& symbolTableEntry : header.GetSymbolTableEntries()) {
                ss << "\t\tname_offset: " << symbolTableEntry.ReadNameOffset()
                   << " symbol name: " << symbolTableEntry.ReadNameFrom(stringTable)
                   << " symbol offset: " << symbolTableEntry.ReadValue()
                   << " table index: " << symbolTableEntry.ReadSectionHeaderTableIndex() << '\n';
                if (symbolTableEntry.ReadNameFrom(stringTable) == "main") {
                    ss << "FOUND MAIN!\n";
                }
            }

            ss << "\t\t\trelocation entries: " << header.GetRelocationEntryCount() << '\n';

            for (const auto& relocationTableEntry : header.GetRelocationEntries()) {
                ss << "\t\t\t\tfile_offset: " << relocationTableEntry.ReadFileOffset()
                   << " symbol: " << relocationTableEntry.ReadSymbol() << " symbol name: "
                   << fileHeader.GetSymbolTable()
                          .GetSymbolTableEntry(relocationTableEntry.ReadSymbol())
                          .ReadNameFrom(stringTable)
                   << '\n';
            }

            ss << "\t\t\trelocation addend entries: " << header.GetRelocationAddendEntryCount() << '\n';

            for (const auto& relocationAddendTableEntry : header.GetRelocationAddendEntries()) {
                ss << "\t\t\t\tfile_offset: " << relocationAddendTableEntry.ReadFileOffset()
                   << " symbol: " << relocationAddendTableEntry.ReadSymbol() << " symbol name: "
                   << fileHeader.GetSymbolTable()
                          .GetSymbolTableEntry(relocationAddendTableEntry.ReadSymbol())
                          .ReadNameFrom(stringTable)
                   << '\n';
            }
        }
        Log(LogType::ELFSummary, "{}", ss.str());
    }

    return CompilationResult {
        sourceFile, std::regex_replace(std::string { assembly.begin(), assembly.end() }, strip_attributes, ""),
        std::move(elfFile), parseDisassembly(disassembly, sectionOffsets)
    };
}

std::filesystem::path Compiler::GetPath() const noexcept {
    return m_compiler;
}

END_NAMESPACE
