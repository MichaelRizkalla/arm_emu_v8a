#if !defined(ENUMUTILS_H_INCLUDED_959EC823_B1F9_4531_A3D0_30F69A99FBF0)
    #define ENUMUTILS_H_INCLUDED_959EC823_B1F9_4531_A3D0_30F69A99FBF0

    #include <API/Api.h>
    #include <type_traits>

namespace arm_emu {

    template < class Ty >
    concept enum_type = std::is_enum_v< Ty >;

} // namespace arm_emu

#endif // !defined(ENUMUTILS_H_INCLUDED_959EC823_B1F9_4531_A3D0_30F69A99FBF0)
