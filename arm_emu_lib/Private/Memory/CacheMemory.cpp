/*
Please note this implementation is meant for single thread access.
TODO: Implement multi threading access
*/

#include <Memory/CacheMemory.h>
#include <Memory/MemoryWatcher.h>
#include <memory_resource>

BEGIN_NAMESPACE

namespace {
    static const std::pmr::vector< CacheWriteStrategy > supportedCacheWriteStrategies {
        CacheWriteStrategy::WriteThrough
    };
    static const std::pmr::vector< CacheMemoryMapping > supportedMemoryMappings { CacheMemoryMapping::DirectMapping };
} // namespace

// Direct Mapping implementation
enum class State
{
    Empty,
    Clean,
    Modified,
};
struct CacheLineData {
    ICacheMemory::Address                                               m_segmentLocInUpstream { 0 };
    std::array< ICacheMemory::DataUnit, ICacheMemory::Cache_line_size > m_data {};
    std::array< State, ICacheMemory::Cache_line_size >                  m_state {};
};

// Interface for different cache types
class [[nodiscard]] CacheMemory::Impl {
  public:
    Impl() = default;
    DEFAULT_MOVE_CLASS(Impl)
    DELETE_COPY_CLASS(Impl)
    virtual ~Impl() = default;

    [[nodiscard]] virtual DataUnit              Read(Address address) noexcept                        = 0;
    [[nodiscard]] virtual DataBlock< DataUnit > ReadBlock(Address start, std::uint64_t dataUnitCount) = 0;

    virtual void Write(Address address, DataUnit data)                        = 0;
    virtual void WriteBlock(Address start, const DataBlock< DataUnit >& data) = 0;

    [[nodiscard]] virtual Address Size() const noexcept = 0;

    [[nodiscard]] virtual const MemoryWatcher& GetMemoryWatcher() const noexcept  = 0;
    [[nodiscard]] virtual const IMemory* const GetUpStreamMemory() const noexcept = 0;

    virtual void ClearCache() noexcept = 0;
};

class [[nodiscard]] CacheMemory::DirectAccessWriteThroughCacheMemory final : public CacheMemory::Impl {
  public:
    DirectAccessWriteThroughCacheMemory(Object* logger, IMemory* upStreamMemory, Address addressableSize) :
        m_size(addressableSize),
        m_cache(BuildInitialMemory(m_size)),
        m_upStreamMemory(upStreamMemory),
        m_watcher(MemoryWatcher::Hint::CacheMemory),
        m_debugObject(*logger),
        m_segmentCount(ComputeAddressMask(m_size, m_upStreamMemory->Size())) {
        m_debugObject.Log(LogType::Construction, "Memory construction {}!",
                          m_cache.size() > 0 ? "succeeded" : "failed");
    }

    [[nodiscard]] DataUnit Read(Address address) noexcept final {
        m_debugObject.LogTrace(LogType::Other, "Read memory at address: {}", address);

        const auto segmentLoc = address / m_size;
        assert(segmentLoc < m_segmentCount);

        const auto cacheLineEntry = address / Cache_line_size;
        const auto dataUnitEntry  = address % Cache_line_size;

        const auto& cacheLine       = m_cache.at(cacheLineEntry);
        const auto  isRequestedData = cacheLine.m_segmentLocInUpstream == segmentLoc;

        if (isRequestedData) {
            const auto isValid = cacheLine.m_state.at(dataUnitEntry) == State::Clean;

            if (isValid) {
                m_watcher.RecordAccessResult(MemoryAccessResult::Hit);
                m_watcher.RecordAccessType(MemoryAccessType::Read);
                return cacheLine.m_data.at(dataUnitEntry);
            }
        }

        m_watcher.RecordAccessResult(MemoryAccessResult::Miss);

        auto blockStartAddr  = (segmentLoc * m_size) + (cacheLineEntry * Cache_line_size);
        auto correctDataUnit = m_upStreamMemory->ReadBlock(blockStartAddr, Cache_line_size);
        m_watcher.RecordAccessType(MemoryAccessType::UpStreamReadBlock);

        WriteBlockFromUpStreamToCacheOnly(segmentLoc, cacheLineEntry, correctDataUnit);
        const auto& cacheLineModified = m_cache.at(cacheLineEntry);
        m_watcher.RecordAccessType(MemoryAccessType::Read);

        return cacheLineModified.m_data.at(dataUnitEntry);
    }

