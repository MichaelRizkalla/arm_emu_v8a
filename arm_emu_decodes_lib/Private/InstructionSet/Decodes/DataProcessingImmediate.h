#if !defined(DATAPROCESSINGIMMEDIATE_H_INCLUDED_322A1D8D_5887_46D3_A5D8_932293947363)
    #define DATAPROCESSINGIMMEDIATE_H_INCLUDED_322A1D8D_5887_46D3_A5D8_932293947363

// Based on C4.1.2 A64 instruction set encoding

    #include <API/Api.h>
    #include <InstructionSet/Decodes/DecodeGroup.h>
    #include <Utility/Singleton.h>
    #include <Utility/Utilities.h>
    #include <concepts>
    #include <cstdint>
    #include <numeric>

BEGIN_NAMESPACE

enum class A64DataProcessingImmediateGroup : std::uint32_t
{
    PC_RELATIVE_ADDRESSING,
    ADD_SUBTRACT_IMMEDIATE,
    ADD_SUBTRACT_IMMEDIATE_TAG,
    LOGICAL_IMMEDIATE,
    MOVE_WIDE_IMMEDIATE,
    BITFIELD,
    EXTRACT,

    ARM_EMU_COUNT,

    Undefined = std::numeric_limits< std::underlying_type_t< A64DataProcessingImmediateGroup > >::max(),
};

static constexpr Table< A64DataProcessingImmediateGroup, std::underlying_type_t< A64DataProcessingImmediateGroup >,
                        enum_size_v< A64DataProcessingImmediateGroup > >
    A64DataProcessingImmediateGroupTable { { {
        { 0b0001'1111'0000'0000'0000'0000'0000'0000, 0b0001'0000'0000'0000'0000'0000'0000'0000,
          A64DataProcessingImmediateGroup::PC_RELATIVE_ADDRESSING },
        { 0b0001'1111'1000'0000'0000'0000'0000'0000, 0b0001'0001'0000'0000'0000'0000'0000'0000,
          A64DataProcessingImmediateGroup::ADD_SUBTRACT_IMMEDIATE },
        { 0b0001'1111'1000'0000'0000'0000'0000'0000, 0b0001'0001'1000'0000'0000'0000'0000'0000,
          A64DataProcessingImmediateGroup::ADD_SUBTRACT_IMMEDIATE_TAG },
        { 0b0001'1111'1000'0000'0000'0000'0000'0000, 0b0001'0010'0000'0000'0000'0000'0000'0000,
          A64DataProcessingImmediateGroup::LOGICAL_IMMEDIATE },
        { 0b0001'1111'1000'0000'0000'0000'0000'0000, 0b0001'0010'1000'0000'0000'0000'0000'0000,
          A64DataProcessingImmediateGroup::MOVE_WIDE_IMMEDIATE },
        { 0b0001'1111'1000'0000'0000'0000'0000'0000, 0b0001'0011'0000'0000'0000'0000'0000'0000,
          A64DataProcessingImmediateGroup::BITFIELD },
        { 0b0001'1111'1000'0000'0000'0000'0000'0000, 0b0001'0011'1000'0000'0000'0000'0000'0000,
          A64DataProcessingImmediateGroup::EXTRACT },
    } } };

class ARMEMU_API DataProcessingImmediateGroup final : public DecodeGroup< A64DataProcessingImmediateGroup > {
  protected:
    [[nodiscard]] DataProcessingImmediateGroup() noexcept = default;
    DELETE_COPY_CLASS(DataProcessingImmediateGroup)
    DELETE_MOVE_CLASS(DataProcessingImmediateGroup)

  public:
    DECLARE_AS_SINGLETON(DataProcessingImmediateGroup, true)

