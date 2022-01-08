#if !defined(IPROCESSINGUNITWATCHER_H_INCLUDED_83584665_0720_4E16_BDC7_062230A3AE14)
    #define IPROCESSINGUNITWATCHER_H_INCLUDED_83584665_0720_4E16_BDC7_062230A3AE14

    #include <API/Api.h>
    #include <cstddef>

BEGIN_NAMESPACE

enum class InstructionType
{
    // A64 groups
    BranchExceptionSystem,
    DataProcessingImmediate,
    DataProcessingRegister,
    DataProcessingScalarFloatingPointAdvancedSIMD,
    LoadStore,
    Reserved,
    ScalableVectorExtension,
};

struct [[nodiscard]] IProcessingUnitWatcher {
  public:
    virtual void RecordInstructionHandled(InstructionType type) noexcept = 0;
    virtual void RecordProcessHandled() noexcept                         = 0;
    virtual void RecordProcessInterrupted() noexcept                     = 0;

    [[nodiscard]] virtual std::size_t GetInstructionCountFor(InstructionType type) const noexcept = 0;
    [[nodiscard]] virtual std::size_t GetProcessCount() const noexcept                            = 0;
    [[nodiscard]] virtual std::size_t GetInterruptCount() const noexcept                          = 0;
};

END_NAMESPACE

#endif // !defined(IPROCESSINGUNITWATCHER_H_INCLUDED_83584665_0720_4E16_BDC7_062230A3AE14)
