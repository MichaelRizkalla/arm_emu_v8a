#if !defined(IPROCESSINGUNIT_H_INCLUDED_23E7732A_6223_466F_8087_945323852C58)
    #define IPROCESSINGUNIT_H_INCLUDED_23E7732A_6223_466F_8087_945323852C58

    #include <API/Api.h>
    #include <DebugUtils/Object.h>
    #include <Memory/ICacheMemory.h>
    #include <Memory/IMemory.h>
    #include <ProcessingUnit/IProcessingUnitWatcher.h>
    #include <Program/ControlledResult.h>
    #include <Program/Program.h>
    #include <Program/Result.h>
    #include <cstdint>
    #include <vector>

BEGIN_NAMESPACE

class [[nodiscard]] IProcessingUnit : public Object {
  public:
    struct ExceptionRecord;
    struct ProcessState;

    using InstructionSize = std::uint32_t;

    enum class AbortMode : std::uint32_t;
    enum class AccessType : std::uint32_t;
    enum class ArchitectureProfile : std::uint32_t;
    enum class BranchType : std::uint32_t;
    enum class ConditionCode : std::uint32_t;
    enum class Constraint : std::uint32_t;
    enum class Extension : std::uint32_t;
    enum class Exception : std::uint32_t;
    enum class ExceptionLevel : std::uint32_t;
    enum class ExtendType : std::uint32_t;
    enum class ExtensionVersion : std::uint32_t;
    enum class Feature : std::uint32_t;
    enum class InstructionSet : std::uint32_t;
    enum class RoundingMode : std::uint32_t;
    enum class SecurityState : std::uint32_t;
    enum class SelectedStackPointer : std::uint32_t;
    enum class ShiftType : std::uint32_t;
    enum class WidthOption : std::uint32_t;

    using ExecutionState = ArchitectureProfile;

    enum class ProcessStatus
    {
        Idle,
        Running,
        Interrupted,
    };

    IProcessingUnit(std::string name = "IProcessingUnit") : Object(std::move(name)) {
    }
    IProcessingUnit(IProcessingUnit&&) = default;
    IProcessingUnit& operator=(IProcessingUnit&&) = default;
    virtual ~IProcessingUnit()                    = default;

    IProcessingUnit(const IProcessingUnit&) = delete;
    IProcessingUnit& operator=(const IProcessingUnit&) = delete;

    [[nodiscard]] virtual ArchitectureProfile                  GetExecutionState() const noexcept                 = 0;
    [[nodiscard]] virtual InstructionSet                       GetInstructionSet() const noexcept                 = 0;
    [[nodiscard]] virtual ExtensionVersion                     GetCurrentExtensionVersion() const noexcept        = 0;
    [[nodiscard]] virtual const std::pmr::vector< Extension >& GetSupportedExtensions() const noexcept            = 0;
    [[nodiscard]] virtual bool                                 IsFeatureSupported(Feature feature) const noexcept = 0;
    [[nodiscard]] virtual bool                IsExceptionSupported(ExceptionLevel exceptionLevel) const noexcept  = 0;
    [[nodiscard]] virtual ICacheMemory const* GetUpStreamMemory() const noexcept                                  = 0;
    [[nodiscard]] virtual IMemory const*      GetStackMemory() const noexcept                                     = 0;
    [[nodiscard]] virtual IMemory const*      GetCurrentProgramMemory() const noexcept                            = 0;
    [[nodiscard]] virtual ProcessStatus       GetStatus() const noexcept                                          = 0;
    [[nodiscard]] virtual const IProcessingUnitWatcher& GetProcessingUnitWatcher() const noexcept                 = 0;

    [[nodiscard]] virtual const ProcessState* const GetCurrentProcessState() const noexcept = 0;

    virtual Result           Run(Program program)    = 0;
    virtual ControlledResult StepIn(Program program) = 0;
    virtual void             Stop()                  = 0;
};

END_NAMESPACE

    #include <ProcessingUnit/Enums/AbortMode.h>
    #include <ProcessingUnit/Enums/AccessType.h>
    #include <ProcessingUnit/Enums/ArchitectureProfile.h>
    #include <ProcessingUnit/Enums/BranchType.h>
    #include <ProcessingUnit/Enums/ConditionCode.h>
    #include <ProcessingUnit/Enums/Constraint.h>
    #include <ProcessingUnit/Enums/Exception.h>
    #include <ProcessingUnit/Enums/ExceptionLevel.h>
    #include <ProcessingUnit/Enums/ExtendType.h>
    #include <ProcessingUnit/Enums/Extension.h>
    #include <ProcessingUnit/Enums/ExtensionVersion.h>
    #include <ProcessingUnit/Enums/Feature.h>
    #include <ProcessingUnit/Enums/InstructionSet.h>
    #include <ProcessingUnit/Enums/RoundingMode.h>
    #include <ProcessingUnit/Enums/SecurityState.h>
    #include <ProcessingUnit/Enums/SelectedStackPointer.h>
    #include <ProcessingUnit/Enums/ShiftType.h>
    #include <ProcessingUnit/Enums/WidthOption.h>

    #include <ProcessingUnit/Structs/ExceptionRecord.h>
    #include <ProcessingUnit/Structs/ProcessState.h>

#endif // !defined(IPROCESSINGUNIT_H_INCLUDED_23E7732A_6223_466F_8087_945323852C58)
