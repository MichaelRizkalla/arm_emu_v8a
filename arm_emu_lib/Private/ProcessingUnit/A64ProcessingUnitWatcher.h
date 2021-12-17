#if !defined(PROCESSINGUNITWATCHER_H_INCLUDED_7D8602C9_6C0E_49D4_B4C3_F16356474CE4)
    #define PROCESSINGUNITWATCHER_H_INCLUDED_7D8602C9_6C0E_49D4_B4C3_F16356474CE4

    #include <API/Api.h>
    #include <ProcessingUnit/IProcessingUnitWatcher.h>
    #include <cstdint>

BEGIN_NAMESPACE

struct [[nodiscard]] A64ProcessingUnitWatcher : public IProcessingUnitWatcher {
  public:
    A64ProcessingUnitWatcher();
    DEFAULT_COPY_CLASS(A64ProcessingUnitWatcher)
    DEFAULT_MOVE_CLASS(A64ProcessingUnitWatcher)
    VIRTUAL_DTOR(A64ProcessingUnitWatcher)

    void RecordInstructionHandled(InstructionType type) noexcept final;
    void RecordProcessHandled() noexcept final;
    void RecordProcessInterrupted() noexcept final;

    [[nodiscard]] std::size_t GetInstructionCountFor(InstructionType type) const noexcept final;
    [[nodiscard]] std::size_t GetProcessCount() const noexcept final;
    [[nodiscard]] std::size_t GetInterruptCount() const noexcept final;

  private:
    std::size_t m_branchExceptionSystemInstructions;
    std::size_t m_dataProcessingImmediateInstructions;
    std::size_t m_dataProcessingRegisterInstructions;
    std::size_t m_dataProcessingScalarFloatingPointAdvancedSIMDInstructions;
    std::size_t m_loadStoreInstructions;
    std::size_t m_reservedInstructions;
    std::size_t m_scalableVectorExtension;

    std::size_t m_processCount;
    std::size_t m_interruptCount;
};

END_NAMESPACE

#endif // !defined(PROCESSINGUNITWATCHER_H_INCLUDED_7D8602C9_6C0E_49D4_B4C3_F16356474CE4)
