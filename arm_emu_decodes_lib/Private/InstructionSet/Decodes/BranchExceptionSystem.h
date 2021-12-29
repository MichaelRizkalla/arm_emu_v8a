#if !defined(BRANCHEXCEPTIONSYSTEM_H_INCLUDED_2B58F91A_257C_4F26_AFE4_606A1F5C2B86)
    #define BRANCHEXCEPTIONSYSTEM_H_INCLUDED_2B58F91A_257C_4F26_AFE4_606A1F5C2B86

// Based on C4.1.3 A64 instruction set encoding

    #include <API/Api.h>
    #include <InstructionSet/Decodes/DecodeGroup.h>
    #include <Utility/Singleton.h>
    #include <Utility/Utilities.h>
    #include <concepts>
    #include <cstdint>
    #include <numeric>

BEGIN_NAMESPACE

enum class A64BranchExceptionSystemGroup : std::uint32_t
{
    CONDITIONAL_BRANCHING,
    EXCEPTION_GENERATION,
    HINTS,
    BARRIERS,
    PSTATE,
    SYSTEM_INSTRUCTIONS,
    SYSTEM_REGISTER_MOVE,
    UNCONDITIONAL_BRANCH_REGISTER,
    UNCONDITIONAL_BRANCH_IMMEDIATE,
    COMPARE_AND_BRANCH_IMMEDIATE,
    TEST_AND_BRANCH_IMMEDIATE,

    ARM_EMU_COUNT,

    Undefined = std::numeric_limits< std::underlying_type_t< A64BranchExceptionSystemGroup > >::max(),
};

static constexpr Table< A64BranchExceptionSystemGroup, std::underlying_type_t< A64BranchExceptionSystemGroup >,
                        enum_size_v< A64BranchExceptionSystemGroup > >
    A64BranchExceptionSystemGroupTable { { {
        { 0b1111'1110'0000'0000'0000'0000'0000'0000, 0b0101'0100'0000'0000'0000'0000'0000'0000,
          A64BranchExceptionSystemGroup::CONDITIONAL_BRANCHING },
        { 0b1111'1111'0000'0000'0000'0000'0000'0000, 0b1101'0100'0000'0000'0000'0000'0000'0000,
          A64BranchExceptionSystemGroup::EXCEPTION_GENERATION },
        { 0b1111'1111'1111'1111'1111'0000'0001'1111, 0b1101'0101'0000'0011'0010'0000'0001'1111,
          A64BranchExceptionSystemGroup::HINTS },
        { 0b1111'1111'1111'1111'1111'0000'0000'0000, 0b1101'0101'0000'0011'0011'0000'0000'0000,
          A64BranchExceptionSystemGroup::BARRIERS },
        { 0b1111'1111'1111'1000'1111'0000'0000'0000, 0b1101'0101'0000'0000'0100'0000'0000'0000,
          A64BranchExceptionSystemGroup::PSTATE },
        { 0b1111'1111'1101'1000'0000'0000'0000'0000, 0b1101'0101'0000'1000'0000'0000'0000'0000,
          A64BranchExceptionSystemGroup::SYSTEM_INSTRUCTIONS },
        { 0b1111'1111'1101'0000'0000'0000'0000'0000, 0b1101'0101'0001'0000'0000'0000'0000'0000,
          A64BranchExceptionSystemGroup::SYSTEM_REGISTER_MOVE },
        { 0b1111'1110'0000'0000'0000'0000'0000'0000, 0b1101'0110'0000'0000'0000'0000'0000'0000,
          A64BranchExceptionSystemGroup::UNCONDITIONAL_BRANCH_REGISTER },
        { 0b0111'1100'0000'0000'0000'0000'0000'0000, 0b0001'0100'0000'0000'0000'0000'0000'0000,
          A64BranchExceptionSystemGroup::UNCONDITIONAL_BRANCH_IMMEDIATE },
        { 0b0111'1110'0000'0000'0000'0000'0000'0000, 0b0011'0100'0000'0000'0000'0000'0000'0000,
          A64BranchExceptionSystemGroup::COMPARE_AND_BRANCH_IMMEDIATE },
        { 0b0111'1110'0000'0000'0000'0000'0000'0000, 0b0011'0110'0000'0000'0000'0000'0000'0000,
          A64BranchExceptionSystemGroup::TEST_AND_BRANCH_IMMEDIATE },
    } } };

