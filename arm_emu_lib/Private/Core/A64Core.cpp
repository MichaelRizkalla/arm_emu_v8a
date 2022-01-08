
#include <Core/A64Core.h>

BEGIN_NAMESPACE

A64Core::A64Core(UniqueRef< IProcessingUnit > processingUnit, UniqueRef< ICacheMemory > cacheMemory) :
    ICore(Default_name), m_processingUnit(std::move(processingUnit)), m_memory(std::move(cacheMemory)) {
}

A64Core::A64Core(std::string name, UniqueRef< IProcessingUnit > processingUnit, UniqueRef< ICacheMemory > cacheMemory) :
    ICore(std::move(name)), m_processingUnit(std::move(processingUnit)), m_memory(std::move(cacheMemory)) {
}

A64Core::A64Core(A64Core&&) noexcept = default;

A64Core& A64Core::operator=(A64Core&&) noexcept = default;

A64Core::~A64Core() {
    // Processing unit uses m_memory until destroyed, so ensure it's being destroyed first
    m_processingUnit.reset();
    m_memory.reset();
}

IProcessingUnit::ArchitectureProfile A64Core::GetExecutionState() const noexcept {
    return m_processingUnit->GetExecutionState();
}

IProcessingUnit::InstructionSet A64Core::GetInstructionSet() const noexcept {
    return m_processingUnit->GetInstructionSet();
}

IProcessingUnit::ExtensionVersion A64Core::GetCurrentExtensionVersion() const noexcept {
    return m_processingUnit->GetCurrentExtensionVersion();
}

const std::pmr::vector< IProcessingUnit::Extension >& A64Core::GetSupportedExtensions() const noexcept {
    return m_processingUnit->GetSupportedExtensions();
}

bool A64Core::IsFeatureSupported(IProcessingUnit::Feature feature) const noexcept {
    return m_processingUnit->IsFeatureSupported(feature);
}

bool A64Core::IsExceptionSupported(IProcessingUnit::ExceptionLevel exceptionLevel) const noexcept {
    return m_processingUnit->IsExceptionSupported(exceptionLevel);
}

IMemory const* A64Core::GetCurrentProgramMemory() const noexcept {
    return m_processingUnit->GetCurrentProgramMemory();
}

IProcessingUnit::ProcessStatus A64Core::GetStatus() const noexcept {
    return m_processingUnit->GetStatus();
}

const IProcessingUnitWatcher& A64Core::GetProcessingUnitWatcher() const noexcept {
    return m_processingUnit->GetProcessingUnitWatcher();
}

const IProcessingUnit::ProcessState* const A64Core::GetCurrentProcessState() const noexcept {
    return m_processingUnit->GetCurrentProcessState();
}

Result A64Core::Run(Program program) {
    return m_processingUnit->Run(std::move(program));
}

ControlledResult A64Core::StepIn(Program program) {
    // TODO: balance load
    return m_processingUnit->StepIn(std::move(program));
}

void A64Core::Stop() {
    m_processingUnit->Stop();
}

void A64Core::Reset() noexcept {
    m_processingUnit->Reset();
}

END_NAMESPACE
