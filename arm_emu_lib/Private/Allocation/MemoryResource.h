#if !defined(MEMORYRESOURCE_H_INCLUDED_9ACE30AE_4E7A_4ECE_BC6D_1F1762A547F0)
    #define MEMORYRESOURCE_H_INCLUDED_9ACE30AE_4E7A_4ECE_BC6D_1F1762A547F0

    #include <API/Api.h>
    #include <DebugUtils/Object.h>
    #include <cstdint>
    #include <memory>
    #include <memory_resource>

BEGIN_NAMESPACE

/// <summary>
/// At startup of the library, this memory resource will be the default using
/// std::pmr::set_default_resource.
/// It's implemented with MemoryResourceProbe in mind to track allocations
/// </summary>
class MemoryResource : public std::pmr::memory_resource, private Object {
  public:
    static constexpr const char* Default_name = "MemoryResource";

    [[nodiscard]] MemoryResource(std::string name = Default_name);
    [[nodiscard]] MemoryResource(std::pmr::memory_resource* upStream);
    [[nodiscard]] MemoryResource(std::string name, std::pmr::memory_resource* upStream);

    [[nodiscard]] MemoryResource(const MemoryResource&) = delete;
    [[nodiscard]] MemoryResource(MemoryResource&&)      = delete;
    [[nodiscard]] MemoryResource& operator=(const MemoryResource&) = delete;
    [[nodiscard]] MemoryResource& operator=(MemoryResource&&) = delete;

  private:
    std::pmr::memory_resource* m_upStream;

    [[nodiscard]] void* do_allocate(std::size_t bytes, std::size_t alignment) override;
    void                do_deallocate(void* ptr, std::size_t bytes, std::size_t alignment) override;
    [[nodiscard]] bool  do_is_equal(const memory_resource& rhs) const noexcept override;
};

extern "C" std::pmr::memory_resource* GetMemoryResource() noexcept;

END_NAMESPACE

#endif // !defined(MEMORYRESOURCE_H_INCLUDED_9ACE30AE_4E7A_4ECE_BC6D_1F1762A547F0)