class ARMEMU_API BranchExceptionSystemGroup final : public DecodeGroup< A64BranchExceptionSystemGroup > {
  protected:
    [[nodiscard]] BranchExceptionSystemGroup() noexcept = default;
    DELETE_COPY_CLASS(BranchExceptionSystemGroup)
    DELETE_MOVE_CLASS(BranchExceptionSystemGroup)

  public:
    DECLARE_AS_SINGLETON(BranchExceptionSystemGroup)

    enum class ConditionalBranching : std::uint32_t
    {
        BCond,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< ConditionalBranching > >::max(),
    };
    enum class ExceptionGeneration : std::uint32_t
    {
        SVC,
        HVC,
        SMC,
        BRK,
        HLT,
        DCPS1,
        DCPS2,
        DCPS3,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< ExceptionGeneration > >::max(),
    };
    enum class Hints : std::uint32_t
    {
        NOP,
        YIELD,
        WFE,
        WFI,
        SEV,
        SEVL,
        DGH,
        XPACD,
        PACIA1716,
        PACIB1716,
        AUTIA1716,
        AUTIB1716,
        ESB,
        PSB_CSYNC,
        TSB_CSYNC,
        CSDB,
        PACIAZ,
        PACIASP,
        PACIBZ,
        PACIBSP,
        AUTIAZ,
        AUTIASP,
        AUTIBZ,
        AUTIBSP,
        BTI,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< Hints > >::max(),
    };
    enum class Barriers : std::uint32_t
    {
        CLREX,
        DMB,
        ISB,
        SB,
        DSB,
        SSBB,
        PSSBB,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< Barriers > >::max(),
    };
    enum class PState : std::uint32_t
    {
        CFINV, // Variations of MSRi
        XAFLAG,
        AXFLAG,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< PState > >::max(),
    };
    enum class SystemInstruction : std::uint32_t
    {
        SYS,
        SYSL,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< SystemInstruction > >::max(),
    };
    enum class SystemRegisterMove : std::uint32_t
    {
        MSRr,
        MRS,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< SystemRegisterMove > >::max(),
    };
    enum class UnconditionalBranchRegister : std::uint32_t
    {
        BR,
        BRAAZ,
        BRABZ,
        BLR,
        BLRAAZ,
        BLRABZ,
        RET,
        RETAA,
        RETAB,
        ERET,
        ERETAA,
        ERETAB,
        DRPS,
        BRAA,
        BRAB,
        BLRAA,
        BLRAB,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< UnconditionalBranchRegister > >::max(),
    };
    enum class UnconditionalBranchImmediate : std::uint32_t
    {
        B,
        BL,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< UnconditionalBranchImmediate > >::max(),
    };
    enum class CompareAndBranchImmediate : std::uint32_t
    {
        CBZ_32BIT,
        CBNZ_32BIT,
        CBZ_64BIT,
        CBNZ_64BIT,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< CompareAndBranchImmediate > >::max(),
    };
    enum class TestAndBranchImmediate : std::uint32_t
    {
        TBZ,
        TBNZ,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< TestAndBranchImmediate > >::max(),
    };

