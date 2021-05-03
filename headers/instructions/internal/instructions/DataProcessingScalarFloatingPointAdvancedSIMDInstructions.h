#pragma once
#include "BaseInstruction.hpp"

namespace arm_emu {

/*
    Data Processing Scalar Floating Point Advanced SIMD
*/
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDGroup : public AbstractInstruction< std::uint32_t, DataProcessing_ScalarFloatingPoint_AdvancedSIMDGroup > {
    [[nodiscard]] auto getInstructionClass() const noexcept { return DataProcessing_ScalarFloatingPoint_AdvancedSIMDA64::getInstructionClass(mInstruction); }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDGroup(BaseInstruction instruction) noexcept : AbstractInstruction(instruction.data()) {}
    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDGroup(const DataProcessing_ScalarFloatingPoint_AdvancedSIMDGroup& instruction) noexcept :
        AbstractInstruction(instruction.mInstruction) {};
    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDGroup(DataProcessing_ScalarFloatingPoint_AdvancedSIMDGroup&& instruction) noexcept :
        AbstractInstruction(instruction.mInstruction) {};
    NULL_ASSINGMENT(DataProcessing_ScalarFloatingPoint_AdvancedSIMDGroup)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDGroup() = default;
};

using DP_SFP_ASIMDGroup = DataProcessing_ScalarFloatingPoint_AdvancedSIMDGroup;
using DP_SFP_ASIMDA64   = DataProcessing_ScalarFloatingPoint_AdvancedSIMDA64;

template < DP_SFP_ASIMDA64::InstructionGroupEnum >
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction;

template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::CRYPTOGRAPHIC_AES > final : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::CRYPTOGRAPHIC_AES >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::CRYPTOHRAPHIC_THREE_REGISTER_SHA > final : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::CRYPTOHRAPHIC_THREE_REGISTER_SHA >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::CRYPTOGRAPHIC_TWO_REGISTER_SHA > final : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::CRYPTOGRAPHIC_TWO_REGISTER_SHA >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_SCALAR_COPY > final : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_SCALAR_COPY >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMG_SCALAR_THREE_SAME_FP16 > final
    : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMG_SCALAR_THREE_SAME_FP16 >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_SCALAR_TWO_REGISTER_MISCELLANEOUS_FP16 > final
    : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_SCALAR_TWO_REGISTER_MISCELLANEOUS_FP16 >(
            mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_SCALAR_THREE_SAME_EXTRACTION > final
    : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_SCALAR_THREE_SAME_EXTRACTION >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_SCALAR_TWO_REGISTER_MISCELLANEOUS > final
    : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_SCALAR_TWO_REGISTER_MISCELLANEOUS >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_SCALAR_PAIRWISE > final : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_SCALAR_PAIRWISE >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_SCALAR_THREE_DIFFERENT > final
    : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_SCALAR_THREE_DIFFERENT >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_SCALAR_THREE_SAME > final : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_SCALAR_THREE_SAME >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_SCALAR_SHIFT_BY_IMMEDIATE > final
    : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_SCALAR_SHIFT_BY_IMMEDIATE >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_SCALAR_X_INDEXED_ELEMENT > final
    : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_SCALAR_X_INDEXED_ELEMENT >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_TABLE_LOOKUP > final : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_TABLE_LOOKUP >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_PERMUTE > final : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_PERMUTE >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_EXTRACT > final : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_EXTRACT >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_COPY > final : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_COPY >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_THREE_SAME_FP16 > final : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_THREE_SAME_FP16 >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_TWO_REGISTER_MISCELLANEOUS_FP16 > final
    : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_TWO_REGISTER_MISCELLANEOUS_FP16 >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_THREE_REGISTER_EXTENSION > final
    : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_THREE_REGISTER_EXTENSION >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_TWO_REGISTER_MISCELLANEOUS > final
    : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_TWO_REGISTER_MISCELLANEOUS >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_ACROSS_LANES > final : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_ACROSS_LANES >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_THREE_DIFFERENT > final : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_THREE_DIFFERENT >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_THREE_SAME > final : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_THREE_SAME >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_MODIFIED_IMMEDIATE > final : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_MODIFIED_IMMEDIATE >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_SHIFT_BY_IMMEDIATE > final : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_SHIFT_BY_IMMEDIATE >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_VECTOR_X_INDEXED_ELEMENT > final
    : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::ADVANCED_SIMD_VECTOR_X_INDEXED_ELEMENT >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::CRYPTOGRAPHIC_THREE_REGISTER_IMM2 > final
    : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::CRYPTOGRAPHIC_THREE_REGISTER_IMM2 >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::CRYPTOGRAPHIC_THREE_REGISTER_SHA512 > final
    : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::CRYPTOGRAPHIC_THREE_REGISTER_SHA512 >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::CRYPTOGRAPHIC_FOUR_REGISTER > final : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::CRYPTOGRAPHIC_FOUR_REGISTER >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::XAR > final : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::XAR >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::CRYPTOGRAPHIC_TWO_REGISTER_SHA512 > final
    : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::CRYPTOGRAPHIC_TWO_REGISTER_SHA512 >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::CONVERSION_FLOATING_POINT_AND_FIXED_POINT > final
    : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::CONVERSION_FLOATING_POINT_AND_FIXED_POINT >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::CONVERSION_FLOATING_POINT_AND_INTEGER > final
    : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::CONVERSION_FLOATING_POINT_AND_INTEGER >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::FLOATING_POINT_DATA_PROCESSING_ONE_SOURCE > final
    : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::FLOATING_POINT_DATA_PROCESSING_ONE_SOURCE >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::FLOATING_POINT_COMPARE > final : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::FLOATING_POINT_COMPARE >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::FLOATING_POINT_IMMEDIATE > final : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::FLOATING_POINT_IMMEDIATE >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::FLOATING_POINT_CONDITIONAL_COMPARE > final
    : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::FLOATING_POINT_CONDITIONAL_COMPARE >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::FLOATING_POINT_DATA_PROCESSING_TWO_SOURCE > final
    : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::FLOATING_POINT_DATA_PROCESSING_TWO_SOURCE >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::FLOATING_POINT_CONDITIONAL_SELECT > final
    : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::FLOATING_POINT_CONDITIONAL_SELECT >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};
template <>
struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction< DP_SFP_ASIMDA64::InstructionGroupEnum::FLOATING_POINT_DATA_PROCESSING_THREE_SOURCE > final
    : public DP_SFP_ASIMDGroup {
    [[nodiscard]] auto getInstructionType() const noexcept {
        return DP_SFP_ASIMDA64::getInstructionType< DP_SFP_ASIMDA64::InstructionGroupEnum::FLOATING_POINT_DATA_PROCESSING_THREE_SOURCE >(mInstruction);
    }

    explicit DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction(const DP_SFP_ASIMDGroup& instruction) noexcept : DP_SFP_ASIMDGroup(instruction) {};
    NULL_COPY_MOVE(DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction)
    ~DataProcessing_ScalarFloatingPoint_AdvancedSIMDInstruction() = default;
};

} // namespace arm_emu