    [[nodiscard]] DataBlock< DataUnit > ReadBlock(Address start, std::uint64_t dataUnitCount) final {
        m_debugObject.LogTrace(LogType::Other, "ReadBlock of memory starting from address: {} with length: {}", start,
                               dataUnitCount);

        const auto endBlockAddr  = start + dataUnitCount;
        const auto segmentLoc    = start / m_size;
        const auto endSegmentLoc = endBlockAddr / m_size;

        assert(segmentLoc < m_segmentCount);
        assert(segmentLoc == endSegmentLoc); // Assume only requesting one cache line per call

        const auto cacheLineEntry     = start / Cache_line_size;
        const auto startDataUnitEntry = start % Cache_line_size;

        const auto& cacheLine = m_cache.at(cacheLineEntry);

        const auto isRequestedData = cacheLine.m_segmentLocInUpstream == segmentLoc;

        if (isRequestedData) {
            const auto isInvalid =
                std::any_of(cacheLine.m_state.begin() + startDataUnitEntry,
                            cacheLine.m_state.begin() + startDataUnitEntry + dataUnitCount,
                            [](auto& state) { return state == State::Modified || state == State::Empty; });

            if (!isInvalid) {
                m_watcher.RecordAccessResult(MemoryAccessResult::Hit);
                m_watcher.RecordAccessType(MemoryAccessType::ReadBlock);

                return DataBlock< DataUnit > { dataUnitCount, cacheLine.m_data.data() + startDataUnitEntry };
            }
        }

        m_watcher.RecordAccessResult(MemoryAccessResult::Miss);

        auto blockStartAddr  = (segmentLoc * m_size) + (cacheLineEntry * Cache_line_size);
        auto correctDataUnit = m_upStreamMemory->ReadBlock(blockStartAddr, Cache_line_size);
        m_watcher.RecordAccessType(MemoryAccessType::UpStreamReadBlock);

        WriteBlockFromUpStreamToCacheOnly(segmentLoc, cacheLineEntry, correctDataUnit);
        const auto& cacheLineModified = m_cache.at(cacheLineEntry);
        m_watcher.RecordAccessType(MemoryAccessType::ReadBlock);

        return DataBlock< DataUnit > { dataUnitCount, cacheLineModified.m_data.data() + startDataUnitEntry };
    }

    void Write(Address address, DataUnit data) final {
        m_debugObject.LogTrace(LogType::Other, "Write DataUnit: {} at address: {}", data, address);

        const auto segmentLoc = address / m_size;
        assert(segmentLoc < m_segmentCount);

        const auto cacheLineEntry = address / Cache_line_size;
        const auto dataUnitEntry  = address % Cache_line_size;

        if (m_cache.at(cacheLineEntry).m_segmentLocInUpstream != segmentLoc) {
            m_upStreamMemory->Write(address, data);
            m_watcher.RecordAccessType(MemoryAccessType::UpStreamWrite);
            return;
        }

        WriteToCacheOnly(address, data);
        auto blockStartAddr = (segmentLoc * m_size) + (cacheLineEntry * Cache_line_size);

        m_upStreamMemory->WriteBlock(blockStartAddr, { Cache_line_size, m_cache.at(cacheLineEntry).m_data.data() });
        m_cache.at(cacheLineEntry).m_state.at(dataUnitEntry) = State::Clean;

        m_watcher.RecordAccessType(MemoryAccessType::UpStreamWriteBlock);
    }

