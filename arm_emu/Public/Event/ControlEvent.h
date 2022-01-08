#if !defined(CONTROLEVENT_H_INCLUDED_BC290638_D01C_47AC_80B0_E7E50EC205BA)
    #define CONTROLEVENT_H_INCLUDED_BC290638_D01C_47AC_80B0_E7E50EC205BA

    #include <Event/IEvent.h>

namespace arm_emu {

    struct ShowProgramControlsEvent : public IEvent {
        EventType                               GetEventType() const noexcept override;
        std::underlying_type_t< EventCategory > GetEventCategories() const noexcept override;
    };

    struct HideProgramControlsEvent : public IEvent {
        EventType                               GetEventType() const noexcept override;
        std::underlying_type_t< EventCategory > GetEventCategories() const noexcept override;
    };

} // namespace arm_emu

#endif // !defined(CONTROLEVENT_H_INCLUDED_BC290638_D01C_47AC_80B0_E7E50EC205BA)
