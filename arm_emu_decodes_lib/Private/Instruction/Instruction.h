#if !defined(BASEINSTRUCTION_H_INCLUDED_7B839D39_E31F_4C00_8A84_467D28F8A946)
    #define BASEINSTRUCTION_H_INCLUDED_7B839D39_E31F_4C00_8A84_467D28F8A946

    #include <API/Api.h>
    // #include <ProcessingUnit/IProcessingUnit.h>
    #include <concepts>
    #include <cstdint>
    #include <memory>
    #include <type_traits>

BEGIN_NAMESPACE

struct [[nodiscard]] Instruction {
  public:
    using Type = std::uint32_t;

    explicit constexpr Instruction(const Type& instruction) noexcept : mInstruction(instruction) {
    }

    explicit constexpr Instruction(Type&& instruction) noexcept : mInstruction(std::move(instruction)) {
    }

    [[nodiscard]] constexpr auto operator&(const Type rhs) const noexcept {
        return mInstruction & rhs;
    }

    [[nodiscard]] constexpr auto operator|(const Type rhs) const noexcept {
        return mInstruction | rhs;
    }

    [[nodiscard]] constexpr friend auto operator&(const Type lhs, const Instruction rhs) noexcept {
        return lhs & rhs.mInstruction;
    }

    [[nodiscard]] constexpr friend auto operator|(const Type lhs, const Instruction rhs) noexcept {
        return lhs | rhs.mInstruction;
    }

    template < class BitLoc >
    [[nodiscard]] constexpr bool ReadBit(BitLoc bit) const noexcept requires(std::integral< BitLoc >) {
        return static_cast< bool >(mInstruction & (static_cast< Type >(1) << bit));
    }

    [[nodiscard]] constexpr Type Get() const noexcept {
        return mInstruction;
    }

  private:
    Type mInstruction;
};

END_NAMESPACE

#endif // !defined(BASEINSTRUCTION_H_INCLUDED_7B839D39_E31F_4C00_8A84_467D28F8A946)
