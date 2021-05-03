// Based on C4.1.2 A64 instruction set encoding

#pragma once
#include <utilities.hpp>

namespace arm_emu {

    enum class DataProcessingImmediateGroupA64 : std::uint32_t
    {
        PC_RELATIVE_ADDRESSING,
        ADD_SUBTRACT_IMMEDIATE,
        ADD_SUBTRACT_IMMEDIATE_TAG,
        LOGICAL_IMMEDIATE,
        MOVE_WIDE_IMMEDIATE,
        BITFIELD,
        EXTRACT,

        ARM_EMU_COUNT,

        Undefined = std::numeric_limits< std::underlying_type_t< DataProcessingImmediateGroupA64 > >::max(),
    };

    static Table< DataProcessingImmediateGroupA64, enum_size_v< DataProcessingImmediateGroupA64 > > DataProcessingImmediateGroupA64Table { { {
        { 0b0001'1111'0000'0000'0000'0000'0000'0000, 0b0001'0000'0000'0000'0000'0000'0000'0000, DataProcessingImmediateGroupA64::PC_RELATIVE_ADDRESSING },
        { 0b0001'1111'1000'0000'0000'0000'0000'0000, 0b0001'0001'0000'0000'0000'0000'0000'0000, DataProcessingImmediateGroupA64::ADD_SUBTRACT_IMMEDIATE },
        { 0b0001'1111'1000'0000'0000'0000'0000'0000, 0b0001'0001'1000'0000'0000'0000'0000'0000, DataProcessingImmediateGroupA64::ADD_SUBTRACT_IMMEDIATE_TAG },
        { 0b0001'1111'1000'0000'0000'0000'0000'0000, 0b0001'0010'0000'0000'0000'0000'0000'0000, DataProcessingImmediateGroupA64::LOGICAL_IMMEDIATE },
        { 0b0001'1111'1000'0000'0000'0000'0000'0000, 0b0001'0010'1000'0000'0000'0000'0000'0000, DataProcessingImmediateGroupA64::MOVE_WIDE_IMMEDIATE },
        { 0b0001'1111'1000'0000'0000'0000'0000'0000, 0b0001'0011'0000'0000'0000'0000'0000'0000, DataProcessingImmediateGroupA64::BITFIELD },
        { 0b0001'1111'1000'0000'0000'0000'0000'0000, 0b0001'0011'1000'0000'0000'0000'0000'0000, DataProcessingImmediateGroupA64::EXTRACT },
    } } };

    class DataProcessingImmediateA64 {
      public:
        using InstructionGroupEnum = DataProcessingImmediateGroupA64;

