
#include <API/HiddenAPI.h>
#include <Event/ProgramEvent.h>

BEGIN_NAMESPACE

LoadProgramEvent::LoadProgramEvent(CompilationResult::ObjectData objectData, Program::EntryPoint entryPoint) :
    m_objectData(objectData), m_entryPoint(entryPoint) {
}

EventType LoadProgramEvent::GetEventType() const noexcept {
    return EventType::LoadProgram;
}

std::underlying_type_t< EventCategory > LoadProgramEvent::GetEventCategories() const noexcept {
    return static_cast< std::underlying_type_t< EventCategory > >(EventCategory::ProgramEvent);
}

const CompilationResult::ObjectData& LoadProgramEvent::GetObjectData() const noexcept {
    return m_objectData;
}

Program::EntryPoint LoadProgramEvent::GetEntryPoint() const noexcept {
    return m_entryPoint;
}

LoadProgramSuccessEvent::LoadProgramSuccessEvent(IResult* result) : m_result(std::move(result)) {
}

EventType LoadProgramSuccessEvent::GetEventType() const noexcept {
    return EventType::LoadProgramSuccessful;
}

std::underlying_type_t< EventCategory > LoadProgramSuccessEvent::GetEventCategories() const noexcept {
    return static_cast< std::underlying_type_t< EventCategory > >(EventCategory::ProgramEvent);
}

IResult* LoadProgramSuccessEvent::GetResult() const noexcept {
    return m_result;
}

LoadProgramFailureEvent::LoadProgramFailureEvent(std::string failureMessage) :
    m_failureMessage(std::move(failureMessage)) {
}

EventType LoadProgramFailureEvent::GetEventType() const noexcept {
    return EventType::LoadProgramFailure;
}

std::underlying_type_t< EventCategory > LoadProgramFailureEvent::GetEventCategories() const noexcept {
    return static_cast< std::underlying_type_t< EventCategory > >(EventCategory::ProgramEvent);
}

const std::string& LoadProgramFailureEvent::GetFailureMessage() const noexcept {
    return m_failureMessage;
}

EventType StepInProgramAvailableEvent::GetEventType() const noexcept {
    return EventType::StepInProgramAvailable;
}

std::underlying_type_t< EventCategory > StepInProgramAvailableEvent::GetEventCategories() const noexcept {
    return static_cast< std::underlying_type_t< EventCategory > >(EventCategory::ProgramEvent);
}

EventType StepInProgramEvent::GetEventType() const noexcept {
    return EventType::StepInProgram;
}

std::underlying_type_t< EventCategory > StepInProgramEvent::GetEventCategories() const noexcept {
    return static_cast< std::underlying_type_t< EventCategory > >(EventCategory::ProgramEvent);
}

EventType StepInProgramFinishedEvent::GetEventType() const noexcept {
    return EventType::StepInProgramFinished;
}

std::underlying_type_t< EventCategory > StepInProgramFinishedEvent::GetEventCategories() const noexcept {
    return static_cast< std::underlying_type_t< EventCategory > >(EventCategory::ProgramEvent);
}

END_NAMESPACE