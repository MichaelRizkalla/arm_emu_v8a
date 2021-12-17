#if !defined(CONSTRAINT_H_INCLUDED_C0D3A1CE_F18E_43EC_BAC0_3D083A087E57)
    #define CONSTRAINT_H_INCLUDED_C0D3A1CE_F18E_43EC_BAC0_3D083A087E57

    #include <API/Api.h>
    #include <cstdint>

BEGIN_NAMESPACE

enum class IProcessingUnit::Constraint : std::uint32_t
{
    NONE,
    UNKNOWN,
    UNDEF,
    NOP,
};

END_NAMESPACE

#endif // !defined(CONSTRAINT_H_INCLUDED_C0D3A1CE_F18E_43EC_BAC0_3D083A087E57)
