
#include <API/HiddenAPI.h>
#include <Event/AppEvent.h>

BEGIN_NAMESPACE

EventType TerminateEvent::GetEventType() const noexcept {
    return EventType::Terminate;
}

std::underlying_type_t< EventCategory > TerminateEvent::GetEventCategories() const noexcept {
    return static_cast< std::underlying_type_t< EventCategory > >(EventCategory::AppEvent);
}

END_NAMESPACE
