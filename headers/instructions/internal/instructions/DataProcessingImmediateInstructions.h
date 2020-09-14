#pragma once
#include "BaseInstruction.hpp"

namespace arm_emu {

/*
Data Processing Immediate Group
*/
struct DataProcessingImmediateGroup : public AbstractInstruction< std::uint32_t, DataProcessingImmediateGroup > {
    [[nodiscard]] constexpr auto getInstructionClass() const noexcept { return DataProcessingImmediateA64::getInstructionClass(mInstruction); }

    constexpr explicit DataProcessingImmediateGroup(BaseInstruction instruction) noexcept : AbstractInstruction(instruction.data()) {}
    constexpr explicit DataProcessingImmediateGroup(const DataProcessingImmediateGroup& instruction) noexcept : AbstractInstruction(instruction.mInstruction) {};
    constexpr explicit DataProcessingImmediateGroup(DataProcessingImmediateGroup&& instruction) noexcept : AbstractInstruction(instruction.mInstruction) {};
    NULL_ASSINGMENT(DataProcessingImmediateGroup)
    ~DataProcessingImmediateGroup() = default;
};

template < DataProcessingImmediateA64::InstructionGroupEnum >
struct DataProcessingImmediateInstruction;

template <>
struct DataProcessingImmediateInstruction< DataProcessingImmediateA64::InstructionGroupEnum::PC_RELATIVE_ADDRESSING > final : public DataProcessingImmediateGroup {
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        return DataProcessingImmediateA64::getInstructionType<DataProcessingImmediateA64::InstructionGroupEnum::PC_RELATIVE_ADDRESSING>(mInstruction);
    }

    [[nodiscard]] constexpr auto get_Rd() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 0));
    }
    [[nodiscard]] constexpr auto get_immhi() const noexcept {
        return static_cast<std::uint32_t>(static_cast<std::uint32_t>(0x7FFFF) & (mInstruction >> 5));
    }
    [[nodiscard]] constexpr auto get_immlo() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11) & (mInstruction >> 29));
    }

    constexpr explicit DataProcessingImmediateInstruction(const DataProcessingImmediateGroup& instruction) noexcept : DataProcessingImmediateGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingImmediateInstruction)
    ~DataProcessingImmediateInstruction() = default;
};
template <>
struct DataProcessingImmediateInstruction< DataProcessingImmediateA64::InstructionGroupEnum::ADD_SUBTRACT_IMMEDIATE > final : public DataProcessingImmediateGroup {
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        return DataProcessingImmediateA64::getInstructionType< DataProcessingImmediateA64::InstructionGroupEnum::ADD_SUBTRACT_IMMEDIATE >(mInstruction);
    }

    [[nodiscard]] constexpr auto get_Rd() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 0));
    }
    [[nodiscard]] constexpr auto get_Rn() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 5));
    }
    [[nodiscard]] constexpr auto get_imm12() const noexcept {
        return static_cast<std::uint16_t>(static_cast<std::uint32_t>(0xFFF) & (mInstruction >> 10));
    }
    [[nodiscard]] constexpr auto get_sh() const noexcept {
        return readBit(22);
    }

    constexpr explicit DataProcessingImmediateInstruction(const DataProcessingImmediateGroup& instruction) noexcept : DataProcessingImmediateGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingImmediateInstruction)
    ~DataProcessingImmediateInstruction() = default;
};
template <>
struct DataProcessingImmediateInstruction< DataProcessingImmediateA64::InstructionGroupEnum::ADD_SUBTRACT_IMMEDIATE_TAG > final : public DataProcessingImmediateGroup {
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        return DataProcessingImmediateA64::getInstructionType< DataProcessingImmediateA64::InstructionGroupEnum::ADD_SUBTRACT_IMMEDIATE_TAG >(mInstruction);
    }

    [[nodiscard]] constexpr auto get_Xd() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 0));
    }
    [[nodiscard]] constexpr auto get_Xn() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 5));
    }
    [[nodiscard]] constexpr auto get_uimm4() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b1111) & (mInstruction >> 10));
    }
    [[nodiscard]] constexpr auto get_uimm6() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b111111) & (mInstruction >> 16));
    }

    constexpr explicit DataProcessingImmediateInstruction(const DataProcessingImmediateGroup& instruction) noexcept : DataProcessingImmediateGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingImmediateInstruction)
    ~DataProcessingImmediateInstruction() = default;
};
template <>
struct DataProcessingImmediateInstruction< DataProcessingImmediateA64::InstructionGroupEnum::LOGICAL_IMMEDIATE > final : public DataProcessingImmediateGroup {
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        return DataProcessingImmediateA64::getInstructionType< DataProcessingImmediateA64::InstructionGroupEnum::LOGICAL_IMMEDIATE >(mInstruction);
    }

    [[nodiscard]] constexpr auto get_Rd() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 0));
    }
    [[nodiscard]] constexpr auto get_Rn() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 5));
    }
    [[nodiscard]] constexpr auto get_imms() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b111111) & (mInstruction >> 10));
    }
    [[nodiscard]] constexpr auto get_immr() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b111111) & (mInstruction >> 16));
    }
    [[nodiscard]] constexpr auto get_N() const noexcept {
        return readBit(22);
    }

    constexpr explicit DataProcessingImmediateInstruction(const DataProcessingImmediateGroup& instruction) noexcept : DataProcessingImmediateGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingImmediateInstruction)
    ~DataProcessingImmediateInstruction() = default;
};
template <>
struct DataProcessingImmediateInstruction< DataProcessingImmediateA64::InstructionGroupEnum::MOVE_WIDE_IMMEDIATE > final : public DataProcessingImmediateGroup {
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        return DataProcessingImmediateA64::getInstructionType< DataProcessingImmediateA64::InstructionGroupEnum::MOVE_WIDE_IMMEDIATE >(mInstruction);
    }

    [[nodiscard]] constexpr auto get_Rd() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 0));
    }
    [[nodiscard]] constexpr auto get_imm16() const noexcept {
        return static_cast<std::uint16_t>(static_cast<std::uint32_t>(0xFFFF) & (mInstruction >> 5));
    }
    [[nodiscard]] constexpr auto get_hw() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11) & (mInstruction >> 21));
    }

    constexpr explicit DataProcessingImmediateInstruction(const DataProcessingImmediateGroup& instruction) noexcept : DataProcessingImmediateGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingImmediateInstruction)
    ~DataProcessingImmediateInstruction() = default;
};
template <>
struct DataProcessingImmediateInstruction< DataProcessingImmediateA64::InstructionGroupEnum::BITFIELD > final : public DataProcessingImmediateGroup {
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        return DataProcessingImmediateA64::getInstructionType< DataProcessingImmediateA64::InstructionGroupEnum::BITFIELD >(mInstruction);
    }

    constexpr explicit DataProcessingImmediateInstruction(const DataProcessingImmediateGroup& instruction) noexcept : DataProcessingImmediateGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingImmediateInstruction)
    ~DataProcessingImmediateInstruction() = default;
};
template <>
struct DataProcessingImmediateInstruction< DataProcessingImmediateA64::InstructionGroupEnum::EXTRACT > final : public DataProcessingImmediateGroup {
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        return DataProcessingImmediateA64::getInstructionType< DataProcessingImmediateA64::InstructionGroupEnum::EXTRACT >(mInstruction);
    }

    constexpr explicit DataProcessingImmediateInstruction(const DataProcessingImmediateGroup& instruction) noexcept : DataProcessingImmediateGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingImmediateInstruction)
    ~DataProcessingImmediateInstruction() = default;
};

} // namespace arm_emu
