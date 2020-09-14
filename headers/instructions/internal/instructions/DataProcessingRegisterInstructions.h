#pragma once
#include "BaseInstruction.hpp"

namespace arm_emu {
/*
Data Processing Register
*/
struct DataProcessingRegisterGroup : public AbstractInstruction< std::uint32_t, DataProcessingRegisterGroup > {
    [[nodiscard]] constexpr auto getInstructionClass() const noexcept { return DataProcessingRegisterA64::getInstructionClass(mInstruction); }

    constexpr explicit DataProcessingRegisterGroup(BaseInstruction instruction) noexcept : AbstractInstruction(instruction.data()) {}
    constexpr explicit DataProcessingRegisterGroup(const DataProcessingRegisterGroup& instruction) noexcept : AbstractInstruction(instruction.mInstruction) {};
    constexpr explicit DataProcessingRegisterGroup(DataProcessingRegisterGroup&& instruction) noexcept : AbstractInstruction(instruction.mInstruction) {};
    NULL_ASSINGMENT(DataProcessingRegisterGroup)
    ~DataProcessingRegisterGroup() = default;
};

template < DataProcessingRegisterA64::InstructionGroupEnum >
struct DataProcessingRegisterInstruction;

template <>
struct DataProcessingRegisterInstruction< DataProcessingRegisterA64::InstructionGroupEnum::DATA_PROCESSING_TWO_SOURCE > final : public DataProcessingRegisterGroup {
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        return DataProcessingRegisterA64::getInstructionType< DataProcessingRegisterA64::InstructionGroupEnum::DATA_PROCESSING_TWO_SOURCE >(mInstruction);
    }

