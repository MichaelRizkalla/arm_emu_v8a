
#include <API/HiddenAPI.h>
#include <DebugUtils/LogType.h>
#include <Utility/StreamableEnum.h>
#include <Utility/Utilities.h>
#include <string_view>

BEGIN_NAMESPACE

namespace {

    using namespace std::literals::string_view_literals;

    static constexpr SmallConstMap< LogType, std::string_view, 18 > logTypeStringMap {
        { { { LogType::None, "None"sv },

            { LogType::Allocation, "Allocation"sv },
            { LogType::Deallocation, "Deallocation"sv },
            { LogType::Construction, "Construction"sv },
            { LogType::Destruction, "Destruction"sv },

            { LogType::MMU, "MMU"sv },
            { LogType::Instruction, "Instruction"sv },

            { LogType::Compiler, "Compiler"sv },
            { LogType::SymbolResolving, "SymbolResolving"sv },
            { LogType::ParsingObjectFile, "ParsingObjectFile"sv },
            { LogType::ParsingDisassembly, "ParsingDisassembly"sv },
            { LogType::CompileOutput, "CompileOutput"sv },
            { LogType::ELFSummary, "ELFSummary"sv },

            { LogType::Other, "Other"sv },
            { LogType::ObjectLifetime, "ObjectLifetime"sv },
            { LogType::MemoryResource, "MemoryResource"sv },
            { LogType::Compilation, "Compilation"sv },
            { LogType::All, "All"sv } } }
    };

} // namespace

template <>
ARMEMU_API std::string_view Enum::ToChar(LogType logType) {
    return logTypeStringMap.At(logType);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, LogType logType) {
    os << Enum::ToChar(logType);
    return os;
}

END_NAMESPACE
