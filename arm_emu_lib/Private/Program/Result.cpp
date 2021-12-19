
#include <API/HiddenAPI.h>
#include <Program/Result.h>
#include <Program/ResultElement.h>
#include <cassert>
#include <memory_resource>

BEGIN_NAMESPACE

class Result::Impl final {
  public:
    Impl(std::shared_ptr< ResultElement >&& resultElement) : m_resultElement(std::move(resultElement)) {
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

  private:
    std::shared_ptr< ResultElement > m_resultElement;
};

UniqueRef< Result::Impl > Result::ConstructResultImpl(std::shared_ptr< ResultElement >&& resultElement) {
    std::pmr::polymorphic_allocator< Result::Impl > alloc {};

    return allocate_unique< Result::Impl >(alloc, std::move(resultElement));
}

Result::Result(std::shared_ptr< ResultElement > resultElement) :
    m_result(ConstructResultImpl(std::move(resultElement))) {
}

Result::Result(Result&&) noexcept = default;

Result& Result::operator=(Result&&) noexcept = default;

Result::~Result() = default;

bool Result::IsReady() const noexcept {
    return m_result->IsReady();
}

IResult::State Result::GetState() const noexcept {
    return m_result->GetState();
}

std::uint64_t Result::GetGPRegisterValue(std::uint8_t registerLocation) const {
    assert(registerLocation < 30 && registerLocation >= 0);
    return m_result->GetGPRegisterValue(registerLocation);
}

std::uint64_t Result::GetPC() const noexcept {
    return m_result->GetPC();
}

void Result::WaitReady() {
    m_result->WaitReady();
}

void Result::WaitForState(Result::State state) {
    m_result->WaitForState(state);
}

bool Result::CanStepIn() const noexcept {
    return false;
}

END_NAMESPACE
