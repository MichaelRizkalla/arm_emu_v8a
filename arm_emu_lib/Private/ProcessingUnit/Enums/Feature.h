#if !defined(FEATURE_H_INCLUDED_A4432685_0441_4050_A18A_B2C1CD02178C)
    #define FEATURE_H_INCLUDED_A4432685_0441_4050_A18A_B2C1CD02178C

    #include <API/Api.h>
    #include <cstdint>

BEGIN_NAMESPACE

enum class IProcessingUnit::Feature : std::uint32_t
{
    FEAT_SB,            // Speculation Barrier
    FEAT_SSBS,          // Speculative Store Bypass Safe
    FEAT_CSV2,          // Cache Speculation Variant 2
    FEAT_CSV2_2,        // Cache Speculation Variant 2
    FEAT_CSV2_1p1,      // Cache Speculation Variant 2
    EAT_CSV2_1p2,       // Cache Speculation Variant 2
    FEAT_CSV3,          // Cache Speculation Variant 3
    FEAT_SPECRES,       // Speculation restriction instructions
    FEAT_CP15SDISABLE2, //
    CP15SDISABLE2,      //
    FEAT_DoubleLock,    // Double Lock
    FEAT_DGH,           // Data Gathering Hint
    FEAT_ETS,           // Enhanced Translation Synchronization
    FEAT_nTLBPA,        // Intermediate caching of translation table walks
    FEAT_PCSRv8,        // PC Sample-based Profiling Extension

    // Armv8 Cryptographic Extension
    FEAT_AES,
    FEAT_PMULL,
    FEAT_SHA1,
    FEAT_SHA256,

    FEAT_RAS,   // The Reliability, Availability, and Serviceability Extension
    FEAT_SPE,   // The Statistical Profiling Extension (SPE)
    FEAT_SVE,   // The Scalable Vector Extension (SVE)
    FEAT_AMUv1, // The Activity Monitors Extension (AMU)
    FEAT_MPAM,  // The Memory Partitioning and Monitoring (MPAM) Extension

    // Armv8.2 extensions to the Cryptographic Extension
    FEAT_SHA512, // Advanced SIMD SHA512 instructions
    FEAT_SHA3,   // Advanced SIMD SHA3 instructions
    FEAT_SM3,    // Advanced SIMD SM3 instructions
    FEAT_SM4,    // Advanced SIMD SM4 instructions

    // Armv8.1 architecture extension
    FEAT_LSE,     // Large System Extensions
    FEAT_RDM,     // Advanced SIMD rounding double multiply accumulate instructions
    FEAT_LOR,     // Limited ordering regions
    FEAT_HPDS,    // Hierarchical permission disables
    FEAT_HAFDBS,  // Hardware management of the Access flag and dirty state
    FEAT_PAN,     // Privileged access never
    FEAT_VMID16,  // 16-bit VMID
    FEAT_VHE,     // Virtualization Host Extensions
    FEAT_PMUv3p1, // PMU Extensions v3.1
    FEAT_PAN3,    // Support for SCTLR_ELx.EPAN

    // Armv8.2 architecture extension
    FEAT_ASMv8p2,   // Armv8.2 changes to the A64 ISA
    FEAT_PAN2,      // AT S1E1R and AT S1E1W instruction variants affected by PSTATE.PAN
    FEAT_FP16,      // Half-precision floating-point data processing
    FEAT_DotProd,   // Advanced SIMD dot product instructions
    FEAT_FHM,       // Floating-point half-precision multiplication instructions
    FEAT_LSMAOC,    // AArch32 Load/Store Multiple instruction atomicity and ordering controls
    FEAT_UAO,       // Unprivileged Access Override control
    FEAT_DPB,       // DC CVAP instruction
    FEAT_VPIPT,     // VMID-aware PIPT instruction cache
    FEAT_AA32HPD,   // AArch32 hierarchical permission disables
    FEAT_HPDS2,     // Translation table page-based hardware attributes
    FEAT_LPA,       // Large PA and IPA support
    FEAT_LVA,       // Large VA support
    FEAT_TTCNP,     // Translation table Common not private translations
    FEAT_XNX,       // Translation table stage 2 Unprivileged Execute - never
    FEAT_Debugv8p2, // Debug v8.2
    FEAT_PCSRv8p2,  // PC Sample-based profiling
    FEAT_IESB,      // Implicit Error Synchronization event
    FEAT_EVT,       // Enhanced Virtualization Traps
    FEAT_DPB2,      // DC CVADP instruction
    FEAT_BF16,      // AArch64 BFloat16 instructions
    FEAT_AA32BF16,  // AArch32 BFloat16 instructions
    FEAT_I8MM,      // AArch64 Int8 matrix multiplication instructions
    FEAT_AA32I8MM,  // AArch32 Int8 matrix multiplication instructions

