
#include <API/HiddenAPI.h>
#include <DebugUtils/LogType.h>
#include <Utility/StreamableEnum.h>
#include <Utility/Utilities.h>
#include <string_view>

BEGIN_NAMESPACE

namespace {

    using namespace std::literals::string_view_literals;

    static constexpr SmallConstMap< LogType, std::string_view, 11 > logTypeStringMap {
        { { { LogType::None, "None"sv },
            { LogType::Allocation, "Allocation"sv },
            { LogType::Deallocation, "Deallocation"sv },
            { LogType::Construction, "Construction"sv },
            { LogType::Destruction, "Destruction"sv },
            { LogType::MMU, "MMU"sv },
            { LogType::Instruction, "Instruction"sv },
            { LogType::Other, "Other"sv },
            { LogType::ObjectLifetime, "ObjectLifetime"sv },
            { LogType::MemoryResource, "MemoryResource"sv },
            { LogType::All, "All"sv } } }
    };

} // namespace

template <>
static std::string_view Enum::ToChar(LogType logType) {
    return logTypeStringMap.At(logType);
}

template <>
std::ostream& operator<<(std::ostream& os, LogType logType) {
    os << Enum::ToChar(logType);
    return os;
}

END_NAMESPACE
