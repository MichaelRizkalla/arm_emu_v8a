#if !defined(PROCESSINGUNIT_H_INCLUDED_9AA523D6_9095_43C4_968C_09AFC093BC76)
    #define PROCESSINGUNIT_H_INCLUDED_9AA523D6_9095_43C4_968C_09AFC093BC76

    #include <Memory/IMemory.h>
    #include <ProcessingUnit/IProcessingUnit.h>
    #include <Utility/UniqueRef.h>
    #include <string>

BEGIN_NAMESPACE

class [[nodiscard]] A64ProcessingUnit final : public IProcessingUnit {
  public:
    static constexpr const char* Default_name = "ProcessingUnit";
    A64ProcessingUnit(std::string name, ICacheMemory* upStreamMemory, UniqueRef< IMemory > stackMemory);
    A64ProcessingUnit(ICacheMemory* upStreamMemory, UniqueRef< IMemory > stackMemory);
    A64ProcessingUnit(A64ProcessingUnit&&) noexcept;
    A64ProcessingUnit& operator=(A64ProcessingUnit&&) noexcept;
    ~A64ProcessingUnit() final;

    A64ProcessingUnit(const A64ProcessingUnit&) = delete;
    A64ProcessingUnit& operator=(const A64ProcessingUnit&) = delete;

    [[nodiscard]] ArchitectureProfile                  GetExecutionState() const noexcept final;
    [[nodiscard]] InstructionSet                       GetInstructionSet() const noexcept final;
    [[nodiscard]] ExtensionVersion                     GetCurrentExtensionVersion() const noexcept final;
    [[nodiscard]] const std::pmr::vector< Extension >& GetSupportedExtensions() const noexcept final;
    [[nodiscard]] bool                                 IsFeatureSupported(Feature feature) const noexcept final;
    [[nodiscard]] bool                IsExceptionSupported(ExceptionLevel exceptionLevel) const noexcept final;
    [[nodiscard]] ICacheMemory const* GetUpStreamMemory() const noexcept final;
    [[nodiscard]] IMemory const*      GetStackMemory() const noexcept final;
    [[nodiscard]] IMemory const*      GetCurrentProgramMemory() const noexcept final;
    [[nodiscard]] ProcessStatus       GetStatus() const noexcept final;
    [[nodiscard]] const IProcessingUnitWatcher& GetProcessingUnitWatcher() const noexcept final;

    [[nodiscard]] const ProcessState* const GetCurrentProcessState() const noexcept final;

    Result           Run(Program program) final;
    ControlledResult StepIn(Program program) final;
    void             Stop() final;

  private:
    void ResetProcessState() noexcept;

    class Impl;
    UniqueRef< Impl > m_processingUnit;

    template < class ImplDetail >
    [[nodiscard]] static UniqueRef< Impl > ConstructProcessingUnit(ICacheMemory*        upStreamMemory,
                                                                   UniqueRef< IMemory > stackMemory, ImplDetail detail);
};

END_NAMESPACE

#endif // !defined(PROCESSINGUNIT_H_INCLUDED_9AA523D6_9095_43C4_968C_09AFC093BC76)
