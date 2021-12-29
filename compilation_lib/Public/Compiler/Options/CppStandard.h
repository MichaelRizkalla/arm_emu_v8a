#if !defined(CPPSTANDARD_H_INCLUDED_B0CC088F_2779_4723_B827_46A11F472856)
    #define CPPSTANDARD_H_INCLUDED_B0CC088F_2779_4723_B827_46A11F472856

    #include <API/Api.h>
    #include <cstdint>

namespace arm_emu {

    enum class CppStandard : std::uint32_t
    {
        Cpp11,
        Cpp17,
        Cpp20,
        Cpp23,
    };

} // namespace arm_emu

#endif // !defined(CPPSTANDARD_H_INCLUDED_B0CC088F_2779_4723_B827_46A11F472856)
