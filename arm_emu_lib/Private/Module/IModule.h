#if !defined(IMODULE_H_INCLUDED_BEB26F36_E257_4738_B9E8_280005C4FAAA)
    #define IMODULE_H_INCLUDED_BEB26F36_E257_4738_B9E8_280005C4FAAA

    #include <API/Api.h>
    #include <DebugUtils/Object.h>
    #include <ProcessingUnit/IProcessingUnit.h>
    #include <Program/Result.h>

BEGIN_NAMESPACE

class [[nodiscard]] IModule : public Object {
  public:
    IModule(std::string name = "IModule") : Object(std::move(name)) {
    }
    IModule(IModule&&) = default;
    IModule& operator=(IModule&&) = default;
    virtual ~IModule()            = default;

    IModule(const IModule&) = delete;
    IModule& operator=(const IModule&) = delete;

    [[nodiscard]] virtual IProcessingUnit::ArchitectureProfile
                                                          GetExecutionState(std::uint8_t coreNumber) const noexcept = 0;
    [[nodiscard]] virtual IProcessingUnit::InstructionSet GetInstructionSet(std::uint8_t coreNumber) const noexcept = 0;
    [[nodiscard]] virtual IProcessingUnit::ExtensionVersion
        GetCurrentExtensionVersion(std::uint8_t coreNumber) const noexcept = 0;
    [[nodiscard]] virtual const std::pmr::vector< IProcessingUnit::Extension >&
                                              GetSupportedExtensions(std::uint8_t coreNumber) const noexcept      = 0;
    [[nodiscard]] virtual bool                IsFeatureSupported(std::uint8_t             coreNumber,
                                                                 IProcessingUnit::Feature feature) const noexcept = 0;
    [[nodiscard]] virtual bool                IsExceptionSupported(std::uint8_t                    coreNumber,
                                                                   IProcessingUnit::ExceptionLevel exceptionLevel) const noexcept = 0;
    [[nodiscard]] virtual ICacheMemory const* GetUpStreamMemory(std::uint8_t coreNumber) const noexcept       = 0;
    [[nodiscard]] virtual IMemory const*      GetStackMemory(std::uint8_t coreNumber) const noexcept          = 0;
    [[nodiscard]] virtual IMemory const*      GetCurrentProgramMemory(std::uint8_t coreNumber) const noexcept = 0;
    [[nodiscard]] virtual IProcessingUnit::ProcessStatus GetStatus(std::uint8_t coreNumber) const noexcept    = 0;
    [[nodiscard]] virtual const IProcessingUnit::ProcessState* const
        GetCurrentProcessState(std::uint8_t coreNumber) const noexcept = 0;
    [[nodiscard]] virtual const IProcessingUnitWatcher&
        GetProcessingUnitWatcher(std::uint8_t coreNumber) const noexcept = 0;

    virtual Result           Run(Program program)    = 0;
    virtual ControlledResult StepIn(Program program) = 0;
    virtual void             Stop()                  = 0;

    [[nodiscard]] virtual std::uint8_t GetCoreCount() const noexcept = 0;
};

END_NAMESPACE

#endif // !defined(IMODULE_H_INCLUDED_BEB26F36_E257_4738_B9E8_280005C4FAAA)
