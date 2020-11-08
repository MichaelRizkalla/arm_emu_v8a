#pragma once
#include "BaseInstruction.hpp"

namespace arm_emu {

/*
Branch Exception System Instruction Group
*/
struct Branch_Exception_SystemGroup : public AbstractInstruction< std::uint32_t, Branch_Exception_SystemGroup > {
    [[nodiscard]] constexpr auto getInstructionClass() const noexcept { return Branch_Exception_SystemA64::getInstructionClass(mInstruction); }

    constexpr explicit Branch_Exception_SystemGroup(BaseInstruction instruction) noexcept : AbstractInstruction(instruction.data()) {}
    constexpr explicit Branch_Exception_SystemGroup(const Branch_Exception_SystemGroup& instruction) noexcept : AbstractInstruction(instruction.mInstruction) {};
    constexpr explicit Branch_Exception_SystemGroup(Branch_Exception_SystemGroup&& instruction) noexcept : AbstractInstruction(instruction.mInstruction) {};
    NULL_ASSINGMENT(Branch_Exception_SystemGroup)
    ~Branch_Exception_SystemGroup() = default;
};

template < Branch_Exception_SystemA64::InstructionGroupEnum >
struct Branch_Exception_SystemInstruction;

template <>
struct Branch_Exception_SystemInstruction< Branch_Exception_SystemA64::InstructionGroupEnum::CONDITIONAL_BRANCHING > final : public Branch_Exception_SystemGroup {
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        return Branch_Exception_SystemA64::getInstructionType< Branch_Exception_SystemA64::InstructionGroupEnum::CONDITIONAL_BRANCHING >(mInstruction);
    }

