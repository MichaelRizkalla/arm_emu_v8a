
#include <Program/ResultElement.h>
#include <cassert>

BEGIN_NAMESPACE

ResultElement::~ResultElement() {
    if (m_stepInCondVar) {
        m_stepInSource->Trigger();
        m_stepInCondVar->notify_all();
    }
}

void ResultElement::Signal(IResult::State state) noexcept {
    m_state = state;
    m_condVar.notify_all();
}

void ResultElement::StoreGPRegisters(std::array< std::bitset< 64 >, 31 > registerData) noexcept {
    m_data = registerData;
}

IResult::State ResultElement::GetState() const noexcept {
    return m_state;
}

std::uint64_t ResultElement::GetGPRegisterValue(std::uint8_t registerLocation) const {
    assert(registerLocation < 30 && registerLocation >= 0);
    return m_data[registerLocation].to_ullong();
}

void ResultElement::WaitReady() {
    std::unique_lock lock { m_mutex };
    m_condVar.wait(lock, [&]() { return m_state == IResult::State::Ready; });
}

void ResultElement::WaitForState(IResult::State state) {
    std::unique_lock lock { m_mutex };
    m_condVar.wait(lock, [&]() { return m_state == state; });
}

void ResultElement::StepInSetup(std::condition_variable_any& condVar) noexcept {
    m_stepInCondVar = &condVar;
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
    if (m_stepInCondVar) {
        m_stepInSource->Trigger();
        m_stepInCondVar->notify_one();
    }
}

END_NAMESPACE
