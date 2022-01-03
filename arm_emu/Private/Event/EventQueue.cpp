
#include <Event/EventQueue.h>

BEGIN_NAMESPACE

void EventQueue::PostEvent(UniqueRef< IEvent > anEvent) {
    std::scoped_lock lock { m_mutex };
    m_queue.push(std::move(anEvent));
}

EventQueue::Queue& EventQueue::GetQueue() noexcept {
    return m_queue;
}

EventQueue::Queue EventQueue::m_queue {};
std::mutex        EventQueue::m_mutex {};

END_NAMESPACE