    enum class PCRelativeAddressing : std::uint32_t
    {
        ADR,
        ADRP,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< PCRelativeAddressing > >::max(),
    };
    enum class AddSubtractImmediate : std::uint32_t
    {
        ADDi_32BIT,
        ADDSi_32BIT,
        SUBi_32BIT,
        SUBSi_32BIT,
        ADDi_64BIT,
        ADDSi_64BIT,
        SUBi_64BIT,
        SUBSi_64BIT,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< AddSubtractImmediate > >::max(),
    };
    enum class AddSubtractImmediateTag : std::uint32_t
    {
        ADDG,
        SUBG,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< AddSubtractImmediateTag > >::max(),
    };
    enum class LogicalImmediate : std::uint32_t
    {
        AND_32BIT,
        ORR_32BIT,
        EOR_32BIT,
        ANDS_32BIT,
        AND_64BIT,
        ORR_64BIT,
        EOR_64BIT,
        ANDS_64BIT,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< LogicalImmediate > >::max(),
    };
    enum class MoveWideImmediate : std::uint32_t
    {
        MOVN_32BIT,
        MOVZ_32BIT,
        MOVK_32BIT,
        MOVN_64BIT,
        MOVZ_64BIT,
        MOVK_64BIT,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< MoveWideImmediate > >::max(),
    };
    enum class Bitfield : std::uint32_t
    {
        SBFM_32BIT,
        BFM_32BIT,
        UBFM_32BIT,
        SBFM_64BIT,
        BFM_64BIT,
        UBFM_64BIT,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< Bitfield > >::max(),
    };
    enum class Extract : std::uint32_t
    {
        EXTR_32BIT,
        EXTR_64BIT,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< Extract > >::max(),
    };

