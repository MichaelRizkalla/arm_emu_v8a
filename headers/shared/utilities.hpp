#pragma once
#include <type_traits>
#include <cstdint>
#include <array>
#include <bitset>
#include <string>
#include <exception>

#include <concepts>

namespace arm_emu {
// Used to enforce compile time errors (Unresolved symbol) in constexpr functions
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

    constexpr Table(std::array< TableEntry, Size > input) noexcept : mEntries(input) {};

    std::array< TableEntry, Size > mEntries;
};

//////////////////////////// Traits ////////////////////////////
template < typename TEnum >
struct enum_size {
    static constexpr std::underlying_type_t< TEnum > value = static_cast< std::underlying_type_t< TEnum > >(TEnum::ARM_EMU_COUNT);
};

template < typename TEnum >
static constexpr auto enum_size_v = enum_size< TEnum >::value;

struct size_checker {
    template < typename _Tx, typename... _Args >
    static constexpr auto call(_Tx*) -> decltype(std::declval< _Tx& >().size(std::declval< _Args >()...));
};

struct constructor_checker {
    template < typename _Tx, typename... _Args >
    static constexpr auto call(_Tx*) -> decltype(_Tx { std::declval< _Args >()... });
};

// https://codereview.stackexchange.com/questions/92993/template-method-checker
template < typename, typename, typename _Tx >
struct has_function {
    static_assert(std::integral_constant< _Tx, false >::value, "Third parameter has to be of function type.");
};

template < typename _Checker, typename _Ty, typename _Ret, typename... _Args >
struct has_function< _Checker, _Ty, _Ret(_Args...) > {
  private:
    template < typename _Chk, typename _Tx>
    static constexpr auto check(_Tx*) -> decltype(_Chk::template call< _Tx, _Args... >(0));

    // Sink hole
    template < typename, typename >
    static constexpr std::false_type check(...);

  public:
    static constexpr bool value = std::is_same_v< decltype(check< _Checker, _Ty >(0)), _Ret >;
};

template < class _Checker, typename _Ty, typename _Ret, typename... _Args >
static constexpr auto has_function_v = has_function< _Checker, _Ty, _Ret, _Args... >::value;

template < class _Checker, typename _Ty, typename _Ret, typename... _Args >
concept function_in = has_function_v< _Checker, _Ty, _Ret, _Args... >;

/// <summary>
/// Credits to https://stackoverflow.com/questions/13830158/check-if-a-variable-type-is-iterable
/// </summary>
/// <typeparam name="_Ty"></typeparam>
template < typename _Ty >
struct is_iteratable {
  private:
    template < typename _Tx >
    static constexpr auto check(_Tx*)
        -> decltype(std::begin(std::declval< _Tx& >()) != std::end(std::declval< _Tx& >()), ++std::declval< decltype(std::begin(std::declval< _Tx& >()))& >(),
                    --std::declval< decltype(std::begin(std::declval< _Tx& >()))& >(), ++std::declval< decltype(std::end(std::declval< _Tx& >()))& >(),
                    --std::declval< decltype(std::end(std::declval< _Tx& >()))& >(), void(*std::begin(std::declval< _Tx& >())), void(*std::end(std::declval< _Tx& >())),
                    std::true_type {});

    // Sink hole
    template < typename >
    static constexpr std::false_type check(...);

  public:
    static constexpr bool value = decltype(check< _Ty >(0))::value;
};

template < typename _Ty >
static constexpr auto is_iteratable_v = is_iteratable< _Ty >::value;

template < typename _Ty >
concept iteratable = is_iteratable_v< _Ty >;

template < typename _Ty >
struct is_indexable {
  private:
    template < typename _Tx >
    static constexpr auto check(_Tx*) -> decltype(std::declval< _Tx& >()[std::declval< const typename _Tx::size_type >()], std::true_type {});

    // Sink hole
    template < typename >
    static constexpr std::false_type check(...);

  public:
    static constexpr bool value = decltype(check< std::remove_cvref_t< _Ty > >(0))::value;
};

template < typename _Ty >
static constexpr auto is_indexable_v = is_indexable< _Ty >::value;

template < typename _Ty >
concept indexable = is_indexable_v< _Ty >;

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

template < class _Ty >
concept enum_type = std::is_enum_v< _Ty >;

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
    static constexpr bool result() noexcept { return TQueryTarget::template IsImplemented< TEnumType, TValue >(); }
};

// Figure a way to write 1 version of concate that mimics the behaviour of the 4
template < std::size_t S1, std::size_t S2 >
constexpr auto concate(std::bitset< S1 >&& set1, std::bitset< S2 >&& set2) noexcept {
    std::string s1 = set1.to_string();
    std::string s2 = set2.to_string();
    return std::bitset< S1 + S2 >(s1 + s2);
}
template < std::size_t S1, std::size_t S2 >
constexpr auto concate(const std::bitset< S1 >& set1, std::bitset< S2 >&& set2) noexcept {
    std::string s1 = set1.to_string();
    std::string s2 = set2.to_string();
    return std::bitset< S1 + S2 >(s1 + s2);
}
template < std::size_t S1, std::size_t S2 >
constexpr auto concate(std::bitset< S1 >&& set1, const std::bitset< S2 >& set2) noexcept {
    std::string s1 = set1.to_string();
    std::string s2 = set2.to_string();
    return std::bitset< S1 + S2 >(s1 + s2);
}
template < std::size_t S1, std::size_t S2 >
constexpr auto concate(const std::bitset< S1 >& set1, const std::bitset< S2 >& set2) noexcept {
    std::string s1 = set1.to_string();
    std::string s2 = set2.to_string();
    return std::bitset< S1 + S2 >(s1 + s2);
}

} // namespace arm_emu