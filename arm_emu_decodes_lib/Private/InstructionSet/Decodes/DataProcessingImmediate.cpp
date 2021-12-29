
#include <InstructionSet/A64InstructionSet.h>
#include <Utility/StreamableEnum.h>
#include <ostream>
#include <string_view>

BEGIN_NAMESPACE

namespace {

    using namespace std::literals::string_view_literals;

    static constexpr SmallConstMap< A64DataProcessingImmediateGroup, std::string_view,
                                    enum_size_v< A64DataProcessingImmediateGroup > >
        A64DataProcessingImmediateGroupStringMap { { {
            { A64DataProcessingImmediateGroup::PC_RELATIVE_ADDRESSING, "PC_RELATIVE_ADDRESSING"sv },
            { A64DataProcessingImmediateGroup::ADD_SUBTRACT_IMMEDIATE, "ADD_SUBTRACT_IMMEDIATE"sv },
            { A64DataProcessingImmediateGroup::ADD_SUBTRACT_IMMEDIATE_TAG, "ADD_SUBTRACT_IMMEDIATE_TAG"sv },
            { A64DataProcessingImmediateGroup::LOGICAL_IMMEDIATE, "LOGICAL_IMMEDIATE"sv },
            { A64DataProcessingImmediateGroup::MOVE_WIDE_IMMEDIATE, "MOVE_WIDE_IMMEDIATE"sv },
            { A64DataProcessingImmediateGroup::BITFIELD, "BITFIELD"sv },
            { A64DataProcessingImmediateGroup::EXTRACT, "EXTRACT"sv },
        } } };

    static constexpr SmallConstMap< DataProcessingImmediateGroup::PCRelativeAddressing, std::string_view,
                                    enum_size_v< DataProcessingImmediateGroup::PCRelativeAddressing > >
        PCRelativeAddressingStringMap { { {
            { DataProcessingImmediateGroup::PCRelativeAddressing::ADR, "ADR"sv },
            { DataProcessingImmediateGroup::PCRelativeAddressing::ADRP, "ADRP"sv },
        } } };

    static constexpr SmallConstMap< DataProcessingImmediateGroup::AddSubtractImmediate, std::string_view,
                                    enum_size_v< DataProcessingImmediateGroup::AddSubtractImmediate > >
        AddSubtractImmediateStringMap { { {
            { DataProcessingImmediateGroup::AddSubtractImmediate::ADDi_32BIT, "ADDi_32BIT"sv },
            { DataProcessingImmediateGroup::AddSubtractImmediate::ADDSi_32BIT, "ADDSi_32BIT"sv },
            { DataProcessingImmediateGroup::AddSubtractImmediate::SUBi_32BIT, "SUBi_32BIT"sv },
            { DataProcessingImmediateGroup::AddSubtractImmediate::SUBSi_32BIT, "SUBSi_32BIT"sv },
            { DataProcessingImmediateGroup::AddSubtractImmediate::ADDi_64BIT, "ADDi_64BIT"sv },
            { DataProcessingImmediateGroup::AddSubtractImmediate::ADDSi_64BIT, "ADDSi_64BIT"sv },
            { DataProcessingImmediateGroup::AddSubtractImmediate::SUBi_64BIT, "SUBi_64BIT"sv },
            { DataProcessingImmediateGroup::AddSubtractImmediate::SUBSi_64BIT, "SUBSi_64BIT"sv },
        } } };

    static constexpr SmallConstMap< DataProcessingImmediateGroup::AddSubtractImmediateTag, std::string_view,
                                    enum_size_v< DataProcessingImmediateGroup::AddSubtractImmediateTag > >
        AddSubtractImmediateTagStringMap { { {
            { DataProcessingImmediateGroup::AddSubtractImmediateTag::ADDG, "ADDG"sv },
            { DataProcessingImmediateGroup::AddSubtractImmediateTag::SUBG, "SUBG"sv },
        } } };

    static constexpr SmallConstMap< DataProcessingImmediateGroup::LogicalImmediate, std::string_view,
                                    enum_size_v< DataProcessingImmediateGroup::LogicalImmediate > >
        LogicalImmediateStringMap { { {
            { DataProcessingImmediateGroup::LogicalImmediate::AND_32BIT, "AND_32BIT"sv },
            { DataProcessingImmediateGroup::LogicalImmediate::ORR_32BIT, "ORR_32BIT"sv },
            { DataProcessingImmediateGroup::LogicalImmediate::EOR_32BIT, "EOR_32BIT"sv },
            { DataProcessingImmediateGroup::LogicalImmediate::ANDS_32BIT, "ANDS_32BIT"sv },
            { DataProcessingImmediateGroup::LogicalImmediate::AND_64BIT, "AND_64BIT"sv },
            { DataProcessingImmediateGroup::LogicalImmediate::ORR_64BIT, "ORR_64BIT"sv },
            { DataProcessingImmediateGroup::LogicalImmediate::EOR_64BIT, "EOR_64BIT"sv },
            { DataProcessingImmediateGroup::LogicalImmediate::ANDS_64BIT, "ANDS_64BIT"sv },
        } } };

