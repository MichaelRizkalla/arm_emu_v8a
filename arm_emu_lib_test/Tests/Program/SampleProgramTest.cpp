
#include <CPU/SystemCreator.h>
#include <DebugUtils/Log.h>
#include <Tests/Program/SampleProgramTest.h>

BEGIN_NAMESPACE

namespace test {

    SampleProgramTest::SampleProgramTest() = default;

    SampleProgramTest::~SampleProgramTest() = default;

    void SampleProgramTest::CheckSampleProgram(std::uint64_t programNumber) {
        using namespace arm_emu::literals;

        arm_emu::SystemSettings sys {};
        sys.cpuType         = arm_emu::CPUType::A64;
        sys.nCores          = 4;
        sys.nThreadsPerCore = 2;
        sys.L1CacheSize     = 256_KB;
        sys.L2CacheSize     = 1_MB;
        sys.L3CacheSize     = 12_MB;
        sys.StackSize       = 12_KB;
        sys.RamSize         = 100_MB;

        auto m_cpu    = arm_emu::SystemCreator::CreateCPU(sys);
        auto prog     = arm_emu::test::GetSampleProgram(programNumber);
        auto stepCtrl = m_cpu->StepIn(std::move(prog));

        while (!stepCtrl.CanStepIn())
            ;
        while (stepCtrl.CanStepIn()) {
            stepCtrl.StepIn();
        }

        auto resultFrame = stepCtrl.GetResultFrame();

        ASSERT_EQ(resultFrame.GetGPRegisterValue(0), 5);
        ASSERT_EQ(resultFrame.GetPC(), std::numeric_limits< std::uint64_t >::max());
    }

    TEST_F(SampleProgramTest, RunSampleProgram0) {
        CheckSampleProgram(0);
    }

} // namespace test

END_NAMESPACE
