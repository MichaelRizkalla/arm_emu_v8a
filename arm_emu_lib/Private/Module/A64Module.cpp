
#include <Module/A64Module.h>
#include <cassert>

BEGIN_NAMESPACE

struct A64Module::A64CoreControl {
  public:
    std::pmr::vector< std::uint32_t > m_load { 0 };
};

[[nodiscard]] UniqueRef< A64Module::A64CoreControl > A64Module::ConstructCoreController() {
    std::pmr::polymorphic_allocator< A64Module::A64CoreControl > alloc {};

    return allocate_unique< A64Module::A64CoreControl >(alloc);
}

A64Module::A64Module(std::pmr::vector< UniqueRef< ICore > >&& cores, UniqueRef< ICacheMemory > cacheMemory) :
    IModule(Default_name),
    m_cores(std::move(cores)),
    m_cacheMemory(std::move(cacheMemory)),
    m_coreControl(ConstructCoreController()) {
    assert(m_cores.size() > 0 && "Can't construct a core with 0 processing units!");
}

A64Module::A64Module(std::string name, std::pmr::vector< UniqueRef< ICore > >&& cores,
                     UniqueRef< ICacheMemory > cacheMemory) :
    IModule(std::move(name)),
    m_cores(std::move(cores)),
    m_cacheMemory(std::move(cacheMemory)),
    m_coreControl(ConstructCoreController()) {
    assert(m_cores.size() > 0 && "Can't construct a core with 0 processing units!");
}

A64Module::A64Module(A64Module&&) noexcept = default;

A64Module& A64Module::operator=(A64Module&&) noexcept = default;

A64Module::~A64Module() = default;

IProcessingUnit::ArchitectureProfile A64Module::GetExecutionState(std::uint8_t threadNumber) const noexcept {
    CorePreConditions(threadNumber);
    return m_cores.at(threadNumber)->GetExecutionState();
}

IProcessingUnit::InstructionSet A64Module::GetInstructionSet(std::uint8_t threadNumber) const noexcept {
    CorePreConditions(threadNumber);
    return m_cores.at(threadNumber)->GetInstructionSet();
}

IProcessingUnit::ExtensionVersion A64Module::GetCurrentExtensionVersion(std::uint8_t threadNumber) const noexcept {
    CorePreConditions(threadNumber);
    return m_cores.at(threadNumber)->GetCurrentExtensionVersion();
}

const std::pmr::vector< IProcessingUnit::Extension >&
    A64Module::GetSupportedExtensions(std::uint8_t threadNumber) const noexcept {
    CorePreConditions(threadNumber);
    return m_cores.at(threadNumber)->GetSupportedExtensions();
}

bool A64Module::IsFeatureSupported(std::uint8_t threadNumber, IProcessingUnit::Feature feature) const noexcept {
    CorePreConditions(threadNumber);
    return m_cores.at(threadNumber)->IsFeatureSupported(feature);
}

bool A64Module::IsExceptionSupported(std::uint8_t                    threadNumber,
                                     IProcessingUnit::ExceptionLevel exceptionLevel) const noexcept {
    CorePreConditions(threadNumber);
    return m_cores.at(threadNumber)->IsExceptionSupported(exceptionLevel);
}

ICacheMemory const* A64Module::GetUpStreamMemory(std::uint8_t threadNumber) const noexcept {
    CorePreConditions(threadNumber);
    return m_cores.at(threadNumber)->GetUpStreamMemory();
}

IMemory const* A64Module::GetStackMemory(std::uint8_t threadNumber) const noexcept {
    CorePreConditions(threadNumber);
    return m_cores.at(threadNumber)->GetStackMemory();
}

IMemory const* A64Module::GetCurrentProgramMemory(std::uint8_t threadNumber) const noexcept {
    CorePreConditions(threadNumber);
    return m_cores.at(threadNumber)->GetCurrentProgramMemory();
}

IProcessingUnit::ProcessStatus A64Module::GetStatus(std::uint8_t threadNumber) const noexcept {
    CorePreConditions(threadNumber);
    return m_cores.at(threadNumber)->GetStatus();
}

const IProcessingUnit::ProcessState* const A64Module::GetCurrentProcessState(std::uint8_t threadNumber) const noexcept {
    CorePreConditions(threadNumber);
    return m_cores.at(threadNumber)->GetCurrentProcessState();
}

const IProcessingUnitWatcher& A64Module::GetProcessingUnitWatcher(std::uint8_t threadNumber) const noexcept {
    CorePreConditions(threadNumber);
    return m_cores.at(threadNumber)->GetProcessingUnitWatcher();
}

Result A64Module::Run(Program program) {
    // TODO: balance load
    return m_cores.at(0)->Run(std::move(program));
}

ControlledResult A64Module::StepIn(Program program) {
    // TODO: balance load
    return m_cores.at(0)->StepIn(std::move(program));
}

void A64Module::Stop() {
    for (auto& processingUnit : m_cores) {
        processingUnit->Stop();
    }
}

std::uint8_t A64Module::GetCoreCount() const noexcept {
    return m_cores.size();
}

void A64Module::CorePreConditions(std::uint8_t threadNumber) const noexcept {
    assert(m_cores.size() > threadNumber && threadNumber >= 0 && "Invalid core index!");
}

END_NAMESPACE
