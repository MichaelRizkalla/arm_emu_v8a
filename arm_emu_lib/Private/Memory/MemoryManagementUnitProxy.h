#if !defined(MEMORYMANAGEMENTUNITPROXY_H_INCLUDED_EBDF7A82_C844_48E6_827F_9625E48C01E5)
    #define MEMORYMANAGEMENTUNITPROXY_H_INCLUDED_EBDF7A82_C844_48E6_827F_9625E48C01E5

    #include <API/Api.h>
    #include <Memory/MemoryManagementUnit.h>
    #include <Utility/UniqueRef.h>

BEGIN_NAMESPACE

class [[nodiscard]] MemoryManagementUnitProxy {
  public:
    MemoryManagementUnitProxy(SharedRef< MemoryManagementUnit > mmu);

    void                           Attach(void* processAddress) noexcept;
    [[nodiscard]] IMemory::Address Translate(IMemory::Address virtualAddress) const;

  private:
    SharedRef< MemoryManagementUnit > m_mmu;
    void*                             m_processAddress;
};

END_NAMESPACE

#endif // !defined(MEMORYMANAGEMENTUNITPROXY_H_INCLUDED_EBDF7A82_C844_48E6_827F_9625E48C01E5)
