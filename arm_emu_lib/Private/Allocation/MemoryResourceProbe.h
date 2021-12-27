#if !defined(MEMORYRESOURCEPROBE_H_INCLUDED_38B5DB57_75BD_4DC8_94C8_3838DC0FA857)
    #define MEMORYRESOURCEPROBE_H_INCLUDED_38B5DB57_75BD_4DC8_94C8_3838DC0FA857

    #include <API/Api.h>
    #include <DebugUtils/Object.h>
    #include <Utility/Singleton.h>
    #include <cstdint>
    #include <mutex>
    #include <utility>
    #include <vector>

BEGIN_NAMESPACE

/// <summary>
/// A class that tracks all allocations done through MemoryResource.
/// A singleton class
/// </summary>
class MemoryResourceProbe {
  protected:
    [[nodiscard]] MemoryResourceProbe() noexcept;
    DELETE_COPY_CLASS(MemoryResourceProbe)
    DELETE_MOVE_CLASS(MemoryResourceProbe)

  public:
    using AllocationSize      = size_t;
    using AllocationAlignment = size_t;
    using AllocationRequest   = std::pair< AllocationSize, AllocationAlignment >;

    DECLARE_AS_SINGLETON(MemoryResourceProbe)

    void OnAllocation(AllocationSize allocationSize, AllocationAlignment alignment) noexcept;
    void OnDeallocation(AllocationSize deallocationSize, AllocationAlignment alignment) noexcept;

    [[nodiscard]] const std::vector< AllocationRequest >& GetAllocatedMemory() const noexcept;
    [[nodiscard]] size_t                                  GetTotalAllocationCount() const noexcept;
    [[nodiscard]] size_t                                  GetCurrentAllocationCount() const noexcept;
    [[nodiscard]] size_t                                  GetDeallocationCount() const noexcept;

  private:
    std::mutex                       m_mutex; // Needed to ensure vector atomic access
    std::vector< AllocationRequest > m_allocatedMemory;
    std::size_t                      m_allocationCount;
    std::size_t                      m_freeCount;
};

END_NAMESPACE

#endif // !defined(MEMORYRESOURCEPROBE_H_INCLUDED_38B5DB57_75BD_4DC8_94C8_3838DC0FA857)
