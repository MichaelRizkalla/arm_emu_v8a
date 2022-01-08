
#include <InstructionSet/A64InstructionSet.h>
#include <Utility/StreamableEnum.h>
#include <ostream>
#include <string_view>

BEGIN_NAMESPACE

namespace {

    using namespace std::literals::string_view_literals;

    static constexpr SmallConstMap< A64BranchExceptionSystemGroup, std::string_view,
                                    enum_size_v< A64BranchExceptionSystemGroup > >
        A64BranchExceptionSystemGroupStringMap { { {
            { A64BranchExceptionSystemGroup::CONDITIONAL_BRANCHING, "CONDITIONAL_BRANCHING"sv },
            { A64BranchExceptionSystemGroup::EXCEPTION_GENERATION, "EXCEPTION_GENERATION"sv },
            { A64BranchExceptionSystemGroup::HINTS, "HINTS"sv },
            { A64BranchExceptionSystemGroup::BARRIERS, "BARRIERS"sv },
            { A64BranchExceptionSystemGroup::PSTATE, "PSTATE"sv },
            { A64BranchExceptionSystemGroup::SYSTEM_INSTRUCTIONS, "SYSTEM_INSTRUCTIONS"sv },
            { A64BranchExceptionSystemGroup::SYSTEM_REGISTER_MOVE, "SYSTEM_REGISTER_MOVE"sv },
            { A64BranchExceptionSystemGroup::UNCONDITIONAL_BRANCH_REGISTER, "UNCONDITIONAL_BRANCH_REGISTER"sv },
            { A64BranchExceptionSystemGroup::UNCONDITIONAL_BRANCH_IMMEDIATE, "UNCONDITIONAL_BRANCH_IMMEDIATE"sv },
            { A64BranchExceptionSystemGroup::COMPARE_AND_BRANCH_IMMEDIATE, "COMPARE_AND_BRANCH_IMMEDIATE"sv },
            { A64BranchExceptionSystemGroup::TEST_AND_BRANCH_IMMEDIATE, "TEST_AND_BRANCH_IMMEDIATE"sv },
        } } };

    static constexpr SmallConstMap< BranchExceptionSystemGroup::ConditionalBranching, std::string_view,
                                    enum_size_v< BranchExceptionSystemGroup::ConditionalBranching > >
        A64ConditionalBranchingStringMap { { {
            { BranchExceptionSystemGroup::ConditionalBranching::BCond, "BCond"sv },
        } } };

    static constexpr SmallConstMap< BranchExceptionSystemGroup::ExceptionGeneration, std::string_view,
                                    enum_size_v< BranchExceptionSystemGroup::ExceptionGeneration > >
        A64ExceptionGenerationStringMap { { {
            { BranchExceptionSystemGroup::ExceptionGeneration::SVC, "SVC"sv },
            { BranchExceptionSystemGroup::ExceptionGeneration::HVC, "HVC"sv },
            { BranchExceptionSystemGroup::ExceptionGeneration::SMC, "SMC"sv },
            { BranchExceptionSystemGroup::ExceptionGeneration::BRK, "BRK"sv },
            { BranchExceptionSystemGroup::ExceptionGeneration::HLT, "HLT"sv },
            { BranchExceptionSystemGroup::ExceptionGeneration::DCPS1, "DCPS1"sv },
            { BranchExceptionSystemGroup::ExceptionGeneration::DCPS2, "DCPS2"sv },
            { BranchExceptionSystemGroup::ExceptionGeneration::DCPS3, "DCPS3"sv },
        } } };

