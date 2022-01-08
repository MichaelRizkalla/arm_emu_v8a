
#include <Interrupt/Interrupt.h>
#include <cassert>
#include <memory_resource>

BEGIN_NAMESPACE

void Interrupt_::Trigger() noexcept {
    m_interrupted.store(true, std::memory_order_seq_cst);
}

void Interrupt_::Reset() noexcept {
    m_interrupted.store(false, std::memory_order_seq_cst);
}

bool Interrupt_::IsTriggered() const noexcept {
    return m_interrupted.load(std::memory_order_seq_cst);
}

Interrupt CreateInterrupt() {
    std::pmr::polymorphic_allocator< Interrupt_ > alloc {};
    return std::allocate_shared< Interrupt_ >(alloc);
}

END_NAMESPACE
