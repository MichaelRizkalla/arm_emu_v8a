
#include <ProcessingUnit/A64InstructionSet/A64InstructionSet.h>
#include <Utility/StreamableEnum.h>
#include <ostream>
#include <string_view>

BEGIN_NAMESPACE

IMPLEMENT_AS_SINGLETON(BranchExceptionSystemGroup)
IMPLEMENT_AS_SINGLETON(DataProcessingImmediateGroup)
IMPLEMENT_AS_SINGLETON(DataProcessingRegisterGroup)
IMPLEMENT_AS_SINGLETON(DataProcessingScalarFloatingPointAdvancedSIMDGroup)
IMPLEMENT_AS_SINGLETON(LoadStoreGroup)
IMPLEMENT_AS_SINGLETON(ReservedGroup)

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
static std::string_view Enum::ToChar(A64DecodeGroup A64DecodeGroupType) {
    return A64DecodeGroupStringMap.At(A64DecodeGroupType);
}

template <>
std::ostream& operator<<(std::ostream& os, A64DecodeGroup A64DecodeGroupType) {
    os << Enum::ToChar(A64DecodeGroupType);
    return os;
}

END_NAMESPACE
