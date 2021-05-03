#pragma once
#include "BaseInstruction.hpp"

namespace arm_emu {
/*
Data Processing Register
*/
struct DataProcessingRegisterGroup : public AbstractInstruction< std::uint32_t, DataProcessingRegisterGroup > {
    [[nodiscard]] inline auto getInstructionClass() const noexcept { return DataProcessingRegisterA64::getInstructionClass(mInstruction); }

    explicit DataProcessingRegisterGroup(BaseInstruction instruction) noexcept : AbstractInstruction(instruction.data()) {}
    explicit DataProcessingRegisterGroup(const DataProcessingRegisterGroup& instruction) noexcept : AbstractInstruction(instruction.mInstruction) {};
    explicit DataProcessingRegisterGroup(DataProcessingRegisterGroup&& instruction) noexcept : AbstractInstruction(instruction.mInstruction) {};
    NULL_ASSINGMENT(DataProcessingRegisterGroup)
    ~DataProcessingRegisterGroup() = default;
};

template < DataProcessingRegisterA64::InstructionGroupEnum >
struct DataProcessingRegisterInstruction;

template <>
struct DataProcessingRegisterInstruction< DataProcessingRegisterA64::InstructionGroupEnum::DATA_PROCESSING_TWO_SOURCE > final : public DataProcessingRegisterGroup {
    [[nodiscard]] inline auto getInstructionType() const noexcept {
        return DataProcessingRegisterA64::getInstructionType< DataProcessingRegisterA64::InstructionGroupEnum::DATA_PROCESSING_TWO_SOURCE >(mInstruction);
    }

    [[nodiscard]] inline auto get_Rd() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 0));
    }
    [[nodiscard]] inline auto get_Rn() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 5));
    }
    [[nodiscard]] inline auto get_Rm() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 16));
    }
    [[nodiscard]] inline auto get_op2() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11) & (mInstruction >> 10));
    }

    explicit DataProcessingRegisterInstruction(const DataProcessingRegisterGroup& instruction) noexcept : DataProcessingRegisterGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingRegisterInstruction)
    ~DataProcessingRegisterInstruction() = default;
};
template <>
struct DataProcessingRegisterInstruction< DataProcessingRegisterA64::InstructionGroupEnum::DATA_PROCESSING_ONE_SOURCE > final : public DataProcessingRegisterGroup {
    [[nodiscard]] inline auto getInstructionType() const noexcept {
        return DataProcessingRegisterA64::getInstructionType< DataProcessingRegisterA64::InstructionGroupEnum::DATA_PROCESSING_ONE_SOURCE >(mInstruction);
    }

    [[nodiscard]] inline auto get_Rd() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 0));
    }
    [[nodiscard]] inline auto get_Rn() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 5));
    }
    [[nodiscard]] inline auto get_Z() const noexcept {
        return readBit(13);
    }

    explicit DataProcessingRegisterInstruction(const DataProcessingRegisterGroup& instruction) noexcept : DataProcessingRegisterGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingRegisterInstruction)
    ~DataProcessingRegisterInstruction() = default;
};
template <>
struct DataProcessingRegisterInstruction< DataProcessingRegisterA64::InstructionGroupEnum::LOGICAL_SHIFTED_REGISTER > final : public DataProcessingRegisterGroup {
    [[nodiscard]] inline auto getInstructionType() const noexcept {
        return DataProcessingRegisterA64::getInstructionType< DataProcessingRegisterA64::InstructionGroupEnum::LOGICAL_SHIFTED_REGISTER >(mInstruction);
    }

    [[nodiscard]] inline auto get_Rd() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 0));
    }
    [[nodiscard]] inline auto get_Rn() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 5));
    }
    [[nodiscard]] inline auto get_Rm() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 16));
    }
    [[nodiscard]] inline auto get_imm6() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b111111) & (mInstruction >> 10));
    }
    [[nodiscard]] inline auto get_shift() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11) & (mInstruction >> 22));
    }
    [[nodiscard]] inline auto get_N() const noexcept {
        return readBit(21);
    }

    explicit DataProcessingRegisterInstruction(const DataProcessingRegisterGroup& instruction) noexcept : DataProcessingRegisterGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingRegisterInstruction)
    ~DataProcessingRegisterInstruction() = default;
};
template <>
struct DataProcessingRegisterInstruction< DataProcessingRegisterA64::InstructionGroupEnum::ADD_SUBTRACT_SHIFTED_REGISTER > final : public DataProcessingRegisterGroup {
    [[nodiscard]] inline auto getInstructionType() const noexcept {
        return DataProcessingRegisterA64::getInstructionType< DataProcessingRegisterA64::InstructionGroupEnum::ADD_SUBTRACT_SHIFTED_REGISTER >(
            mInstruction);
    }

