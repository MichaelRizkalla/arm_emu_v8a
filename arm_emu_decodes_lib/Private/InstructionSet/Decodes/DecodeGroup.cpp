
#include <InstructionSet/A64InstructionSet.h>
#include <Utility/StreamableEnum.h>
#include <ostream>
#include <string_view>

BEGIN_NAMESPACE

namespace {

    using namespace std::literals::string_view_literals;

    static constexpr SmallConstMap< A64DecodeGroup, std::string_view, enum_size_v< A64DecodeGroup > >
        A64DecodeGroupStringMap { { {
            { A64DecodeGroup::Reserved, "Reserved"sv },
            { A64DecodeGroup::ScalableVectorExtension, "ScalableVectorExtension"sv },
            { A64DecodeGroup::DataProcessingImmediate, "DataProcessingImmediate"sv },
            { A64DecodeGroup::BranchExceptionSystem, "BranchExceptionSystem"sv },
            { A64DecodeGroup::LoadStore, "LoadStore"sv },
            { A64DecodeGroup::DataProcessingRegister, "DataProcessingRegister"sv },
            { A64DecodeGroup::DataProcessingScalarFloatingPointAdvancedSIMD,
              "DataProcessingScalarFloatingPointAdvancedSIMD"sv },
        } } };

} // namespace

template <>
ARMEMU_API std::string_view Enum::ToChar(A64DecodeGroup enumValue) {
    return A64DecodeGroupStringMap.At(enumValue);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, A64DecodeGroup enumValue) {
    os << Enum::ToChar(enumValue);
    return os;
}

END_NAMESPACE
