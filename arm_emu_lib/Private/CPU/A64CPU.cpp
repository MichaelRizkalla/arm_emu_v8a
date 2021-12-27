
#include <CPU/A64CPU.h>
#include <Core/A64Core.h>
#include <Memory/CacheMemory.h>
#include <Memory/MemoryManagementUnit.h>
#include <Memory/RandomAccessMemory.h>
#include <Module/A64Module.h>
#include <ProcessingUnit/A64ProcessingUnit.h>
#include <memory>

BEGIN_NAMESPACE

// TODO: Add load balancer

class A64CPU::Impl {
  public:
    Impl(Object* logger, const SystemSettings& settings) :
        m_debugObject(*logger),
        m_ram(ConstructRAM(settings.RamSize)),
        m_l3Cache(ConstructL3Cache(m_ram.get(), settings.L3CacheSize)),
        m_cores(static_cast< std::size_t >(settings.nCores)),
        m_mmu(
            std::allocate_shared< MemoryManagementUnit >(std::pmr::polymorphic_allocator< MemoryManagementUnit > {})) {

        std::pmr::vector< UniqueRef< ICacheMemory > > l1Caches { static_cast< std::size_t >(settings.nCores) *
                                                                 static_cast< std::size_t >(settings.nThreadsPerCore) };
        std::pmr::vector< UniqueRef< ICacheMemory > > l2Caches { static_cast< std::size_t >(settings.nCores) };

        std::pmr::vector< UniqueRef< IProcessingUnit > > processingUnits {
            static_cast< std::size_t >(settings.nCores) * static_cast< std::size_t >(settings.nThreadsPerCore)
        };
        std::pmr::vector< std::pmr::vector< UniqueRef< ICore > > > cores { static_cast< std::size_t >(
            settings.nCores) };

        std::pmr::polymorphic_allocator< CacheMemory > cacheAlloc {};

        std::pmr::polymorphic_allocator< RandomAccessMemory > ramAlloc {};
        std::pmr::polymorphic_allocator< A64ProcessingUnit >  processingUnitAlloc {};
        std::pmr::polymorphic_allocator< A64Core >            coreAlloc {};
        std::pmr::polymorphic_allocator< A64Module >          moduleAlloc {};

        CacheMemory::Config cacheConfig {};
        cacheConfig.m_cacheWriteStrategy = CacheWriteStrategy::WriteThrough;
        cacheConfig.m_memoryMapping      = CacheMemoryMapping::DirectMapping;

        for (auto& l2Cache : l2Caches) {
            l2Cache = allocate_unique< ICacheMemory, CacheMemory >(cacheAlloc, "L2Cache", cacheConfig, m_l3Cache.get(),
                                                                   settings.L2CacheSize);
        }
        std::uint64_t cIdx = 0;
        for (auto& l1Cache : l1Caches) {
            l1Cache = allocate_unique< ICacheMemory, CacheMemory >(
                cacheAlloc, "L1Cache", cacheConfig, l2Caches.at(cIdx / settings.nCores).get(), settings.L1CacheSize);
            ++cIdx;
        }

        const auto processMemorySize = settings.StackSize + settings.HeapSize;
        const auto neededSize        = processMemorySize * processingUnits.size();

        if (neededSize > settings.RamSize) {
            throw cpu_creation_failure { "CPU requires more RAM size than specified" };
        }

        cIdx = 0;
        for (auto& processingUnit : processingUnits) {
            processingUnit = allocate_unique< IProcessingUnit, A64ProcessingUnit >(
                processingUnitAlloc, "ProcessingUnit", l1Caches.at(cIdx).get(), processMemorySize,
                MemoryManagementUnitProxy { m_mmu });

            m_mmu->AddProcess(processingUnit.get(), cIdx * processMemorySize, (cIdx + 1) * processMemorySize);
            ++cIdx;
        }
        cIdx = 0;
        for (auto& coreVec : cores) {
            coreVec.resize(static_cast< std::size_t >(settings.nThreadsPerCore));
            for (auto& core : coreVec) {
                core = allocate_unique< ICore, A64Core >(coreAlloc, "Thread",
                                                         std::move(std::exchange(processingUnits[cIdx], nullptr)),
                                                         std::move(std::exchange(l1Caches[cIdx], nullptr)));
                ++cIdx;
            }
        }
        cIdx = 0;
        for (auto& module_ : m_cores) {
            module_ =
                allocate_unique< IModule, A64Module >(moduleAlloc, "Core", std::move(std::exchange(cores[cIdx], {})),
                                                      std::move(std::exchange(l2Caches[cIdx], nullptr)));
            ++cIdx;
        }

        m_debugObject.Log(LogType::Construction, "Constructing CPU succeeded!");
    }

    Result Run(Program program) {
        // TODO: load balance
        return m_cores.at(0)->Run(std::move(program));
    }

    ControlledResult StepIn(Program program) {
        // TODO: load balance
        return m_cores.at(0)->StepIn(std::move(program));
    }

    void Stop() {
        for (auto& core : m_cores) {
            core->Stop();
        }
    }

    std::uint8_t GetCoreCount() const noexcept {
        return m_cores.size();
    }

    std::uint8_t GetThreadsPerCoreCount() const noexcept {
        return m_cores.at(0)->GetCoreCount();
    }

    ArchitectureProfile GetExecutionState() const noexcept {
        return static_cast< ArchitectureProfile >(m_cores.at(0)->GetExecutionState(0));
    }

