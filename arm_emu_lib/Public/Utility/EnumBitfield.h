#if !defined(ENUMBITFIELD_H_INCLUDED_99C32E3C_ECC0_4A0F_AE23_9B2EB47A407C)
    #define ENUMBITFIELD_H_INCLUDED_99C32E3C_ECC0_4A0F_AE23_9B2EB47A407C

    #include <API/Api.h>
    #include <Utility/EnumUtils.h>
    #include <type_traits>

namespace arm_emu {

    namespace detail {
    }

    template < typename TEnum >
    TEnum operator|(TEnum lhs, TEnum rhs) requires(enum_type< TEnum >) {
        using TUnderlying = std::underlying_type_t< TEnum >;
        return static_cast< TEnum >(static_cast< TUnderlying >(lhs) | static_cast< TUnderlying >(rhs));
    }

} // namespace arm_emu

#endif // !defined(ENUMBITFIELD_H_INCLUDED_99C32E3C_ECC0_4A0F_AE23_9B2EB47A407C)
