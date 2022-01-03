#if !defined(APPEVENT_H_INCLUDED_EEE5ADF1_F0E8_4EB4_9D08_2B29411A51CE)
    #define APPEVENT_H_INCLUDED_EEE5ADF1_F0E8_4EB4_9D08_2B29411A51CE

    #include <Event/IEvent.h>

namespace arm_emu {

    struct TerminateEvent : public IEvent {
        EventType                               GetEventType() const noexcept override;
        std::underlying_type_t< EventCategory > GetEventCategories() const noexcept override;
    };

} // namespace arm_emu

#endif // !defined(APPEVENT_H_INCLUDED_EEE5ADF1_F0E8_4EB4_9D08_2B29411A51CE)