    static constexpr SmallConstMap< BranchExceptionSystemGroup::Hints, std::string_view,
                                    enum_size_v< BranchExceptionSystemGroup::Hints > >
        A64HintsStringMap { { {
            { BranchExceptionSystemGroup::Hints::NOP, "NOP"sv },
            { BranchExceptionSystemGroup::Hints::YIELD, "YIELD"sv },
            { BranchExceptionSystemGroup::Hints::WFE, "WFE"sv },
            { BranchExceptionSystemGroup::Hints::WFI, "WFI"sv },
            { BranchExceptionSystemGroup::Hints::SEV, "SEV"sv },
            { BranchExceptionSystemGroup::Hints::SEVL, "SEVL"sv },
            { BranchExceptionSystemGroup::Hints::DGH, "DGH"sv },
            { BranchExceptionSystemGroup::Hints::XPACD, "XPACD"sv },
            { BranchExceptionSystemGroup::Hints::PACIA1716, "PACIA1716"sv },
            { BranchExceptionSystemGroup::Hints::PACIB1716, "PACIB1716"sv },
            { BranchExceptionSystemGroup::Hints::AUTIA1716, "AUTIA1716"sv },
            { BranchExceptionSystemGroup::Hints::AUTIB1716, "AUTIB1716"sv },
            { BranchExceptionSystemGroup::Hints::ESB, "ESB"sv },
            { BranchExceptionSystemGroup::Hints::PSB_CSYNC, "PSB_CSYNC"sv },
            { BranchExceptionSystemGroup::Hints::TSB_CSYNC, "TSB_CSYNC"sv },
            { BranchExceptionSystemGroup::Hints::CSDB, "CSDB"sv },
            { BranchExceptionSystemGroup::Hints::PACIAZ, "PACIAZ"sv },
            { BranchExceptionSystemGroup::Hints::PACIASP, "PACIASP"sv },
            { BranchExceptionSystemGroup::Hints::PACIBZ, "PACIBZ"sv },
            { BranchExceptionSystemGroup::Hints::PACIBSP, "PACIBSP"sv },
            { BranchExceptionSystemGroup::Hints::AUTIAZ, "AUTIAZ"sv },
            { BranchExceptionSystemGroup::Hints::AUTIASP, "AUTIASP"sv },
            { BranchExceptionSystemGroup::Hints::AUTIBZ, "AUTIBZ"sv },
            { BranchExceptionSystemGroup::Hints::AUTIBSP, "AUTIBSP"sv },
            { BranchExceptionSystemGroup::Hints::BTI, "BTI"sv },
        } } };

    static constexpr SmallConstMap< BranchExceptionSystemGroup::Barriers, std::string_view,
                                    enum_size_v< BranchExceptionSystemGroup::Barriers > >
        A64BarriersStringMap { { {
            { BranchExceptionSystemGroup::Barriers::CLREX, "CLREX"sv },
            { BranchExceptionSystemGroup::Barriers::DMB, "DMB"sv },
            { BranchExceptionSystemGroup::Barriers::ISB, "ISB"sv },
            { BranchExceptionSystemGroup::Barriers::SB, "SB"sv },
            { BranchExceptionSystemGroup::Barriers::DSB, "DSB"sv },
            { BranchExceptionSystemGroup::Barriers::SSBB, "SSBB"sv },
            { BranchExceptionSystemGroup::Barriers::PSSBB, "PSSBB"sv },
        } } };

    static constexpr SmallConstMap< BranchExceptionSystemGroup::PState, std::string_view,
                                    enum_size_v< BranchExceptionSystemGroup::PState > >
        A64PStateStringMap { { {
            { BranchExceptionSystemGroup::PState::CFINV, "CFINV"sv },
            { BranchExceptionSystemGroup::PState::XAFLAG, "XAFLAG"sv },
            { BranchExceptionSystemGroup::PState::AXFLAG, "AXFLAG"sv },
        } } };

    static constexpr SmallConstMap< BranchExceptionSystemGroup::SystemInstruction, std::string_view,
                                    enum_size_v< BranchExceptionSystemGroup::SystemInstruction > >
        A64SystemInstructionStringMap { { {
            { BranchExceptionSystemGroup::SystemInstruction::SYS, "SYS"sv },
            { BranchExceptionSystemGroup::SystemInstruction::SYSL, "SYSL"sv },
        } } };

    static constexpr SmallConstMap< BranchExceptionSystemGroup::SystemRegisterMove, std::string_view,
                                    enum_size_v< BranchExceptionSystemGroup::SystemRegisterMove > >
        A64SystemRegisterMoveStringMap { { {
            { BranchExceptionSystemGroup::SystemRegisterMove::MSRr, "MSRr"sv },
            { BranchExceptionSystemGroup::SystemRegisterMove::MRS, "MRS"sv },
        } } };

