#pragma once
#include <cstdint>
#include <limits>
#include <type_traits>

namespace arm_emu {

	enum class Exception : std::uint32_t {
        Uncategorized, // Uncategorized or unknown reason
        WFxTrap, // Trapped WFI or WFE instruction
        CP15RTTrap, // Trapped AArch32 MCR or MRC access to CP15
        CP15RRTTrap, // Trapped AArch32 MCRR or MRRC access to CP15
        CP14RTTrap, // Trapped AArch32 MCR or MRC access to CP14
        CP14DTTrap, // Trapped AArch32 LDC or STC access to CP14
        AdvSIMDFPAccessTrap, // HCPTR-trapped access to SIMD or FP
        FPIDTrap, // Trapped access to SIMD or FP ID register Trapped BXJ instruction not supported in Armv8
        PACTrap, // Trapped invalid PAC use
        CP14RRTTrap, // Trapped MRRC access to CP14 from AArch32
        IllegalState, // Illegal Execution state
        SupervisorCall, // Supervisor Call
        HypervisorCall, // Hypervisor Call
        MonitorCall, // Monitor Call or Trapped SMC instruction
        SystemRegisterTrap, // Trapped MRS or MSR system register access
        ERetTrap, // Trapped invalid ERET use
        InstructionAbort, // Instruction Abort or Prefetch Abort
        PCAlignment, // PC alignment fault
        DataAbort, // Data Abort
        NV2DataAbort, // Data abort at EL1 reported as being from EL2
        PACFail, // PAC Authentication failure
        SPAlignment, // SP alignment fault
        FPTrappedException, // IEEE trapped FP exception
        SError, // SError interrupt
        Breakpoint, // (Hardware) Breakpoint
        SoftwareStep, // Software Step
        Watchpoint, // Watchpoint
        NV2Watchpoint, // Watchpoint at EL1 reported as being from EL2
        SoftwareBreakpoint, // Software Breakpoint Instruction
        VectorCatch, // AArch32 Vector Catch
        IRQ, // IRQ interrupt
        SVEAccessTrap, // HCPTR trapped access to SVE
        BranchTarget, // Branch Target Identification
        FIQ, // FIQ interrupt

		ARM_EMU_COUNT,

		Undefined = std::numeric_limits< std::underlying_type_t< Exception > >::max(),
	};

}