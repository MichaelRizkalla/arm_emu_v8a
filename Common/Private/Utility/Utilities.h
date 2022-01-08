#if !defined(UTILITIES_HPP_INCLUDED_1951FB0D_26AD_4BA0_A459_0120D0ED63C9)
    #define UTILITIES_HPP_INCLUDED_1951FB0D_26AD_4BA0_A459_0120D0ED63C9

    #include <API/Api.h>
    #include <array>
    #include <bitset>
    #include <concepts>
    #include <cstdint>
    #include <memory>
    #include <numeric>
    #include <string>
    #include <type_traits>

BEGIN_NAMESPACE

    // Used to enforce compile time errors (Unresolved symbol) in functions
    extern int ARM_EMU_EXCEPTION;

    template < typename TEnum, class Representation, std::underlying_type_t< TEnum > Size >
    struct Table {
        using EnumType = TEnum;

        struct TableEntry {
            Representation code;
            Representation expected;
            TEnum          type;
        };

        constexpr Table(std::array< TableEntry, Size > input) noexcept : mEntries(input) {};

        constexpr TEnum Lookup(Representation value) const noexcept {
            for (const auto& entry : mEntries) {
                if ((value & entry.code) == entry.expected) {
                    return entry.type;
                }
            }
            return static_cast< TEnum >(std::numeric_limits< std::underlying_type_t< TEnum > >::max());
        }

        std::array< TableEntry, Size > mEntries;
    };

    template < class Key, class Value, std::size_t Size >
    struct SmallConstMap {
        constexpr SmallConstMap(std::array< std::pair< Key, Value >, Size > data) : m_data(std::move(data)) {
        }

        Value At(Key key) const noexcept(std::is_nothrow_default_constructible_v< Value >) {
            for (const auto& [key_, value_] : m_data) {
                if (key_ == key) {
                    return value_;
                }
            }
            return Value {};
        }

      private:
        std::array< std::pair< Key, Value >, Size > m_data;
    };

    //////////////////////////// Traits ////////////////////////////
    template < typename TEnum >
    struct enum_size {
        constexpr static std::underlying_type_t< TEnum > value =
            static_cast< std::underlying_type_t< TEnum > >(TEnum::ARM_EMU_COUNT);
    };

    template < typename TEnum >
    constexpr static auto enum_size_v = enum_size< TEnum >::value;

    //////////////////////////// Helper functions ////////////////////////////
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

END_NAMESPACE

#endif // !defined(UTILITIES_HPP_INCLUDED_1951FB0D_26AD_4BA0_A459_0120D0ED63C9)