        DataProcessingImmediateA64()  = delete;
        ~DataProcessingImmediateA64() = delete;
        NULL_COPY_MOVE(DataProcessingImmediateA64)

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
        static inline Table< PCRelativeAddressing, enum_size_v< PCRelativeAddressing > >       PCRelativeAddressingTable { { {
            { 0b1001'1111'0000'0000'0000'0000'0000'0000, 0b0001'0000'0000'0000'0000'0000'0000'0000, PCRelativeAddressing::ADR },
            { 0b1001'1111'0000'0000'0000'0000'0000'0000, 0b1001'0000'0000'0000'0000'0000'0000'0000, PCRelativeAddressing::ADRP },
        } } };
        static inline Table< AddSubtractImmediate, enum_size_v< AddSubtractImmediate > >       AddSubtractImmediateTable { { {
            { 0b1111'1111'1000'0000'0000'0000'0000'0000, 0b0001'0001'0000'0000'0000'0000'0000'0000, AddSubtractImmediate::ADDi_32BIT },
            { 0b1111'1111'1000'0000'0000'0000'0000'0000, 0b0011'0001'0000'0000'0000'0000'0000'0000, AddSubtractImmediate::ADDSi_32BIT },
            { 0b1111'1111'1000'0000'0000'0000'0000'0000, 0b0101'0001'0000'0000'0000'0000'0000'0000, AddSubtractImmediate::SUBi_32BIT },
            { 0b1111'1111'1000'0000'0000'0000'0000'0000, 0b0111'0001'0000'0000'0000'0000'0000'0000, AddSubtractImmediate::SUBSi_32BIT },
            { 0b1111'1111'1000'0000'0000'0000'0000'0000, 0b1001'0001'0000'0000'0000'0000'0000'0000, AddSubtractImmediate::ADDi_64BIT },
            { 0b1111'1111'1000'0000'0000'0000'0000'0000, 0b1011'0001'0000'0000'0000'0000'0000'0000, AddSubtractImmediate::ADDSi_64BIT },
            { 0b1111'1111'1000'0000'0000'0000'0000'0000, 0b1101'0001'0000'0000'0000'0000'0000'0000, AddSubtractImmediate::SUBi_64BIT },
            { 0b1111'1111'1000'0000'0000'0000'0000'0000, 0b1111'0001'0000'0000'0000'0000'0000'0000, AddSubtractImmediate::SUBSi_64BIT },
        } } };
        static inline Table< AddSubtractImmediateTag, enum_size_v< AddSubtractImmediateTag > > AddSubtractImmediateTagTable { { {
            { 0b1111'1111'1100'0000'0000'0000'0000'0000, 0b1001'0001'1000'0000'0000'0000'0000'0000, AddSubtractImmediateTag::ADDG },
            { 0b1111'1111'1100'0000'0000'0000'0000'0000, 0b1101'0001'1000'0000'0000'0000'0000'0000, AddSubtractImmediateTag::SUBG },
        } } };
        static inline Table< LogicalImmediate, enum_size_v< LogicalImmediate > >               LogicalImmediateTable { { {
            { 0b1111'1111'1100'0000'0000'0000'0000'0000, 0b0001'0010'0000'0000'0000'0000'0000'0000, LogicalImmediate::AND_32BIT },
            { 0b1111'1111'1100'0000'0000'0000'0000'0000, 0b0011'0010'0000'0000'0000'0000'0000'0000, LogicalImmediate::ORR_32BIT },
            { 0b1111'1111'1100'0000'0000'0000'0000'0000, 0b0101'0010'0000'0000'0000'0000'0000'0000, LogicalImmediate::EOR_32BIT },
            { 0b1111'1111'1100'0000'0000'0000'0000'0000, 0b0111'0010'0000'0000'0000'0000'0000'0000, LogicalImmediate::ANDS_32BIT },
            { 0b1111'1111'1000'0000'0000'0000'0000'0000, 0b1001'0010'0000'0000'0000'0000'0000'0000, LogicalImmediate::AND_64BIT },
            { 0b1111'1111'1000'0000'0000'0000'0000'0000, 0b1011'0010'0000'0000'0000'0000'0000'0000, LogicalImmediate::ORR_64BIT },
            { 0b1111'1111'1000'0000'0000'0000'0000'0000, 0b1101'0010'0000'0000'0000'0000'0000'0000, LogicalImmediate::EOR_64BIT },
            { 0b1111'1111'1000'0000'0000'0000'0000'0000, 0b1111'0010'0000'0000'0000'0000'0000'0000, LogicalImmediate::ANDS_64BIT },
        } } };
        static inline Table< MoveWideImmediate, enum_size_v< MoveWideImmediate > >             MoveWideImmediateTable { { {
            { 0b1111'1111'1100'0000'0000'0000'0000'0000, 0b0001'0010'1000'0000'0000'0000'0000'0000, MoveWideImmediate::MOVN_32BIT },
            { 0b1111'1111'1100'0000'0000'0000'0000'0000, 0b0101'0010'1000'0000'0000'0000'0000'0000, MoveWideImmediate::MOVZ_32BIT },
            { 0b1111'1111'1100'0000'0000'0000'0000'0000, 0b0111'0010'1000'0000'0000'0000'0000'0000, MoveWideImmediate::MOVK_32BIT },
            { 0b1111'1111'1000'0000'0000'0000'0000'0000, 0b1001'0010'1000'0000'0000'0000'0000'0000, MoveWideImmediate::MOVN_64BIT },
            { 0b1111'1111'1000'0000'0000'0000'0000'0000, 0b1101'0010'1000'0000'0000'0000'0000'0000, MoveWideImmediate::MOVZ_64BIT },
            { 0b1111'1111'1000'0000'0000'0000'0000'0000, 0b1111'0010'1000'0000'0000'0000'0000'0000, MoveWideImmediate::MOVK_64BIT },
        } } };
        static inline Table< Bitfield, enum_size_v< Bitfield > >                               BitfieldTable { { {
            { 0b1111'1111'1100'0000'0000'0000'0000'0000, 0b0001'0011'0000'0000'0000'0000'0000'0000, Bitfield::SBFM_32BIT },
            { 0b1111'1111'1100'0000'0000'0000'0000'0000, 0b0011'0011'0000'0000'0000'0000'0000'0000, Bitfield::BFM_32BIT },
            { 0b1111'1111'1100'0000'0000'0000'0000'0000, 0b0101'0011'0000'0000'0000'0000'0000'0000, Bitfield::UBFM_32BIT },
            { 0b1111'1111'1100'0000'0000'0000'0000'0000, 0b1001'0011'0100'0000'0000'0000'0000'0000, Bitfield::SBFM_64BIT },
            { 0b1111'1111'1100'0000'0000'0000'0000'0000, 0b1011'0011'0100'0000'0000'0000'0000'0000, Bitfield::BFM_64BIT },
            { 0b1111'1111'1100'0000'0000'0000'0000'0000, 0b1101'0011'0100'0000'0000'0000'0000'0000, Bitfield::UBFM_64BIT },
        } } };
        static inline Table< Extract, enum_size_v< Extract > >                                 ExtractTable { { {
            { 0b1111'1111'1110'0000'1000'0000'0000'0000, 0b0001'0011'1000'0000'0000'0000'0000'0000, Extract::EXTR_32BIT },
            { 0b1111'1111'1110'0000'0000'0000'0000'0000, 0b1001'0011'1100'0000'0000'0000'0000'0000, Extract::EXTR_64BIT },
        } } };