    void WriteBlock(Address start, const DataBlock< DataUnit >& data) final {
        m_debugObject.LogTrace(LogType::Other, "WriteBlock of DataUnits of size: {} starting from address: {}",
                               data.Size(), start);

        const auto endBlockAddr  = start + data.Size();
        const auto segmentLoc    = start / m_size;
        const auto endSegmentLoc = endBlockAddr / m_size;

        assert(segmentLoc < m_segmentCount);
        assert(segmentLoc == endSegmentLoc); // Assume only requesting one cache line per call

        const auto cacheLineEntry = start / Cache_line_size;
        const auto dataUnitEntry  = start % Cache_line_size;

        if (m_cache.at(cacheLineEntry).m_segmentLocInUpstream != segmentLoc) {
            m_upStreamMemory->WriteBlock(start, data);
            m_watcher.RecordAccessType(MemoryAccessType::UpStreamWriteBlock);
            return;
        }

        WriteBlockFromDownStreamToCacheOnly(segmentLoc, cacheLineEntry, dataUnitEntry, data);
        auto blockStartAddr = (segmentLoc * m_size) + (cacheLineEntry * Cache_line_size);

        m_upStreamMemory->WriteBlock(blockStartAddr, { Cache_line_size, m_cache.at(cacheLineEntry).m_data.data() });

        auto stateBegin = m_cache.at(cacheLineEntry).m_state.begin() + dataUnitEntry;
        std::fill(stateBegin, stateBegin + data.Size(), State::Clean);

        m_watcher.RecordAccessType(MemoryAccessType::UpStreamWriteBlock);
    }

    [[nodiscard]] Address Size() const noexcept final {
        return m_size;
    }

    [[nodiscard]] const MemoryWatcher& GetMemoryWatcher() const noexcept final {
        return m_watcher;
    }

    [[nodiscard]] const IMemory* const GetUpStreamMemory() const noexcept final {
        return m_upStreamMemory;
    }

    void ClearCache() noexcept final {
        m_cache.clear();
    }

  private:
    static std::pmr::vector< CacheLineData > BuildInitialMemory(Address size) {
        return std::pmr::vector< CacheLineData >(size / Cache_line_size, CacheLineData {});
    }

    static Address ComputeAddressMask(Address size, Address upStreamMemorySize) noexcept {
        return (upStreamMemorySize / Cache_line_size) / (size / Cache_line_size);
    }

    void WriteToCacheOnly(Address address, DataUnit data) {
        const auto segmentLoc = address / m_size;
        assert(segmentLoc < m_segmentCount);

        const auto cacheLineEntry = address / Cache_line_size;
        const auto dataUnitEntry  = address % Cache_line_size;

        m_cache.at(cacheLineEntry).m_data.at(dataUnitEntry)  = std::move(data);
        m_cache.at(cacheLineEntry).m_state.at(dataUnitEntry) = State::Modified;
        m_watcher.RecordAccessType(MemoryAccessType::Write);
    }

    void WriteBlockFromUpStreamToCacheOnly /*Clean write*/ (std::uint64_t segment, std::uint64_t cacheLineEntry,
                                                            const DataBlock< DataUnit >& data) {
        CacheLineData cacheLineData {};
        cacheLineData.m_segmentLocInUpstream = segment;
        cacheLineData.m_state.fill(State::Clean);
        std::copy(data.Begin(), data.End(), cacheLineData.m_data.data());

        m_cache.at(cacheLineEntry) = std::move(cacheLineData);
        m_watcher.RecordAccessType(MemoryAccessType::WriteBlock);
    }
    void WriteBlockFromDownStreamToCacheOnly /*Modified write*/ (std::uint64_t segment, std::uint64_t cacheLineEntry,
                                                                 Address start, const DataBlock< DataUnit >& data) {
        CacheLineData cacheLineData {};
        cacheLineData.m_segmentLocInUpstream = segment;
        cacheLineData.m_state.fill(State::Clean);
        cacheLineData.m_data = std::move(m_cache.at(cacheLineEntry).m_data);
        std::copy(data.Begin(), data.End(), cacheLineData.m_data.data() + start);

        m_cache.at(cacheLineEntry) = std::move(cacheLineData);
        m_watcher.RecordAccessType(MemoryAccessType::WriteBlock);
    }

    Address                           m_size;
    std::pmr::vector< CacheLineData > m_cache;
    IMemory*                          m_upStreamMemory;
    mutable MemoryWatcher             m_watcher;
    const std::uint64_t               m_segmentCount;
    Object&                           m_debugObject;
};

