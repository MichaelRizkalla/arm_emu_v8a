#if !defined(EXTENSIONVERSION_H_INCLUDED_8C6C4B57_1660_43C8_B432_3F915502202B)
    #define EXTENSIONVERSION_H_INCLUDED_8C6C4B57_1660_43C8_B432_3F915502202B

    #include <API/Api.h>
    #include <cstdint>

BEGIN_NAMESPACE

enum class IProcessingUnit::ExtensionVersion : std::uint32_t
{
    Armv8p0,
    Armv8p1,
    Armv8p2,
    Armv8p3,
    Armv8p4,
    Armv8p5,
    Armv8p6,
    Armv8p7,
};

END_NAMESPACE

#endif // !defined(EXTENSIONVERSION_H_INCLUDED_8C6C4B57_1660_43C8_B432_3F915502202B)
