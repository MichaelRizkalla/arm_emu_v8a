
#include <Program/ResultElement.h>
#include <cassert>

BEGIN_NAMESPACE

ResultElement::ResultElement(bool isStepInAllowed) : m_isStepInAllowed(isStepInAllowed) {
}

ResultElement::~ResultElement() {
    if (m_stepInCondVar) {
        m_stepInSource->Trigger();
        m_stepInCondVar->notify_all();
    }
    if (m_steppedInInterrupt && !m_steppedInInterrupt->IsTriggered()) {
        m_steppedInInterrupt->Trigger();
    }
}

void ResultElement::Signal(IResult::State state) noexcept {
    m_state = state;
    m_condVar.notify_all();
}

void ResultElement::StoreGPRegisters(GPRegisters::Arch64Registers registerData) noexcept {
    m_data = registerData;
}

void ResultElement::StorePC(std::uint64_t programCounter) noexcept {
    m_programCounter = programCounter;
}

IResult::State ResultElement::GetState() const noexcept {
    return m_state;
}

std::uint64_t ResultElement::GetGPRegisterValue(std::uint8_t registerLocation) const {
    assert(registerLocation < 30 && registerLocation >= 0);
    return m_data[registerLocation].to_ullong();
}

std::uint64_t ResultElement::GetPC() const noexcept {
    return m_programCounter;
}

void ResultElement::WaitReady() {
    std::unique_lock lock { m_mutex };
    m_condVar.wait(lock, [&]() { return m_state == IResult::State::Ready; });
}

void ResultElement::WaitForState(IResult::State state) {
    std::unique_lock lock { m_mutex };
    m_condVar.wait(lock, [&]() { return m_state == state; });
}

bool ResultElement::CanStepIn() const noexcept {
    return m_isStepInAllowed && static_cast< bool >(m_steppedInInterrupt) && static_cast< bool >(m_stepInCondVar) &&
           (m_state == IResult::State::StepInMode);
}

std::condition_variable_any& ResultElement::StepInSetup(std::condition_variable_any& condVarToTrigger,
                                                        Interrupt                    stepInDone) noexcept {
    m_stepInCondVar      = &condVarToTrigger;
    m_steppedInInterrupt = std::move(stepInDone);

    return m_steppedIn;
}

void ResultElement::StepInFinalize() noexcept {
    m_stepInCondVar = nullptr;
}

Interrupt ResultElement::GetStepInInterrupt() noexcept { /* The caller guarantees only one step_token is used per
                                                            ResultElement */
    if (m_stepInSource->IsTriggered()) {
        m_stepInSource->Reset();
    }

    return m_stepInSource;
}

void ResultElement::StepIn() {
    m_steppedInInterrupt->Reset();

    if (m_stepInCondVar) {
        m_stepInSource->Trigger();
        m_stepInCondVar->notify_one();
    }

    std::unique_lock lock { m_mutex };
    m_steppedIn.wait(lock, [&]() { return m_steppedInInterrupt->IsTriggered(); });
}

void ResultElement::SignalStepInValidity(bool isStepInAllowed) noexcept {
    m_isStepInAllowed = isStepInAllowed;
}

END_NAMESPACE
