#if !defined(INTERRUPT_H_INCLUDED_872A6495_498C_42D0_A712_936D80859C7C)
    #define INTERRUPT_H_INCLUDED_872A6495_498C_42D0_A712_936D80859C7C

    #include <API/Api.h>
    #include <Utility/UniqueRef.h>
    #include <atomic>
    #include <memory>

BEGIN_NAMESPACE

struct Interrupt_ {
    DEFAULT_CONSTEXPR_CTOR(Interrupt_)
    DELETE_COPY_CLASS(Interrupt_)
    DELETE_MOVE_CLASS(Interrupt_)
    DEFAULT_CONSTEXPR_DTOR(Interrupt_)

    void Trigger() noexcept;

    void Reset() noexcept;

    bool IsTriggered() const noexcept;

  private:
    std::atomic< bool > m_interrupted { false };
};

using Interrupt = SharedRef< Interrupt_ >;

Interrupt CreateInterrupt();

END_NAMESPACE

#endif // !defined(INTERRUPT_H_INCLUDED_872A6495_498C_42D0_A712_936D80859C7C)
