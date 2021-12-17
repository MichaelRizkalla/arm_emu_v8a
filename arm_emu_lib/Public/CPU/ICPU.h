#if !defined(ICPU_H_INCLUDED_50521AF1_4ED0_4252_B164_15832D479C6D)
    #define ICPU_H_INCLUDED_50521AF1_4ED0_4252_B164_15832D479C6D

    #include <API/Api.h>
    #include <Memory/IMemory.h>
    #include <Program/ControlledResult.h>
    #include <Program/Program.h>
    #include <Program/Result.h>
    #include <vector>

namespace arm_emu {

    enum class ArchitectureProfile
    {
    };
    enum class InstructionSet
    {
    };
    enum class ExtensionVersion
    {
    };
    enum class Extension
    {
    };
    enum class Feature
    {
    };
    enum class ExceptionLevel
    {
    };

    /// <summary>
    /// CPUDebugger will be used to extract MemoryWatcher and ProcessingUnitWatcher as well as peeking into cache memory
    /// </summary>
    class CPUDebugger;

    struct ICPU {
      public:
        ICPU()        = default;
        ICPU(ICPU&&)  = default;
        ICPU& operator=(ICPU&&) = default;
        virtual ~ICPU()         = default;

        ICPU(const ICPU&) = delete;
        ICPU& operator=(const ICPU&) = delete;

        virtual Result           Run(Program program)    = 0;
        virtual ControlledResult StepIn(Program program) = 0;
        virtual void             Stop()                  = 0;

        [[nodiscard]] virtual std::uint8_t GetCoreCount() const noexcept           = 0;
        [[nodiscard]] virtual std::uint8_t GetThreadsPerCoreCount() const noexcept = 0;

        [[nodiscard]] virtual ArchitectureProfile                 GetExecutionState() const noexcept          = 0;
        [[nodiscard]] virtual InstructionSet                      GetInstructionSet() const noexcept          = 0;
        [[nodiscard]] virtual ExtensionVersion                    GetCurrentExtensionVersion() const noexcept = 0;
        [[nodiscard]] virtual const std::pmr::vector< Extension > GetSupportedExtensions() const noexcept     = 0;
        [[nodiscard]] virtual bool IsFeatureSupported(Feature feature) const noexcept                         = 0;
        [[nodiscard]] virtual bool IsExceptionSupported(ExceptionLevel exceptionLevel) const noexcept         = 0;

        friend class CPUDebugger;
    };

} // namespace arm_emu

#endif // !defined(ICPU_H_INCLUDED_50521AF1_4ED0_4252_B164_15832D479C6D)