    InstructionSet GetInstructionSet() const noexcept {
        return static_cast< InstructionSet >(m_cores.at(0)->GetInstructionSet(0));
    }

    ExtensionVersion GetCurrentExtensionVersion() const noexcept {
        return static_cast< ExtensionVersion >(m_cores.at(0)->GetCurrentExtensionVersion(0));
    }

    const std::pmr::vector< Extension > GetSupportedExtensions() const noexcept {
        auto&                         extensions = m_cores.at(0)->GetSupportedExtensions(0);
        std::pmr::vector< Extension > returnExts { extensions.size() };
        for (auto idx = 0; idx < extensions.size(); ++idx) {
            returnExts[idx] = static_cast< Extension >(extensions[idx]);
        }
        return returnExts;
    }

    bool IsFeatureSupported(Feature feature) const noexcept {
        return m_cores.at(0)->IsFeatureSupported(0, static_cast< IProcessingUnit::Feature >(feature));
    }

    bool IsExceptionSupported(ExceptionLevel exceptionLevel) const noexcept {
        return m_cores.at(0)->IsExceptionSupported(0, static_cast< IProcessingUnit::ExceptionLevel >(exceptionLevel));
    }

  private:
    static UniqueRef< IMemory > ConstructRAM(std::uint64_t ramSize) {
        std::pmr::polymorphic_allocator< RandomAccessMemory > alloc {};

        return allocate_unique< IMemory, RandomAccessMemory >(alloc, "RAM", ramSize);
    }

    static UniqueRef< IMemory > ConstructL3Cache(IMemory* upStream, std::uint64_t L3Size) {
        std::pmr::polymorphic_allocator< CacheMemory > alloc {};

        CacheMemory::Config config {};
        config.m_cacheWriteStrategy = CacheWriteStrategy::WriteThrough;
        config.m_memoryMapping      = CacheMemoryMapping::DirectMapping;

        return allocate_unique< IMemory, CacheMemory >(alloc, "L3Cache", config, upStream, L3Size);
    }

    Object&                                  m_debugObject;
    std::pmr::vector< UniqueRef< IModule > > m_cores;
    UniqueRef< IMemory >                     m_ram;
    UniqueRef< IMemory >                     m_l3Cache;
    SharedRef< MemoryManagementUnit >        m_mmu;
};

template < class ImplDetail >
UniqueRef< A64CPU::Impl > A64CPU::ConstructCPU(const SystemSettings& settings, ImplDetail myself) {
    assert(settings.nCores > 0 && settings.nThreadsPerCore > 0 && settings.L1CacheSize > 0 &&
           settings.L2CacheSize > 0 && settings.L3CacheSize > 0 && settings.RamSize > 0 && settings.StackSize > 0 &&
           "All CPU system settings has to be larger than 0 for a valid CPU!");
    myself->Log(LogType::Construction,
                "Constructing CPU with {} cores, {} threads per core, {} L1 cache, {} L2 cache, {} L3 cache, {} ram "
                "and {} stack",
                settings.nCores, settings.nThreadsPerCore, settings.L1CacheSize, settings.L2CacheSize,
                settings.L3CacheSize, settings.StackSize, settings.RamSize);

    std::pmr::polymorphic_allocator< A64CPU::Impl > alloc {};

    return allocate_unique< A64CPU::Impl >(alloc, myself, settings);
}

A64CPU::A64CPU(const SystemSettings& settings) : Object(Default_name), m_cpu(ConstructCPU(settings, this)) {
}

A64CPU::A64CPU(std::string name, const SystemSettings& settings) :
    Object(std::move(name)), m_cpu(ConstructCPU(settings, this)) {
}

A64CPU::A64CPU(A64CPU&&) noexcept = default;

A64CPU& A64CPU::operator=(A64CPU&&) noexcept = default;

A64CPU::~A64CPU() {
    m_cpu->Stop();
    Log(LogType::Destruction, "Destroying CPU!");
}

Result A64CPU::Run(Program program) {
    return m_cpu->Run(std::move(program));
}

ControlledResult A64CPU::StepIn(Program program) {
    // TODO: load balance
    return m_cpu->StepIn(std::move(program));
}

void A64CPU::Stop() {
    m_cpu->Stop();
}

std::uint8_t A64CPU::GetCoreCount() const noexcept {
    return m_cpu->GetCoreCount();
}

std::uint8_t A64CPU::GetThreadsPerCoreCount() const noexcept {
    return m_cpu->GetThreadsPerCoreCount();
}

ArchitectureProfile A64CPU::GetExecutionState() const noexcept {
    return m_cpu->GetExecutionState();
}

InstructionSet A64CPU::GetInstructionSet() const noexcept {
    return m_cpu->GetInstructionSet();
}

ExtensionVersion A64CPU::GetCurrentExtensionVersion() const noexcept {
    return m_cpu->GetCurrentExtensionVersion();
}

const std::pmr::vector< Extension > A64CPU::GetSupportedExtensions() const noexcept {
    return m_cpu->GetSupportedExtensions();
}

bool A64CPU::IsFeatureSupported(Feature feature) const noexcept {
    return m_cpu->IsFeatureSupported(feature);
}

bool A64CPU::IsExceptionSupported(ExceptionLevel exceptionLevel) const noexcept {
    return m_cpu->IsExceptionSupported(exceptionLevel);
}

END_NAMESPACE
