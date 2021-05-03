#pragma once
#include "BaseInstruction.hpp"

namespace arm_emu {

/*
Reserved Class
*/
// Special case (no instruction class)
struct ReservedGroup : public AbstractInstruction< std::uint32_t, ReservedGroup > {
    [[nodiscard]] bool udf() const noexcept { return !((mInstruction >> 16) & 0xFFFFFFFF); }
    [[nodiscard]] auto getInstructionType() const noexcept {
        for (auto&& e : ReservedGroupA64Table.mEntries) {
            if ((mInstruction & e.code) == e.expected) { return e.type; }
        }
        return ReservedGroupA64::Undefined;
    }

    explicit ReservedGroup(BaseInstruction instruction) noexcept : AbstractInstruction(instruction.data()) {}
    explicit ReservedGroup(const ReservedGroup& instruction) noexcept : AbstractInstruction(instruction.mInstruction) {};
    explicit ReservedGroup(ReservedGroup&& instruction) noexcept : AbstractInstruction(instruction.mInstruction) {};
    NULL_ASSINGMENT(ReservedGroup)
    ~ReservedGroup() = default;
};

template < ReservedGroupA64 >
struct ReservedInstruction;

template <>
struct ReservedInstruction< ReservedGroupA64::UDP > final : ReservedGroup {

    explicit ReservedInstruction(const ReservedGroup& instruction) noexcept : ReservedGroup(instruction) {};
    NULL_COPY_MOVE(ReservedInstruction)
    ~ReservedInstruction() = default;
};

} // namespace arm_emu