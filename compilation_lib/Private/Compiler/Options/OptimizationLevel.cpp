
#include <Compiler/Options/OptimizationLevel.h>
#include <Utility/StreamableEnum.h>
#include <Utility/Utilities.h>
#include <string_view>

BEGIN_NAMESPACE

namespace {

    using namespace std::literals::string_view_literals;

    static constexpr SmallConstMap< OptimizationLevel, std::string_view, 4 > optimizationLevelStringMap { { {
        { OptimizationLevel::O0, "O0"sv },
        { OptimizationLevel::O1, "O1"sv },
        { OptimizationLevel::O2, "O2"sv },
        { OptimizationLevel::O3, "O3"sv },
    } } };

} // namespace

template <>
std::string_view Enum::ToChar(OptimizationLevel enumValue) {
    return optimizationLevelStringMap.At(enumValue);
}

template <>
std::ostream& operator<<(std::ostream& os, OptimizationLevel enumValue) {
    os << Enum::ToChar(enumValue);
    return os;
}

END_NAMESPACE
