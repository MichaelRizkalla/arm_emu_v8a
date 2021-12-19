
#include <ProcessingUnit/A64InstructionSet/A64InstructionSet.h>
#include <Utility/StreamableEnum.h>
#include <ostream>
#include <string_view>

BEGIN_NAMESPACE

namespace {

    using namespace std::literals::string_view_literals;

    static constexpr SmallConstMap< A64DataProcessingRegisterGroup, std::string_view,
                                    enum_size_v< A64DataProcessingRegisterGroup > >
        A64DataProcessingRegisterGroupStringMap { { {
            { A64DataProcessingRegisterGroup::DATA_PROCESSING_TWO_SOURCE, "DATA_PROCESSING_TWO_SOURCE"sv },
            { A64DataProcessingRegisterGroup::DATA_PROCESSING_ONE_SOURCE, "DATA_PROCESSING_ONE_SOURCE"sv },
            { A64DataProcessingRegisterGroup::LOGICAL_SHIFTED_REGISTER, "LOGICAL_SHIFTED_REGISTER"sv },
            { A64DataProcessingRegisterGroup::ADD_SUBTRACT_SHIFTED_REGISTER, "ADD_SUBTRACT_SHIFTED_REGISTER"sv },
            { A64DataProcessingRegisterGroup::ADD_SUBTRACT_EXTENDED_REGISTER, "ADD_SUBTRACT_EXTENDED_REGISTER"sv },
            { A64DataProcessingRegisterGroup::ADD_SUBTRACT_CARRY, "ADD_SUBTRACT_CARRY"sv },
            { A64DataProcessingRegisterGroup::ROTATE_RIGHT_INTO_FLAGS, "ROTATE_RIGHT_INTO_FLAGS"sv },
            { A64DataProcessingRegisterGroup::EVALUATE_INTO_FLAGS, "EVALUATE_INTO_FLAGS"sv },
            { A64DataProcessingRegisterGroup::CONDITIONAL_COMPARE_REGISTER, "CONDITIONAL_COMPARE_REGISTER"sv },
            { A64DataProcessingRegisterGroup::CONDITIONAL_COMPARE_IMMEDIATE, "CONDITIONAL_COMPARE_IMMEDIATE"sv },
            { A64DataProcessingRegisterGroup::CONDITIONAL_SELECT, "CONDITIONAL_SELECT"sv },
            { A64DataProcessingRegisterGroup::DATA_PROCESSING_THREE_SOURCE, "DATA_PROCESSING_THREE_SOURCE"sv },
        } } };

    static constexpr SmallConstMap< DataProcessingRegisterGroup::DataProcessingTwoSource, std::string_view,
                                    enum_size_v< DataProcessingRegisterGroup::DataProcessingTwoSource > >
        DataProcessingTwoSourceStringMap { { {
            { DataProcessingRegisterGroup::DataProcessingTwoSource::UDIV_32BIT, "UDIV_32BIT"sv },
            { DataProcessingRegisterGroup::DataProcessingTwoSource::SDIV_32BIT, "SDIV_32BIT"sv },
            { DataProcessingRegisterGroup::DataProcessingTwoSource::LSLV_32BIT, "LSLV_32BIT"sv },
            { DataProcessingRegisterGroup::DataProcessingTwoSource::LSRV_32BIT, "LSRV_32BIT"sv },
            { DataProcessingRegisterGroup::DataProcessingTwoSource::ASRV_32BIT, "ASRV_32BIT"sv },
            { DataProcessingRegisterGroup::DataProcessingTwoSource::RORV_32BIT, "RORV_32BIT"sv },
            { DataProcessingRegisterGroup::DataProcessingTwoSource::CRC32B, "CRC32B"sv },
            { DataProcessingRegisterGroup::DataProcessingTwoSource::CRC32H, "CRC32H"sv },
            { DataProcessingRegisterGroup::DataProcessingTwoSource::CRC32W, "CRC32W"sv },
            { DataProcessingRegisterGroup::DataProcessingTwoSource::CRC32CB, "CRC32CB"sv },
            { DataProcessingRegisterGroup::DataProcessingTwoSource::CRC32CH, "CRC32CH"sv },
            { DataProcessingRegisterGroup::DataProcessingTwoSource::CRC32CW, "CRC32CW"sv },
            { DataProcessingRegisterGroup::DataProcessingTwoSource::SUBP, "SUBP"sv },
            { DataProcessingRegisterGroup::DataProcessingTwoSource::UDIV_64BIT, "UDIV_64BIT"sv },
            { DataProcessingRegisterGroup::DataProcessingTwoSource::SDIV_64BIT, "SDIV_64BIT"sv },
            { DataProcessingRegisterGroup::DataProcessingTwoSource::IRG, "IRG"sv },
            { DataProcessingRegisterGroup::DataProcessingTwoSource::GMI, "GMI"sv },
            { DataProcessingRegisterGroup::DataProcessingTwoSource::LSLV_64BIT, "LSLV_64BIT"sv },
            { DataProcessingRegisterGroup::DataProcessingTwoSource::LSRV_64BIT, "LSRV_64BIT"sv },
            { DataProcessingRegisterGroup::DataProcessingTwoSource::ASRV_64BIT, "ASRV_64BIT"sv },
            { DataProcessingRegisterGroup::DataProcessingTwoSource::RORV_64BIT, "RORV_64BIT"sv },
            { DataProcessingRegisterGroup::DataProcessingTwoSource::PACGA, "PACGA"sv },
            { DataProcessingRegisterGroup::DataProcessingTwoSource::CRC32X, "CRC32X"sv },
            { DataProcessingRegisterGroup::DataProcessingTwoSource::CRC32CX, "CRC32CX"sv },
            { DataProcessingRegisterGroup::DataProcessingTwoSource::SUBPS, "SUBPS"sv },
        } } };

