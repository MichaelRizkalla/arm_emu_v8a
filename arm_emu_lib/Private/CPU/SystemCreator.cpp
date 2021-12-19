
#include <API/HiddenAPI.h>
#include <CPU/SystemCreator.h>

#include <CPU/A64CPU.h>

BEGIN_NAMESPACE

template < class Type >
void CPUDeleterFunc(void* ptr) noexcept {
    static_assert(0 < sizeof(Type), "can't delete an incomplete type");

    auto realCPU = static_cast< Type* >(ptr);

    std::pmr::polymorphic_allocator< Type > alloc {};

    std::allocator_traits< decltype(alloc) >::destroy(alloc, realCPU);

    alloc.deallocate(realCPU, 1);
}

UniqueRef< ICPU > SystemCreator::CreateCPU(const SystemSettings& settings) {
    switch (settings.CPUType) {
        case CPUType::A64: {
            std::pmr::polymorphic_allocator< A64CPU > alloc {};

            auto _cpu = alloc.allocate(1);

            std::allocator_traits< decltype(alloc) >::construct(
                alloc, _cpu, settings.nCores, settings.nThreadsPerCore, settings.L1CacheSize, settings.L2CacheSize,
                settings.L3CacheSize, settings.StackSize, settings.RamSize);

            return UniqueRef< ICPU >(_cpu, CPUDeleterFunc< A64CPU >);
        } break;
        case CPUType::A32: {
            return { nullptr, nullptr };
        } break;
        case CPUType::T32: {
            return { nullptr, nullptr };
        } break;
    }
    return { nullptr, nullptr };
}

UniqueRef< ICPU > SystemCreator::CreateCPU(std::string name, const SystemSettings& settings) {
    switch (settings.CPUType) {
        case CPUType::A64: {
            std::pmr::polymorphic_allocator< A64CPU > alloc {};

            auto _cpu = alloc.allocate(1);

            std::allocator_traits< decltype(alloc) >::construct(
                alloc, _cpu, name, settings.nCores, settings.nThreadsPerCore, settings.L1CacheSize,
                settings.L2CacheSize, settings.L3CacheSize, settings.StackSize, settings.RamSize);

            return UniqueRef< ICPU >(_cpu, CPUDeleterFunc< A64CPU >);
        } break;
        case CPUType::A32: {
            return { nullptr, nullptr };
        } break;
        case CPUType::T32: {
            return { nullptr, nullptr };
        } break;
    }
    return { nullptr, nullptr };
}

END_NAMESPACE