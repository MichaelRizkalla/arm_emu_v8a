
#include <Memory/MemoryManagementUnitProxy.h>

BEGIN_NAMESPACE

MemoryManagementUnitProxy::MemoryManagementUnitProxy(SharedRef< MemoryManagementUnit > mmu) :
    m_mmu(std::move(mmu)), m_processAddress(nullptr) {
}

void MemoryManagementUnitProxy::Attach(void* processAddress) noexcept {
    m_processAddress = processAddress;
}

IMemory::Address MemoryManagementUnitProxy::Translate(IMemory::Address virtualAddress) const {
    return m_mmu->Translate(m_processAddress, virtualAddress);
}

END_NAMESPACE
