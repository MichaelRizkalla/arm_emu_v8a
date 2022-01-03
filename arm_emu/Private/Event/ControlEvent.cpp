
#include <API/HiddenAPI.h>
#include <Event/ControlEvent.h>

BEGIN_NAMESPACE

EventType ShowProgramControlsEvent::GetEventType() const noexcept {
    return EventType::ShowProgramControls;
}

std::underlying_type_t< EventCategory > ShowProgramControlsEvent::GetEventCategories() const noexcept {
    return static_cast< std::underlying_type_t< EventCategory > >(EventCategory::ControlEvent);
}

EventType HideProgramControlsEvent::GetEventType() const noexcept {
    return EventType::HideProgramControls;
}

std::underlying_type_t< EventCategory > HideProgramControlsEvent::GetEventCategories() const noexcept {
    return static_cast< std::underlying_type_t< EventCategory > >(EventCategory::ControlEvent);
}

END_NAMESPACE