    static constexpr SmallConstMap< DataProcessingRegisterGroup::DataProcessingOneSource, std::string_view,
                                    enum_size_v< DataProcessingRegisterGroup::DataProcessingOneSource > >
        DataProcessingOneSourceStringMap { { {
            { DataProcessingRegisterGroup::DataProcessingOneSource::RBIT_32BIT, "RBIT_32BIT"sv },
            { DataProcessingRegisterGroup::DataProcessingOneSource::REV16_32BIT, "REV16_32BIT"sv },
            { DataProcessingRegisterGroup::DataProcessingOneSource::REV_32BIT, "REV_32BIT"sv },
            { DataProcessingRegisterGroup::DataProcessingOneSource::CLZ_32BIT, "CLZ_32BIT"sv },
            { DataProcessingRegisterGroup::DataProcessingOneSource::CLS_32BIT, "CLS_32BIT"sv },
            { DataProcessingRegisterGroup::DataProcessingOneSource::RBIT_64BIT, "RBIT_64BIT"sv },
            { DataProcessingRegisterGroup::DataProcessingOneSource::REV16_64BIT, "REV16_64BIT"sv },
            { DataProcessingRegisterGroup::DataProcessingOneSource::REV32, "REV32"sv },
            { DataProcessingRegisterGroup::DataProcessingOneSource::REV_64BIT, "REV_64BIT"sv },
            { DataProcessingRegisterGroup::DataProcessingOneSource::CLZ_64BIT, "CLZ_64BIT"sv },
            { DataProcessingRegisterGroup::DataProcessingOneSource::CLS_64BIT, "CLS_64BIT"sv },
            { DataProcessingRegisterGroup::DataProcessingOneSource::PACIA, "PACIA"sv },
            { DataProcessingRegisterGroup::DataProcessingOneSource::PACIB, "PACIB"sv },
            { DataProcessingRegisterGroup::DataProcessingOneSource::PACDA, "PACDA"sv },
            { DataProcessingRegisterGroup::DataProcessingOneSource::PACDB, "PACDB"sv },
            { DataProcessingRegisterGroup::DataProcessingOneSource::AUTIA, "AUTIA"sv },
            { DataProcessingRegisterGroup::DataProcessingOneSource::AUTIB, "AUTIB"sv },
            { DataProcessingRegisterGroup::DataProcessingOneSource::AUTDA, "AUTDA"sv },
            { DataProcessingRegisterGroup::DataProcessingOneSource::AUTDB, "AUTDB"sv },
            { DataProcessingRegisterGroup::DataProcessingOneSource::PACIZA, "PACIZA"sv },
            { DataProcessingRegisterGroup::DataProcessingOneSource::PACIZB, "PACIZB"sv },
            { DataProcessingRegisterGroup::DataProcessingOneSource::PACDZA, "PACDZA"sv },
            { DataProcessingRegisterGroup::DataProcessingOneSource::PACDZB, "PACDZB"sv },
            { DataProcessingRegisterGroup::DataProcessingOneSource::AUTIZA, "AUTIZA"sv },
            { DataProcessingRegisterGroup::DataProcessingOneSource::AUTIZB, "AUTIZB"sv },
            { DataProcessingRegisterGroup::DataProcessingOneSource::AUTDZA, "AUTDZA"sv },
            { DataProcessingRegisterGroup::DataProcessingOneSource::AUTDZB, "AUTDZB"sv },
            { DataProcessingRegisterGroup::DataProcessingOneSource::XPACI, "XPACI"sv },
            { DataProcessingRegisterGroup::DataProcessingOneSource::XPACD, "XPACD"sv },
        } } };

