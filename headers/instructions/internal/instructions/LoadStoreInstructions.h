#pragma once
#include "BaseInstruction.hpp"

namespace arm_emu {

/*
Load Store Group
*/
struct LoadStoreGroup : public AbstractInstruction< std::uint32_t, LoadStoreGroup > {
    [[nodiscard]] auto getInstructionClass() const noexcept { return LoadStoreA64::getInstructionClass(mInstruction); }

    explicit LoadStoreGroup(BaseInstruction instruction) noexcept : AbstractInstruction(instruction.data()) {}
    explicit LoadStoreGroup(const LoadStoreGroup& instruction) noexcept : AbstractInstruction(instruction.mInstruction) {};
    explicit LoadStoreGroup(LoadStoreGroup&& instruction) noexcept : AbstractInstruction(instruction.mInstruction) {};
    NULL_ASSINGMENT(LoadStoreGroup)
    ~LoadStoreGroup() = default;
};

template < LoadStoreA64::InstructionGroupEnum >
struct LoadStoreInstruction;

template <>
struct LoadStoreInstruction< LoadStoreA64::InstructionGroupEnum::ADVANCED_SIMD_LOAD_STORE_MULTIPLE_STRUCTURES > final : public LoadStoreGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return LoadStoreA64::getInstructionType< LoadStoreA64::InstructionGroupEnum::ADVANCED_SIMD_LOAD_STORE_MULTIPLE_STRUCTURES >(mInstruction);
    }

    explicit LoadStoreInstruction(const LoadStoreGroup& instruction) noexcept : LoadStoreGroup(instruction) {};
    NULL_COPY_MOVE(LoadStoreInstruction)
    ~LoadStoreInstruction() = default;
};
template <>
struct LoadStoreInstruction< LoadStoreA64::InstructionGroupEnum::ADVANCED_SIMD_LOAD_STORE_MULTIPLE_STRUCTURES_POST_INDEXED > final : public LoadStoreGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return LoadStoreA64::getInstructionType< LoadStoreA64::InstructionGroupEnum::ADVANCED_SIMD_LOAD_STORE_MULTIPLE_STRUCTURES_POST_INDEXED >(mInstruction);
    }

    explicit LoadStoreInstruction(const LoadStoreGroup& instruction) noexcept : LoadStoreGroup(instruction) {};
    NULL_COPY_MOVE(LoadStoreInstruction)
    ~LoadStoreInstruction() = default;
};
template <>
struct LoadStoreInstruction< LoadStoreA64::InstructionGroupEnum::ADVANCED_SIMD_LOAD_STORE_SINGLE_STRUCTURE > final : public LoadStoreGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return LoadStoreA64::getInstructionType< LoadStoreA64::InstructionGroupEnum::ADVANCED_SIMD_LOAD_STORE_SINGLE_STRUCTURE >(mInstruction);
    }

    explicit LoadStoreInstruction(const LoadStoreGroup& instruction) noexcept : LoadStoreGroup(instruction) {};
    NULL_COPY_MOVE(LoadStoreInstruction)
    ~LoadStoreInstruction() = default;
};
template <>
struct LoadStoreInstruction< LoadStoreA64::InstructionGroupEnum::ADVANCED_SIMD_LOAD_STORE_SINGLE_STRUCTURE_POST_INDEXED > final : public LoadStoreGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return LoadStoreA64::getInstructionType< LoadStoreA64::InstructionGroupEnum::ADVANCED_SIMD_LOAD_STORE_SINGLE_STRUCTURE_POST_INDEXED >(mInstruction);
    }

    explicit LoadStoreInstruction(const LoadStoreGroup& instruction) noexcept : LoadStoreGroup(instruction) {};
    NULL_COPY_MOVE(LoadStoreInstruction)
    ~LoadStoreInstruction() = default;
};
template <>
struct LoadStoreInstruction< LoadStoreA64::InstructionGroupEnum::LOAD_STORE_MEMORY_TAG > final : public LoadStoreGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return LoadStoreA64::getInstructionType< LoadStoreA64::InstructionGroupEnum::LOAD_STORE_MEMORY_TAG >(mInstruction);
    }

    explicit LoadStoreInstruction(const LoadStoreGroup& instruction) noexcept : LoadStoreGroup(instruction) {};
    NULL_COPY_MOVE(LoadStoreInstruction)
    ~LoadStoreInstruction() = default;
};
template <>
struct LoadStoreInstruction< LoadStoreA64::InstructionGroupEnum::LOAD_STORE_EXCLUSIVE > final : public LoadStoreGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return LoadStoreA64::getInstructionType< LoadStoreA64::InstructionGroupEnum::LOAD_STORE_EXCLUSIVE >(mInstruction);
    }

    explicit LoadStoreInstruction(const LoadStoreGroup& instruction) noexcept : LoadStoreGroup(instruction) {};
    NULL_COPY_MOVE(LoadStoreInstruction)
    ~LoadStoreInstruction() = default;
};
template <>
struct LoadStoreInstruction< LoadStoreA64::InstructionGroupEnum::LDAPR_STLR_UNSCALED_IMMEDIATE > final : public LoadStoreGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return LoadStoreA64::getInstructionType< LoadStoreA64::InstructionGroupEnum::LDAPR_STLR_UNSCALED_IMMEDIATE >(mInstruction);
    }

    explicit LoadStoreInstruction(const LoadStoreGroup& instruction) noexcept : LoadStoreGroup(instruction) {};
    NULL_COPY_MOVE(LoadStoreInstruction)
    ~LoadStoreInstruction() = default;
};
template <>
struct LoadStoreInstruction< LoadStoreA64::InstructionGroupEnum::LOAD_REGISTER_LITERAL > final : public LoadStoreGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return LoadStoreA64::getInstructionType< LoadStoreA64::InstructionGroupEnum::LOAD_REGISTER_LITERAL >(mInstruction);
    }

    explicit LoadStoreInstruction(const LoadStoreGroup& instruction) noexcept : LoadStoreGroup(instruction) {};
    NULL_COPY_MOVE(LoadStoreInstruction)
    ~LoadStoreInstruction() = default;
};
template <>
struct LoadStoreInstruction< LoadStoreA64::InstructionGroupEnum::LOAD_STORE_NO_ALLOCATE_PAIR_OFFSET > final : public LoadStoreGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return LoadStoreA64::getInstructionType< LoadStoreA64::InstructionGroupEnum::LOAD_STORE_NO_ALLOCATE_PAIR_OFFSET >(mInstruction);
    }

    explicit LoadStoreInstruction(const LoadStoreGroup& instruction) noexcept : LoadStoreGroup(instruction) {};
    NULL_COPY_MOVE(LoadStoreInstruction)
    ~LoadStoreInstruction() = default;
};
template <>
struct LoadStoreInstruction< LoadStoreA64::InstructionGroupEnum::LOAD_STORE_REGISTER_PAIR_POST_INDEXED > final : public LoadStoreGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return LoadStoreA64::getInstructionType< LoadStoreA64::InstructionGroupEnum::LOAD_STORE_REGISTER_PAIR_POST_INDEXED >(mInstruction);
    }

    explicit LoadStoreInstruction(const LoadStoreGroup& instruction) noexcept : LoadStoreGroup(instruction) {};
    NULL_COPY_MOVE(LoadStoreInstruction)
    ~LoadStoreInstruction() = default;
};
template <>
struct LoadStoreInstruction< LoadStoreA64::InstructionGroupEnum::LOAD_STORE_REGISTER_PAIR_OFFSET > final : public LoadStoreGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return LoadStoreA64::getInstructionType< LoadStoreA64::InstructionGroupEnum::LOAD_STORE_REGISTER_PAIR_OFFSET >(mInstruction);
    }

    explicit LoadStoreInstruction(const LoadStoreGroup& instruction) noexcept : LoadStoreGroup(instruction) {};
    NULL_COPY_MOVE(LoadStoreInstruction)
    ~LoadStoreInstruction() = default;
};
template <>
struct LoadStoreInstruction< LoadStoreA64::InstructionGroupEnum::LOAD_STORE_REGISTER_PAIR_PRE_INDEXED > final : public LoadStoreGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return LoadStoreA64::getInstructionType< LoadStoreA64::InstructionGroupEnum::LOAD_STORE_REGISTER_PAIR_PRE_INDEXED >(mInstruction);
    }

    explicit LoadStoreInstruction(const LoadStoreGroup& instruction) noexcept : LoadStoreGroup(instruction) {};
    NULL_COPY_MOVE(LoadStoreInstruction)
    ~LoadStoreInstruction() = default;
};
template <>
struct LoadStoreInstruction< LoadStoreA64::InstructionGroupEnum::LOAD_STORE_REGISTER_UNSCALED_IMMEDIATE > final : public LoadStoreGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return LoadStoreA64::getInstructionType< LoadStoreA64::InstructionGroupEnum::LOAD_STORE_REGISTER_UNSCALED_IMMEDIATE >(mInstruction);
    }

    explicit LoadStoreInstruction(const LoadStoreGroup& instruction) noexcept : LoadStoreGroup(instruction) {};
    NULL_COPY_MOVE(LoadStoreInstruction)
    ~LoadStoreInstruction() = default;
};
template <>
struct LoadStoreInstruction< LoadStoreA64::InstructionGroupEnum::LOAD_STORE_REGISTER_IMMEDIATE_POST_INDEXED > final : public LoadStoreGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return LoadStoreA64::getInstructionType< LoadStoreA64::InstructionGroupEnum::LOAD_STORE_REGISTER_IMMEDIATE_POST_INDEXED >(mInstruction);
    }

    [[nodiscard]] auto get_Rt() const noexcept { return static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b11111) & (mInstruction >> 0)); }
    [[nodiscard]] auto get_Rn() const noexcept { return static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b11111) & (mInstruction >> 5)); }
    [[nodiscard]] auto get_imm9() const noexcept { return static_cast< std::uint16_t >(static_cast< std::uint32_t >(0x1FF) & (mInstruction >> 12)); }
    [[nodiscard]] auto get_size() const noexcept { return static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b11) & (mInstruction >> 30)); }

    explicit LoadStoreInstruction(const LoadStoreGroup& instruction) noexcept : LoadStoreGroup(instruction) {};
    NULL_COPY_MOVE(LoadStoreInstruction)
    ~LoadStoreInstruction() = default;
};
template <>
struct LoadStoreInstruction< LoadStoreA64::InstructionGroupEnum::LOAD_STORE_REGISTER_UNPRIVILEGED > final : public LoadStoreGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return LoadStoreA64::getInstructionType< LoadStoreA64::InstructionGroupEnum::LOAD_STORE_REGISTER_UNPRIVILEGED >(mInstruction);
    }

    explicit LoadStoreInstruction(const LoadStoreGroup& instruction) noexcept : LoadStoreGroup(instruction) {};
    NULL_COPY_MOVE(LoadStoreInstruction)
    ~LoadStoreInstruction() = default;
};
template <>
struct LoadStoreInstruction< LoadStoreA64::InstructionGroupEnum::LOAD_STORE_REGISTER_IMMEDIATE_PRE_INDEXED > final : public LoadStoreGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return LoadStoreA64::getInstructionType< LoadStoreA64::InstructionGroupEnum::LOAD_STORE_REGISTER_IMMEDIATE_PRE_INDEXED >(mInstruction);
    }

    explicit LoadStoreInstruction(const LoadStoreGroup& instruction) noexcept : LoadStoreGroup(instruction) {};
    NULL_COPY_MOVE(LoadStoreInstruction)
    ~LoadStoreInstruction() = default;
};
template <>
struct LoadStoreInstruction< LoadStoreA64::InstructionGroupEnum::ATOMIC_MEMORY_OPERATION > final : public LoadStoreGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return LoadStoreA64::getInstructionType< LoadStoreA64::InstructionGroupEnum::ATOMIC_MEMORY_OPERATION >(mInstruction);
    }

    explicit LoadStoreInstruction(const LoadStoreGroup& instruction) noexcept : LoadStoreGroup(instruction) {};
    NULL_COPY_MOVE(LoadStoreInstruction)
    ~LoadStoreInstruction() = default;
};
template <>
struct LoadStoreInstruction< LoadStoreA64::InstructionGroupEnum::LOAD_STORE_REGISTER_REGISTER_OFFSET > final : public LoadStoreGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return LoadStoreA64::getInstructionType< LoadStoreA64::InstructionGroupEnum::LOAD_STORE_REGISTER_REGISTER_OFFSET >(mInstruction);
    }

    explicit LoadStoreInstruction(const LoadStoreGroup& instruction) noexcept : LoadStoreGroup(instruction) {};
    NULL_COPY_MOVE(LoadStoreInstruction)
    ~LoadStoreInstruction() = default;
};
template <>
struct LoadStoreInstruction< LoadStoreA64::InstructionGroupEnum::LOAD_STORE_REGISTER_PAC > final : public LoadStoreGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return LoadStoreA64::getInstructionType< LoadStoreA64::InstructionGroupEnum::LOAD_STORE_REGISTER_PAC >(mInstruction);
    }

    explicit LoadStoreInstruction(const LoadStoreGroup& instruction) noexcept : LoadStoreGroup(instruction) {};
    NULL_COPY_MOVE(LoadStoreInstruction)
    ~LoadStoreInstruction() = default;
};
template <>
struct LoadStoreInstruction< LoadStoreA64::InstructionGroupEnum::LOAD_STORE_REGISTER_UNSIGNED_IMMEDIATE > final : public LoadStoreGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return LoadStoreA64::getInstructionType< LoadStoreA64::InstructionGroupEnum::LOAD_STORE_REGISTER_UNSIGNED_IMMEDIATE >(mInstruction);
    }

    [[nodiscard]] auto get_Rt() const noexcept { return static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b11111) & (mInstruction >> 0)); }
    [[nodiscard]] auto get_Rn() const noexcept { return static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b11111) & (mInstruction >> 5)); }
    [[nodiscard]] auto get_imm12() const noexcept { return static_cast< std::uint16_t >(static_cast< std::uint32_t >(0xFFF) & (mInstruction >> 10)); }
    [[nodiscard]] auto get_size() const noexcept { return static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b11) & (mInstruction >> 30)); }

    explicit LoadStoreInstruction(const LoadStoreGroup& instruction) noexcept : LoadStoreGroup(instruction) {};
    NULL_COPY_MOVE(LoadStoreInstruction)
    ~LoadStoreInstruction() = default;
};

} // namespace arm_emu