      public:
        template < DataProcessingImmediateGroupA64 InstructionClass >
        [[nodiscard]] inline constexpr static auto getClassTable() {
            if constexpr (InstructionClass == DataProcessingImmediateGroupA64::PC_RELATIVE_ADDRESSING) {
                return PCRelativeAddressingTable;
            } else if constexpr (InstructionClass == DataProcessingImmediateGroupA64::ADD_SUBTRACT_IMMEDIATE) {
                return AddSubtractImmediateTable;
            } else if constexpr (InstructionClass == DataProcessingImmediateGroupA64::ADD_SUBTRACT_IMMEDIATE_TAG) {
                return AddSubtractImmediateTagTable;
            } else if constexpr (InstructionClass == DataProcessingImmediateGroupA64::LOGICAL_IMMEDIATE) {
                return LogicalImmediateTable;
            } else if constexpr (InstructionClass == DataProcessingImmediateGroupA64::MOVE_WIDE_IMMEDIATE) {
                return MoveWideImmediateTable;
            } else if constexpr (InstructionClass == DataProcessingImmediateGroupA64::BITFIELD) {
                return BitfieldTable;
            } else if constexpr (InstructionClass == DataProcessingImmediateGroupA64::EXTRACT) {
                return ExtractTable;
            } else {
                throw ARM_EMU_EXCEPTION; // Compile time error detection
            }
        }

        [[nodiscard]] inline static auto getInstructionClass(const std::uint32_t& instruction) noexcept {
            for (auto&& e : DataProcessingImmediateGroupA64Table.mEntries) {
                if ((instruction & e.code) == e.expected) {
                    return e.type;
                }
            }
            return DataProcessingImmediateGroupA64::Undefined;
        }

        template < DataProcessingImmediateGroupA64 InstructionClass >
        [[nodiscard]] inline static auto getInstructionType(const std::uint32_t& instruction) noexcept {
            auto mTable = getClassTable< InstructionClass >();
            for (auto&& e : mTable.mEntries) {
                if ((instruction & e.code) == e.expected) {
                    return e.type;
                }
            }
            return static_cast< typename decltype(mTable)::EnumType >(DataProcessingImmediateGroupA64::Undefined);
        }
    };
} // namespace arm_emu