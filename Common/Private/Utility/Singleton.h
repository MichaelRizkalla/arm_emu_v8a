#if !defined(SINGLETON_H_INCLUDED_EA66C745_2EA4_44B5_BFA8_1518092125FF)
    #define SINGLETON_H_INCLUDED_EA66C745_2EA4_44B5_BFA8_1518092125FF

    #include <API/Api.h>
    #include <memory>
    #include <type_traits>

BEGIN_NAMESPACE

template < class Type, class Allocator = std::allocator< Type > >
class DefaultSingletonManager {
    Type m_instance;

  public:
    [[nodiscard]] DefaultSingletonManager() noexcept = default;
    DELETE_COPY_CLASS(DefaultSingletonManager)
    DELETE_MOVE_CLASS(DefaultSingletonManager)

    [[nodiscard]] Type& GetInstance() noexcept {
        return m_instance;
    }
};

template < class Type, class Allocator = std::allocator< Type > >
class SingletonManager {
    Type m_instance;

  public:
    template < class... Args, class Enable = std::enable_if_t< std::is_constructible_v< Type, Args... > > >
    [[nodiscard]] SingletonManager(Args&&... args) : m_instance(std::forward< Args >(args)...) {
    }
    DELETE_COPY_CLASS(SingletonManager)
    DELETE_MOVE_CLASS(SingletonManager)

    [[nodiscard]] Type& GetInstance() noexcept {
        return m_instance;
    }
};

// In header file use:
    #define DECLARE_AS_SINGLETON(Typename)                             \
      private:                                                         \
        static DefaultSingletonManager< Typename > m_singletonManager; \
        friend decltype(m_singletonManager);                           \
                                                                       \
      public:                                                          \
        [[nodiscard]] static Typename& GetInstance() noexcept(noexcept(m_singletonManager.GetInstance()));

    #define DECLARE_AS_PARAMETERED_SINGLETON(Typename)          \
      private:                                                  \
        static SingletonManager< Typename > m_singletonManager; \
        friend decltype(m_singletonManager);                    \
                                                                \
      public:                                                   \
        [[nodiscard]] static Typename& GetInstance() noexcept(noexcept(m_singletonManager.GetInstance()));

// In source file use:
// #define IMPLEMENT_AS_SINGLETON(Typename) decltype(Typename::m_singletonManager) Typename::m_singletonManager {};

    #define IMPLEMENT_AS_SINGLETON(Typename, ...)                                                          \
        decltype(Typename::m_singletonManager) Typename::m_singletonManager { __VA_ARGS__ };               \
                                                                                                           \
        Typename& Typename::GetInstance() noexcept(noexcept(Typename::m_singletonManager.GetInstance())) { \
            return m_singletonManager.GetInstance();                                                       \
        }

END_NAMESPACE

#endif // !defined(SINGLETON_H_INCLUDED_EA66C745_2EA4_44B5_BFA8_1518092125FF)