  private:
    inline static constexpr Table< ConditionalBranching, std::underlying_type_t< ConditionalBranching >,
                                   enum_size_v< ConditionalBranching > >
        ConditionalBranchingTable { { {
            { 0b1111'1111'0000'0000'0000'0000'0001'0000, 0b0101'0100'0000'0000'0000'0000'0000'0000,
              ConditionalBranching::BCond },
        } } };
    inline static constexpr Table< ExceptionGeneration, std::underlying_type_t< ExceptionGeneration >,
                                   enum_size_v< ExceptionGeneration > >
        ExceptionGenerationTable { { {
            { 0b1111'1111'1110'0000'0000'0000'0001'1111, 0b1101'0100'0000'0000'0000'0000'0000'0001,
              ExceptionGeneration::SVC },
            { 0b1111'1111'1110'0000'0000'0000'0001'1111, 0b1101'0100'0000'0000'0000'0000'0000'0010,
              ExceptionGeneration::HVC },
            { 0b1111'1111'1110'0000'0000'0000'0001'1111, 0b1101'0100'0000'0000'0000'0000'0000'0011,
              ExceptionGeneration::SMC },
            { 0b1111'1111'1110'0000'0000'0000'0001'1111, 0b1101'0100'0010'0000'0000'0000'0000'0000,
              ExceptionGeneration::BRK },
            { 0b1111'1111'1110'0000'0000'0000'0001'1111, 0b1101'0100'0100'0000'0000'0000'0000'0000,
              ExceptionGeneration::HLT },
            { 0b1111'1111'1110'0000'0000'0000'0001'1111, 0b1101'0100'1010'0000'0000'0000'0000'0001,
              ExceptionGeneration::DCPS1 },
            { 0b1111'1111'1110'0000'0000'0000'0001'1111, 0b1101'0100'1010'0000'0000'0000'0000'0010,
              ExceptionGeneration::DCPS2 },
            { 0b1111'1111'1110'0000'0000'0000'0001'1111, 0b1101'0100'1010'0000'0000'0000'0000'0011,
              ExceptionGeneration::DCPS3 },
        } } };
    inline static constexpr Table< Hints, std::underlying_type_t< Hints >, enum_size_v< Hints > > HintsTable { { {
        { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0010'0000'0001'1111, Hints::NOP },
        { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0010'0000'0011'1111, Hints::YIELD },
        { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0010'0000'0101'1111, Hints::WFE },
        { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0010'0000'0111'1111, Hints::WFI },
        { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0010'0000'1001'1111, Hints::SEV },
        { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0010'0000'1011'1111, Hints::SEVL },
        { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0010'0000'1101'1111, Hints::DGH },
        { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0010'0000'1111'1111, Hints::XPACD },
        { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0010'0001'0001'1111, Hints::PACIA1716 },
        { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0010'0001'0101'1111, Hints::PACIB1716 },
        { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0010'0001'1001'1111, Hints::AUTIA1716 },
        { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0010'0001'1101'1111, Hints::AUTIB1716 },
        { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0010'0010'0001'1111, Hints::ESB },
        { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0010'0010'0011'1111, Hints::PSB_CSYNC },
        { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0010'0010'0101'1111, Hints::TSB_CSYNC },
        { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0010'0010'1001'1111, Hints::CSDB },
        { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0010'0011'0001'1111, Hints::PACIAZ },
        { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0010'0011'0011'1111, Hints::PACIASP },
        { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0010'0011'0101'1111, Hints::PACIBZ },
        { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0010'0011'0111'1111, Hints::PACIBSP },
        { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0010'0011'1001'1111, Hints::AUTIAZ },
        { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0010'0011'1011'1111, Hints::AUTIASP },
        { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0010'0011'1101'1111, Hints::AUTIBZ },
        { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0010'0011'1111'1111, Hints::AUTIBSP },
        { 0b1111'1111'1111'1111'1111'1111'0011'1111, 0b1101'0101'0000'0011'0010'0100'0001'1111, Hints::BTI },
    } } };
    inline static constexpr Table< Barriers, std::underlying_type_t< Barriers >, enum_size_v< Barriers > + 14 >
                                                                                                     BarriersTable { { {
            { 0b1111'1111'1111'1111'1111'0000'1111'1111, 0b1101'0101'0000'0011'0011'0000'0101'1111, Barriers::CLREX },
            { 0b1111'1111'1111'1111'1111'0000'1111'1111, 0b1101'0101'0000'0011'0011'0000'1011'1111, Barriers::DMB },
            { 0b1111'1111'1111'1111'1111'0000'1111'1111, 0b1101'0101'0000'0011'0011'0000'1101'1111, Barriers::ISB },
            { 0b1111'1111'1111'1111'1111'0000'1111'1111, 0b1101'0101'0000'0011'0011'0000'1111'1111, Barriers::SB },
            { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0011'0001'1001'1111, Barriers::DSB },
            { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0011'0010'1001'1111, Barriers::DSB },
            { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0011'0011'1001'1111, Barriers::DSB },
            { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0011'0100'1001'1111, Barriers::DSB },
            { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0011'0101'1001'1111, Barriers::DSB },
            { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0011'0110'1001'1111, Barriers::DSB },
            { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0011'0111'1001'1111, Barriers::DSB },
            { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0011'1000'1001'1111, Barriers::DSB },
            { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0011'1001'1001'1111, Barriers::DSB },
            { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0011'1010'1001'1111, Barriers::DSB },
            { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0011'1011'1001'1111, Barriers::DSB },
            { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0011'1100'1001'1111, Barriers::DSB },
            { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0011'1101'1001'1111, Barriers::DSB },
            { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0011'1110'1001'1111, Barriers::DSB },
            { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0011'1111'1001'1111, Barriers::DSB },
            { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0011'0000'1001'1111, Barriers::SSBB },
            { 0b1111'1111'1111'1111'1111'1111'1111'1111, 0b1101'0101'0000'0011'0011'0100'1001'1111, Barriers::PSSBB },
        } } };
    inline static constexpr Table< PState, std::underlying_type_t< PState >, enum_size_v< PState > > PStateTable { { {
        { 0b1111'1111'1111'1111'1111'0000'1111'1111, 0b1101'0101'0000'0000'0100'0000'0001'1111, PState::CFINV },
        { 0b1111'1111'1111'1111'1111'0000'1111'1111, 0b1101'0101'0000'0000'0100'0000'0011'1111, PState::XAFLAG },
        { 0b1111'1111'1111'1111'1111'0000'1111'1111, 0b1101'0101'0000'0000'0100'0000'0101'1111, PState::AXFLAG },
    } } };
    inline static constexpr Table< SystemInstruction, std::underlying_type_t< SystemInstruction >,
                                   enum_size_v< SystemInstruction > >
        SystemInstructionTable { { {
            { 0b1111'1111'1111'1000'0000'0000'0000'0000, 0b1101'0101'0000'1000'0000'0000'0000'0000,
              SystemInstruction::SYS },
            { 0b1111'1111'1111'1000'0000'0000'0000'0000, 0b1101'0101'0010'1000'0000'0000'0000'0000,
              SystemInstruction::SYSL },
        } } };
    inline static constexpr Table< SystemRegisterMove, std::underlying_type_t< SystemRegisterMove >,
                                   enum_size_v< SystemRegisterMove > >
        SystemRegisterMoveTable { { {
            { 0b1111'1111'1111'0000'0000'0000'0000'0000, 0b1101'0101'0001'0000'0000'0000'0000'0000,
              SystemRegisterMove::MSRr },
            { 0b1111'1111'1111'0000'0000'0000'0000'0000, 0b1101'0101'0011'0000'0000'0000'0000'0000,
              SystemRegisterMove::MRS },
        } } };
    inline static constexpr Table< UnconditionalBranchRegister, std::underlying_type_t< UnconditionalBranchRegister >,
                                   enum_size_v< UnconditionalBranchRegister > >
        UnconditionalBranchRegisterTable { { {
            { 0b1111'1111'1111'1111'1111'1100'0001'1111, 0b1101'0110'0001'1111'0000'0000'0000'0000,
              UnconditionalBranchRegister::BR },
            { 0b1111'1111'1111'1111'1111'1100'0001'1111, 0b1101'0110'0001'1111'0000'1000'0001'1111,
              UnconditionalBranchRegister::BRAAZ },
            { 0b1111'1111'1111'1111'1111'1100'0001'1111, 0b1101'0110'0001'1111'0000'1100'0001'1111,
              UnconditionalBranchRegister::BRABZ },
            { 0b1111'1111'1111'1111'1111'1100'0001'1111, 0b1101'0110'0011'1111'0000'0000'0000'0000,
              UnconditionalBranchRegister::BLR },
            { 0b1111'1111'1111'1111'1111'1100'0001'1111, 0b1101'0110'0011'1111'0000'1000'0001'1111,
              UnconditionalBranchRegister::BLRAAZ },
            { 0b1111'1111'1111'1111'1111'1100'0001'1111, 0b1101'0110'0011'1111'0000'1100'0001'1111,
              UnconditionalBranchRegister::BLRABZ },
            { 0b1111'1111'1111'1111'1111'1100'0001'1111, 0b1101'0110'0101'1111'0000'0000'0000'0000,
              UnconditionalBranchRegister::RET },
            { 0b1111'1111'1111'1111'1111'1100'0001'1111, 0b1101'0110'0101'1111'0000'1011'1111'1111,
              UnconditionalBranchRegister::RETAA },
            { 0b1111'1111'1111'1111'1111'1100'0001'1111, 0b1101'0110'0101'1111'0000'1111'1111'1111,
              UnconditionalBranchRegister::RETAB },
            { 0b1111'1111'1111'1111'1111'1100'0001'1111, 0b1101'0110'1001'1111'0000'0011'1110'0000,
              UnconditionalBranchRegister::ERET },
            { 0b1111'1111'1111'1111'1111'1100'0001'1111, 0b1101'0110'1001'1111'0000'1011'1111'1111,
              UnconditionalBranchRegister::ERETAA },
            { 0b1111'1111'1111'1111'1111'1100'0001'1111, 0b1101'0110'1001'1111'0000'1111'1111'1111,
              UnconditionalBranchRegister::ERETAA },
            { 0b1111'1111'1111'1111'1111'1100'0001'1111, 0b1101'0110'1011'1111'0000'0011'1110'0000,
              UnconditionalBranchRegister::DRPS },
            { 0b1111'1111'1111'1111'1111'1100'0000'0000, 0b1101'0111'0001'1111'0000'1000'0000'0000,
              UnconditionalBranchRegister::BRAA },
            { 0b1111'1111'1111'1111'1111'1100'0000'0000, 0b1101'0111'0001'1111'0000'1100'0000'0000,
              UnconditionalBranchRegister::BRAB },
            { 0b1111'1111'1111'1111'1111'1100'0000'0000, 0b1101'0111'0011'1111'0000'1000'0000'0000,
              UnconditionalBranchRegister::BLRAA },
            { 0b1111'1111'1111'1111'1111'1100'0000'0000, 0b1101'0111'0011'1111'0000'1100'0000'0000,
              UnconditionalBranchRegister::BLRAB },
        } } };
    inline static constexpr Table< UnconditionalBranchImmediate, std::underlying_type_t< UnconditionalBranchImmediate >,
                                   enum_size_v< UnconditionalBranchImmediate > >
        UnconditionalBranchImmediateTable { { {
            { 0b1111'1100'0000'0000'0000'0000'0000'0000, 0b0001'0100'0000'0000'0000'0000'0000'0000,
              UnconditionalBranchImmediate::B },
            { 0b1111'1100'0000'0000'0000'0000'0000'0000, 0b1001'0100'0000'0000'0000'0000'0000'0000,
              UnconditionalBranchImmediate::BL },
        } } };
    inline static constexpr Table< CompareAndBranchImmediate, std::underlying_type_t< CompareAndBranchImmediate >,
                                   enum_size_v< CompareAndBranchImmediate > >
        CompareAndBranchImmediateTable { { {
            { 0b1111'1111'0000'0000'0000'0000'0000'0000, 0b0011'0100'0000'0000'0000'0000'0000'0000,
              CompareAndBranchImmediate::CBZ_32BIT },
            { 0b1111'1111'0000'0000'0000'0000'0000'0000, 0b0011'0101'0000'0000'0000'0000'0000'0000,
              CompareAndBranchImmediate::CBNZ_32BIT },
            { 0b1111'1111'0000'0000'0000'0000'0000'0000, 0b1011'0100'0000'0000'0000'0000'0000'0000,
              CompareAndBranchImmediate::CBZ_64BIT },
            { 0b1111'1111'0000'0000'0000'0000'0000'0000, 0b1011'0101'0000'0000'0000'0000'0000'0000,
              CompareAndBranchImmediate::CBNZ_64BIT },
        } } };
    inline static constexpr Table< TestAndBranchImmediate, std::underlying_type_t< TestAndBranchImmediate >,
                                   enum_size_v< TestAndBranchImmediate > >
        TestAndBranchImmediateTable { { {
            { 0b0111'1111'0000'0000'0000'0000'0000'0000, 0b0011'0110'0000'0000'0000'0000'0000'0000,
              TestAndBranchImmediate::TBZ },
            { 0b0111'1111'0000'0000'0000'0000'0000'0000, 0b0011'0111'0000'0000'0000'0000'0000'0000,
              TestAndBranchImmediate::TBNZ },
        } } };

    template < A64BranchExceptionSystemGroup InstructionClass >
    [[nodiscard]] auto GetInstructionClassTableInternal() const noexcept {
        if constexpr (InstructionClass == A64BranchExceptionSystemGroup::CONDITIONAL_BRANCHING) {
            return ConditionalBranchingTable;
        } else if constexpr (InstructionClass == A64BranchExceptionSystemGroup::EXCEPTION_GENERATION) {
            return ExceptionGenerationTable;
        } else if constexpr (InstructionClass == A64BranchExceptionSystemGroup::HINTS) {
            return HintsTable;
        } else if constexpr (InstructionClass == A64BranchExceptionSystemGroup::BARRIERS) {
            return BarriersTable;
        } else if constexpr (InstructionClass == A64BranchExceptionSystemGroup::PSTATE) {
            return PStateTable;
        } else if constexpr (InstructionClass == A64BranchExceptionSystemGroup::SYSTEM_INSTRUCTIONS) {
            return SystemInstructionTable;
        } else if constexpr (InstructionClass == A64BranchExceptionSystemGroup::SYSTEM_REGISTER_MOVE) {
            return SystemRegisterMoveTable;
        } else if constexpr (InstructionClass == A64BranchExceptionSystemGroup::UNCONDITIONAL_BRANCH_REGISTER) {
            return UnconditionalBranchRegisterTable;
        } else if constexpr (InstructionClass == A64BranchExceptionSystemGroup::UNCONDITIONAL_BRANCH_IMMEDIATE) {
            return UnconditionalBranchImmediateTable;
        } else if constexpr (InstructionClass == A64BranchExceptionSystemGroup::COMPARE_AND_BRANCH_IMMEDIATE) {
            return CompareAndBranchImmediateTable;
        } else if constexpr (InstructionClass == A64BranchExceptionSystemGroup::TEST_AND_BRANCH_IMMEDIATE) {
            return TestAndBranchImmediateTable;
        } else {
            throw ARM_EMU_EXCEPTION; // Compile time error detection
        }
    }

    [[nodiscard]] A64BranchExceptionSystemGroup
        GetInstructionClassInternal(const Instruction& instruction) const noexcept final {
        return A64BranchExceptionSystemGroupTable.Lookup(instruction.Get());
    }

  public:
    template < InstructionGroup InstructionClass >
    [[nodiscard]] auto GetInstructionType(const Instruction& instruction) const noexcept {
        const auto& mTable = GetInstructionClassTableInternal< InstructionClass >();
        return mTable.Lookup(instruction.Get());
    }
};

END_NAMESPACE

#endif // !defined(BRANCHEXCEPTIONSYSTEM_H_INCLUDED_2B58F91A_257C_4F26_AFE4_606A1F5C2B86)