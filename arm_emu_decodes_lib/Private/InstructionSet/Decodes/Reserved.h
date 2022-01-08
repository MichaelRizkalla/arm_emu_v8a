#if !defined(RESERVED_H_INCLUDED_500C9B98_6748_4723_AD33_162AEAD1620D)
    #define RESERVED_H_INCLUDED_500C9B98_6748_4723_AD33_162AEAD1620D

    #include <API/Api.h>
    #include <InstructionSet/Decodes/DecodeGroup.h>
    #include <Utility/Singleton.h>
    #include <Utility/Utilities.h>
    #include <cstdint>
    #include <numeric>

BEGIN_NAMESPACE

enum class A64ReservedGroup : std::uint32_t
{
    // Reserved
    UDP,

    ARM_EMU_COUNT,

    Undefined = std::numeric_limits< std::underlying_type_t< A64ReservedGroup > >::max(),
};

static constexpr Table< A64ReservedGroup, std::underlying_type_t< A64ReservedGroup >, enum_size_v< A64ReservedGroup > >
    A64ReservedGroupTable { { {
        { 0b1111'1111'1111'1111'0000'0000'0000'0000, 0b0000'0000'0000'0000'0000'0000'0000'0000, A64ReservedGroup::UDP },
    } } };

class ARMEMU_API ReservedGroup final : public DecodeGroup< A64ReservedGroup > {
  protected:
    [[nodiscard]] ReservedGroup() noexcept = default;
    DELETE_COPY_CLASS(ReservedGroup)
    DELETE_MOVE_CLASS(ReservedGroup)

  public:
    DECLARE_AS_SINGLETON(ReservedGroup, true)

  private:
    template < A64ReservedGroup InstructionClass >
    [[nodiscard]] auto GetInstructionClassTableInternal() const noexcept {
        throw ARM_EMU_EXCEPTION; // Compile time error detection
    }

    [[nodiscard]] A64ReservedGroup GetInstructionClassInternal(const Instruction& instruction) const noexcept final {
        return A64ReservedGroupTable.Lookup(instruction.Get());
    }

  public:
    template < InstructionGroup InstructionClass >
    [[nodiscard]] auto GetInstructionType(const Instruction& instruction) const noexcept {
        const auto& mTable = GetInstructionClassTableInternal< InstructionClass >();
        return mTable.Lookup(instruction.Get());
    }
};

END_NAMESPACE

// Based on C4.1.1 A64 instruction set encoding
#endif // !defined(RESERVED_H_INCLUDED_500C9B98_6748_4723_AD33_162AEAD1620D)
