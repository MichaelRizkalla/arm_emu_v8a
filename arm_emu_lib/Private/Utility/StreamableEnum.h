#if !defined(STREAMABLEENUM_H_INCLUDED_509067CB_5CEF_423D_8E6F_63AFB21B324A)
    #define STREAMABLEENUM_H_INCLUDED_509067CB_5CEF_423D_8E6F_63AFB21B324A

    #include <API/Api.h>
    #include <Utility/EnumUtils.h>
    #include <ostream>
    #include <string_view>
    #include <type_traits>

namespace arm_emu {

    // Any streamable enum will implement and export these two functions

    struct Enum {
        template < class TEnum >
        [[nodiscard]] static std::string_view ToChar(TEnum enumValue);
    };

    template < class TEnum >
    std::ostream& operator<<(std::ostream& os, TEnum enumValue) requires(enum_type< TEnum >);

} // namespace arm_emu

#endif // !defined(STREAMABLEENUM_H_INCLUDED_509067CB_5CEF_423D_8E6F_63AFB21B324A)
