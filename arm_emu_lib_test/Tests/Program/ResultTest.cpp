
#include <Tests/Program/ResultTest.h>

BEGIN_NAMESPACE

namespace test {

    ResultTest::ResultTest() = default;

    ResultTest::~ResultTest() = default;

    void ResultTest::CheckResultElement(bool stepIn) {
        using namespace arm_emu;

        // Arbitrary values for test
        constexpr std::uint64_t Reg1  = 14;
        constexpr std::uint64_t Reg4  = 29;
        constexpr std::uint64_t PCVal = 235;

        m_resultElement = std::make_shared< ResultElement >(stepIn);

        m_resultElement->Signal(IResult::State::Running);
        ASSERT_EQ(m_resultElement->GetState(), IResult::State::Running);

        ASSERT_EQ(m_resultElement->CanStepIn(), false);

        m_resultElement->Signal(IResult::State::StepInMode);
        ASSERT_EQ(m_resultElement->GetState(), IResult::State::StepInMode);

        ASSERT_EQ(m_resultElement->CanStepIn(), false);

        if (stepIn) { // simulate a step in
            std::condition_variable_any condVar {};
            auto                        interrupt = CreateInterrupt();

            auto& stepInCondVar = m_resultElement->StepInSetup(condVar, interrupt);

            ASSERT_EQ(m_resultElement->CanStepIn(), stepIn);

            interrupt->Trigger();
            auto trigger = std::thread { [&]() {
                std::mutex mMutex {};
                std::unique_lock lock { mMutex };

                condVar.wait(lock, [&]() { return !interrupt->IsTriggered(); });
                interrupt->Trigger();
                stepInCondVar.notify_one();
            } };
            m_resultElement->StepIn();

            m_resultElement->StepInFinalize();

            ASSERT_EQ(m_resultElement->CanStepIn(), false);

            if (trigger.joinable()) {
                trigger.join();
            }
        }

        m_resultElement->Signal(IResult::State::Waiting);
        ASSERT_EQ(m_resultElement->GetState(), IResult::State::Waiting);

        ASSERT_EQ(m_resultElement->GetGPRegisterValue(0), 0);

        GPRegisters::Arch64Registers registers {};
        registers.at(1) = Reg1;
        registers.at(4) = Reg4;

        m_resultElement->StoreGPRegisters(registers);
        m_resultElement->StorePC(PCVal);

        ASSERT_EQ(m_resultElement->GetState(), IResult::State::Waiting);
        ASSERT_EQ(m_resultElement->GetGPRegisterValue(1), Reg1);
        ASSERT_EQ(m_resultElement->GetGPRegisterValue(4), Reg4);
        ASSERT_EQ(m_resultElement->GetGPRegisterValue(5), 0);
        ASSERT_NE(m_resultElement->GetGPRegisterValue(10), ~0);
        ASSERT_EQ(m_resultElement->GetPC(), PCVal);
    }

    void ResultTest::CheckResult() {
    }

    void ResultTest::CheckControlledResult() {
    }

    TEST_F(ResultTest, TestResultElement) {
        CheckResultElement(true);
        CheckResultElement(false);
    }

    TEST_F(ResultTest, TestResult) {
        CheckResult();
    }

    TEST_F(ResultTest, TestControlledResult) {
        CheckControlledResult();
    }

} // namespace test

END_NAMESPACE