    static constexpr SmallConstMap< DataProcessingRegisterGroup::LogicalShiftedRegister, std::string_view,
                                    enum_size_v< DataProcessingRegisterGroup::LogicalShiftedRegister > >
        LogicalShiftedRegisterStringMap { { {
            { DataProcessingRegisterGroup::LogicalShiftedRegister::AND_32BIT_SHIFTED, "AND_32BIT_SHIFTED"sv },
            { DataProcessingRegisterGroup::LogicalShiftedRegister::BIC_32BIT_SHIFTED, "BIC_32BIT_SHIFTED"sv },
            { DataProcessingRegisterGroup::LogicalShiftedRegister::ORR_32BIT_SHIFTED, "ORR_32BIT_SHIFTED"sv },
            { DataProcessingRegisterGroup::LogicalShiftedRegister::ORN_32BIT_SHIFTED, "ORN_32BIT_SHIFTED"sv },
            { DataProcessingRegisterGroup::LogicalShiftedRegister::EOR_32BIT_SHIFTED, "EOR_32BIT_SHIFTED"sv },
            { DataProcessingRegisterGroup::LogicalShiftedRegister::EON_32BIT_SHIFTED, "EON_32BIT_SHIFTED"sv },
            { DataProcessingRegisterGroup::LogicalShiftedRegister::ANDS_32BIT_SHIFTED, "ANDS_32BIT_SHIFTED"sv },
            { DataProcessingRegisterGroup::LogicalShiftedRegister::BICS_32BIT_SHIFTED, "BICS_32BIT_SHIFTED"sv },
            { DataProcessingRegisterGroup::LogicalShiftedRegister::AND_64BIT_SHIFTED, "AND_64BIT_SHIFTED"sv },
            { DataProcessingRegisterGroup::LogicalShiftedRegister::BIC_64BIT_SHIFTED, "BIC_64BIT_SHIFTED"sv },
            { DataProcessingRegisterGroup::LogicalShiftedRegister::ORR_64BIT_SHIFTED, "ORR_64BIT_SHIFTED"sv },
            { DataProcessingRegisterGroup::LogicalShiftedRegister::ORN_64BIT_SHIFTED, "ORN_64BIT_SHIFTED"sv },
            { DataProcessingRegisterGroup::LogicalShiftedRegister::EOR_64BIT_SHIFTED, "EOR_64BIT_SHIFTED"sv },
            { DataProcessingRegisterGroup::LogicalShiftedRegister::EON_64BIT_SHIFTED, "EON_64BIT_SHIFTED"sv },
            { DataProcessingRegisterGroup::LogicalShiftedRegister::ANDS_64BIT_SHIFTED, "ANDS_64BIT_SHIFTED"sv },
            { DataProcessingRegisterGroup::LogicalShiftedRegister::BICS_64BIT_SHIFTED, "BICS_64BIT_SHIFTED"sv },
        } } };

