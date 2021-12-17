#if !defined(DECODEGROUP_H_INCLUDED_91432BE9_09EB_4872_B379_58C602771968)
    #define DECODEGROUP_H_INCLUDED_91432BE9_09EB_4872_B379_58C602771968

// Based on C4.1 A64 instruction set encoding

    #include <API/Api.h>
    #include <ProcessingUnit/A64Instruction/Instruction.h>
    #include <Utility/Utilities.h>
    #include <concepts>
    #include <cstdint>
    #include <numeric>

BEGIN_NAMESPACE

enum class A64DecodeGroup : std::uint32_t
{
    Reserved,

    ScalableVectorExtension,

    DataProcessingImmediate,

    BranchExceptionSystem,

    LoadStore,

    DataProcessingRegister,

    DataProcessingScalarFloatingPointAdvancedSIMD,

    ARM_EMU_COUNT,

    Undefined = std::numeric_limits< std::underlying_type_t< A64DecodeGroup > >::max(),
};

static constexpr Table< A64DecodeGroup, std::uint8_t, 7 > A64DecodeGroupTable { { {
    { 0b0000'1111, 0b0000'0000, A64DecodeGroup::Reserved },
    { 0b0000'1111, 0b0000'0010, A64DecodeGroup::ScalableVectorExtension },
    { 0b0000'1110, 0b0000'1000, A64DecodeGroup::DataProcessingImmediate },
    { 0b0000'1110, 0b0000'1010, A64DecodeGroup::BranchExceptionSystem },
    { 0b0000'0111, 0b0000'0101, A64DecodeGroup::DataProcessingRegister },
    { 0b0000'0111, 0b0000'0111, A64DecodeGroup::DataProcessingScalarFloatingPointAdvancedSIMD },
    { 0b0000'0101, 0b0000'0100, A64DecodeGroup::LoadStore },
} } };

template < class GroupType >
class DecodeGroup {
  protected:
    [[nodiscard]] DecodeGroup() noexcept = default;
    DELETE_COPY_CLASS(DecodeGroup)
    DELETE_MOVE_CLASS(DecodeGroup)

  public:
    using InstructionGroup = GroupType;

    [[nodiscard]] InstructionGroup GetInstructionClass(const Instruction& instruction) const noexcept {
        return GetInstructionClassInternal(instruction);
    }

  private:
    virtual InstructionGroup GetInstructionClassInternal(const Instruction& instruction) const noexcept = 0;
};

END_NAMESPACE

#endif // !defined(DECODEGROUP_H_INCLUDED_91432BE9_09EB_4872_B379_58C602771968)
