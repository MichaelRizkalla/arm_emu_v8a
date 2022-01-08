
#include <InstructionSet/A64InstructionSet.h>
#include <Utility/StreamableEnum.h>
#include <ostream>
#include <string_view>

BEGIN_NAMESPACE

namespace {

    using namespace std::literals::string_view_literals;

    static constexpr SmallConstMap< A64ReservedGroup, std::string_view, enum_size_v< A64ReservedGroup > >
        A64ReservedGroupStringMap { { {
            { A64ReservedGroup::UDP, "UDP"sv },
        } } };

} // namespace

template <>
ARMEMU_API std::string_view Enum::ToChar(A64ReservedGroup enumValue) {
    return A64ReservedGroupStringMap.At(enumValue);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, A64ReservedGroup enumValue) {
    os << Enum::ToChar(enumValue);
    return os;
}

END_NAMESPACE