    static constexpr SmallConstMap< DataProcessingImmediateGroup::MoveWideImmediate, std::string_view,
                                    enum_size_v< DataProcessingImmediateGroup::MoveWideImmediate > >
        MoveWideImmediateStringMap { { {
            { DataProcessingImmediateGroup::MoveWideImmediate::MOVN_32BIT, "MOVN_32BIT"sv },
            { DataProcessingImmediateGroup::MoveWideImmediate::MOVZ_32BIT, "MOVZ_32BIT"sv },
            { DataProcessingImmediateGroup::MoveWideImmediate::MOVK_32BIT, "MOVK_32BIT"sv },
            { DataProcessingImmediateGroup::MoveWideImmediate::MOVN_64BIT, "MOVN_64BIT"sv },
            { DataProcessingImmediateGroup::MoveWideImmediate::MOVZ_64BIT, "MOVZ_64BIT"sv },
            { DataProcessingImmediateGroup::MoveWideImmediate::MOVK_64BIT, "MOVK_64BIT"sv },
        } } };

    static constexpr SmallConstMap< DataProcessingImmediateGroup::Bitfield, std::string_view,
                                    enum_size_v< DataProcessingImmediateGroup::Bitfield > >
        BitfieldStringMap { { {
            { DataProcessingImmediateGroup::Bitfield::SBFM_32BIT, "SBFM_32BIT"sv },
            { DataProcessingImmediateGroup::Bitfield::BFM_32BIT, "BFM_32BIT"sv },
            { DataProcessingImmediateGroup::Bitfield::UBFM_32BIT, "UBFM_32BIT"sv },
            { DataProcessingImmediateGroup::Bitfield::SBFM_64BIT, "SBFM_64BIT"sv },
            { DataProcessingImmediateGroup::Bitfield::BFM_64BIT, "BFM_64BIT"sv },
            { DataProcessingImmediateGroup::Bitfield::UBFM_64BIT, "UBFM_64BIT"sv },
        } } };

    static constexpr SmallConstMap< DataProcessingImmediateGroup::Extract, std::string_view,
                                    enum_size_v< DataProcessingImmediateGroup::Extract > >
        ExtractStringMap { { {
            { DataProcessingImmediateGroup::Extract::EXTR_32BIT, "EXTR_32BIT"sv },
            { DataProcessingImmediateGroup::Extract::EXTR_64BIT, "EXTR_64BIT"sv },
        } } };

} // namespace

template <>
ARMEMU_API std::string_view Enum::ToChar(A64DataProcessingImmediateGroup enumValue) {
    return A64DataProcessingImmediateGroupStringMap.At(enumValue);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, A64DataProcessingImmediateGroup enumValue) {
    os << Enum::ToChar(enumValue);
    return os;
}

template <>
ARMEMU_API std::string_view Enum::ToChar(DataProcessingImmediateGroup::PCRelativeAddressing enumValue) {
    return PCRelativeAddressingStringMap.At(enumValue);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, DataProcessingImmediateGroup::PCRelativeAddressing enumValue) {
    os << Enum::ToChar(enumValue);
    return os;
}

template <>
ARMEMU_API std::string_view Enum::ToChar(DataProcessingImmediateGroup::AddSubtractImmediate enumValue) {
    return AddSubtractImmediateStringMap.At(enumValue);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, DataProcessingImmediateGroup::AddSubtractImmediate enumValue) {
    os << Enum::ToChar(enumValue);
    return os;
}

template <>
ARMEMU_API std::string_view Enum::ToChar(DataProcessingImmediateGroup::AddSubtractImmediateTag enumValue) {
    return AddSubtractImmediateTagStringMap.At(enumValue);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, DataProcessingImmediateGroup::AddSubtractImmediateTag enumValue) {
    os << Enum::ToChar(enumValue);
    return os;
}

template <>
ARMEMU_API std::string_view Enum::ToChar(DataProcessingImmediateGroup::LogicalImmediate enumValue) {
    return LogicalImmediateStringMap.At(enumValue);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, DataProcessingImmediateGroup::LogicalImmediate enumValue) {
    os << Enum::ToChar(enumValue);
    return os;
}

template <>
ARMEMU_API std::string_view Enum::ToChar(DataProcessingImmediateGroup::MoveWideImmediate enumValue) {
    return MoveWideImmediateStringMap.At(enumValue);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, DataProcessingImmediateGroup::MoveWideImmediate enumValue) {
    os << Enum::ToChar(enumValue);
    return os;
}

template <>
ARMEMU_API std::string_view Enum::ToChar(DataProcessingImmediateGroup::Bitfield enumValue) {
    return BitfieldStringMap.At(enumValue);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, DataProcessingImmediateGroup::Bitfield enumValue) {
    os << Enum::ToChar(enumValue);
    return os;
}

template <>
ARMEMU_API std::string_view Enum::ToChar(DataProcessingImmediateGroup::Extract enumValue) {
    return ExtractStringMap.At(enumValue);
}

template <>
ARMEMU_API std::ostream& operator<<(std::ostream& os, DataProcessingImmediateGroup::Extract enumValue) {
    os << Enum::ToChar(enumValue);
    return os;
}

END_NAMESPACE
