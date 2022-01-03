
#include <API/HiddenAPI.h>
#include <Event/InputEvent.h>

BEGIN_NAMESPACE

EventType ClearInputEvent::GetEventType() const noexcept {
    return EventType::ClearInput;
}

std::underlying_type_t< EventCategory > ClearInputEvent::GetEventCategories() const noexcept {
    return static_cast< std::underlying_type_t< EventCategory > >(EventCategory::UIInputEvent);
}

EventType SaveInputEvent::GetEventType() const noexcept {
    return EventType::SaveInput;
}

std::underlying_type_t< EventCategory > SaveInputEvent::GetEventCategories() const noexcept {
    return static_cast< std::underlying_type_t< EventCategory > >(EventCategory::UIInputEvent);
}

EventType LoadInputFromFileEvent::GetEventType() const noexcept {
    return EventType::LoadInputFromFile;
}

std::underlying_type_t< EventCategory > LoadInputFromFileEvent::GetEventCategories() const noexcept {
    return static_cast< std::underlying_type_t< EventCategory > >(EventCategory::UIInputEvent);
}

END_NAMESPACE