    [[nodiscard]] inline constexpr auto get_cond() const noexcept { return static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b1111) & (mInstruction >> 0)); }
    [[nodiscard]] inline constexpr auto get_imm19() const noexcept { return static_cast< std::uint32_t >(static_cast< std::uint32_t >(0x7FFFF) & (mInstruction >> 5)); }

    constexpr explicit Branch_Exception_SystemInstruction(const Branch_Exception_SystemGroup& instruction) noexcept : Branch_Exception_SystemGroup(instruction) {};
    NULL_COPY_MOVE(Branch_Exception_SystemInstruction)
    ~Branch_Exception_SystemInstruction() = default;
};
template <>
struct Branch_Exception_SystemInstruction< Branch_Exception_SystemA64::InstructionGroupEnum::EXCEPTION_GENERATION > final : public Branch_Exception_SystemGroup {
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        return Branch_Exception_SystemA64::getInstructionType< Branch_Exception_SystemA64::InstructionGroupEnum::EXCEPTION_GENERATION >(mInstruction);
    }

    [[nodiscard]] inline constexpr auto get_imm16() const noexcept { return static_cast< std::uint16_t >(static_cast< std::uint32_t >(0xFFFF) & (mInstruction >> 5)); }

    constexpr explicit Branch_Exception_SystemInstruction(const Branch_Exception_SystemGroup& instruction) noexcept : Branch_Exception_SystemGroup(instruction) {};
    NULL_COPY_MOVE(Branch_Exception_SystemInstruction)
    ~Branch_Exception_SystemInstruction() = default;
};
template <>
struct Branch_Exception_SystemInstruction< Branch_Exception_SystemA64::InstructionGroupEnum::HINTS > final : public Branch_Exception_SystemGroup {
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        return Branch_Exception_SystemA64::getInstructionType< Branch_Exception_SystemA64::InstructionGroupEnum::HINTS >(mInstruction);
    }

    constexpr explicit Branch_Exception_SystemInstruction(const Branch_Exception_SystemGroup& instruction) noexcept : Branch_Exception_SystemGroup(instruction) {};
    NULL_COPY_MOVE(Branch_Exception_SystemInstruction)
    ~Branch_Exception_SystemInstruction() = default;
};
template <>
struct Branch_Exception_SystemInstruction< Branch_Exception_SystemA64::InstructionGroupEnum::BARRIERS > final : public Branch_Exception_SystemGroup {
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        return Branch_Exception_SystemA64::getInstructionType< Branch_Exception_SystemA64::InstructionGroupEnum::BARRIERS >(mInstruction);
    }

    constexpr explicit Branch_Exception_SystemInstruction(const Branch_Exception_SystemGroup& instruction) noexcept : Branch_Exception_SystemGroup(instruction) {};
    NULL_COPY_MOVE(Branch_Exception_SystemInstruction)
    ~Branch_Exception_SystemInstruction() = default;
};
template <>
struct Branch_Exception_SystemInstruction< Branch_Exception_SystemA64::InstructionGroupEnum::PSTATE > final : public Branch_Exception_SystemGroup {
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        return Branch_Exception_SystemA64::getInstructionType< Branch_Exception_SystemA64::InstructionGroupEnum::PSTATE >(mInstruction);
    }

    constexpr explicit Branch_Exception_SystemInstruction(const Branch_Exception_SystemGroup& instruction) noexcept : Branch_Exception_SystemGroup(instruction) {};
    NULL_COPY_MOVE(Branch_Exception_SystemInstruction)
    ~Branch_Exception_SystemInstruction() = default;
};
template <>
struct Branch_Exception_SystemInstruction< Branch_Exception_SystemA64::InstructionGroupEnum::SYSTEM_INSTRUCTIONS > final : public Branch_Exception_SystemGroup {
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        return Branch_Exception_SystemA64::getInstructionType< Branch_Exception_SystemA64::InstructionGroupEnum::SYSTEM_INSTRUCTIONS >(mInstruction);
    }

    [[nodiscard]] constexpr auto get_Rt() const noexcept { return static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b11111) & (mInstruction >> 0)); }
    [[nodiscard]] constexpr auto get_op2() const noexcept { return static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b111) & (mInstruction >> 5)); }
    [[nodiscard]] constexpr auto get_CRm() const noexcept { return static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b1111) & (mInstruction >> 8)); }
    [[nodiscard]] constexpr auto get_CRn() const noexcept { return static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b1111) & (mInstruction >> 12)); }
    [[nodiscard]] constexpr auto get_op1() const noexcept { return static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b111) & (mInstruction >> 16)); }
    [[nodiscard]] constexpr auto get_L() const noexcept { return readBit(21); }

    constexpr explicit Branch_Exception_SystemInstruction(const Branch_Exception_SystemGroup& instruction) noexcept : Branch_Exception_SystemGroup(instruction) {};
    NULL_COPY_MOVE(Branch_Exception_SystemInstruction)
    ~Branch_Exception_SystemInstruction() = default;
};
template <>
struct Branch_Exception_SystemInstruction< Branch_Exception_SystemA64::InstructionGroupEnum::SYSTEM_REGISTER_MOVE > final : public Branch_Exception_SystemGroup {
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        return Branch_Exception_SystemA64::getInstructionType< Branch_Exception_SystemA64::InstructionGroupEnum::SYSTEM_REGISTER_MOVE >(mInstruction);
    }

    constexpr explicit Branch_Exception_SystemInstruction(const Branch_Exception_SystemGroup& instruction) noexcept : Branch_Exception_SystemGroup(instruction) {};
    NULL_COPY_MOVE(Branch_Exception_SystemInstruction)
    ~Branch_Exception_SystemInstruction() = default;
};
template <>
struct Branch_Exception_SystemInstruction< Branch_Exception_SystemA64::InstructionGroupEnum::UNCONDITIONAL_BRANCH_REGISTER > final : public Branch_Exception_SystemGroup {
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        return Branch_Exception_SystemA64::getInstructionType< Branch_Exception_SystemA64::InstructionGroupEnum::UNCONDITIONAL_BRANCH_REGISTER >(mInstruction);
    }

    [[nodiscard]] inline constexpr auto get_Rm() const noexcept { return static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b11111) & (mInstruction >> 0)); }

    [[nodiscard]] inline constexpr auto get_Rn() const noexcept { return static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b11111) & (mInstruction >> 5)); }

    [[nodiscard]] inline constexpr auto get_M() const noexcept { return readBit(10); }

    [[nodiscard]] inline constexpr auto get_A() const noexcept { return readBit(11); }

    [[nodiscard]] inline constexpr auto get_Z() const noexcept { return readBit(24); }

    constexpr explicit Branch_Exception_SystemInstruction(const Branch_Exception_SystemGroup& instruction) noexcept : Branch_Exception_SystemGroup(instruction) {};
    NULL_COPY_MOVE(Branch_Exception_SystemInstruction)
    ~Branch_Exception_SystemInstruction() = default;
};
template <>
struct Branch_Exception_SystemInstruction< Branch_Exception_SystemA64::InstructionGroupEnum::UNCONDITIONAL_BRANCH_IMMEDIATE > final
    : public Branch_Exception_SystemGroup {
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        return Branch_Exception_SystemA64::getInstructionType< Branch_Exception_SystemA64::InstructionGroupEnum::UNCONDITIONAL_BRANCH_IMMEDIATE >(mInstruction);
    }

    [[nodiscard]] inline constexpr auto get_imm26() const noexcept { return static_cast< std::uint32_t >(static_cast< std::uint32_t >(0x3FFFFFF) & (mInstruction >> 0)); }

    constexpr explicit Branch_Exception_SystemInstruction(const Branch_Exception_SystemGroup& instruction) noexcept : Branch_Exception_SystemGroup(instruction) {};
    NULL_COPY_MOVE(Branch_Exception_SystemInstruction)
    ~Branch_Exception_SystemInstruction() = default;
};
template <>
struct Branch_Exception_SystemInstruction< Branch_Exception_SystemA64::InstructionGroupEnum::COMPARE_AND_BRANCH_IMMEDIATE > final : public Branch_Exception_SystemGroup {
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        return Branch_Exception_SystemA64::getInstructionType< Branch_Exception_SystemA64::InstructionGroupEnum::COMPARE_AND_BRANCH_IMMEDIATE >(mInstruction);
    }

    [[nodiscard]] inline constexpr auto get_Rt() const noexcept { return static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b11111) & (mInstruction >> 0)); }

    [[nodiscard]] inline constexpr auto get_imm19() const noexcept { return static_cast< std::uint32_t >(static_cast< std::uint32_t >(0x3FFFF) & (mInstruction >> 5)); }

    constexpr explicit Branch_Exception_SystemInstruction(const Branch_Exception_SystemGroup& instruction) noexcept : Branch_Exception_SystemGroup(instruction) {};
    NULL_COPY_MOVE(Branch_Exception_SystemInstruction)
    ~Branch_Exception_SystemInstruction() = default;
};
template <>
struct Branch_Exception_SystemInstruction< Branch_Exception_SystemA64::InstructionGroupEnum::TEST_AND_BRANCH_IMMEDIATE > final : public Branch_Exception_SystemGroup {
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        return Branch_Exception_SystemA64::getInstructionType< Branch_Exception_SystemA64::InstructionGroupEnum::TEST_AND_BRANCH_IMMEDIATE >(mInstruction);
    }

    constexpr explicit Branch_Exception_SystemInstruction(const Branch_Exception_SystemGroup& instruction) noexcept : Branch_Exception_SystemGroup(instruction) {};
    NULL_COPY_MOVE(Branch_Exception_SystemInstruction)
    ~Branch_Exception_SystemInstruction() = default;
};

} // namespace arm_emu