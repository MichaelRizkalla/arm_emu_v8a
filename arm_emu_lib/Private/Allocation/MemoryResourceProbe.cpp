
#include <Allocation/MemoryResourceProbe.h>
#include <cassert>   // assert
#include <exception> // std::terminate

BEGIN_NAMESPACE

IMPLEMENT_AS_SINGLETON(MemoryResourceProbe)

MemoryResourceProbe::MemoryResourceProbe() noexcept : m_allocatedMemory(0), m_allocationCount(0), m_freeCount(0) {
}

void MemoryResourceProbe::OnAllocation(AllocationSize allocationSize, AllocationAlignment alignment) noexcept {
    m_allocatedMemory.emplace_back(allocationSize, alignment);
    ++m_allocationCount;
}

void MemoryResourceProbe::OnDeallocation(AllocationSize deallocationSize, AllocationAlignment alignment) noexcept {
    auto allocation = std::find(m_allocatedMemory.begin(), m_allocatedMemory.end(),
                                AllocationRequest { deallocationSize, alignment });
    if (allocation != m_allocatedMemory.end()) {
        m_allocatedMemory.erase(allocation);
        ++m_freeCount;
        return;
    }
    assert(false && "This code path should never be reachable!");
    std::terminate();
}

const std::vector< MemoryResourceProbe::AllocationRequest >& MemoryResourceProbe::GetAllocatedMemory() const noexcept {
    return m_allocatedMemory;
}

size_t MemoryResourceProbe::GetTotalAllocationCount() const noexcept {
    return m_allocationCount;
}

size_t MemoryResourceProbe::GetCurrentAllocationCount() const noexcept {
    return m_allocationCount - m_freeCount;
}

size_t MemoryResourceProbe::GetDeallocationCount() const noexcept {
    return m_freeCount;
}

END_NAMESPACE
