
#include <API/HiddenAPI.h>
#include <Event/CPUEvent.h>

BEGIN_NAMESPACE

CreateCPUEvent::CreateCPUEvent(SystemSettings settings) : m_settings(settings) {
}

EventType CreateCPUEvent::GetEventType() const noexcept {
    return EventType::CreateCPU;
}

std::underlying_type_t< EventCategory > CreateCPUEvent::GetEventCategories() const noexcept {
    return static_cast< std::underlying_type_t< EventCategory > >(EventCategory::CPUEvent);
}

SystemSettings CreateCPUEvent::GetSystemSettings() const noexcept {
    return m_settings;
}

CPUCreationSuccessEvent::CPUCreationSuccessEvent(SystemSettings settings) : m_settings(settings) {
}

EventType CPUCreationSuccessEvent::GetEventType() const noexcept {
    return EventType::CPUCreationSuccessful;
}

std::underlying_type_t< EventCategory > CPUCreationSuccessEvent::GetEventCategories() const noexcept {
    return static_cast< std::underlying_type_t< EventCategory > >(EventCategory::CPUEvent);
}

SystemSettings CPUCreationSuccessEvent::GetSystemSettings() const noexcept {
    return m_settings;
}

CPUCreationFailureEvent::CPUCreationFailureEvent(std::string failureMessage) : m_failureMessage(failureMessage) {
}

EventType CPUCreationFailureEvent::GetEventType() const noexcept {
    return EventType::CPUCreationFailure;
}

std::underlying_type_t< EventCategory > CPUCreationFailureEvent::GetEventCategories() const noexcept {
    return static_cast< std::underlying_type_t< EventCategory > >(EventCategory::CPUEvent);
}

std::string CPUCreationFailureEvent::GetMessage() const noexcept {
    return m_failureMessage;
}

END_NAMESPACE
