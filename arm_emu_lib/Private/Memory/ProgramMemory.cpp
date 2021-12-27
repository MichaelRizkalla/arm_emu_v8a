
#include <Memory/MemoryWatcher.h>
#include <Memory/ProgramMemory.h>

BEGIN_NAMESPACE

class ProgramMemory::Impl final {
  public:
    Impl(Object* logger, Address addressableSize) :
        m_size(addressableSize), m_memory(BuildInitialMemory(m_size)), m_watcher(), m_debugObject(*logger) {
        m_debugObject.Log(LogType::Construction, "Memory construction {}!",
                          m_memory.size() > 0 ? "succeeded" : "failed");
    }

    DataUnit Read(Address address) noexcept {
        m_debugObject.LogTrace(LogType::Other, "Read memory at address: {}", address);
        Preconditions(address);

        m_watcher.RecordAccessType(MemoryAccessType::Read);

        if (m_memory.size() < address) {
            return Empty_data_unit;
        }

        return m_memory[address];
    }

    DataBlock< DataUnit > ReadBlock(Address start, std::uint64_t dataUnitCount) {
        m_debugObject.LogTrace(LogType::Other, "ReadBlock of memory starting from address: {} with length: {}", start,
                               dataUnitCount);

        auto data = DataBlock< DataUnit >(dataUnitCount, Empty_data_unit);

        // Adjust reading range in order not to overflow
        const auto currentSize  = m_memory.size();
        const auto blockEndAddr = start + dataUnitCount;

        Preconditions(blockEndAddr - 1);

        m_watcher.RecordAccessType(MemoryAccessType::ReadBlock);

        if (start >= currentSize) {
            return data; // Empty memory location
        }

        auto startCopyLoc = m_memory.begin() + start;
        if (blockEndAddr < currentSize) {
            std::copy(startCopyLoc, startCopyLoc + dataUnitCount, data.Data());
        } else {
            std::copy(startCopyLoc, m_memory.end(), data.Data());
        }
        return data;
    }

    void Write(Address address, DataUnit data) noexcept {
        m_debugObject.LogTrace(LogType::Other, "Write DataUnit: {} at address: {}", data, address);
        Preconditions(address);

        if (address > m_memory.size()) {
            m_memory.resize(address + 1, Empty_data_unit);
        }

        m_memory.at(address) = data;

        // Wait until the end of the write because resize might throw
        m_watcher.RecordAccessType(MemoryAccessType::Write);
    }

    void WriteBlock(Address start, const DataBlock< DataUnit >& data) {
        m_debugObject.LogTrace(LogType::Other, "WriteBlock of DataUnits of size: {} starting from address: {}",
                               data.Size(), start);

        const auto dataSize     = data.Size();
        const auto blockEndAddr = start + dataSize;

        // TODO: think about adding a Singleton<ErrorTracer> (?) to hold all possible error at runtime, and print stack trace
        Preconditions(blockEndAddr - 1);

        if (blockEndAddr > m_memory.size()) {
            m_memory.resize(blockEndAddr, Empty_data_unit);
        }

        std::copy(begin(data), end(data), m_memory.begin() + start);

        // Wait until the end of the write because resize might throw
        m_watcher.RecordAccessType(MemoryAccessType::WriteBlock);
    }

    Address Size() const noexcept {
        return m_size;
    }

    const MemoryWatcher& GetMemoryWatcher() const noexcept {
        return m_watcher;
    }

  private:
    void Preconditions(Address addr) const noexcept {
        assert(addr < m_size && "Out of memory bound access, undefined behavior!");
    }

    static std::pmr::vector< DataUnit > BuildInitialMemory(Address size) {
        if (size < Maximum_first_allocation_size) {
            return std::pmr::vector< DataUnit >(size, Empty_data_unit);
        } else {
            return std::pmr::vector< DataUnit >(Maximum_first_allocation_size, Empty_data_unit);
        }
    }

    inline static constexpr std::uint64_t Maximum_first_allocation_size = 10_KB;

    Address                      m_size;
    std::pmr::vector< DataUnit > m_memory;
    mutable MemoryWatcher        m_watcher;
    Object&                      m_debugObject;
};

template < class ImplDetail >
UniqueRef< ProgramMemory::Impl > ProgramMemory::ConstructMemory(Address arg, ImplDetail myself) {
    myself->Log(LogType::Construction, "Constructing memory of size {} Addresses!", arg);
    std::pmr::polymorphic_allocator< ProgramMemory::Impl > alloc {};

    return allocate_unique< ProgramMemory::Impl >(alloc, myself, arg);
}

ProgramMemory::ProgramMemory(std::string name, Address addressableSize) :
    m_memory(ConstructMemory(addressableSize, this)), IMemory(std::move(name)) {
}

ProgramMemory::ProgramMemory(ProgramMemory&&) noexcept = default;

ProgramMemory& ProgramMemory::operator=(ProgramMemory&&) noexcept = default;

ProgramMemory::~ProgramMemory() {
    Log(LogType::Destruction, "Destroying memory of size {} Addresses!", m_memory->Size());
}

ProgramMemory::DataUnit ProgramMemory::Read(Address address) noexcept {
    return m_memory->Read(address);
}

DataBlock< ProgramMemory::DataUnit > ProgramMemory::ReadBlock(Address start, std::uint64_t dataUnitCount) {
    return m_memory->ReadBlock(start, dataUnitCount);
}

void ProgramMemory::Write(Address address, DataUnit data) noexcept {
    return m_memory->Write(address, data);
}

void ProgramMemory::WriteBlock(Address start, const DataBlock< DataUnit >& data) {
    return m_memory->WriteBlock(start, data);
}

ProgramMemory::Address ProgramMemory::Size() const noexcept {
    return m_memory->Size();
}

const MemoryWatcher& ProgramMemory::GetMemoryWatcher() const noexcept {
    return m_memory->GetMemoryWatcher();
}

END_NAMESPACE
