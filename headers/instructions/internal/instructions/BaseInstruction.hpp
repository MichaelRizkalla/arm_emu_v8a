#pragma once
#include <type_traits>
#include <cstdint>
#include <memory>

#include <A64InstructionSet.h>

namespace arm_emu {

template < class Ty, class CRTP = void, class Enable = std::enable_if_t< std::is_same_v< Ty, std::uint32_t > > >
struct AbstractInstruction {
    using Type = Ty;

    [[nodiscard]] auto operator&(const Ty rhs) const noexcept { return mInstruction & rhs; }
    [[nodiscard]] auto operator|(const Ty rhs) const noexcept { return mInstruction | rhs; }

    [[nodiscard]] friend auto operator&(const Ty lhs, const CRTP rhs) noexcept { return lhs & rhs.mInstruction; }
    [[nodiscard]] friend auto operator|(const Ty lhs, const CRTP rhs) noexcept { return lhs | rhs.mInstruction; }

    [[nodiscard]] auto data() const noexcept { return mInstruction; }
    template < class BitLoc, class E = std::enable_if_t< std::conjunction_v< std::is_integral< BitLoc >, std::negation< std::is_same< BitLoc, std::uint64_t > >,
                                                                              std::negation< std::is_same< BitLoc, std::int64_t > > > > >
    [[nodiscard]] bool readBit(BitLoc bit) const noexcept {
        return static_cast< bool >(mInstruction & (static_cast< Ty >(1) << bit));
    }
    [[nodiscard]] bool is64() const noexcept { return readBit(static_cast< std::uint8_t >(31)); }

    explicit AbstractInstruction(const Ty& instruction) noexcept : mInstruction(instruction) {}
    explicit AbstractInstruction(Ty&& instruction) noexcept : mInstruction(std::move(instruction)) {}

  protected:
    const Ty mInstruction;
};

struct BaseInstruction : public AbstractInstruction< std::uint32_t, BaseInstruction > {

    [[nodiscard]] auto getDecodeGroup() const noexcept {
        const auto val = static_cast< std::uint8_t >((mInstruction >> 25) & 0b1111);
        for (auto&& e : DecodeGroupA64Table) {
            if ((val & e.code) == e.expected) { return e.type; }
        }
        return DecodeGroupA64::Undefined;
    }

    explicit BaseInstruction(const std::uint32_t& instruction) noexcept : AbstractInstruction(instruction) {}
    explicit BaseInstruction(std::uint32_t&& instruction) noexcept : AbstractInstruction(std::move(instruction)) {}
    BaseInstruction(const BaseInstruction& instruction) noexcept : AbstractInstruction(instruction.mInstruction) {}
    BaseInstruction(BaseInstruction&& instruction) noexcept : AbstractInstruction(instruction.mInstruction) {}
    // Construct new object using "Placement new" to change "const member" value
    BaseInstruction& operator=(const BaseInstruction& instruction) noexcept { return *(new (this) BaseInstruction { instruction.mInstruction }); }
    BaseInstruction& operator=(BaseInstruction&& instruction) noexcept { return *(new (this) BaseInstruction { instruction.mInstruction }); }

    friend struct ReservedGroup;
    friend struct DateProcessingImmediateGroup;
    friend struct Branch_Exception_SystemGroup;
    friend struct LoadStoreGroup;
    friend struct DataProcessingRegisterGroup;
    friend struct DataProcessing_ScalarFloatingPoint_AdvancedSIMDGroup;
};

} // namespace arm_emu