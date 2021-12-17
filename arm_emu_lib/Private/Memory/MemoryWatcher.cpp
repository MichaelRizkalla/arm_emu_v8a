
#include <Memory/MemoryWatcher.h>
#include <cassert>

BEGIN_NAMESPACE

MemoryWatcher::MemoryWatcher(Hint hint) : m_hits(0), m_misses(0), m_memory(), m_upStreamMemory(), m_hint(hint) {
    assert((hint == Hint::RandomAccessMemory) || (hint == Hint::CacheMemory) && "Undefined MemoryWatcherHint");
}

MemoryWatcher::MemoryWatcher(MemoryWatcher&&) noexcept = default;

MemoryWatcher& MemoryWatcher::operator=(MemoryWatcher&&) noexcept = default;

MemoryWatcher::~MemoryWatcher() = default;

void MemoryWatcher::RecordAccessType(MemoryAccessType type) {
    switch (type) {
        case MemoryAccessType::Read:
            ++m_memory.m_readCount;
            break;
        case MemoryAccessType::ReadBlock:
            ++m_memory.m_readBlockCount;
            break;
        case MemoryAccessType::Write:
            ++m_memory.m_writeCount;
            break;
        case MemoryAccessType::WriteBlock:
            ++m_memory.m_writeBlockCount;
            break;
        case MemoryAccessType::UpStreamRead:
            ++m_upStreamMemory.m_readCount;
            break;
        case MemoryAccessType::UpStreamReadBlock:
            ++m_upStreamMemory.m_readBlockCount;
            break;
        case MemoryAccessType::UpStreamWrite:
            ++m_upStreamMemory.m_writeCount;
            break;
        case MemoryAccessType::UpStreamWriteBlock:
            ++m_upStreamMemory.m_writeBlockCount;
            break;
        default:
            assert(false && "Invalid MemoryAccessType!");
    }
}

void MemoryWatcher::RecordAccessResult(MemoryAccessResult result) {
    switch (result) {
        case MemoryAccessResult::Hit:
            ++m_hits;
            break;
        case MemoryAccessResult::Miss:
            ++m_misses;
            break;
        default:
            assert(false && "Invalid MemoryAccessResult!");
    }
}

size_t MemoryWatcher::GetHitCount() const noexcept {
    return m_hits;
}

size_t MemoryWatcher::GetMissCount() const noexcept {
    return m_misses;
}

double MemoryWatcher::GetMissRatio() const noexcept {
    return m_misses / static_cast< double >(m_hits + m_misses);
}

double MemoryWatcher::GetHitRatio() const noexcept {
    return m_hits / static_cast< double >(m_hits + m_misses);
}

size_t MemoryWatcher::GetReadCount() const noexcept {
    return m_memory.m_readCount;
}

size_t MemoryWatcher::GetReadBlockCount() const noexcept {
    return m_memory.m_readBlockCount;
}

size_t MemoryWatcher::GetAllReadCount() const noexcept {
    return m_memory.m_readCount + m_memory.m_readBlockCount;
}

size_t MemoryWatcher::GetWriteCount() const noexcept {
    return m_memory.m_writeCount;
}

size_t MemoryWatcher::GetWriteBlockCount() const noexcept {
    return m_memory.m_writeBlockCount;
}

size_t MemoryWatcher::GetAllWriteCount() const noexcept {
    return m_memory.m_writeCount + m_memory.m_writeBlockCount;
}

size_t MemoryWatcher::GetUpStreamReadCount() const noexcept {
    return m_upStreamMemory.m_readCount;
}

size_t MemoryWatcher::GetUpStreamReadBlockCount() const noexcept {
    return m_upStreamMemory.m_readBlockCount;
}

size_t MemoryWatcher::GetAllUpStreamReadCount() const noexcept {
    return m_upStreamMemory.m_readCount + m_upStreamMemory.m_readBlockCount;
}

size_t MemoryWatcher::GetUpStreamWriteCount() const noexcept {
    return m_upStreamMemory.m_writeCount;
}

size_t MemoryWatcher::GetUpStreamWriteBlockCount() const noexcept {
    return m_upStreamMemory.m_writeBlockCount;
}

size_t MemoryWatcher::GetAllUpStreamWriteCount() const noexcept {
    return m_upStreamMemory.m_writeCount + m_upStreamMemory.m_writeBlockCount;
}

size_t MemoryWatcher::GetMemoryAccessCount() const noexcept {
    switch (m_hint) {
        case Hint::RandomAccessMemory:
            return GetAllReadCount() + GetAllWriteCount();
            break;
        case Hint::CacheMemory:
            return GetMissCount() + GetHitCount();
            break;
        default:
            assert(false && "Undefined MemoryWatcherHint");
            return 0;
    }
}

END_NAMESPACE