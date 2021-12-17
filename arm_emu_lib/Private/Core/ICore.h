#if !defined(ICORE_H_INCLUDED_EE887D1F_5F27_4442_8E7F_C411D7BDC7EB)
    #define ICORE_H_INCLUDED_EE887D1F_5F27_4442_8E7F_C411D7BDC7EB

    #include <API/Api.h>
    #include <DebugUtils/Object.h>
    #include <ProcessingUnit/IProcessingUnit.h>

BEGIN_NAMESPACE

class [[nodiscard]] ICore : public Object {
  public:
    ICore(std::string name = "ICore") : Object(std::move(name)) {
    }
    ICore(ICore&&) = default;
    ICore& operator=(ICore&&) = default;
    virtual ~ICore()          = default;

    ICore(const ICore&) = delete;
    ICore& operator=(const ICore&) = delete;

    [[nodiscard]] virtual IProcessingUnit::ArchitectureProfile GetExecutionState() const noexcept          = 0;
    [[nodiscard]] virtual IProcessingUnit::InstructionSet      GetInstructionSet() const noexcept          = 0;
    [[nodiscard]] virtual IProcessingUnit::ExtensionVersion    GetCurrentExtensionVersion() const noexcept = 0;
    [[nodiscard]] virtual const std::pmr::vector< IProcessingUnit::Extension >&
                               GetSupportedExtensions() const noexcept                                             = 0;
    [[nodiscard]] virtual bool IsFeatureSupported(IProcessingUnit::Feature feature) const noexcept                 = 0;
    [[nodiscard]] virtual bool IsExceptionSupported(IProcessingUnit::ExceptionLevel exceptionLevel) const noexcept = 0;
    [[nodiscard]] virtual ICacheMemory const*            GetUpStreamMemory() const noexcept                        = 0;
    [[nodiscard]] virtual IMemory const*                 GetStackMemory() const noexcept                           = 0;
    [[nodiscard]] virtual IMemory const*                 GetCurrentProgramMemory() const noexcept                  = 0;
    [[nodiscard]] virtual IProcessingUnit::ProcessStatus GetStatus() const noexcept                                = 0;
    [[nodiscard]] virtual const IProcessingUnitWatcher&  GetProcessingUnitWatcher() const noexcept                 = 0;

    [[nodiscard]] virtual const IProcessingUnit::ProcessState* const GetCurrentProcessState() const noexcept = 0;

    virtual Result           Run(Program program)    = 0;
    virtual ControlledResult StepIn(Program program) = 0;
    virtual void             Stop()                  = 0;
};

END_NAMESPACE

#endif // !defined(ICORE_H_INCLUDED_EE887D1F_5F27_4442_8E7F_C411D7BDC7EB)
