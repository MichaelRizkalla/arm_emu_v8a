#if !defined(INPUTEVENT_H_INCLUDED_11C931A6_7537_4468_B12E_93903A5253EA)
    #define INPUTEVENT_H_INCLUDED_11C931A6_7537_4468_B12E_93903A5253EA

    #include <Event/IEvent.h>

namespace arm_emu {

    struct ClearInputEvent : public IEvent {
        EventType                               GetEventType() const noexcept override;
        std::underlying_type_t< EventCategory > GetEventCategories() const noexcept override;
    };

    struct SaveInputEvent : public IEvent {
        EventType                               GetEventType() const noexcept override;
        std::underlying_type_t< EventCategory > GetEventCategories() const noexcept override;
    };

    struct LoadInputFromFileEvent : public IEvent {
        EventType                               GetEventType() const noexcept override;
        std::underlying_type_t< EventCategory > GetEventCategories() const noexcept override;
    };

} // namespace arm_emu

#endif // !defined(INPUTEVENT_H_INCLUDED_11C931A6_7537_4468_B12E_93903A5253EA)
