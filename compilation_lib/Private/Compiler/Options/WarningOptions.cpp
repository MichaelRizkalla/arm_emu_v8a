
#include <Compiler/Options/WarningOptions.h>
#include <Utility/StreamableEnum.h>
#include <Utility/Utilities.h>
#include <string_view>

BEGIN_NAMESPACE

namespace {

    using namespace std::literals::string_view_literals;

    static constexpr SmallConstMap< WarningOptions, std::string_view, 6 > warningOptionsStringMap { { {
        { WarningOptions::Wnone, "w"sv },
        { WarningOptions::Wall, "Wall"sv },
        { WarningOptions::Wextra, "Wextra"sv },
        { WarningOptions::Wpendantic, "Wpendantic"sv },
        { WarningOptions::Weverything, "Weverything"sv },
        { WarningOptions::Werror, "Werror"sv },
    } } };

} // namespace

template <>
std::string_view Enum::ToChar(WarningOptions value) {
    return warningOptionsStringMap.At(value);
}

template <>
std::ostream& operator<<(std::ostream& os, WarningOptions value) {
    os << Enum::ToChar(value);
    return os;
}

END_NAMESPACE
