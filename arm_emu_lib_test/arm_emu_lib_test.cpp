#if !defined(ARM_EMU_CPP_INCLUDED_A63B7D40_599D_495D_8EBF_C540CBF48585)
    #define ARM_EMU_CPP_INCLUDED_A63B7D40_599D_495D_8EBF_C540CBF48585

    #include <CPU/SystemCreator.h>
    #include <DebugUtils/Log.h>
    #include <Memory/IMemory.h>

    #include <iostream>

int main() {
    arm_emu::Log::SetLogType(arm_emu::LogType::Instruction);
    arm_emu::Log::SetLogTarget(std::addressof(std::cout));

    {
        using namespace arm_emu::literals;

        // Instruction instr { 0xd65f03c0 };
        arm_emu::SystemSettings sys {};
        sys.CPUType         = arm_emu::CPUType::A64;
        sys.nCores          = 4;
        sys.nThreadsPerCore = 2;
        sys.L1CacheSize     = 256_KB;
        sys.L2CacheSize     = 1_MB;
        sys.L3CacheSize     = 12_MB;
        sys.StackSize       = 12_KB;
        sys.RamSize         = 100_MB;

        constexpr auto v = 12_KB;

        auto m_cpu = arm_emu::SystemCreator::CreateCPU(sys);

        auto prog = arm_emu::GetTestProgram();

        auto stepper = m_cpu->StepIn(std::move(prog));

        while (!stepper.CanStepIn())
            ;

        while (stepper.CanStepIn()) {
            stepper.StepIn();
        }
    }
}

#endif // !defined(ARM_EMU_CPP_INCLUDED_A63B7D40_599D_495D_8EBF_C540CBF48585)