    static constexpr SmallConstMap< DataProcessingRegisterGroup::AddSubtractShiftedRegister, std::string_view,
                                    enum_size_v< DataProcessingRegisterGroup::AddSubtractShiftedRegister > >
        AddSubtractShiftedRegisterStringMap { { {
            { DataProcessingRegisterGroup::AddSubtractShiftedRegister::ADD_32BIT_SHIFTED, "ADD_32BIT_SHIFTED"sv },
            { DataProcessingRegisterGroup::AddSubtractShiftedRegister::ADDS_32BIT_SHIFTED, "ADDS_32BIT_SHIFTED"sv },
            { DataProcessingRegisterGroup::AddSubtractShiftedRegister::SUB_32BIT_SHIFTED, "SUB_32BIT_SHIFTED"sv },
            { DataProcessingRegisterGroup::AddSubtractShiftedRegister::SUBS_32BIT_SHIFTED, "SUBS_32BIT_SHIFTED"sv },
            { DataProcessingRegisterGroup::AddSubtractShiftedRegister::ADD_64BIT_SHIFTED, "ADD_64BIT_SHIFTED"sv },
            { DataProcessingRegisterGroup::AddSubtractShiftedRegister::ADDS_64BIT_SHIFTED, "ADDS_64BIT_SHIFTED"sv },
            { DataProcessingRegisterGroup::AddSubtractShiftedRegister::SUB_64BIT_SHIFTED, "SUB_64BIT_SHIFTED"sv },
            { DataProcessingRegisterGroup::AddSubtractShiftedRegister::SUBS_64BIT_SHIFTED, "SUBS_64BIT_SHIFTED"sv },
        } } };

    static constexpr SmallConstMap< DataProcessingRegisterGroup::AddSubtractExtendedRegister, std::string_view,
                                    enum_size_v< DataProcessingRegisterGroup::AddSubtractExtendedRegister > >
        AddSubtractExtendedRegisterStringMap { { {
            { DataProcessingRegisterGroup::AddSubtractExtendedRegister::ADD_32BIT_EXTENDED, "ADD_32BIT_EXTENDED"sv },
            { DataProcessingRegisterGroup::AddSubtractExtendedRegister::ADDS_32BIT_EXTENDED, "ADDS_32BIT_EXTENDED"sv },
            { DataProcessingRegisterGroup::AddSubtractExtendedRegister::SUB_32BIT_EXTENDED, "SUB_32BIT_EXTENDED"sv },
            { DataProcessingRegisterGroup::AddSubtractExtendedRegister::SUBS_32BIT_EXTENDED, "SUBS_32BIT_EXTENDED"sv },
            { DataProcessingRegisterGroup::AddSubtractExtendedRegister::ADD_64BIT_EXTENDED, "ADD_64BIT_EXTENDED"sv },
            { DataProcessingRegisterGroup::AddSubtractExtendedRegister::ADDS_64BIT_EXTENDED, "ADDS_64BIT_EXTENDED"sv },
            { DataProcessingRegisterGroup::AddSubtractExtendedRegister::SUB_64BIT_EXTENDED, "SUB_64BIT_EXTENDED"sv },
            { DataProcessingRegisterGroup::AddSubtractExtendedRegister::SUBS_64BIT_EXTENDED, "SUBS_64BIT_EXTENDED"sv },
        } } };

    static constexpr SmallConstMap< DataProcessingRegisterGroup::AddSubtractCarry, std::string_view,
                                    enum_size_v< DataProcessingRegisterGroup::AddSubtractCarry > >
        AddSubtractCarryStringMap { { {
            { DataProcessingRegisterGroup::AddSubtractCarry::ADC_32BIT, "ADC_32BIT"sv },
            { DataProcessingRegisterGroup::AddSubtractCarry::ADCS_32BIT, "ADCS_32BIT"sv },
            { DataProcessingRegisterGroup::AddSubtractCarry::SBC_32BIT, "SBC_32BIT"sv },
            { DataProcessingRegisterGroup::AddSubtractCarry::SBCS_32BIT, "SBCS_32BIT"sv },
            { DataProcessingRegisterGroup::AddSubtractCarry::ADC_64BIT, "ADC_64BIT"sv },
            { DataProcessingRegisterGroup::AddSubtractCarry::ADCS_64BIT, "ADCS_64BIT"sv },
            { DataProcessingRegisterGroup::AddSubtractCarry::SBC_64BIT, "SBC_64BIT"sv },
            { DataProcessingRegisterGroup::AddSubtractCarry::SBCS_64BIT, "SBCS_64BIT"sv },
        } } };

