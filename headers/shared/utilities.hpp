#pragma once
#include <type_traits>
#include <cstdint>
#include <array>
#include <bitset>
#include <string>
#include <exception>

#include <concepts>

namespace arm_emu {
// Used to enforce compile time errors (Unresolved symbol) in functions
extern int ARM_EMU_EXCEPTION;

#define NULL_COPY(Type)                     \
    Type(const Type&)             = delete; \
    Type& operator==(const Type&) = delete;

#define NULL_MOVE(Type)                \
    Type(Type&&)             = delete; \
    Type& operator==(Type&&) = delete;

#define NULL_COPY_MOVE(Type) NULL_COPY(Type) NULL_MOVE(Type)

#define NULL_ASSINGMENT(Type)               \
    Type& operator==(const Type&) = delete; \
    Type& operator==(Type&&)      = delete;

template < typename TEnum, std::underlying_type_t< TEnum > Size >
struct Table {
    using EnumType = TEnum;

    struct TableEntry {
        std::uint32_t code;
        std::uint32_t expected;
        TEnum         type;
    };

    Table(std::array< TableEntry, Size > input) noexcept : mEntries(input) {};

    std::array< TableEntry, Size > mEntries;
};

//////////////////////////// Traits ////////////////////////////
template < typename TEnum >
struct enum_size {
    constexpr static std::underlying_type_t< TEnum > value = static_cast< std::underlying_type_t< TEnum > >(TEnum::ARM_EMU_COUNT);
};

template < typename TEnum >
constexpr static auto enum_size_v = enum_size< TEnum >::value;

struct size_checker {
    template < typename Tx, typename... Args >
    constexpr static auto call(Tx*) -> decltype(std::declval< Tx& >().size(std::declval< Args >()...));
};

struct constructor_checker {
    template < typename Tx, typename... Args >
    constexpr static auto call(Tx*) -> decltype(Tx { std::declval< Args >()... });
};

// https://codereview.stackexchange.com/questions/92993/template-method-checker
template < typename, typename, typename Tx >
struct has_function {
    static_assert(std::integral_constant< Tx, false >::value, "Third parameter has to be of function type.");
};

template < typename Checker, typename Ty, typename Ret, typename... Args >
struct has_function< Checker, Ty, Ret(Args...) > {
  private:
    template < typename Chk, typename Tx>
    constexpr static auto check(Tx*) -> decltype(Chk::template call< Tx, Args... >(0));

    // Sink hole
    template < typename, typename >
    constexpr static std::false_type check(...);

  public:
    constexpr static bool value = std::is_same_v< decltype(check< Checker, Ty >(0)), Ret >;
};

template < class Checker, typename Ty, typename Ret, typename... Args >
constexpr static auto has_function_v = has_function< Checker, Ty, Ret, Args... >::value;

template < class Checker, typename Ty, typename Ret, typename... Args >
concept function_in = has_function_v< Checker, Ty, Ret, Args... >;

/// <summary>
/// Credits to https://stackoverflow.com/questions/13830158/check-if-a-variable-type-is-iterable
/// </summary>
/// <typeparam name="_Ty"></typeparam>
template < typename Ty >
struct is_iteratable {
  private:
    template < typename Tx >
    constexpr static auto check(Tx*)
        -> decltype(std::begin(std::declval< Tx& >()) != std::end(std::declval< Tx& >()), ++std::declval< decltype(std::begin(std::declval< Tx& >()))& >(),
                    --std::declval< decltype(std::begin(std::declval< Tx& >()))& >(), ++std::declval< decltype(std::end(std::declval< Tx& >()))& >(),
                    --std::declval< decltype(std::end(std::declval< Tx& >()))& >(), void(*std::begin(std::declval< Tx& >())), void(*std::end(std::declval< Tx& >())),
                    std::true_type {});

    // Sink hole
    template < typename >
    constexpr static std::false_type check(...);

  public:
    constexpr static bool value = decltype(check< Ty >(0))::value;
};

template < typename Ty >
constexpr static auto is_iteratable_v = is_iteratable< Ty >::value;

template < typename Ty >
concept iteratable = is_iteratable_v< Ty >;

template < typename Ty >
struct is_indexable {
  private:
    template < typename Tx >
    constexpr static auto check(Tx*) -> decltype(std::declval< Tx& >()[std::declval< const typename Tx::size_type >()], std::true_type {});

    // Sink hole
    template < typename >
    constexpr static std::false_type check(...);

  public:
    constexpr static bool value = decltype(check< std::remove_cvref_t< Ty > >(0))::value;
};

template < typename _Ty >
constexpr static auto is_indexable_v = is_indexable< _Ty >::value;

template < typename Ty >
concept indexable = is_indexable_v< Ty >;

template < std::size_t N >
struct get_type {};

template <>
struct get_type< 8 > {
    using type = std::uint8_t;
};
template <>
struct get_type< 16 > {
    using type = std::uint16_t;
};
template <>
struct get_type< 32 > {
    using type = std::uint32_t;
};
template <>
struct get_type< 64 > {
    using type = std::uint64_t;
};

template < std::size_t N >
using get_type_t = typename get_type< N >::type;

template < class Ty >
concept enum_type = std::is_enum_v< Ty >;

//////////////////////////// Exceptions ////////////////////////////
class ram_not_initialized : public std::exception {
    [[nodiscard]] virtual const char* what() const noexcept override { return "RAM block is not initialized"; }
};

class undefined_register_access : public std::exception {
    [[nodiscard]] virtual const char* what() const noexcept override { return "Register is in undefined state"; }
};

class undefined_behaviour : public std::exception {
    [[nodiscard]] virtual const char* what() const noexcept override { return "Undefined behaviour"; }
};

class not_implemented_feature : public std::exception {
    [[nodiscard]] virtual const char* what() const noexcept override { return "Not implemented yet!"; }
};

class undefined_instruction : public std::exception {
    [[nodiscard]] virtual const char* what() const noexcept override { return "Undefined instruction due to unsupported feature"; }
};

//////////////////////////// Helper functions ////////////////////////////
template < typename TQueryTarget, typename TEnumType, TEnumType TValue >
struct Query {
    constexpr static bool result() noexcept {
        return TQueryTarget::template IsImplemented< TEnumType, TValue >();
    }
};

// Figure a way to write 1 version of concate that mimics the behaviour of the 4
template < std::size_t S1, std::size_t S2 >
auto concate(std::bitset< S1 >&& set1, std::bitset< S2 >&& set2) noexcept {
    std::string s1 = set1.to_string();
    std::string s2 = set2.to_string();
    return std::bitset< S1 + S2 >(std::move(s1) + std::move(s2));
}
template < std::size_t S1, std::size_t S2 >
auto concate(const std::bitset< S1 >& set1, std::bitset< S2 >&& set2) noexcept {
    std::string s1 = set1.to_string();
    std::string s2 = set2.to_string();
    return std::bitset< S1 + S2 >(std::move(s1) + std::move(s2));
}
template < std::size_t S1, std::size_t S2 >
auto concate(std::bitset< S1 >&& set1, const std::bitset< S2 >& set2) noexcept {
    std::string s1 = set1.to_string();
    std::string s2 = set2.to_string();
    return std::bitset< S1 + S2 >(std::move(s1) + std::move(s2));
}
template < std::size_t S1, std::size_t S2 >
auto concate(const std::bitset< S1 >& set1, const std::bitset< S2 >& set2) noexcept {
    std::string s1 = set1.to_string();
    std::string s2 = set2.to_string();
    return std::bitset< S1 + S2 >(std::move(s1) + std::move(s2));
}

} // namespace arm_emu