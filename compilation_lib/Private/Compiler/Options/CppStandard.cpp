
#include <Compiler/Options/CppStandard.h>
#include <Utility/StreamableEnum.h>
#include <Utility/Utilities.h>
#include <string_view>

BEGIN_NAMESPACE

namespace {

    using namespace std::literals::string_view_literals;

    static constexpr SmallConstMap< CppStandard, std::string_view, 4 > cppStandardStringMap { { {
        { CppStandard::Cpp11, "c++11"sv },
        { CppStandard::Cpp17, "c++17"sv },
        { CppStandard::Cpp20, "c++20"sv },
        { CppStandard::Cpp23, "c++23"sv },
    } } };

} // namespace

template <>
std::string_view Enum::ToChar(CppStandard enumValue) {
    return cppStandardStringMap.At(enumValue);
}

template <>
std::ostream& operator<<(std::ostream& os, CppStandard enumValue) {
    os << Enum::ToChar(enumValue);
    return os;
}

END_NAMESPACE
