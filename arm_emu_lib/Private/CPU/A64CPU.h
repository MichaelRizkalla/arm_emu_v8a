#if !defined(A64CPU_H_INCLUDED_3B3FA0F6_F437_4CB5_9975_07A621F81A47)
    #define A64CPU_H_INCLUDED_3B3FA0F6_F437_4CB5_9975_07A621F81A47

    #include <API/Api.h>
    #include <CPU/ICPU.h>
    #include <CPU/SystemSettings.h>
    #include <DebugUtils/Object.h>
    #include <Utility/UniqueRef.h>
    #include <vector>

BEGIN_NAMESPACE

class A64CPUImpl;

class [[nodiscard]] A64CPU : public ICPU, public Object {
  public:
    static constexpr const char* Default_name = "A64CPU";

    A64CPU(const SystemSettings& settings);
    A64CPU(std::string name, const SystemSettings& settings);
    A64CPU(A64CPU&&) noexcept;
    A64CPU& operator=(A64CPU&&) noexcept;
    virtual ~A64CPU();

    A64CPU(const A64CPU&) = delete;
    A64CPU& operator=(const A64CPU&) = delete;

    Result           Run(Program program) final;
    ControlledResult StepIn(Program program) final;
    void             Stop() final;

    [[nodiscard]] std::uint8_t GetCoreCount() const noexcept final;
    [[nodiscard]] std::uint8_t GetThreadsPerCoreCount() const noexcept final;

    [[nodiscard]] ArchitectureProfile                 GetExecutionState() const noexcept final;
    [[nodiscard]] InstructionSet                      GetInstructionSet() const noexcept final;
    [[nodiscard]] ExtensionVersion                    GetCurrentExtensionVersion() const noexcept final;
    [[nodiscard]] const std::pmr::vector< Extension > GetSupportedExtensions() const noexcept final;
    [[nodiscard]] bool                                IsFeatureSupported(Feature feature) const noexcept final;
    [[nodiscard]] bool IsExceptionSupported(ExceptionLevel exceptionLevel) const noexcept final;

  private:
    class Impl;
    UniqueRef< Impl > m_cpu;

    template < class ImplDetail >
    [[nodiscard]] static UniqueRef< Impl > ConstructCPU(const SystemSettings& settings, ImplDetail detail);
};

END_NAMESPACE

#endif // !defined(A64CPU_H_INCLUDED_3B3FA0F6_F437_4CB5_9975_07A621F81A47)