template < class ImplDetail >
UniqueRef< CacheMemory::Impl > CacheMemory::ConstructMemory(IMemory* upStreamMemory, Address size, const Config& config,
                                                            ImplDetail detail) {
    detail->Log(LogType::Construction, "Constructing Memory of size {} Addresses!", size);
    if (config.m_cacheWriteStrategy == CacheWriteStrategy::WriteThrough) {
        if (config.m_memoryMapping == CacheMemoryMapping::DirectMapping) {
            std::pmr::polymorphic_allocator< CacheMemory::DirectAccessWriteThroughCacheMemory > alloc {};

            return allocate_unique< CacheMemory::Impl, CacheMemory::DirectAccessWriteThroughCacheMemory >(
                alloc, detail, upStreamMemory, size);
        } else if (config.m_memoryMapping == CacheMemoryMapping::FullyAssociative) {
            assert(false && "Unreachable code path!");
            return {};
        } else if (config.m_memoryMapping == CacheMemoryMapping::SetAssociative) {
            assert(false && "Unreachable code path!");
            return {};
        }
        assert(false && "Unreachable code path!");
        std::terminate();
    } else if (config.m_cacheWriteStrategy == CacheWriteStrategy::WriteThrough) {
        if (config.m_memoryMapping == CacheMemoryMapping::DirectMapping) {
            assert(false && "Unreachable code path!");
            return {};
        } else if (config.m_memoryMapping == CacheMemoryMapping::FullyAssociative) {
            assert(false && "Unreachable code path!");
            return {};

        } else if (config.m_memoryMapping == CacheMemoryMapping::SetAssociative) {
            assert(false && "Unreachable code path!");
            return {};
        }
        assert(false && "Unreachable code path!");
        std::terminate();
    }
    assert(false && "Unreachable code path!");
    std::terminate();
}

CacheMemory::CacheMemory(Config config, IMemory* upStreamMemory, Address cacheSize) :
    ICacheMemory(Default_name), m_memory(ConstructMemory(upStreamMemory, cacheSize, config, this)) {
    assert(cacheSize <= Max_cache_size);
}

CacheMemory::CacheMemory(std::string name, Config config, IMemory* upStreamMemory, Address cacheSize) :
    ICacheMemory(std::move(name)), m_memory(ConstructMemory(upStreamMemory, cacheSize, config, this)) {
    assert(cacheSize <= Max_cache_size);
}

CacheMemory::CacheMemory(CacheMemory&&) noexcept = default;

CacheMemory& CacheMemory::operator=(CacheMemory&&) noexcept = default;

CacheMemory::~CacheMemory() {
    Log(LogType::Destruction, "Destroying memory of size {} Addresses!", m_memory->Size());
}

IMemory::DataUnit CacheMemory::Read(Address address) noexcept {
    return m_memory->Read(address);
}

DataBlock< IMemory::DataUnit > CacheMemory::ReadBlock(Address start, std::uint64_t dataUnitCount) {
    return m_memory->ReadBlock(start, dataUnitCount);
}

void CacheMemory::Write(IMemory::Address address, DataUnit data) {
    return m_memory->Write(address, data);
}

void CacheMemory::WriteBlock(Address start, const DataBlock< DataUnit >& data) {
    return m_memory->WriteBlock(start, data);
}

CacheMemory::Address CacheMemory::Size() const noexcept {
    return m_memory->Size();
}

const MemoryWatcher& CacheMemory::GetMemoryWatcher() const noexcept {
    return m_memory->GetMemoryWatcher();
}

const IMemory* const CacheMemory::GetUpStreamMemory() const noexcept {
    return m_memory->GetUpStreamMemory();
}

void CacheMemory::ClearCache() noexcept {
    m_memory->ClearCache();
}

constexpr const std::pmr::vector< CacheWriteStrategy >& CacheMemory::GetSupportedWriteStrategies() noexcept {
    return supportedCacheWriteStrategies;
}

constexpr const std::pmr::vector< CacheMemoryMapping >& CacheMemory::GetSupportedMemoryMappings() noexcept {
    return supportedMemoryMappings;
}

END_NAMESPACE