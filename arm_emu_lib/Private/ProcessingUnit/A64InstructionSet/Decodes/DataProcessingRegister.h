#if !defined(DATAPROCESSINGREGISTER_H_INCLUDED_73F4F724_7B63_4963_91BA_F59C9ABA9B35)
    #define DATAPROCESSINGREGISTER_H_INCLUDED_73F4F724_7B63_4963_91BA_F59C9ABA9B35

// Based on C4.1.5 A64 instruction set encoding

    #include <API/Api.h>
    #include <ProcessingUnit/A64InstructionSet/Decodes/DecodeGroup.h>
    #include <Utility/Singleton.h>
    #include <Utility/Utilities.h>
    #include <concepts>
    #include <cstdint>
    #include <numeric>

BEGIN_NAMESPACE

enum class A64DataProcessingRegisterGroup : std::uint32_t
{
    DATA_PROCESSING_TWO_SOURCE,
    DATA_PROCESSING_ONE_SOURCE,
    LOGICAL_SHIFTED_REGISTER,
    ADD_SUBTRACT_SHIFTED_REGISTER,
    ADD_SUBTRACT_EXTENDED_REGISTER,
    ADD_SUBTRACT_CARRY,
    ROTATE_RIGHT_INTO_FLAGS,
    EVALUATE_INTO_FLAGS,
    CONDITIONAL_COMPARE_REGISTER,
    CONDITIONAL_COMPARE_IMMEDIATE,
    CONDITIONAL_SELECT,
    DATA_PROCESSING_THREE_SOURCE,

    ARM_EMU_COUNT,

    Undefined = std::numeric_limits< std::underlying_type_t< A64DataProcessingRegisterGroup > >::max(),
};

static constexpr Table< A64DataProcessingRegisterGroup, std::underlying_type_t< A64DataProcessingRegisterGroup >,
                        enum_size_v< A64DataProcessingRegisterGroup > >
    A64DataProcessingRegisterGroupTable { { {
        { 0b0101'1111'1110'0000'0000'0000'0000'0000, 0b0001'1010'1100'0000'0000'0000'0000'0000,
          A64DataProcessingRegisterGroup::DATA_PROCESSING_TWO_SOURCE },
        { 0b0101'1111'1110'0000'0000'0000'0000'0000, 0b0101'1010'1100'0000'0000'0000'0000'0000,
          A64DataProcessingRegisterGroup::DATA_PROCESSING_ONE_SOURCE },
        { 0b0001'1111'0000'0000'0000'0000'0000'0000, 0b0000'1010'0000'0000'0000'0000'0000'0000,
          A64DataProcessingRegisterGroup::LOGICAL_SHIFTED_REGISTER },
        { 0b0001'1111'0010'0000'0000'0000'0000'0000, 0b0000'1011'0000'0000'0000'0000'0000'0000,
          A64DataProcessingRegisterGroup::ADD_SUBTRACT_SHIFTED_REGISTER },
        { 0b0001'1111'0010'0000'0000'0000'0000'0000, 0b0000'1011'0010'0000'0000'0000'0000'0000,
          A64DataProcessingRegisterGroup::ADD_SUBTRACT_EXTENDED_REGISTER },
        { 0b0001'1111'1110'0000'1111'1100'0000'0000, 0b0001'1010'0000'0000'0000'0000'0000'0000,
          A64DataProcessingRegisterGroup::ADD_SUBTRACT_CARRY },
        { 0b0001'1111'1110'0000'0111'1100'0000'0000, 0b0001'1010'0000'0000'0000'0100'0000'0000,
          A64DataProcessingRegisterGroup::ROTATE_RIGHT_INTO_FLAGS },
        { 0b0001'1111'1110'0000'0011'1100'0000'0000, 0b0001'1010'0000'0000'0000'1000'0000'0000,
          A64DataProcessingRegisterGroup::EVALUATE_INTO_FLAGS },
        { 0b0001'1111'1110'0000'0000'1000'0000'0000, 0b0001'1010'0100'0000'0000'0000'0000'0000,
          A64DataProcessingRegisterGroup::CONDITIONAL_COMPARE_REGISTER },
        { 0b0001'1111'1110'0000'0000'1000'0000'0000, 0b0001'1010'0100'0000'0000'1000'0000'0000,
          A64DataProcessingRegisterGroup::CONDITIONAL_COMPARE_IMMEDIATE },
        { 0b0001'1111'1110'0000'0000'0000'0000'0000, 0b0001'1010'1000'0000'0000'0000'0000'0000,
          A64DataProcessingRegisterGroup::CONDITIONAL_SELECT },
        { 0b0001'1111'0000'0000'0000'0000'0000'0000, 0b0001'1011'0000'0000'0000'0000'0000'0000,
          A64DataProcessingRegisterGroup::DATA_PROCESSING_THREE_SOURCE },
    } } };

class DataProcessingRegisterGroup final : public DecodeGroup< A64DataProcessingRegisterGroup > {
  protected:
    [[nodiscard]] DataProcessingRegisterGroup() noexcept = default;
    DELETE_COPY_CLASS(DataProcessingRegisterGroup)
    DELETE_MOVE_CLASS(DataProcessingRegisterGroup)