    static constexpr SmallConstMap< DataProcessingRegisterGroup::RotateRightIntoFlags, std::string_view,
                                    enum_size_v< DataProcessingRegisterGroup::RotateRightIntoFlags > >
        RotateRightIntoFlagsStringMap { { {
            { DataProcessingRegisterGroup::RotateRightIntoFlags::RMIF, "RMIF"sv },
        } } };

    static constexpr SmallConstMap< DataProcessingRegisterGroup::EvaluateIntoFlags, std::string_view,
                                    enum_size_v< DataProcessingRegisterGroup::EvaluateIntoFlags > >
        EvaluateIntoFlagsStringMap { { {
            { DataProcessingRegisterGroup::EvaluateIntoFlags::SETF8, "SETF8"sv },
            { DataProcessingRegisterGroup::EvaluateIntoFlags::SETF16, "SETF16"sv },
        } } };

    static constexpr SmallConstMap< DataProcessingRegisterGroup::ConditionalCompareRegister, std::string_view,
                                    enum_size_v< DataProcessingRegisterGroup::ConditionalCompareRegister > >
        ConditionalCompareRegisterStringMap { { {
            { DataProcessingRegisterGroup::ConditionalCompareRegister::CCMNr_32BIT, "CCMNr_32BIT"sv },
            { DataProcessingRegisterGroup::ConditionalCompareRegister::CCMPr_32BIT, "CCMPr_32BIT"sv },
            { DataProcessingRegisterGroup::ConditionalCompareRegister::CCMNr_64BIT, "CCMNr_64BIT"sv },
            { DataProcessingRegisterGroup::ConditionalCompareRegister::CCMPr_64BIT, "CCMPr_64BIT"sv },
        } } };

    static constexpr SmallConstMap< DataProcessingRegisterGroup::ConditionalCompareImmediate, std::string_view,
                                    enum_size_v< DataProcessingRegisterGroup::ConditionalCompareImmediate > >
        ConditionalCompareImmediateStringMap { { {
            { DataProcessingRegisterGroup::ConditionalCompareImmediate::CCMNi_32BIT, "CCMNi_32BIT"sv },
            { DataProcessingRegisterGroup::ConditionalCompareImmediate::CCMPi_32BIT, "CCMPi_32BIT"sv },
            { DataProcessingRegisterGroup::ConditionalCompareImmediate::CCMNi_64BIT, "CCMNi_64BIT"sv },
            { DataProcessingRegisterGroup::ConditionalCompareImmediate::CCMPi_64BIT, "CCMPi_64BIT"sv },
        } } };

    static constexpr SmallConstMap< DataProcessingRegisterGroup::ConditionalSelect, std::string_view,
                                    enum_size_v< DataProcessingRegisterGroup::ConditionalSelect > >
        ConditionalSelectStringMap { { {
            { DataProcessingRegisterGroup::ConditionalSelect::CSEL_32BIT, "CSEL_32BIT"sv },
            { DataProcessingRegisterGroup::ConditionalSelect::CSINC_32BIT, "CSINC_32BIT"sv },
            { DataProcessingRegisterGroup::ConditionalSelect::CSINV_32BIT, "CSINV_32BIT"sv },
            { DataProcessingRegisterGroup::ConditionalSelect::CSNEG_32BIT, "CSNEG_32BIT"sv },
            { DataProcessingRegisterGroup::ConditionalSelect::CSEL_64BIT, "CSEL_64BIT"sv },
            { DataProcessingRegisterGroup::ConditionalSelect::CSINC_64BIT, "CSINC_64BIT"sv },
            { DataProcessingRegisterGroup::ConditionalSelect::CSINV_64BIT, "CSINV_64BIT"sv },
            { DataProcessingRegisterGroup::ConditionalSelect::CSNEG_64BIT, "CSNEG_64BIT"sv },
        } } };