  private:
    inline static constexpr Table< PCRelativeAddressing, std::underlying_type_t< PCRelativeAddressing >,
                                   enum_size_v< PCRelativeAddressing > >
        PCRelativeAddressingTable { { {
            { 0b1001'1111'0000'0000'0000'0000'0000'0000, 0b0001'0000'0000'0000'0000'0000'0000'0000,
              PCRelativeAddressing::ADR },
            { 0b1001'1111'0000'0000'0000'0000'0000'0000, 0b1001'0000'0000'0000'0000'0000'0000'0000,
              PCRelativeAddressing::ADRP },
        } } };
    inline static constexpr Table< AddSubtractImmediate, std::underlying_type_t< AddSubtractImmediate >,
                                   enum_size_v< AddSubtractImmediate > >
        AddSubtractImmediateTable { { {
            { 0b1111'1111'1000'0000'0000'0000'0000'0000, 0b0001'0001'0000'0000'0000'0000'0000'0000,
              AddSubtractImmediate::ADDi_32BIT },
            { 0b1111'1111'1000'0000'0000'0000'0000'0000, 0b0011'0001'0000'0000'0000'0000'0000'0000,
              AddSubtractImmediate::ADDSi_32BIT },
            { 0b1111'1111'1000'0000'0000'0000'0000'0000, 0b0101'0001'0000'0000'0000'0000'0000'0000,
              AddSubtractImmediate::SUBi_32BIT },
            { 0b1111'1111'1000'0000'0000'0000'0000'0000, 0b0111'0001'0000'0000'0000'0000'0000'0000,
              AddSubtractImmediate::SUBSi_32BIT },
            { 0b1111'1111'1000'0000'0000'0000'0000'0000, 0b1001'0001'0000'0000'0000'0000'0000'0000,
              AddSubtractImmediate::ADDi_64BIT },
            { 0b1111'1111'1000'0000'0000'0000'0000'0000, 0b1011'0001'0000'0000'0000'0000'0000'0000,
              AddSubtractImmediate::ADDSi_64BIT },
            { 0b1111'1111'1000'0000'0000'0000'0000'0000, 0b1101'0001'0000'0000'0000'0000'0000'0000,
              AddSubtractImmediate::SUBi_64BIT },
            { 0b1111'1111'1000'0000'0000'0000'0000'0000, 0b1111'0001'0000'0000'0000'0000'0000'0000,
              AddSubtractImmediate::SUBSi_64BIT },
        } } };
    inline static constexpr Table< AddSubtractImmediateTag, std::underlying_type_t< AddSubtractImmediateTag >,
                                   enum_size_v< AddSubtractImmediateTag > >
        AddSubtractImmediateTagTable { { {
            { 0b1111'1111'1100'0000'0000'0000'0000'0000, 0b1001'0001'1000'0000'0000'0000'0000'0000,
              AddSubtractImmediateTag::ADDG },
            { 0b1111'1111'1100'0000'0000'0000'0000'0000, 0b1101'0001'1000'0000'0000'0000'0000'0000,
              AddSubtractImmediateTag::SUBG },
        } } };
    inline static constexpr Table< LogicalImmediate, std::underlying_type_t< LogicalImmediate >,
                                   enum_size_v< LogicalImmediate > >
        LogicalImmediateTable { { {
            { 0b1111'1111'1100'0000'0000'0000'0000'0000, 0b0001'0010'0000'0000'0000'0000'0000'0000,
              LogicalImmediate::AND_32BIT },
            { 0b1111'1111'1100'0000'0000'0000'0000'0000, 0b0011'0010'0000'0000'0000'0000'0000'0000,
              LogicalImmediate::ORR_32BIT },
            { 0b1111'1111'1100'0000'0000'0000'0000'0000, 0b0101'0010'0000'0000'0000'0000'0000'0000,
              LogicalImmediate::EOR_32BIT },
            { 0b1111'1111'1100'0000'0000'0000'0000'0000, 0b0111'0010'0000'0000'0000'0000'0000'0000,
              LogicalImmediate::ANDS_32BIT },
            { 0b1111'1111'1000'0000'0000'0000'0000'0000, 0b1001'0010'0000'0000'0000'0000'0000'0000,
              LogicalImmediate::AND_64BIT },
            { 0b1111'1111'1000'0000'0000'0000'0000'0000, 0b1011'0010'0000'0000'0000'0000'0000'0000,
              LogicalImmediate::ORR_64BIT },
            { 0b1111'1111'1000'0000'0000'0000'0000'0000, 0b1101'0010'0000'0000'0000'0000'0000'0000,
              LogicalImmediate::EOR_64BIT },
            { 0b1111'1111'1000'0000'0000'0000'0000'0000, 0b1111'0010'0000'0000'0000'0000'0000'0000,
              LogicalImmediate::ANDS_64BIT },
        } } };
    inline static constexpr Table< MoveWideImmediate, std::underlying_type_t< MoveWideImmediate >,
                                   enum_size_v< MoveWideImmediate > >
        MoveWideImmediateTable { { {
            { 0b1111'1111'1100'0000'0000'0000'0000'0000, 0b0001'0010'1000'0000'0000'0000'0000'0000,
              MoveWideImmediate::MOVN_32BIT },
            { 0b1111'1111'1100'0000'0000'0000'0000'0000, 0b0101'0010'1000'0000'0000'0000'0000'0000,
              MoveWideImmediate::MOVZ_32BIT },
            { 0b1111'1111'1100'0000'0000'0000'0000'0000, 0b0111'0010'1000'0000'0000'0000'0000'0000,
              MoveWideImmediate::MOVK_32BIT },
            { 0b1111'1111'1000'0000'0000'0000'0000'0000, 0b1001'0010'1000'0000'0000'0000'0000'0000,
              MoveWideImmediate::MOVN_64BIT },
            { 0b1111'1111'1000'0000'0000'0000'0000'0000, 0b1101'0010'1000'0000'0000'0000'0000'0000,
              MoveWideImmediate::MOVZ_64BIT },
            { 0b1111'1111'1000'0000'0000'0000'0000'0000, 0b1111'0010'1000'0000'0000'0000'0000'0000,
              MoveWideImmediate::MOVK_64BIT },
        } } };
    inline static constexpr Table< Bitfield, std::underlying_type_t< Bitfield >, enum_size_v< Bitfield > >
        BitfieldTable { { {
            { 0b1111'1111'1100'0000'0000'0000'0000'0000, 0b0001'0011'0000'0000'0000'0000'0000'0000,
              Bitfield::SBFM_32BIT },
            { 0b1111'1111'1100'0000'0000'0000'0000'0000, 0b0011'0011'0000'0000'0000'0000'0000'0000,
              Bitfield::BFM_32BIT },
            { 0b1111'1111'1100'0000'0000'0000'0000'0000, 0b0101'0011'0000'0000'0000'0000'0000'0000,
              Bitfield::UBFM_32BIT },
            { 0b1111'1111'1100'0000'0000'0000'0000'0000, 0b1001'0011'0100'0000'0000'0000'0000'0000,
              Bitfield::SBFM_64BIT },
            { 0b1111'1111'1100'0000'0000'0000'0000'0000, 0b1011'0011'0100'0000'0000'0000'0000'0000,
              Bitfield::BFM_64BIT },
            { 0b1111'1111'1100'0000'0000'0000'0000'0000, 0b1101'0011'0100'0000'0000'0000'0000'0000,
              Bitfield::UBFM_64BIT },
        } } };
    inline static constexpr Table< Extract, std::underlying_type_t< Extract >, enum_size_v< Extract > > ExtractTable {
        { {
            { 0b1111'1111'1110'0000'1000'0000'0000'0000, 0b0001'0011'1000'0000'0000'0000'0000'0000,
              Extract::EXTR_32BIT },
            { 0b1111'1111'1110'0000'0000'0000'0000'0000, 0b1001'0011'1100'0000'0000'0000'0000'0000,
              Extract::EXTR_64BIT },
        } }
    };

