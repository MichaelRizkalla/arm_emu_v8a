#pragma once
#include "BaseInstruction.hpp"

namespace arm_emu {

/*
Data Processing Immediate Group
*/
struct DataProcessingImmediateGroup : public AbstractInstruction< std::uint32_t, DataProcessingImmediateGroup > {
    [[nodiscard]] auto getInstructionClass() const noexcept { return DataProcessingImmediateA64::getInstructionClass(mInstruction); }

    explicit DataProcessingImmediateGroup(BaseInstruction instruction) noexcept : AbstractInstruction(instruction.data()) {}
    explicit DataProcessingImmediateGroup(const DataProcessingImmediateGroup& instruction) noexcept : AbstractInstruction(instruction.mInstruction) {};
    explicit DataProcessingImmediateGroup(DataProcessingImmediateGroup&& instruction) noexcept : AbstractInstruction(instruction.mInstruction) {};
    NULL_ASSINGMENT(DataProcessingImmediateGroup)
    ~DataProcessingImmediateGroup() = default;
};

template < DataProcessingImmediateA64::InstructionGroupEnum >
struct DataProcessingImmediateInstruction;

template <>
struct DataProcessingImmediateInstruction< DataProcessingImmediateA64::InstructionGroupEnum::PC_RELATIVE_ADDRESSING > final : public DataProcessingImmediateGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DataProcessingImmediateA64::getInstructionType<DataProcessingImmediateA64::InstructionGroupEnum::PC_RELATIVE_ADDRESSING>(mInstruction);
    }

    [[nodiscard]] inline auto get_Rd() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 0));
    }
    [[nodiscard]] inline auto get_immhi() const noexcept {
        return static_cast<std::uint32_t>(static_cast<std::uint32_t>(0x7FFFF) & (mInstruction >> 5));
    }
    [[nodiscard]] inline auto get_immlo() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11) & (mInstruction >> 29));
    }

    explicit DataProcessingImmediateInstruction(const DataProcessingImmediateGroup& instruction) noexcept : DataProcessingImmediateGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingImmediateInstruction)
    ~DataProcessingImmediateInstruction() = default;
};
template <>
struct DataProcessingImmediateInstruction< DataProcessingImmediateA64::InstructionGroupEnum::ADD_SUBTRACT_IMMEDIATE > final : public DataProcessingImmediateGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DataProcessingImmediateA64::getInstructionType< DataProcessingImmediateA64::InstructionGroupEnum::ADD_SUBTRACT_IMMEDIATE >(mInstruction);
    }

    [[nodiscard]] inline auto get_Rd() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 0));
    }
    [[nodiscard]] inline auto get_Rn() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 5));
    }
    [[nodiscard]] inline auto get_imm12() const noexcept {
        return static_cast<std::uint16_t>(static_cast<std::uint32_t>(0xFFF) & (mInstruction >> 10));
    }
    [[nodiscard]] inline auto get_sh() const noexcept {
        return readBit(22);
    }

    explicit DataProcessingImmediateInstruction(const DataProcessingImmediateGroup& instruction) noexcept : DataProcessingImmediateGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingImmediateInstruction)
    ~DataProcessingImmediateInstruction() = default;
};
template <>
struct DataProcessingImmediateInstruction< DataProcessingImmediateA64::InstructionGroupEnum::ADD_SUBTRACT_IMMEDIATE_TAG > final : public DataProcessingImmediateGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DataProcessingImmediateA64::getInstructionType< DataProcessingImmediateA64::InstructionGroupEnum::ADD_SUBTRACT_IMMEDIATE_TAG >(mInstruction);
    }

    [[nodiscard]] inline auto get_Xd() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 0));
    }
    [[nodiscard]] inline auto get_Xn() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 5));
    }
    [[nodiscard]] inline auto get_uimm4() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b1111) & (mInstruction >> 10));
    }
    [[nodiscard]] inline auto get_uimm6() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b111111) & (mInstruction >> 16));
    }

    explicit DataProcessingImmediateInstruction(const DataProcessingImmediateGroup& instruction) noexcept : DataProcessingImmediateGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingImmediateInstruction)
    ~DataProcessingImmediateInstruction() = default;
};
template <>
struct DataProcessingImmediateInstruction< DataProcessingImmediateA64::InstructionGroupEnum::LOGICAL_IMMEDIATE > final : public DataProcessingImmediateGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DataProcessingImmediateA64::getInstructionType< DataProcessingImmediateA64::InstructionGroupEnum::LOGICAL_IMMEDIATE >(mInstruction);
    }

    [[nodiscard]] inline auto get_Rd() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 0));
    }
    [[nodiscard]] inline auto get_Rn() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 5));
    }
    [[nodiscard]] inline auto get_imms() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b111111) & (mInstruction >> 10));
    }
    [[nodiscard]] inline auto get_immr() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b111111) & (mInstruction >> 16));
    }
    [[nodiscard]] inline auto get_N() const noexcept {
        return readBit(22);
    }

    explicit DataProcessingImmediateInstruction(const DataProcessingImmediateGroup& instruction) noexcept : DataProcessingImmediateGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingImmediateInstruction)
    ~DataProcessingImmediateInstruction() = default;
};
template <>
struct DataProcessingImmediateInstruction< DataProcessingImmediateA64::InstructionGroupEnum::MOVE_WIDE_IMMEDIATE > final : public DataProcessingImmediateGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DataProcessingImmediateA64::getInstructionType< DataProcessingImmediateA64::InstructionGroupEnum::MOVE_WIDE_IMMEDIATE >(mInstruction);
    }

    [[nodiscard]] inline auto get_Rd() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 0));
    }
    [[nodiscard]] inline auto get_imm16() const noexcept {
        return static_cast<std::uint16_t>(static_cast<std::uint32_t>(0xFFFF) & (mInstruction >> 5));
    }
    [[nodiscard]] inline auto get_hw() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11) & (mInstruction >> 21));
    }

    explicit DataProcessingImmediateInstruction(const DataProcessingImmediateGroup& instruction) noexcept : DataProcessingImmediateGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingImmediateInstruction)
    ~DataProcessingImmediateInstruction() = default;
};
template <>
struct DataProcessingImmediateInstruction< DataProcessingImmediateA64::InstructionGroupEnum::BITFIELD > final : public DataProcessingImmediateGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DataProcessingImmediateA64::getInstructionType< DataProcessingImmediateA64::InstructionGroupEnum::BITFIELD >(mInstruction);
    }

    [[nodiscard]] inline auto get_Rd() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 0));
    }
    [[nodiscard]] inline auto get_Rn() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 5));
    }
    [[nodiscard]] inline auto get_imms() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b111111) & (mInstruction >> 10));
    }
    [[nodiscard]] inline auto get_immr() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b111111) & (mInstruction >> 16));
    }
    [[nodiscard]] inline auto get_N() const noexcept {
        return readBit(22);
    }
    
    explicit DataProcessingImmediateInstruction(const DataProcessingImmediateGroup& instruction) noexcept : DataProcessingImmediateGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingImmediateInstruction)
    ~DataProcessingImmediateInstruction() = default;
};
template <>
struct DataProcessingImmediateInstruction< DataProcessingImmediateA64::InstructionGroupEnum::EXTRACT > final : public DataProcessingImmediateGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DataProcessingImmediateA64::getInstructionType< DataProcessingImmediateA64::InstructionGroupEnum::EXTRACT >(mInstruction);
    }

    explicit DataProcessingImmediateInstruction(const DataProcessingImmediateGroup& instruction) noexcept : DataProcessingImmediateGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingImmediateInstruction)
    ~DataProcessingImmediateInstruction() = default;
};

} // namespace arm_emu
