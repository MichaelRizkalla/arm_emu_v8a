#include <Allocation/MemoryResource.h>
#include <Allocation/MemoryResourceProbe.h>

BEGIN_NAMESPACE

MemoryResource::MemoryResource(std::string name) :
    m_upStream(std::pmr::new_delete_resource()), Object(std::move(name)) {
}

MemoryResource::MemoryResource(std::pmr::memory_resource* upStream) : m_upStream(upStream), Object(Default_name) {
}

MemoryResource::MemoryResource(std::string name, std::pmr::memory_resource* upStream) :
    m_upStream(upStream), Object(std::move(name)) {
}

void* MemoryResource::do_allocate(std::size_t bytes, std::size_t alignment) {
    LogTrace(LogType::Allocation, "do_allocate called with -> bytes: {}, alignment: {}", bytes, alignment);
    auto result = m_upStream->allocate(bytes, alignment);
    if (result) {
        auto& probe = MemoryResourceProbe::GetInstance();
        probe.OnAllocation(bytes, alignment);
    }
    return result;
}

void MemoryResource::do_deallocate(void* ptr, std::size_t bytes, std::size_t alignment) {
    LogTrace(LogType::Deallocation, "do_deallocate called with -> ptr: {}, bytes: {}, alignment: {}", ptr, bytes,
             alignment);
    m_upStream->deallocate(ptr, bytes, alignment);
    auto& probe = MemoryResourceProbe::GetInstance();
    probe.OnDeallocation(bytes, alignment);
}

bool MemoryResource::do_is_equal(const memory_resource& rhs) const noexcept {
    auto rhsAddr = std::addressof(rhs);
    LogTrace(LogType::Other, "do_is_equal called with -> this: {}, rhs: {}", static_cast< const void* >(this),
             static_cast< const void* >(rhsAddr));
    return this == rhsAddr;
}

struct InitMemoryResource final {
    std::pmr::memory_resource* m_defaultMemoryResource;

    InitMemoryResource() : m_defaultMemoryResource(std::pmr::get_default_resource()) {
        std::pmr::set_default_resource(GetMemoryResource());
    }

    ~InitMemoryResource() {
        std::pmr::set_default_resource(m_defaultMemoryResource);
    }
};

namespace {
    static MemoryResource     memoryResource {};
    static InitMemoryResource initMemoryResource {};
} // namespace

extern "C" std::pmr::memory_resource* GetMemoryResource() noexcept {
    return std::addressof(memoryResource);
}

END_NAMESPACE