  public:
    DECLARE_AS_SINGLETON(DataProcessingRegisterGroup)

    enum class DataProcessingTwoSource : std::uint32_t
    {
        UDIV_32BIT,
        SDIV_32BIT,
        LSLV_32BIT,
        LSRV_32BIT,
        ASRV_32BIT,
        RORV_32BIT,
        CRC32B,
        CRC32H,
        CRC32W,
        CRC32CB,
        CRC32CH,
        CRC32CW,
        SUBP,
        UDIV_64BIT,
        SDIV_64BIT,
        IRG,
        GMI,
        LSLV_64BIT,
        LSRV_64BIT,
        ASRV_64BIT,
        RORV_64BIT,
        PACGA,
        CRC32X,
        CRC32CX,
        SUBPS,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< DataProcessingTwoSource > >::max()
    };
    enum class DataProcessingOneSource : std::uint32_t
    {
        RBIT_32BIT,
        REV16_32BIT,
        REV_32BIT,
        CLZ_32BIT,
        CLS_32BIT,
        RBIT_64BIT,
        REV16_64BIT,
        REV32,
        REV_64BIT,
        CLZ_64BIT,
        CLS_64BIT,
        PACIA,
        PACIB,
        PACDA,
        PACDB,
        AUTIA,
        AUTIB,
        AUTDA,
        AUTDB,
        PACIZA,
        PACIZB,
        PACDZA,
        PACDZB,
        AUTIZA,
        AUTIZB,
        AUTDZA,
        AUTDZB,
        XPACI,
        XPACD,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< DataProcessingOneSource > >::max()
    };
    enum class LogicalShiftedRegister : std::uint32_t
    {
        AND_32BIT_SHIFTED,
        BIC_32BIT_SHIFTED,
        ORR_32BIT_SHIFTED,
        ORN_32BIT_SHIFTED,
        EOR_32BIT_SHIFTED,
        EON_32BIT_SHIFTED,
        ANDS_32BIT_SHIFTED,
        BICS_32BIT_SHIFTED,
        AND_64BIT_SHIFTED,
        BIC_64BIT_SHIFTED,
        ORR_64BIT_SHIFTED,
        ORN_64BIT_SHIFTED,
        EOR_64BIT_SHIFTED,
        EON_64BIT_SHIFTED,
        ANDS_64BIT_SHIFTED,
        BICS_64BIT_SHIFTED,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< LogicalShiftedRegister > >::max()
    };
    enum class AddSubtractShiftedRegister : std::uint32_t
    {
        ADD_32BIT_SHIFTED,
        ADDS_32BIT_SHIFTED,
        SUB_32BIT_SHIFTED,
        SUBS_32BIT_SHIFTED,
        ADD_64BIT_SHIFTED,
        ADDS_64BIT_SHIFTED,
        SUB_64BIT_SHIFTED,
        SUBS_64BIT_SHIFTED,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< AddSubtractShiftedRegister > >::max()
    };
    enum class AddSubtractExtendedRegister : std::uint32_t
    {
        ADD_32BIT_EXTENDED,
        ADDS_32BIT_EXTENDED,
        SUB_32BIT_EXTENDED,
        SUBS_32BIT_EXTENDED,
        ADD_64BIT_EXTENDED,
        ADDS_64BIT_EXTENDED,
        SUB_64BIT_EXTENDED,
        SUBS_64BIT_EXTENDED,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< AddSubtractExtendedRegister > >::max()
    };
    enum class AddSubtractCarry : std::uint32_t
    {
        ADC_32BIT,
        ADCS_32BIT,
        SBC_32BIT,
        SBCS_32BIT,
        ADC_64BIT,
        ADCS_64BIT,
        SBC_64BIT,
        SBCS_64BIT,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< AddSubtractCarry > >::max()
    };
    enum class RotateRightIntoFlags : std::uint32_t
    {
        RMIF,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< RotateRightIntoFlags > >::max()
    };
    enum class EvaluateIntoFlags : std::uint32_t
    {
        SETF8,
        SETF16,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< EvaluateIntoFlags > >::max()
    };
    enum class ConditionalCompareRegister : std::uint32_t
    {
        CCMNr_32BIT,
        CCMPr_32BIT,
        CCMNr_64BIT,
        CCMPr_64BIT,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< ConditionalCompareRegister > >::max()
    };
    enum class ConditionalCompareImmediate : std::uint32_t
    {
        CCMNi_32BIT,
        CCMPi_32BIT,
        CCMNi_64BIT,
        CCMPi_64BIT,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< ConditionalCompareImmediate > >::max()
    };
    enum class ConditionalSelect : std::uint32_t
    {
        CSEL_32BIT,
        CSINC_32BIT,
        CSINV_32BIT,
        CSNEG_32BIT,
        CSEL_64BIT,
        CSINC_64BIT,
        CSINV_64BIT,
        CSNEG_64BIT,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< ConditionalSelect > >::max()
    };
    enum class DataProcessingThreeSource : std::uint32_t
    {
        MADD_32BIT,
        MSUB_32BIT,
        MADD_64BIT,
        MSUB_64BIT,
        SMADDL,
        SMSUBL,
        SMULH,
        UMADDL,
        UMSUBL,
        UMULH,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< DataProcessingThreeSource > >::max()
    };

