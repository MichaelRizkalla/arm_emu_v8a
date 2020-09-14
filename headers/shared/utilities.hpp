#pragma once
#include <type_traits>
#include <cstdint>
#include <array>
#include <bitset>
#include <string>
#include <exception>

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

    constexpr Table(std::array< TableEntry, Size > input) : mEntries(input) {};

    std::array< TableEntry, Size > mEntries;
};

template < typename TEnum >
struct enum_size {
    static constexpr std::underlying_type_t< TEnum > value = static_cast< std::underlying_type_t< TEnum > >(TEnum::ARM_EMU_COUNT);
};

template < typename TEnum >
static constexpr auto enum_size_v = enum_size< TEnum >::value;

template < typename _Ty >
struct has_size {
  private:
    template < typename _Tx >
    static constexpr auto check(_Tx*) -> typename std::is_same< decltype(std::declval< _Tx& >().size()), std::size_t >::type;

    // Sink hole
    template < typename >
    static constexpr std::false_type check(...);

  public:
    static constexpr bool value = decltype(check< std::remove_cvref_t< _Ty > >(0))::value;
};

template < typename _Ty >
static constexpr auto has_size_v = has_size< _Ty >::value;

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
static constexpr auto get_type_t = get_type< N >::type;

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

template < typename TQueryTarget, typename TEnumType, TEnumType TValue >
struct Query {
    static constexpr bool result() { return TQueryTarget::template IsImplemented< TEnumType, TValue >(); }
};

// Figure a way to write 1 version of concate that mimics the behaviour of the 4
template < std::size_t S1, std::size_t S2 >
constexpr auto concate(std::bitset< S1 >&& set1, std::bitset< S2 >&& set2) {
    std::string s1 = set1.to_string();
    std::string s2 = set2.to_string();
    return std::bitset< S1 + S2 >(s1 + s2);
}
template < std::size_t S1, std::size_t S2 >
constexpr auto concate(const std::bitset< S1 >& set1, std::bitset< S2 >&& set2) {
    std::string s1 = set1.to_string();
    std::string s2 = set2.to_string();
    return std::bitset< S1 + S2 >(s1 + s2);
}
template < std::size_t S1, std::size_t S2 >
constexpr auto concate(std::bitset< S1 >&& set1, const std::bitset< S2 >& set2) {
    std::string s1 = set1.to_string();
    std::string s2 = set2.to_string();
    return std::bitset< S1 + S2 >(s1 + s2);
}
template < std::size_t S1, std::size_t S2 >
constexpr auto concate(const std::bitset< S1 >& set1, const std::bitset< S2 >& set2) {
    std::string s1 = set1.to_string();
    std::string s2 = set2.to_string();
    return std::bitset< S1 + S2 >(s1 + s2);
}
} // namespace arm_emu