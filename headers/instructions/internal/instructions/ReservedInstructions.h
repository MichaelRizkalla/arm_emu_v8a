#pragma once
#include "BaseInstruction.hpp"

namespace arm_emu {

/*
Reserved Class
*/
// Special case (no instruction class)
struct ReservedGroup : public AbstractInstruction< std::uint32_t, ReservedGroup > {
    [[nodiscard]] constexpr bool udf() const noexcept { return !((mInstruction >> 16) & 0xFFFFFFFF); }
    [[nodiscard]] constexpr auto getInstructionType() const noexcept {
        for (auto&& e : ReservedGroupA64Table.mEntries) {
            if ((mInstruction & e.code) == e.expected) { return e.type; }
        }
        return ReservedGroupA64::Undefined;
    }

    constexpr explicit ReservedGroup(BaseInstruction instruction) noexcept : AbstractInstruction(instruction.data()) {}
    constexpr explicit ReservedGroup(const ReservedGroup& instruction) noexcept : AbstractInstruction(instruction.mInstruction) {};
    constexpr explicit ReservedGroup(ReservedGroup&& instruction) noexcept : AbstractInstruction(instruction.mInstruction) {};
    NULL_ASSINGMENT(ReservedGroup)
    ~ReservedGroup() = default;
};

template < ReservedGroupA64 >
struct ReservedInstruction;

template <>
struct ReservedInstruction< ReservedGroupA64::UDP > final : ReservedGroup {

    constexpr explicit ReservedInstruction(const ReservedGroup& instruction) noexcept : ReservedGroup(instruction) {};
    NULL_COPY_MOVE(ReservedInstruction)
    ~ReservedInstruction() = default;
};

} // namespace arm_emu