  private:
    inline static constexpr Table< DataProcessingTwoSource, std::underlying_type_t< DataProcessingTwoSource >,
                                   enum_size_v< DataProcessingTwoSource > >
        DataProcessingTwoSourceTable { { {
            { 0b1111'1111'1110'0000'1111'1100'0000'0000, 0b0001'1010'1100'0000'0000'1000'0000'0000,
              DataProcessingTwoSource::UDIV_32BIT },
            { 0b1111'1111'1110'0000'1111'1100'0000'0000, 0b0001'1010'1100'0000'0000'1100'0000'0000,
              DataProcessingTwoSource::SDIV_32BIT },
            { 0b1111'1111'1110'0000'1111'1100'0000'0000, 0b0001'1010'1100'0000'0010'0000'0000'0000,
              DataProcessingTwoSource::LSLV_32BIT },
            { 0b1111'1111'1110'0000'1111'1100'0000'0000, 0b0001'1010'1100'0000'0010'0100'0000'0000,
              DataProcessingTwoSource::LSRV_32BIT },
            { 0b1111'1111'1110'0000'1111'1100'0000'0000, 0b0001'1010'1100'0000'0010'1000'0000'0000,
              DataProcessingTwoSource::ASRV_32BIT },
            { 0b1111'1111'1110'0000'1111'1100'0000'0000, 0b0001'1010'1100'0000'0010'1100'0000'0000,
              DataProcessingTwoSource::RORV_32BIT },
            { 0b1111'1111'1110'0000'1111'1100'0000'0000, 0b0001'1010'1100'0000'0100'0000'0000'0000,
              DataProcessingTwoSource::CRC32B },
            { 0b1111'1111'1110'0000'1111'1100'0000'0000, 0b0001'1010'1100'0000'0100'0100'0000'0000,
              DataProcessingTwoSource::CRC32H },
            { 0b1111'1111'1110'0000'1111'1100'0000'0000, 0b0001'1010'1100'0000'0100'1000'0000'0000,
              DataProcessingTwoSource::CRC32W },
            { 0b1111'1111'1110'0000'1111'1100'0000'0000, 0b0001'1010'1100'0000'0101'0000'0000'0000,
              DataProcessingTwoSource::CRC32CB },
            { 0b1111'1111'1110'0000'1111'1100'0000'0000, 0b0001'1010'1100'0000'0101'0100'0000'0000,
              DataProcessingTwoSource::CRC32CH },
            { 0b1111'1111'1110'0000'1111'1100'0000'0000, 0b0001'1010'1100'0000'0101'1000'0000'0000,
              DataProcessingTwoSource::CRC32CW },
            { 0b1111'1111'1110'0000'1111'1100'0000'0000, 0b1001'1010'1100'0000'0000'0000'0000'0000,
              DataProcessingTwoSource::SUBP },
            { 0b1111'1111'1110'0000'1111'1100'0000'0000, 0b1001'1010'1100'0000'0000'1000'0000'0000,
              DataProcessingTwoSource::UDIV_64BIT },
            { 0b1111'1111'1110'0000'1111'1100'0000'0000, 0b1001'1010'1100'0000'0000'1100'0000'0000,
              DataProcessingTwoSource::SDIV_64BIT },
            { 0b1111'1111'1110'0000'1111'1100'0000'0000, 0b1001'1010'1100'0000'0001'0000'0000'0000,
              DataProcessingTwoSource::IRG },
            { 0b1111'1111'1110'0000'1111'1100'0000'0000, 0b1001'1010'1100'0000'0001'0100'0000'0000,
              DataProcessingTwoSource::GMI },
            { 0b1111'1111'1110'0000'1111'1100'0000'0000, 0b1001'1010'1100'0000'0010'0000'0000'0000,
              DataProcessingTwoSource::LSLV_64BIT },
            { 0b1111'1111'1110'0000'1111'1100'0000'0000, 0b1001'1010'1100'0000'0010'0100'0000'0000,
              DataProcessingTwoSource::LSRV_64BIT },
            { 0b1111'1111'1110'0000'1111'1100'0000'0000, 0b1001'1010'1100'0000'0010'1000'0000'0000,
              DataProcessingTwoSource::ASRV_64BIT },
            { 0b1111'1111'1110'0000'1111'1100'0000'0000, 0b1001'1010'1100'0000'0010'1100'0000'0000,
              DataProcessingTwoSource::RORV_64BIT },
            { 0b1111'1111'1110'0000'1111'1100'0000'0000, 0b1001'1010'1100'0000'0011'0000'0000'0000,
              DataProcessingTwoSource::PACGA },
            { 0b1111'1111'1110'0000'1111'1100'0000'0000, 0b1001'1010'1100'0000'0100'1100'0000'0000,
              DataProcessingTwoSource::CRC32X },
            { 0b1111'1111'1110'0000'1111'1100'0000'0000, 0b1001'1010'1100'0000'0101'1100'0000'0000,
              DataProcessingTwoSource::CRC32CX },
            { 0b1111'1111'1110'0000'1111'1100'0000'0000, 0b1011'1010'1100'0000'0000'0000'0000'0000,
              DataProcessingTwoSource::SUBPS },
        } } };
    inline static constexpr Table< DataProcessingOneSource, std::underlying_type_t< DataProcessingOneSource >,
                                   enum_size_v< DataProcessingOneSource > >
        DataProcessingOneSourceTable { { {
            { 0b1111'1111'1111'1111'1111'1100'0000'0000, 0b0101'1010'1100'0000'0000'0000'0000'0000,
              DataProcessingOneSource::RBIT_32BIT },
            { 0b1111'1111'1111'1111'1111'1100'0000'0000, 0b0101'1010'1100'0000'0000'0100'0000'0000,
              DataProcessingOneSource::REV16_32BIT },
            { 0b1111'1111'1111'1111'1111'1100'0000'0000, 0b0101'1010'1100'0000'0000'1000'0000'0000,
              DataProcessingOneSource::REV_32BIT },
            { 0b1111'1111'1111'1111'1111'1100'0000'0000, 0b0101'1010'1100'0000'0001'0000'0000'0000,
              DataProcessingOneSource::CLZ_32BIT },
            { 0b1111'1111'1111'1111'1111'1100'0000'0000, 0b0101'1010'1100'0000'0001'0100'0000'0000,
              DataProcessingOneSource::CLS_32BIT },
            { 0b1111'1111'1111'1111'1111'1100'0000'0000, 0b1101'1010'1100'0000'0000'0000'0000'0000,
              DataProcessingOneSource::RBIT_64BIT },
            { 0b1111'1111'1111'1111'1111'1100'0000'0000, 0b1101'1010'1100'0000'0000'0100'0000'0000,
              DataProcessingOneSource::REV16_64BIT },
            { 0b1111'1111'1111'1111'1111'1100'0000'0000, 0b1101'1010'1100'0000'0000'1000'0000'0000,
              DataProcessingOneSource::REV32 },
            { 0b1111'1111'1111'1111'1111'1100'0000'0000, 0b1101'1010'1100'0000'0000'1100'0000'0000,
              DataProcessingOneSource::REV_64BIT },
            { 0b1111'1111'1111'1111'1111'1100'0000'0000, 0b1101'1010'1100'0000'0001'0000'0000'0000,
              DataProcessingOneSource::CLZ_64BIT },
            { 0b1111'1111'1111'1111'1111'1100'0000'0000, 0b1101'1010'1100'0000'0001'0100'0000'0000,
              DataProcessingOneSource::CLS_64BIT },
            { 0b1111'1111'1111'1111'1111'1100'0000'0000, 0b1101'1010'1100'0001'0000'0000'0000'0000,
              DataProcessingOneSource::PACIA },
            { 0b1111'1111'1111'1111'1111'1100'0000'0000, 0b1101'1010'1100'0001'0000'0100'0000'0000,
              DataProcessingOneSource::PACIB },
            { 0b1111'1111'1111'1111'1111'1100'0000'0000, 0b1101'1010'1100'0001'0000'1000'0000'0000,
              DataProcessingOneSource::PACDA },
            { 0b1111'1111'1111'1111'1111'1100'0000'0000, 0b1101'1010'1100'0001'0000'1100'0000'0000,
              DataProcessingOneSource::PACDB },
            { 0b1111'1111'1111'1111'1111'1100'0000'0000, 0b1101'1010'1100'0001'0001'0000'0000'0000,
              DataProcessingOneSource::AUTIA },
            { 0b1111'1111'1111'1111'1111'1100'0000'0000, 0b1101'1010'1100'0001'0001'0100'0000'0000,
              DataProcessingOneSource::AUTIB },
            { 0b1111'1111'1111'1111'1111'1100'0000'0000, 0b1101'1010'1100'0001'0001'1000'0000'0000,
              DataProcessingOneSource::AUTDA },
            { 0b1111'1111'1111'1111'1111'1100'0000'0000, 0b1101'1010'1100'0001'0001'1100'0000'0000,
              DataProcessingOneSource::AUTDB },
            { 0b1111'1111'1111'1111'1111'1111'1110'0000, 0b1101'1010'1100'0001'0010'0011'1110'0000,
              DataProcessingOneSource::PACIZA },
            { 0b1111'1111'1111'1111'1111'1111'1110'0000, 0b1101'1010'1100'0001'0010'0111'1110'0000,
              DataProcessingOneSource::PACIZB },
            { 0b1111'1111'1111'1111'1111'1111'1110'0000, 0b1101'1010'1100'0001'0010'1011'1110'0000,
              DataProcessingOneSource::PACDZA },
            { 0b1111'1111'1111'1111'1111'1111'1110'0000, 0b1101'1010'1100'0001'0010'1111'1110'0000,
              DataProcessingOneSource::PACDZB },
            { 0b1111'1111'1111'1111'1111'1111'1110'0000, 0b1101'1010'1100'0001'0011'0011'1110'0000,
              DataProcessingOneSource::AUTIZA },
            { 0b1111'1111'1111'1111'1111'1111'1110'0000, 0b1101'1010'1100'0001'0011'0111'1110'0000,
              DataProcessingOneSource::AUTIZB },
            { 0b1111'1111'1111'1111'1111'1111'1110'0000, 0b1101'1010'1100'0001'0011'1011'1110'0000,
              DataProcessingOneSource::AUTDZA },
            { 0b1111'1111'1111'1111'1111'1111'1110'0000, 0b1101'1010'1100'0001'0011'1111'1110'0000,
              DataProcessingOneSource::AUTDZB },
            { 0b1111'1111'1111'1111'1111'1111'1110'0000, 0b1101'1010'1100'0001'0100'0011'1110'0000,
              DataProcessingOneSource::XPACI },
            { 0b1111'1111'1111'1111'1111'1111'1110'0000, 0b1101'1010'1100'0001'0100'0111'1110'0000,
              DataProcessingOneSource::XPACD },
        } } };
    inline static constexpr Table< LogicalShiftedRegister, std::underlying_type_t< LogicalShiftedRegister >,
                                   enum_size_v< LogicalShiftedRegister > >
        LogicalShiftedRegisterTable { { {
            { 0b1111'1111'0010'0000'0000'0000'0000'0000, 0b0000'1010'0000'0000'0000'0000'0000'0000,
              LogicalShiftedRegister::AND_32BIT_SHIFTED },
            { 0b1111'1111'0010'0000'0000'0000'0000'0000, 0b0000'1010'0010'0000'0000'0000'0000'0000,
              LogicalShiftedRegister::BIC_32BIT_SHIFTED },
            { 0b1111'1111'0010'0000'0000'0000'0000'0000, 0b0010'1010'0000'0000'0000'0000'0000'0000,
              LogicalShiftedRegister::ORR_32BIT_SHIFTED },
            { 0b1111'1111'0010'0000'0000'0000'0000'0000, 0b0010'1010'0010'0000'0000'0000'0000'0000,
              LogicalShiftedRegister::ORN_32BIT_SHIFTED },
            { 0b1111'1111'0010'0000'0000'0000'0000'0000, 0b0100'1010'0000'0000'0000'0000'0000'0000,
              LogicalShiftedRegister::EOR_32BIT_SHIFTED },
            { 0b1111'1111'0010'0000'0000'0000'0000'0000, 0b0100'1010'0010'0000'0000'0000'0000'0000,
              LogicalShiftedRegister::EON_32BIT_SHIFTED },
            { 0b1111'1111'0010'0000'0000'0000'0000'0000, 0b0110'1010'0000'0000'0000'0000'0000'0000,
              LogicalShiftedRegister::ANDS_32BIT_SHIFTED },
            { 0b1111'1111'0010'0000'0000'0000'0000'0000, 0b0110'1010'0010'0000'0000'0000'0000'0000,
              LogicalShiftedRegister::BICS_32BIT_SHIFTED },
            { 0b1111'1111'0010'0000'0000'0000'0000'0000, 0b1000'1010'0000'0000'0000'0000'0000'0000,
              LogicalShiftedRegister::AND_64BIT_SHIFTED },
            { 0b1111'1111'0010'0000'0000'0000'0000'0000, 0b1000'1010'0010'0000'0000'0000'0000'0000,
              LogicalShiftedRegister::BIC_64BIT_SHIFTED },
            { 0b1111'1111'0010'0000'0000'0000'0000'0000, 0b1010'1010'0000'0000'0000'0000'0000'0000,
              LogicalShiftedRegister::ORR_64BIT_SHIFTED },
            { 0b1111'1111'0010'0000'0000'0000'0000'0000, 0b1010'1010'0010'0000'0000'0000'0000'0000,
              LogicalShiftedRegister::ORN_64BIT_SHIFTED },
            { 0b1111'1111'0010'0000'0000'0000'0000'0000, 0b1100'1010'0000'0000'0000'0000'0000'0000,
              LogicalShiftedRegister::EOR_64BIT_SHIFTED },
            { 0b1111'1111'0010'0000'0000'0000'0000'0000, 0b1100'1010'0010'0000'0000'0000'0000'0000,
              LogicalShiftedRegister::EON_64BIT_SHIFTED },
            { 0b1111'1111'0010'0000'0000'0000'0000'0000, 0b1110'1010'0000'0000'0000'0000'0000'0000,
              LogicalShiftedRegister::ANDS_64BIT_SHIFTED },
            { 0b1111'1111'0010'0000'0000'0000'0000'0000, 0b1110'1010'0010'0000'0000'0000'0000'0000,
              LogicalShiftedRegister::BICS_64BIT_SHIFTED },
        } } };
    inline static constexpr Table< AddSubtractShiftedRegister, std::underlying_type_t< AddSubtractShiftedRegister >,
                                   enum_size_v< AddSubtractShiftedRegister > >
        AddSubtractShiftedRegisterTable { { {
            { 0b1111'1111'0010'0000'0000'0000'0000'0000, 0b0000'1011'0000'0000'0000'0000'0000'0000,
              AddSubtractShiftedRegister::ADD_32BIT_SHIFTED },
            { 0b1111'1111'0010'0000'0000'0000'0000'0000, 0b0010'1011'0000'0000'0000'0000'0000'0000,
              AddSubtractShiftedRegister::ADDS_32BIT_SHIFTED },
            { 0b1111'1111'0010'0000'0000'0000'0000'0000, 0b0100'1011'0000'0000'0000'0000'0000'0000,
              AddSubtractShiftedRegister::SUB_32BIT_SHIFTED },
            { 0b1111'1111'0010'0000'0000'0000'0000'0000, 0b0110'1011'0000'0000'0000'0000'0000'0000,
              AddSubtractShiftedRegister::SUBS_32BIT_SHIFTED },
            { 0b1111'1111'0010'0000'0000'0000'0000'0000, 0b1000'1011'0000'0000'0000'0000'0000'0000,
              AddSubtractShiftedRegister::ADD_64BIT_SHIFTED },
            { 0b1111'1111'0010'0000'0000'0000'0000'0000, 0b1010'1011'0000'0000'0000'0000'0000'0000,
              AddSubtractShiftedRegister::ADDS_64BIT_SHIFTED },
            { 0b1111'1111'0010'0000'0000'0000'0000'0000, 0b1100'1011'0000'0000'0000'0000'0000'0000,
              AddSubtractShiftedRegister::SUB_64BIT_SHIFTED },
            { 0b1111'1111'0010'0000'0000'0000'0000'0000, 0b1110'1011'0000'0000'0000'0000'0000'0000,
              AddSubtractShiftedRegister::SUBS_64BIT_SHIFTED },
        } } };
    inline static constexpr Table< AddSubtractExtendedRegister, std::underlying_type_t< AddSubtractExtendedRegister >,
                                   enum_size_v< AddSubtractExtendedRegister > >
        AddSubtractExtendedRegisterTable { { {
            { 0b1111'1111'1110'0000'0000'0000'0000'0000, 0b0000'1011'0010'0000'0000'0000'0000'0000,
              AddSubtractExtendedRegister::ADD_32BIT_EXTENDED },
            { 0b1111'1111'1110'0000'0000'0000'0000'0000, 0b0010'1011'0010'0000'0000'0000'0000'0000,
              AddSubtractExtendedRegister::ADDS_32BIT_EXTENDED },
            { 0b1111'1111'1110'0000'0000'0000'0000'0000, 0b0100'1011'0010'0000'0000'0000'0000'0000,
              AddSubtractExtendedRegister::SUB_32BIT_EXTENDED },
            { 0b1111'1111'1110'0000'0000'0000'0000'0000, 0b0110'1011'0010'0000'0000'0000'0000'0000,
              AddSubtractExtendedRegister::SUBS_32BIT_EXTENDED },
            { 0b1111'1111'1110'0000'0000'0000'0000'0000, 0b1000'1011'0010'0000'0000'0000'0000'0000,
              AddSubtractExtendedRegister::ADD_64BIT_EXTENDED },
            { 0b1111'1111'1110'0000'0000'0000'0000'0000, 0b1010'1011'0010'0000'0000'0000'0000'0000,
              AddSubtractExtendedRegister::ADDS_64BIT_EXTENDED },
            { 0b1111'1111'1110'0000'0000'0000'0000'0000, 0b1100'1011'0010'0000'0000'0000'0000'0000,
              AddSubtractExtendedRegister::SUB_64BIT_EXTENDED },
            { 0b1111'1111'1110'0000'0000'0000'0000'0000, 0b1110'1011'0010'0000'0000'0000'0000'0000,
              AddSubtractExtendedRegister::SUBS_64BIT_EXTENDED },
        } } };
    inline static constexpr Table< AddSubtractCarry, std::underlying_type_t< AddSubtractCarry >,
                                   enum_size_v< AddSubtractCarry > >
        AddSubtractCarryTable { { {
            { 0b1111'1111'1110'0000'0000'0000'0000'0000, 0b0001'1010'0000'0000'0000'0000'0000'0000,
              AddSubtractCarry::ADC_32BIT },
            { 0b1111'1111'1110'0000'0000'0000'0000'0000, 0b0011'1010'0000'0000'0000'0000'0000'0000,
              AddSubtractCarry::ADCS_32BIT },
            { 0b1111'1111'1110'0000'0000'0000'0000'0000, 0b0101'1010'0000'0000'0000'0000'0000'0000,
              AddSubtractCarry::SBC_32BIT },
            { 0b1111'1111'1110'0000'0000'0000'0000'0000, 0b0111'1010'0000'0000'0000'0000'0000'0000,
              AddSubtractCarry::SBCS_32BIT },
            { 0b1111'1111'1110'0000'0000'0000'0000'0000, 0b1001'1010'0000'0000'0000'0000'0000'0000,
              AddSubtractCarry::ADC_64BIT },
            { 0b1111'1111'1110'0000'0000'0000'0000'0000, 0b1011'1010'0000'0000'0000'0000'0000'0000,
              AddSubtractCarry::ADCS_64BIT },
            { 0b1111'1111'1110'0000'0000'0000'0000'0000, 0b1101'1010'0000'0000'0000'0000'0000'0000,
              AddSubtractCarry::SBC_64BIT },
            { 0b1111'1111'1110'0000'0000'0000'0000'0000, 0b1111'1010'0000'0000'0000'0000'0000'0000,
              AddSubtractCarry::SBCS_64BIT },
        } } };
    inline static constexpr Table< RotateRightIntoFlags, std::underlying_type_t< RotateRightIntoFlags >,
                                   enum_size_v< RotateRightIntoFlags > >
        RotateRightIntoFlagsTable { { {
            { 0b1111'1111'1110'0000'0111'1100'0001'0000, 0b1011'1010'0000'0000'0000'0100'0000'0000,
              RotateRightIntoFlags::RMIF },
        } } };
    inline static constexpr Table< EvaluateIntoFlags, std::underlying_type_t< EvaluateIntoFlags >,
                                   enum_size_v< EvaluateIntoFlags > >
        EvaluateIntoFlagsTable { { {
            { 0b1111'1111'1111'1111'1111'1100'0001'1111, 0b0011'1010'0000'0000'0000'1000'0000'1101,
              EvaluateIntoFlags::SETF8 },
            { 0b1111'1111'1111'1111'1111'1100'0001'1111, 0b0011'1010'0000'0000'0100'1000'0000'1101,
              EvaluateIntoFlags::SETF16 },
        } } };
    inline static constexpr Table< ConditionalCompareRegister, std::underlying_type_t< ConditionalCompareRegister >,
                                   enum_size_v< ConditionalCompareRegister > >
        ConditionalCompareRegisterTable { { {
            { 0b1111'1111'1110'0000'0000'1100'0001'0000, 0b0011'1010'0100'0000'0000'0000'0000'0000,
              ConditionalCompareRegister::CCMNr_32BIT },
            { 0b1111'1111'1110'0000'0000'1100'0001'0000, 0b0111'1010'0100'0000'0000'0000'0000'0000,
              ConditionalCompareRegister::CCMPr_32BIT },
            { 0b1111'1111'1110'0000'0000'1100'0001'0000, 0b1011'1010'0100'0000'0000'0000'0000'0000,
              ConditionalCompareRegister::CCMNr_64BIT },
            { 0b1111'1111'1110'0000'0000'1100'0001'0000, 0b1111'1010'0100'0000'0000'0000'0000'0000,
              ConditionalCompareRegister::CCMPr_64BIT },
        } } };
    inline static constexpr Table< ConditionalCompareImmediate, std::underlying_type_t< ConditionalCompareImmediate >,
                                   enum_size_v< ConditionalCompareImmediate > >
        ConditionalCompareImmediateTable { { {
            { 0b1111'1111'1110'0000'0000'1100'0001'0000, 0b0011'1010'0100'0000'0000'1000'0000'0000,
              ConditionalCompareImmediate::CCMNi_32BIT },
            { 0b1111'1111'1110'0000'0000'1100'0001'0000, 0b0111'1010'0100'0000'0000'1000'0000'0000,
              ConditionalCompareImmediate::CCMPi_32BIT },
            { 0b1111'1111'1110'0000'0000'1100'0001'0000, 0b1011'1010'0100'0000'0000'1000'0000'0000,
              ConditionalCompareImmediate::CCMNi_64BIT },
            { 0b1111'1111'1110'0000'0000'1100'0001'0000, 0b1111'1010'0100'0000'0000'1000'0000'0000,
              ConditionalCompareImmediate::CCMPi_64BIT },
        } } };
    inline static constexpr Table< ConditionalSelect, std::underlying_type_t< ConditionalSelect >,
                                   enum_size_v< ConditionalSelect > >
        ConditionalSelectTable { { {
            { 0b1111'1111'1110'0000'0000'1100'0000'0000, 0b0001'1010'1000'0000'0000'0000'0000'0000,
              ConditionalSelect::CSEL_32BIT },
            { 0b1111'1111'1110'0000'0000'1100'0000'0000, 0b0001'1010'1000'0000'0000'0100'0000'0000,
              ConditionalSelect::CSINC_32BIT },
            { 0b1111'1111'1110'0000'0000'1100'0000'0000, 0b0101'1010'1000'0000'0000'0000'0000'0000,
              ConditionalSelect::CSINV_32BIT },
            { 0b1111'1111'1110'0000'0000'1100'0000'0000, 0b0101'1010'1000'0000'0000'0100'0000'0000,
              ConditionalSelect::CSNEG_32BIT },
            { 0b1111'1111'1110'0000'0000'1100'0000'0000, 0b1001'1010'1000'0000'0000'0000'0000'0000,
              ConditionalSelect::CSEL_64BIT },
            { 0b1111'1111'1110'0000'0000'1100'0000'0000, 0b1001'1010'1000'0000'0000'0100'0000'0000,
              ConditionalSelect::CSINC_64BIT },
            { 0b1111'1111'1110'0000'0000'1100'0000'0000, 0b1101'1010'1000'0000'0000'0000'0000'0000,
              ConditionalSelect::CSINV_64BIT },
            { 0b1111'1111'1110'0000'0000'1100'0000'0000, 0b1101'1010'1000'0000'0000'0100'0000'0000,
              ConditionalSelect::CSNEG_64BIT },
        } } };
    inline static constexpr Table< DataProcessingThreeSource, std::underlying_type_t< DataProcessingThreeSource >,
                                   enum_size_v< DataProcessingThreeSource > >
        DataProcessingThreeSourceTable { { {
            { 0b1111'1111'1110'0000'1000'0000'0000'0000, 0b0001'1011'0000'0000'0000'0000'0000'0000,
              DataProcessingThreeSource::MADD_32BIT },
            { 0b1111'1111'1110'0000'1000'0000'0000'0000, 0b0001'1011'0000'0000'1000'0000'0000'0000,
              DataProcessingThreeSource::MSUB_32BIT },
            { 0b1111'1111'1110'0000'1000'0000'0000'0000, 0b1001'1011'0000'0000'0000'0000'0000'0000,
              DataProcessingThreeSource::MADD_64BIT },
            { 0b1111'1111'1110'0000'1000'0000'0000'0000, 0b1001'1011'0000'0000'1000'0000'0000'0000,
              DataProcessingThreeSource::MSUB_64BIT },
            { 0b1111'1111'1110'0000'1000'0000'0000'0000, 0b1001'1011'0010'0000'0000'0000'0000'0000,
              DataProcessingThreeSource::SMADDL },
            { 0b1111'1111'1110'0000'1000'0000'0000'0000, 0b1001'1011'0010'0000'1000'0000'0000'0000,
              DataProcessingThreeSource::SMSUBL },
            { 0b1111'1111'1110'0000'1000'0000'0000'0000, 0b1001'1011'0100'0000'0000'0000'0000'0000,
              DataProcessingThreeSource::SMULH },
            { 0b1111'1111'1110'0000'1000'0000'0000'0000, 0b1001'1011'1010'0000'1000'0000'0000'0000,
              DataProcessingThreeSource::UMADDL },
            { 0b1111'1111'1110'0000'1000'0000'0000'0000, 0b1001'1011'1010'0000'0000'0000'0000'0000,
              DataProcessingThreeSource::UMSUBL },
            { 0b1111'1111'1110'0000'1000'0000'0000'0000, 0b1001'1011'1100'0000'1000'0000'0000'0000,
              DataProcessingThreeSource::UMULH },
        } } };