    static constexpr SmallConstMap< BranchExceptionSystemGroup::UnconditionalBranchRegister, std::string_view,
                                    enum_size_v< BranchExceptionSystemGroup::UnconditionalBranchRegister > >
        A64UnconditionalBranchRegisterStringMap { { {
            { BranchExceptionSystemGroup::UnconditionalBranchRegister::BR, "BR"sv },
            { BranchExceptionSystemGroup::UnconditionalBranchRegister::BRAAZ, "BRAAZ"sv },
            { BranchExceptionSystemGroup::UnconditionalBranchRegister::BRABZ, "BRABZ"sv },
            { BranchExceptionSystemGroup::UnconditionalBranchRegister::BLR, "BLR"sv },
            { BranchExceptionSystemGroup::UnconditionalBranchRegister::BLRAAZ, "BLRAAZ"sv },
            { BranchExceptionSystemGroup::UnconditionalBranchRegister::BLRABZ, "BLRABZ"sv },
            { BranchExceptionSystemGroup::UnconditionalBranchRegister::RET, "RET"sv },
            { BranchExceptionSystemGroup::UnconditionalBranchRegister::RETAA, "RETAA"sv },
            { BranchExceptionSystemGroup::UnconditionalBranchRegister::RETAB, "RETAB"sv },
            { BranchExceptionSystemGroup::UnconditionalBranchRegister::ERET, "ERET"sv },
            { BranchExceptionSystemGroup::UnconditionalBranchRegister::ERETAA, "ERETAA"sv },
            { BranchExceptionSystemGroup::UnconditionalBranchRegister::ERETAB, "ERETAB"sv },
            { BranchExceptionSystemGroup::UnconditionalBranchRegister::DRPS, "DRPS"sv },
            { BranchExceptionSystemGroup::UnconditionalBranchRegister::BRAA, "BRAA"sv },
            { BranchExceptionSystemGroup::UnconditionalBranchRegister::BRAB, "BRAB"sv },
            { BranchExceptionSystemGroup::UnconditionalBranchRegister::BLRAA, "BLRAA"sv },
            { BranchExceptionSystemGroup::UnconditionalBranchRegister::BLRAB, "BLRAB"sv },
        } } };

    static constexpr SmallConstMap< BranchExceptionSystemGroup::UnconditionalBranchImmediate, std::string_view,
                                    enum_size_v< BranchExceptionSystemGroup::UnconditionalBranchImmediate > >
        A64UnconditionalBranchImmediateStringMap { { {
            { BranchExceptionSystemGroup::UnconditionalBranchImmediate::B, "B"sv },
            { BranchExceptionSystemGroup::UnconditionalBranchImmediate::BL, "BL"sv },
        } } };

    static constexpr SmallConstMap< BranchExceptionSystemGroup::CompareAndBranchImmediate, std::string_view,
                                    enum_size_v< BranchExceptionSystemGroup::CompareAndBranchImmediate > >
        A64CompareAndBranchImmediateStringMap { { {
            { BranchExceptionSystemGroup::CompareAndBranchImmediate::CBZ_32BIT, "CBZ_32BIT"sv },
            { BranchExceptionSystemGroup::CompareAndBranchImmediate::CBNZ_32BIT, "CBNZ_32BIT"sv },
            { BranchExceptionSystemGroup::CompareAndBranchImmediate::CBZ_64BIT, "CBZ_64BIT"sv },
            { BranchExceptionSystemGroup::CompareAndBranchImmediate::CBNZ_64BIT, "CBNZ_64BIT"sv },
        } } };

    static constexpr SmallConstMap< BranchExceptionSystemGroup::TestAndBranchImmediate, std::string_view,
                                    enum_size_v< BranchExceptionSystemGroup::TestAndBranchImmediate > >
        A64TestAndBranchImmediateStringMap { { {
            { BranchExceptionSystemGroup::TestAndBranchImmediate::TBZ, "TBZ"sv },
            { BranchExceptionSystemGroup::TestAndBranchImmediate::TBNZ, "TBNZ"sv },
        } } };

} // namespace