    [[nodiscard]] inline auto get_Rd() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 0));
    }
    [[nodiscard]] inline auto get_Rn() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 5));
    }
    [[nodiscard]] inline auto get_Rm() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 16));
    }
    [[nodiscard]] inline auto get_imm6() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b111111) & (mInstruction >> 10));
    }
    [[nodiscard]] inline auto get_shift() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11) & (mInstruction >> 22));
    }

    explicit DataProcessingRegisterInstruction(const DataProcessingRegisterGroup& instruction) noexcept : DataProcessingRegisterGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingRegisterInstruction)
    ~DataProcessingRegisterInstruction() = default;
};
template <>
struct DataProcessingRegisterInstruction< DataProcessingRegisterA64::InstructionGroupEnum::ADD_SUBTRACT_EXTENDED_REGISTER > final : public DataProcessingRegisterGroup {
    [[nodiscard]] inline auto getInstructionType() const noexcept {
        return DataProcessingRegisterA64::getInstructionType< DataProcessingRegisterA64::InstructionGroupEnum::ADD_SUBTRACT_EXTENDED_REGISTER >(
            mInstruction);
    }

    [[nodiscard]] inline auto get_Rd() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 0));
    }
    [[nodiscard]] inline auto get_Rn() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 5));
    }
    [[nodiscard]] inline auto get_Rm() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 16));
    }
    [[nodiscard]] inline auto get_imm3() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b111) & (mInstruction >> 10));
    }
    [[nodiscard]] inline auto get_option() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b111) & (mInstruction >> 13));
    }

    explicit DataProcessingRegisterInstruction(const DataProcessingRegisterGroup& instruction) noexcept : DataProcessingRegisterGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingRegisterInstruction)
    ~DataProcessingRegisterInstruction() = default;
};
template <>
struct DataProcessingRegisterInstruction< DataProcessingRegisterA64::InstructionGroupEnum::ADD_SUBTRACT_CARRY > final : public DataProcessingRegisterGroup {
    [[nodiscard]] inline auto getInstructionType() const noexcept {
        return DataProcessingRegisterA64::getInstructionType< DataProcessingRegisterA64::InstructionGroupEnum::ADD_SUBTRACT_CARRY >(mInstruction);
    }

    [[nodiscard]] inline auto get_Rd() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 0));
    }
    [[nodiscard]] inline auto get_Rn() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 5));
    }
    [[nodiscard]] inline auto get_Rm() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 16));
    }

    explicit DataProcessingRegisterInstruction(const DataProcessingRegisterGroup& instruction) noexcept : DataProcessingRegisterGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingRegisterInstruction)
    ~DataProcessingRegisterInstruction() = default;
};
template <>
struct DataProcessingRegisterInstruction< DataProcessingRegisterA64::InstructionGroupEnum::ROTATE_RIGHT_INTO_FLAGS > final : public DataProcessingRegisterGroup {
    [[nodiscard]] inline auto getInstructionType() const noexcept {
        return DataProcessingRegisterA64::getInstructionType< DataProcessingRegisterA64::InstructionGroupEnum::ROTATE_RIGHT_INTO_FLAGS >(mInstruction);
    }

    explicit DataProcessingRegisterInstruction(const DataProcessingRegisterGroup& instruction) noexcept : DataProcessingRegisterGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingRegisterInstruction)
    ~DataProcessingRegisterInstruction() = default;
};
template <>
struct DataProcessingRegisterInstruction< DataProcessingRegisterA64::InstructionGroupEnum::EVALUATE_INTO_FLAGS > final : public DataProcessingRegisterGroup {
    [[nodiscard]] inline auto getInstructionType() const noexcept {
        return DataProcessingRegisterA64::getInstructionType< DataProcessingRegisterA64::InstructionGroupEnum::EVALUATE_INTO_FLAGS >(mInstruction);
    }