    // Armv8.3 architecture extension
    FEAT_FCMA,    // Floating-point complex number instructions
    FEAT_JSCVT,   // JavaScript conversion instructions
    FEAT_LRCPC,   // Load-acquire RCpc instructions
    FEAT_NV,      // Nested virtualization support
    FEAT_CCIDX,   // Extended cache index
    FEAT_PAuth,   // Pointer authentication
    FEAT_SPEv1p1, // Armv8.3 Statistical Profiling Extensions
    FEAT_DoPD,    // Debug over Powerdown
    FEAT_PAuth2,  // Enhancements to pointer authentication
    FEAT_FPAC,    // Faulting on AUT* instructions

    // Armv8.4 architecture extension
    FEAT_DIT,         // Data Independent Timing instructions
    FEAT_FlagM,       // Flag manipulation instructions v2
    FEAT_LRCPC2,      // Load-acquire RCpc instructions v2
    FEAT_LSE2,        // Large System Extensions v2
    FEAT_TLBIOS,      // TLB invalidate instructions in Outer Shareable domain
    FEAT_TLBIRANGE,   // TLB invalidate range instructions
    FEAT_TTL,         // Translation Table Level
    FEAT_S2FWB,       // Stage 2 forced Write-Back
    FEAT_TTST,        // Small translation tables
    FEAT_BBM,         // Translation table break-before-make levels
    FEAT_SEL2,        // Secure EL2
    FEAT_NV2,         // Enhanced nested virtualization support
    FEAT_IDST,        // ID space trap handling
    FEAT_CNTSC,       // Generic Counter Scaling
    FEAT_Debugv8p4,   // Debug v8 .4
    FEAT_TRF,         // Self-hosted Trace Extensions
    FEAT_PMUv3p4,     // PMU Extensions v3.4
    FEAT_RASv1p1,     // RAS Extension v1.1
    FEAT_DoubleFault, // Double Fault Extension

    // Armv8.5 architecture extension
    FEAT_FlagM2,  // Enhancements to flag manipulation instructions
    FEAT_FRINTTS, // Floating-point to integer instructions
    FEAT_ExS,     // Context synchronization and exception handling
    FEAT_GTG,     // Guest translation granule size
    FEAT_BTI,     // Branch Target Identification
    FEAT_E0PD,    // Preventing EL0 access to halves of address maps
    FEAT_RNG,     // Random number generator
    FEAT_MTE,     // Memory Tagging Extension
    FEAT_MTE2,    // Memory Tagging Extension
    FEAT_PMUv3p5, // PMU Extensions v3.5
    FEAT_MTE3,    // MTE Asymmetric Fault Handling

    // Armv8.6 architecture extension
    FEAT_ECV,     // Enhanced Counter Virtualization
    FEAT_FGT,     // Fine Grain Traps
    FEAT_TWED,    // Delayed Trapping of WFE
    FEAT_AMUv1p1, // AMU Extensions v1.1
    FEAT_MTPMU,   // Multi-threaded PMU Extensions

    // Armv8.7 architecture extension
    FEAT_AFP,          // Alternate floating-point behavior
    FEAT_RPRES,        // Increased precision of Reciprocal Estimate and Reciprocal Square Root Estimate
    FEAT_LS64,         // Support for 64 byte loads/stores
    FEAT_LS64_V,       // Support for 64 byte loads/stores
    FEAT_LS64_ACCDATA, // Support for 64 byte loads/stores
    FEAT_WFxT,         // WFE and WFI instructions with timeout
    FEAT_HCX,          // Support for the HCRX_EL2 register
    FEAT_LPA2,         // Larger physical address for 4KB and 16KB translation granules
    FEAT_XS,           // XS attribute
    FEAT_PMUv3p7,      // Armv8.7 PMU extensions
    FEAT_SPEv1p2,      // Armv8.7 SPE features
};

END_NAMESPACE

#endif // !defined(FEATURE_H_INCLUDED_A4432685_0441_4050_A18A_B2C1CD02178C)
