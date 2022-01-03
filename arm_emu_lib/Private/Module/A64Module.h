#if !defined(A64MODULE_H_INCLUDED_E6D110A7_1B71_4E06_B788_DB8396E50CB3)
    #define A64MODULE_H_INCLUDED_E6D110A7_1B71_4E06_B788_DB8396E50CB3

    #include <API/Api.h>
    #include <Core/ICore.h>
    #include <Module/IModule.h>
    #include <Utility/UniqueRef.h>
    #include <cstdint>
    #include <memory>
    #include <vector>

BEGIN_NAMESPACE

class [[nodiscard]] A64Module : public IModule {
  public:
    static constexpr const char* Default_name = "A64Module";

    A64Module(std::pmr::vector< UniqueRef< ICore > >&& cores, UniqueRef< ICacheMemory > cacheMemory);
    A64Module(std::string name, std::pmr::vector< UniqueRef< ICore > >&& cores, UniqueRef< ICacheMemory > cacheMemory);
    A64Module(A64Module&&) noexcept;
    A64Module& operator=(A64Module&&) noexcept;
    virtual ~A64Module();

    A64Module(const A64Module&) = delete;
    A64Module& operator=(const A64Module&) = delete;

    [[nodiscard]] IProcessingUnit::ArchitectureProfile
                                                  GetExecutionState(std::uint8_t threadNumber) const noexcept final;
    [[nodiscard]] IProcessingUnit::InstructionSet GetInstructionSet(std::uint8_t threadNumber) const noexcept final;
    [[nodiscard]] IProcessingUnit::ExtensionVersion
        GetCurrentExtensionVersion(std::uint8_t threadNumber) const noexcept final;
    [[nodiscard]] const std::pmr::vector< IProcessingUnit::Extension >&
                                 GetSupportedExtensions(std::uint8_t threadNumber) const noexcept final;
    [[nodiscard]] bool           IsFeatureSupported(std::uint8_t             threadNumber,
                                                    IProcessingUnit::Feature feature) const noexcept final;
    [[nodiscard]] bool           IsExceptionSupported(std::uint8_t                    threadNumber,
                                                      IProcessingUnit::ExceptionLevel exceptionLevel) const noexcept final;
    [[nodiscard]] IMemory const* GetCurrentProgramMemory(std::uint8_t threadNumber) const noexcept final;
    [[nodiscard]] IProcessingUnit::ProcessStatus GetStatus(std::uint8_t threadNumber) const noexcept final;
    [[nodiscard]] const IProcessingUnitWatcher&
        GetProcessingUnitWatcher(std::uint8_t threadNumber) const noexcept final;
    [[nodiscard]] const IProcessingUnit::ProcessState* const
        GetCurrentProcessState(std::uint8_t threadNumber) const noexcept final;

    Result           Run(Program program) final;
    ControlledResult StepIn(Program program) final;
    void             Stop() final;
    void             Reset() noexcept final;

    [[nodiscard]] std::uint8_t GetCoreCount() const noexcept final;

  private:
    void CorePreConditions(std::uint8_t threadNumber) const noexcept;

    struct A64CoreControl;
    std::pmr::vector< UniqueRef< ICore > > m_cores;
    UniqueRef< ICacheMemory >              m_cacheMemory;
    UniqueRef< A64CoreControl >            m_coreControl;

    [[nodiscard]] static UniqueRef< A64CoreControl > ConstructCoreController();
};

END_NAMESPACE

#endif // !defined(A64MODULE_H_INCLUDED_E6D110A7_1B71_4E06_B788_DB8396E50CB3)