    explicit DataProcessingRegisterInstruction(const DataProcessingRegisterGroup& instruction) noexcept : DataProcessingRegisterGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingRegisterInstruction)
    ~DataProcessingRegisterInstruction() = default;
};
template <>
struct DataProcessingRegisterInstruction< DataProcessingRegisterA64::InstructionGroupEnum::CONDITIONAL_COMPARE_REGISTER > final : public DataProcessingRegisterGroup {
    [[nodiscard]] inline auto getInstructionType() const noexcept {
        return DataProcessingRegisterA64::getInstructionType< DataProcessingRegisterA64::InstructionGroupEnum::CONDITIONAL_COMPARE_REGISTER >(mInstruction);
    }

    [[nodiscard]] inline auto get_nzcv() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b1111) & (mInstruction >> 0));
    }
    [[nodiscard]] inline auto get_Rn() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 5));
    }
    [[nodiscard]] inline auto get_cond() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b1111) & (mInstruction >> 12));
    }
    [[nodiscard]] inline auto get_Rm() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 16));
    }

    explicit DataProcessingRegisterInstruction(const DataProcessingRegisterGroup& instruction) noexcept : DataProcessingRegisterGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingRegisterInstruction)
    ~DataProcessingRegisterInstruction() = default;
};
template <>
struct DataProcessingRegisterInstruction< DataProcessingRegisterA64::InstructionGroupEnum::CONDITIONAL_COMPARE_IMMEDIATE > final : public DataProcessingRegisterGroup {
    [[nodiscard]] inline auto getInstructionType() const noexcept {
        return DataProcessingRegisterA64::getInstructionType< DataProcessingRegisterA64::InstructionGroupEnum::CONDITIONAL_COMPARE_IMMEDIATE >(
            mInstruction);
    }

    [[nodiscard]] auto get_nzcv() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b1111) & (mInstruction >> 0));
    }
    [[nodiscard]] auto get_Rn() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 5));
    }
    [[nodiscard]] auto get_cond() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b1111) & (mInstruction >> 12));
    }
    [[nodiscard]] auto get_imm5() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 16));
    }

    explicit DataProcessingRegisterInstruction(const DataProcessingRegisterGroup& instruction) noexcept : DataProcessingRegisterGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingRegisterInstruction)
    ~DataProcessingRegisterInstruction() = default;
};
template <>
struct DataProcessingRegisterInstruction< DataProcessingRegisterA64::InstructionGroupEnum::CONDITIONAL_SELECT > final : public DataProcessingRegisterGroup {
    [[nodiscard]] inline auto getInstructionType() const noexcept {
        return DataProcessingRegisterA64::getInstructionType< DataProcessingRegisterA64::InstructionGroupEnum::CONDITIONAL_SELECT >(mInstruction);
    }

    [[nodiscard]] inline auto get_Rd() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 0));
    }
    [[nodiscard]] inline auto get_Rn() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 5));
    }
    [[nodiscard]] inline auto get_Rm() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 16));
    }
    [[nodiscard]] inline auto get_cond() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b1111) & (mInstruction >> 12));
    }

    explicit DataProcessingRegisterInstruction(const DataProcessingRegisterGroup& instruction) noexcept : DataProcessingRegisterGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingRegisterInstruction)
    ~DataProcessingRegisterInstruction() = default;
};
template <>
struct DataProcessingRegisterInstruction< DataProcessingRegisterA64::InstructionGroupEnum::DATA_PROCESSING_THREE_SOURCE > final : public DataProcessingRegisterGroup {
    [[nodiscard]] inline auto getInstructionType() const noexcept {
        return DataProcessingRegisterA64::getInstructionType< DataProcessingRegisterA64::InstructionGroupEnum::DATA_PROCESSING_THREE_SOURCE >(mInstruction);
    }

    explicit DataProcessingRegisterInstruction(const DataProcessingRegisterGroup& instruction) noexcept : DataProcessingRegisterGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingRegisterInstruction)
    ~DataProcessingRegisterInstruction() = default;
};
} // namespace arm_emu