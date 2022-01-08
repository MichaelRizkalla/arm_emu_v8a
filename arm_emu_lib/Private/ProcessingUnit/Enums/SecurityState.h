#if !defined(SECURITYSTATE_H_INCLUDED_16062E9A_4B2B_48CB_9585_0DF8505A7802)
    #define SECURITYSTATE_H_INCLUDED_16062E9A_4B2B_48CB_9585_0DF8505A7802

    #include <API/Api.h>
    #include <cstdint>

BEGIN_NAMESPACE

enum class IProcessingUnit::SecurityState : std::uint32_t
{
    Secure,
    NonSecure
};

END_NAMESPACE

#endif // !defined(SECURITYSTATE_H_INCLUDED_16062E9A_4B2B_48CB_9585_0DF8505A7802)