    template < A64DataProcessingImmediateGroup InstructionClass >
    [[nodiscard]] auto GetInstructionClassTableInternal() const noexcept {
        if constexpr (InstructionClass == A64DataProcessingImmediateGroup::PC_RELATIVE_ADDRESSING) {
            return PCRelativeAddressingTable;
        } else if constexpr (InstructionClass == A64DataProcessingImmediateGroup::ADD_SUBTRACT_IMMEDIATE) {
            return AddSubtractImmediateTable;
        } else if constexpr (InstructionClass == A64DataProcessingImmediateGroup::ADD_SUBTRACT_IMMEDIATE_TAG) {
            return AddSubtractImmediateTagTable;
        } else if constexpr (InstructionClass == A64DataProcessingImmediateGroup::LOGICAL_IMMEDIATE) {
            return LogicalImmediateTable;
        } else if constexpr (InstructionClass == A64DataProcessingImmediateGroup::MOVE_WIDE_IMMEDIATE) {
            return MoveWideImmediateTable;
        } else if constexpr (InstructionClass == A64DataProcessingImmediateGroup::BITFIELD) {
            return BitfieldTable;
        } else if constexpr (InstructionClass == A64DataProcessingImmediateGroup::EXTRACT) {
            return ExtractTable;
        } else {
            throw ARM_EMU_EXCEPTION; // Compile time error detection
        }
    }

    [[nodiscard]] A64DataProcessingImmediateGroup
        GetInstructionClassInternal(const Instruction& instruction) const noexcept final {
        return A64DataProcessingImmediateGroupTable.Lookup(instruction.Get());
    }

  public:
    template < InstructionGroup InstructionClass >
    [[nodiscard]] auto GetInstructionType(const Instruction& instruction) const noexcept {
        const auto& mTable = GetInstructionClassTableInternal< InstructionClass >();
        return mTable.Lookup(instruction.Get());
    }
};

END_NAMESPACE

#endif // !defined(DATAPROCESSINGIMMEDIATE_H_INCLUDED_322A1D8D_5887_46D3_A5D8_932293947363)
