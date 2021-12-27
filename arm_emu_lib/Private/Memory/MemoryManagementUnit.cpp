
#include <Memory/MemoryManagementUnit.h>

BEGIN_NAMESPACE

MemoryManagementUnit::MemoryManagementUnit(std::string name) : Object(std::move(name)), m_mappings() {
}

void MemoryManagementUnit::AddProcess(void* processAddress, IMemory::Address startAddressSpace,
                                      IMemory::Address endAddressSpace) {
    Log(LogType::MMU, "Tracking process {} with a memory space of start address {} and end address {}", processAddress,
        startAddressSpace, endAddressSpace);

    auto [_, success] = m_mappings.try_emplace(processAddress, startAddressSpace, endAddressSpace);

    if (!success) {
        throw processing_unit_already_tracked {};
    }
}

IMemory::Address MemoryManagementUnit::Translate(void* processAddress, IMemory::Address virtualAddress) const {
    LogTrace(LogType::MMU, "Translating virtual address {} from process {}", virtualAddress, processAddress);

    auto mapping = m_mappings.find(processAddress);

    if (mapping == m_mappings.end()) {
        throw untracked_processing_unit {};
    }

    auto physicalAddress = virtualAddress + mapping->second.m_start;

    if (physicalAddress >= mapping->second.m_end) {
        throw invalid_physical_memory_access {};
    }

    LogTrace(LogType::MMU, "Translated virtual address {} from process {} to physical address {}", virtualAddress,
             processAddress, physicalAddress);

    return physicalAddress;
}

IMemory::Address MemoryManagementUnit::Count() const noexcept {
    return m_mappings.size();
}

END_NAMESPACE