    constexpr explicit DataProcessingRegisterInstruction(const DataProcessingRegisterGroup& instruction) noexcept : DataProcessingRegisterGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingRegisterInstruction)
    ~DataProcessingRegisterInstruction() = default;
};
template <>
struct DataProcessingRegisterInstruction< DataProcessingRegisterA64::InstructionGroupEnum::DATA_PROCESSING_ONE_SOURCE > final : public DataProcessingRegisterGroup {
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        return DataProcessingRegisterA64::getInstructionType< DataProcessingRegisterA64::InstructionGroupEnum::DATA_PROCESSING_ONE_SOURCE >(mInstruction);
    }

    constexpr explicit DataProcessingRegisterInstruction(const DataProcessingRegisterGroup& instruction) noexcept : DataProcessingRegisterGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingRegisterInstruction)
    ~DataProcessingRegisterInstruction() = default;
};
template <>
struct DataProcessingRegisterInstruction< DataProcessingRegisterA64::InstructionGroupEnum::LOGICAL_SHIFTED_REGISTER > final : public DataProcessingRegisterGroup {
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        return DataProcessingRegisterA64::getInstructionType< DataProcessingRegisterA64::InstructionGroupEnum::LOGICAL_SHIFTED_REGISTER >(mInstruction);
    }

    [[nodiscard]] constexpr auto get_Rd() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 0));
    }
    [[nodiscard]] constexpr auto get_Rn() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 5));
    }
    [[nodiscard]] constexpr auto get_Rm() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 16));
    }
    [[nodiscard]] constexpr auto get_imm6() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b111111) & (mInstruction >> 10));
    }
    [[nodiscard]] constexpr auto get_shift() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11) & (mInstruction >> 22));
    }
    [[nodiscard]] constexpr auto get_N() const noexcept {
        return readBit(21);
    }

    constexpr explicit DataProcessingRegisterInstruction(const DataProcessingRegisterGroup& instruction) noexcept : DataProcessingRegisterGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingRegisterInstruction)
    ~DataProcessingRegisterInstruction() = default;
};
template <>
struct DataProcessingRegisterInstruction< DataProcessingRegisterA64::InstructionGroupEnum::ADD_SUBTRACT_SHIFTED_REGISTER > final : public DataProcessingRegisterGroup {
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        return DataProcessingRegisterA64::getInstructionType< DataProcessingRegisterA64::InstructionGroupEnum::ADD_SUBTRACT_SHIFTED_REGISTER >(
            mInstruction);
    }

    [[nodiscard]] constexpr auto get_Rd() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 0));
    }
    [[nodiscard]] constexpr auto get_Rn() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 5));
    }
    [[nodiscard]] constexpr auto get_Rm() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 16));
    }
    [[nodiscard]] constexpr auto get_imm6() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b111111) & (mInstruction >> 10));
    }
    [[nodiscard]] constexpr auto get_shift() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11) & (mInstruction >> 22));
    }

    constexpr explicit DataProcessingRegisterInstruction(const DataProcessingRegisterGroup& instruction) noexcept : DataProcessingRegisterGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingRegisterInstruction)
    ~DataProcessingRegisterInstruction() = default;
};
template <>
struct DataProcessingRegisterInstruction< DataProcessingRegisterA64::InstructionGroupEnum::ADD_SUBTRACT_EXTENDED_REGISTER > final : public DataProcessingRegisterGroup {
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        return DataProcessingRegisterA64::getInstructionType< DataProcessingRegisterA64::InstructionGroupEnum::ADD_SUBTRACT_EXTENDED_REGISTER >(
            mInstruction);
    }

    [[nodiscard]] constexpr auto get_Rd() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 0));
    }
    [[nodiscard]] constexpr auto get_Rn() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 5));
    }
    [[nodiscard]] constexpr auto get_Rm() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 16));
    }
    [[nodiscard]] constexpr auto get_imm3() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b111) & (mInstruction >> 10));
    }
    [[nodiscard]] constexpr auto get_option() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b111) & (mInstruction >> 13));
    }

    constexpr explicit DataProcessingRegisterInstruction(const DataProcessingRegisterGroup& instruction) noexcept : DataProcessingRegisterGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingRegisterInstruction)
    ~DataProcessingRegisterInstruction() = default;
};
template <>
struct DataProcessingRegisterInstruction< DataProcessingRegisterA64::InstructionGroupEnum::ADD_SUBTRACT_CARRY > final : public DataProcessingRegisterGroup {
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        return DataProcessingRegisterA64::getInstructionType< DataProcessingRegisterA64::InstructionGroupEnum::ADD_SUBTRACT_CARRY >(mInstruction);
    }

    [[nodiscard]] constexpr auto get_Rd() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 0));
    }
    [[nodiscard]] constexpr auto get_Rn() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 5));
    }
    [[nodiscard]] constexpr auto get_Rm() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 16));
    }

    constexpr explicit DataProcessingRegisterInstruction(const DataProcessingRegisterGroup& instruction) noexcept : DataProcessingRegisterGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingRegisterInstruction)
    ~DataProcessingRegisterInstruction() = default;
};
template <>
struct DataProcessingRegisterInstruction< DataProcessingRegisterA64::InstructionGroupEnum::ROTATE_RIGHT_INTO_FLAGS > final : public DataProcessingRegisterGroup {
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        return DataProcessingRegisterA64::getInstructionType< DataProcessingRegisterA64::InstructionGroupEnum::ROTATE_RIGHT_INTO_FLAGS >(mInstruction);
    }

    constexpr explicit DataProcessingRegisterInstruction(const DataProcessingRegisterGroup& instruction) noexcept : DataProcessingRegisterGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingRegisterInstruction)
    ~DataProcessingRegisterInstruction() = default;
};
template <>
struct DataProcessingRegisterInstruction< DataProcessingRegisterA64::InstructionGroupEnum::EVALUATE_INTO_FLAGS > final : public DataProcessingRegisterGroup {
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        return DataProcessingRegisterA64::getInstructionType< DataProcessingRegisterA64::InstructionGroupEnum::EVALUATE_INTO_FLAGS >(mInstruction);
    }

    constexpr explicit DataProcessingRegisterInstruction(const DataProcessingRegisterGroup& instruction) noexcept : DataProcessingRegisterGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingRegisterInstruction)
    ~DataProcessingRegisterInstruction() = default;
};
template <>
struct DataProcessingRegisterInstruction< DataProcessingRegisterA64::InstructionGroupEnum::CONDITIONAL_COMPARE_REGISTER > final : public DataProcessingRegisterGroup {
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        return DataProcessingRegisterA64::getInstructionType< DataProcessingRegisterA64::InstructionGroupEnum::CONDITIONAL_COMPARE_REGISTER >(mInstruction);
    }

    constexpr explicit DataProcessingRegisterInstruction(const DataProcessingRegisterGroup& instruction) noexcept : DataProcessingRegisterGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingRegisterInstruction)
    ~DataProcessingRegisterInstruction() = default;
};
template <>
struct DataProcessingRegisterInstruction< DataProcessingRegisterA64::InstructionGroupEnum::CONDITIONAL_COMPARE_IMMEDIATE > final : public DataProcessingRegisterGroup {
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        return DataProcessingRegisterA64::getInstructionType< DataProcessingRegisterA64::InstructionGroupEnum::CONDITIONAL_COMPARE_IMMEDIATE >(
            mInstruction);
    }

    constexpr explicit DataProcessingRegisterInstruction(const DataProcessingRegisterGroup& instruction) noexcept : DataProcessingRegisterGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingRegisterInstruction)
    ~DataProcessingRegisterInstruction() = default;
};
template <>
struct DataProcessingRegisterInstruction< DataProcessingRegisterA64::InstructionGroupEnum::CONDITIONAL_SELECT > final : public DataProcessingRegisterGroup {
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        return DataProcessingRegisterA64::getInstructionType< DataProcessingRegisterA64::InstructionGroupEnum::CONDITIONAL_SELECT >(mInstruction);
    }

    [[nodiscard]] constexpr auto get_Rd() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 0));
    }
    [[nodiscard]] constexpr auto get_Rn() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 5));
    }
    [[nodiscard]] constexpr auto get_Rm() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b11111) & (mInstruction >> 16));
    }
    [[nodiscard]] constexpr auto get_cond() const noexcept {
        return static_cast<std::uint8_t>(static_cast<std::uint32_t>(0b1111) & (mInstruction >> 12));
    }

    constexpr explicit DataProcessingRegisterInstruction(const DataProcessingRegisterGroup& instruction) noexcept : DataProcessingRegisterGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingRegisterInstruction)
    ~DataProcessingRegisterInstruction() = default;
};
template <>
struct DataProcessingRegisterInstruction< DataProcessingRegisterA64::InstructionGroupEnum::DATA_PROCESSING_THREE_SOURCE > final : public DataProcessingRegisterGroup {
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        return DataProcessingRegisterA64::getInstructionType< DataProcessingRegisterA64::InstructionGroupEnum::DATA_PROCESSING_THREE_SOURCE >(mInstruction);
    }

    constexpr explicit DataProcessingRegisterInstruction(const DataProcessingRegisterGroup& instruction) noexcept : DataProcessingRegisterGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessingRegisterInstruction)
    ~DataProcessingRegisterInstruction() = default;
};
} // namespace arm_emu