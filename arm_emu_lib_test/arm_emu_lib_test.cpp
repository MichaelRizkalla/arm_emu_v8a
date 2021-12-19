#if !defined(ARM_EMU_CPP_INCLUDED_A63B7D40_599D_495D_8EBF_C540CBF48585)
    #define ARM_EMU_CPP_INCLUDED_A63B7D40_599D_495D_8EBF_C540CBF48585

    #include <CPU/SystemCreator.h>
    #include <DebugUtils/Log.h>

    #include <chrono>
    #include <thread>

    #include <iostream>

    #include <boost/lexical_cast.hpp>

int main() {
    using namespace arm_emu;
    using namespace std::chrono_literals;

    std::vector< uint64_t > vals {};
    std::vector< uint64_t > pcs {};

    Log::SetLogType(LogType::Instruction);
    Log::SetLogTarget(std::addressof(std::cout));

    {
        // Instruction instr { 0xd65f03c0 };
        SystemSettings sys {};
        sys.CPUType         = CPUType::A64;
        sys.nCores          = 4;
        sys.nThreadsPerCore = 2;
        sys.L1CacheSize     = 256_KB;
        sys.L2CacheSize     = 1_MB;
        sys.L3CacheSize     = 12_MB;
        sys.StackSize       = 12_KB;
        sys.RamSize         = 100_MB;

        constexpr auto v = 12_KB;

        auto m_cpu = SystemCreator::CreateCPU(sys);

        auto prog    = GetTestProgram();
        auto stepper = m_cpu->StepIn(std::move(prog));

        while (!stepper.CanStepIn())
            ;

        while (stepper.CanStepIn()) {
            vals.emplace_back(stepper.GetGPRegisterValue(0));
            pcs.emplace_back(stepper.GetPC());
            stepper.StepIn();
        }
    }
}

#endif // !defined(ARM_EMU_CPP_INCLUDED_A63B7D40_599D_495D_8EBF_C540CBF48585)