    template < A64DataProcessingRegisterGroup InstructionClass >
    [[nodiscard]] auto GetInstructionClassTableInternal() const noexcept {
        if constexpr (InstructionClass == A64DataProcessingRegisterGroup::DATA_PROCESSING_TWO_SOURCE) {
            return DataProcessingTwoSourceTable;
        } else if constexpr (InstructionClass == A64DataProcessingRegisterGroup::DATA_PROCESSING_ONE_SOURCE) {
            return DataProcessingOneSourceTable;
        } else if constexpr (InstructionClass == A64DataProcessingRegisterGroup::LOGICAL_SHIFTED_REGISTER) {
            return LogicalShiftedRegisterTable;
        } else if constexpr (InstructionClass == A64DataProcessingRegisterGroup::ADD_SUBTRACT_SHIFTED_REGISTER) {
            return AddSubtractShiftedRegisterTable;
        } else if constexpr (InstructionClass == A64DataProcessingRegisterGroup::ADD_SUBTRACT_EXTENDED_REGISTER) {
            return AddSubtractExtendedRegisterTable;
        } else if constexpr (InstructionClass == A64DataProcessingRegisterGroup::ADD_SUBTRACT_CARRY) {
            return AddSubtractCarryTable;
        } else if constexpr (InstructionClass == A64DataProcessingRegisterGroup::ROTATE_RIGHT_INTO_FLAGS) {
            return RotateRightIntoFlagsTable;
        } else if constexpr (InstructionClass == A64DataProcessingRegisterGroup::EVALUATE_INTO_FLAGS) {
            return EvaluateIntoFlagsTable;
        } else if constexpr (InstructionClass == A64DataProcessingRegisterGroup::CONDITIONAL_COMPARE_REGISTER) {
            return ConditionalCompareRegisterTable;
        } else if constexpr (InstructionClass == A64DataProcessingRegisterGroup::CONDITIONAL_COMPARE_IMMEDIATE) {
            return ConditionalCompareImmediateTable;
        } else if constexpr (InstructionClass == A64DataProcessingRegisterGroup::CONDITIONAL_SELECT) {
            return ConditionalSelectTable;
        } else if constexpr (InstructionClass == A64DataProcessingRegisterGroup::DATA_PROCESSING_THREE_SOURCE) {
            return DataProcessingThreeSourceTable;
        } else {
            throw ARM_EMU_EXCEPTION; // Compile time error detection
        }
    }

    [[nodiscard]] A64DataProcessingRegisterGroup
        GetInstructionClassInternal(const Instruction& instruction) const noexcept final {
        return A64DataProcessingRegisterGroupTable.Lookup(instruction.Get());
    }

  public:
    template < InstructionGroup InstructionClass >
    [[nodiscard]] auto GetInstructionType(const Instruction& instruction) const noexcept {
        const auto& mTable = GetInstructionClassTableInternal< InstructionClass >();
        return mTable.Lookup(instruction.Get());
    }
};

END_NAMESPACE

#endif // !defined(DATAPROCESSINGREGISTER_H_INCLUDED_73F4F724_7B63_4963_91BA_F59C9ABA9B35)