    static constexpr SmallConstMap< DataProcessingRegisterGroup::DataProcessingThreeSource, std::string_view,
                                    enum_size_v< DataProcessingRegisterGroup::DataProcessingThreeSource > >
        DataProcessingThreeSourceStringMap { { {
            { DataProcessingRegisterGroup::DataProcessingThreeSource::MADD_32BIT, "MADD_32BIT"sv },
            { DataProcessingRegisterGroup::DataProcessingThreeSource::MSUB_32BIT, "MSUB_32BIT"sv },
            { DataProcessingRegisterGroup::DataProcessingThreeSource::MADD_64BIT, "MADD_64BIT"sv },
            { DataProcessingRegisterGroup::DataProcessingThreeSource::MSUB_64BIT, "MSUB_64BIT"sv },
            { DataProcessingRegisterGroup::DataProcessingThreeSource::SMADDL, "SMADDL"sv },
            { DataProcessingRegisterGroup::DataProcessingThreeSource::SMSUBL, "SMSUBL"sv },
            { DataProcessingRegisterGroup::DataProcessingThreeSource::SMULH, "SMULH"sv },
            { DataProcessingRegisterGroup::DataProcessingThreeSource::UMADDL, "UMADDL"sv },
            { DataProcessingRegisterGroup::DataProcessingThreeSource::UMSUBL, "UMSUBL"sv },
            { DataProcessingRegisterGroup::DataProcessingThreeSource::UMULH, "UMULH"sv },
        } } };

} // namespace

template <>
static std::string_view Enum::ToChar(A64DataProcessingRegisterGroup elem) {
    return A64DataProcessingRegisterGroupStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream& os, A64DataProcessingRegisterGroup elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view Enum::ToChar(DataProcessingRegisterGroup::DataProcessingTwoSource elem) {
    return DataProcessingTwoSourceStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream& os, DataProcessingRegisterGroup::DataProcessingTwoSource elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view Enum::ToChar(DataProcessingRegisterGroup::DataProcessingOneSource elem) {
    return DataProcessingOneSourceStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream& os, DataProcessingRegisterGroup::DataProcessingOneSource elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view Enum::ToChar(DataProcessingRegisterGroup::LogicalShiftedRegister elem) {
    return LogicalShiftedRegisterStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream& os, DataProcessingRegisterGroup::LogicalShiftedRegister elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view Enum::ToChar(DataProcessingRegisterGroup::AddSubtractShiftedRegister elem) {
    return AddSubtractShiftedRegisterStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream& os, DataProcessingRegisterGroup::AddSubtractShiftedRegister elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view Enum::ToChar(DataProcessingRegisterGroup::AddSubtractExtendedRegister elem) {
    return AddSubtractExtendedRegisterStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream& os, DataProcessingRegisterGroup::AddSubtractExtendedRegister elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view Enum::ToChar(DataProcessingRegisterGroup::AddSubtractCarry elem) {
    return AddSubtractCarryStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream& os, DataProcessingRegisterGroup::AddSubtractCarry elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view Enum::ToChar(DataProcessingRegisterGroup::RotateRightIntoFlags elem) {
    return RotateRightIntoFlagsStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream& os, DataProcessingRegisterGroup::RotateRightIntoFlags elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view Enum::ToChar(DataProcessingRegisterGroup::EvaluateIntoFlags elem) {
    return EvaluateIntoFlagsStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream& os, DataProcessingRegisterGroup::EvaluateIntoFlags elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view Enum::ToChar(DataProcessingRegisterGroup::ConditionalCompareRegister elem) {
    return ConditionalCompareRegisterStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream& os, DataProcessingRegisterGroup::ConditionalCompareRegister elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view Enum::ToChar(DataProcessingRegisterGroup::ConditionalCompareImmediate elem) {
    return ConditionalCompareImmediateStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream& os, DataProcessingRegisterGroup::ConditionalCompareImmediate elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view Enum::ToChar(DataProcessingRegisterGroup::ConditionalSelect elem) {
    return ConditionalSelectStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream& os, DataProcessingRegisterGroup::ConditionalSelect elem) {
    os << Enum::ToChar(elem);
    return os;
}

template <>
static std::string_view Enum::ToChar(DataProcessingRegisterGroup::DataProcessingThreeSource elem) {
    return DataProcessingThreeSourceStringMap.At(elem);
}

template <>
std::ostream& operator<<(std::ostream& os, DataProcessingRegisterGroup::DataProcessingThreeSource elem) {
    os << Enum::ToChar(elem);
    return os;
}

END_NAMESPACE
