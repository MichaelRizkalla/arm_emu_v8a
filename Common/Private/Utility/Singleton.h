#if !defined(SINGLETON_H_INCLUDED_EA66C745_2EA4_44B5_BFA8_1518092125FF)
    #define SINGLETON_H_INCLUDED_EA66C745_2EA4_44B5_BFA8_1518092125FF

    #include <API/HiddenAPI.h>
    #include <memory>
    #include <type_traits>

BEGIN_NAMESPACE

template < class Type >
class DefaultSingletonManager {
  public:
    STATIC_CLASS(DefaultSingletonManager)

    [[nodiscard]] static Type& GetInstance() noexcept(std::is_nothrow_default_constructible_v< Type >) requires(
        !std::is_constructible_v< Type >) {
        static Type m_instance {};
        return m_instance;
    }
};

// In header file use:
    #define DECLARE_AS_SINGLETON(Typename, x)       \
      private:                                      \
        friend DefaultSingletonManager< Typename >; \
                                                    \
      public:                                       \
        [[nodiscard]] static Typename& GetInstance() noexcept(noexcept(x));

// In source file use:
// #define IMPLEMENT_AS_SINGLETON(Typename) decltype(Typename::m_singletonManager) Typename::m_singletonManager {};

    #define IMPLEMENT_AS_SINGLETON(Typename)                                                                        \
                                                                                                                    \
        Typename& Typename::GetInstance() noexcept(noexcept(std::is_nothrow_default_constructible_v< Typename >)) { \
            return DefaultSingletonManager< Typename >::GetInstance();                                              \
        }

END_NAMESPACE

#endif // !defined(SINGLETON_H_INCLUDED_EA66C745_2EA4_44B5_BFA8_1518092125FF)
