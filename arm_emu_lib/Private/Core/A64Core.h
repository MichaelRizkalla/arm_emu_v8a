#if !defined(A64CORE_H_INCLUDED_3F4BC2E7_D79B_4D99_90DC_6DBF4B824DCB)
    #define A64CORE_H_INCLUDED_3F4BC2E7_D79B_4D99_90DC_6DBF4B824DCB

    #include <API/Api.h>
    #include <Core/ICore.h>
    #include <Utility/UniqueRef.h>
    #include <memory>

BEGIN_NAMESPACE

class [[nodiscard]] A64Core : public ICore {
  public:
    static constexpr const char* Default_name = "A64Core";

    A64Core(UniqueRef< IProcessingUnit > processingUnit, UniqueRef< ICacheMemory > cacheMemory);
    A64Core(std::string name, UniqueRef< IProcessingUnit > processingUnit, UniqueRef< ICacheMemory > cacheMemory);
    A64Core(A64Core&&) noexcept;
    A64Core& operator=(A64Core&&) noexcept;
    virtual ~A64Core();

    A64Core(const A64Core&) = delete;
    A64Core& operator=(const A64Core&) = delete;

    [[nodiscard]] IProcessingUnit::ArchitectureProfile GetExecutionState() const noexcept final;
    [[nodiscard]] IProcessingUnit::InstructionSet      GetInstructionSet() const noexcept final;
    [[nodiscard]] IProcessingUnit::ExtensionVersion    GetCurrentExtensionVersion() const noexcept final;
    [[nodiscard]] const std::pmr::vector< IProcessingUnit::Extension >& GetSupportedExtensions() const noexcept final;
    [[nodiscard]] bool IsFeatureSupported(IProcessingUnit::Feature feature) const noexcept final;
    [[nodiscard]] bool IsExceptionSupported(IProcessingUnit::ExceptionLevel exceptionLevel) const noexcept final;
    [[nodiscard]] ICacheMemory const*            GetUpStreamMemory() const noexcept final;
    [[nodiscard]] IMemory const*                 GetStackMemory() const noexcept final;
    [[nodiscard]] IMemory const*                 GetCurrentProgramMemory() const noexcept final;
    [[nodiscard]] IProcessingUnit::ProcessStatus GetStatus() const noexcept final;
    [[nodiscard]] const IProcessingUnitWatcher&  GetProcessingUnitWatcher() const noexcept final;

    [[nodiscard]] const IProcessingUnit::ProcessState* const GetCurrentProcessState() const noexcept final;

    Result           Run(Program program) final;
    ControlledResult StepIn(Program program) final;
    void             Stop() final;

  private:
    UniqueRef< IProcessingUnit > m_processingUnit;
    UniqueRef< ICacheMemory >    m_memory;
};

END_NAMESPACE

#endif // !defined(A64CORE_H_INCLUDED_3F4BC2E7_D79B_4D99_90DC_6DBF4B824DCB)
