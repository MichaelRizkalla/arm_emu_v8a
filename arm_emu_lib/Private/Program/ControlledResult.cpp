
#include <API/HiddenAPI.h>
#include <Program/ControlledResult.h>
#include <Program/ResultElement.h>
#include <cassert>
#include <memory_resource>

BEGIN_NAMESPACE

class ControlledResult::Impl final {
  public:
    Impl(SharedRef< ResultElement >&& resultElement) : m_resultElement(std::move(resultElement)) {
    }

    Impl(Impl&&)  = default;
    Impl& operator=(Impl&&) = default;

    Impl(const Impl&) = delete;
    Impl& operator=(const Impl&) = delete;

    ~Impl() = default;

    bool IsReady() const noexcept {
        auto state = m_resultElement->GetState();
        return state == IResult::State::Ready || state == IResult::State::Interrupted;
    }

    IResult::State GetState() const noexcept {
        return m_resultElement->GetState();
    }

    std::uint64_t GetGPRegisterValue(std::uint8_t registerLocation) const {
        assert(registerLocation < 30 && registerLocation >= 0);
        return m_resultElement->GetGPRegisterValue(registerLocation);
    }

    std::uint64_t GetPC() const noexcept {
        return m_resultElement->GetPC();
    }

    void WaitReady() {
        m_resultElement->WaitReady();
    }

    void WaitForState(IResult::State state) {
        m_resultElement->WaitForState(state);
    }

    void StepIn() {
        m_resultElement->StepIn();
    }

    bool CanStepIn() {
        return m_resultElement->CanStepIn();
    }

  private:
    SharedRef< ResultElement > m_resultElement;
};

UniqueRef< ControlledResult::Impl > ControlledResult::ConstructResultImpl(SharedRef< ResultElement >&& resultElement) {
    std::pmr::polymorphic_allocator< ControlledResult::Impl > alloc {};

    return allocate_unique< ControlledResult::Impl >(alloc, std::move(resultElement));
}

ControlledResult::ControlledResult(SharedRef< ResultElement > resultElement) :
    m_result(ConstructResultImpl(std::move(resultElement))) {
}

ControlledResult::ControlledResult(ControlledResult&&) noexcept = default;

ControlledResult& ControlledResult::operator=(ControlledResult&&) noexcept = default;

ControlledResult::~ControlledResult() = default;

bool ControlledResult::IsReady() const noexcept {
    return m_result->IsReady();
}

IResult::State ControlledResult::GetState() const noexcept {
    return m_result->GetState();
}

std::uint64_t ControlledResult::GetGPRegisterValue(std::uint8_t registerLocation) const {
    assert(registerLocation < 30 && registerLocation >= 0);
    return m_result->GetGPRegisterValue(registerLocation);
}

std::uint64_t ControlledResult::GetPC() const noexcept {
    return m_result->GetPC();
}

void ControlledResult::WaitReady() {
    m_result->WaitReady();
}

void ControlledResult::WaitForState(IResult::State state) {
    m_result->WaitForState(state);
}

bool ControlledResult::CanStepIn() const noexcept {
    return m_result->CanStepIn();
}

void ControlledResult::StepIn() {
    m_result->StepIn();
}

END_NAMESPACE