template <>
ARMEMU_API std::string_view Enum::ToChar(A64BranchExceptionSystemGroup enumValue) {
    return A64BranchExceptionSystemGroupStringMap.At(enumValue);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, A64BranchExceptionSystemGroup enumValue) {
    os << Enum::ToChar(enumValue);
    return os;
}

template <>
ARMEMU_API std::string_view Enum::ToChar(BranchExceptionSystemGroup::ConditionalBranching enumValue) {
    return A64ConditionalBranchingStringMap.At(enumValue);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, BranchExceptionSystemGroup::ConditionalBranching enumValue) {
    os << Enum::ToChar(enumValue);
    return os;
}

template <>
ARMEMU_API std::string_view Enum::ToChar(BranchExceptionSystemGroup::ExceptionGeneration enumValue) {
    return A64ExceptionGenerationStringMap.At(enumValue);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, BranchExceptionSystemGroup::ExceptionGeneration enumValue) {
    os << Enum::ToChar(enumValue);
    return os;
}

template <>
ARMEMU_API std::string_view Enum::ToChar(BranchExceptionSystemGroup::Hints enumValue) {
    return A64HintsStringMap.At(enumValue);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, BranchExceptionSystemGroup::Hints enumValue) {
    os << Enum::ToChar(enumValue);
    return os;
}

template <>
ARMEMU_API std::string_view Enum::ToChar(BranchExceptionSystemGroup::Barriers enumValue) {
    return A64BarriersStringMap.At(enumValue);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, BranchExceptionSystemGroup::Barriers enumValue) {
    os << Enum::ToChar(enumValue);
    return os;
}

template <>
ARMEMU_API std::string_view Enum::ToChar(BranchExceptionSystemGroup::PState enumValue) {
    return A64PStateStringMap.At(enumValue);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, BranchExceptionSystemGroup::PState enumValue) {
    os << Enum::ToChar(enumValue);
    return os;
}

template <>
ARMEMU_API std::string_view Enum::ToChar(BranchExceptionSystemGroup::SystemInstruction enumValue) {
    return A64SystemInstructionStringMap.At(enumValue);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, BranchExceptionSystemGroup::SystemInstruction enumValue) {
    os << Enum::ToChar(enumValue);
    return os;
}

template <>
ARMEMU_API std::string_view Enum::ToChar(BranchExceptionSystemGroup::SystemRegisterMove enumValue) {
    return A64SystemRegisterMoveStringMap.At(enumValue);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, BranchExceptionSystemGroup::SystemRegisterMove enumValue) {
    os << Enum::ToChar(enumValue);
    return os;
}

template <>
ARMEMU_API std::string_view Enum::ToChar(BranchExceptionSystemGroup::UnconditionalBranchRegister enumValue) {
    return A64UnconditionalBranchRegisterStringMap.At(enumValue);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, BranchExceptionSystemGroup::UnconditionalBranchRegister enumValue) {
    os << Enum::ToChar(enumValue);
    return os;
}

template <>
ARMEMU_API std::string_view Enum::ToChar(BranchExceptionSystemGroup::UnconditionalBranchImmediate enumValue) {
    return A64UnconditionalBranchImmediateStringMap.At(enumValue);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, BranchExceptionSystemGroup::UnconditionalBranchImmediate enumValue) {
    os << Enum::ToChar(enumValue);
    return os;
}

template <>
ARMEMU_API std::string_view Enum::ToChar(BranchExceptionSystemGroup::CompareAndBranchImmediate enumValue) {
    return A64CompareAndBranchImmediateStringMap.At(enumValue);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, BranchExceptionSystemGroup::CompareAndBranchImmediate enumValue) {
    os << Enum::ToChar(enumValue);
    return os;
}

template <>
ARMEMU_API std::string_view Enum::ToChar(BranchExceptionSystemGroup::TestAndBranchImmediate enumValue) {
    return A64TestAndBranchImmediateStringMap.At(enumValue);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, BranchExceptionSystemGroup::TestAndBranchImmediate enumValue) {
    os << Enum::ToChar(enumValue);
    return os;
}

END_NAMESPACE
