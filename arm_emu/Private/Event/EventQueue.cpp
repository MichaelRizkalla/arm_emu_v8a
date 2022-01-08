
#include <Event/EventQueue.h>

BEGIN_NAMESPACE

void EventQueue::PostEvent(std::unique_ptr< IEvent > anEvent) {
    std::scoped_lock lock { m_mutex };
    m_queue.push(std::move(anEvent));
}

std::unique_ptr< IEvent > EventQueue::PopEvent() noexcept {
    std::scoped_lock          lock { m_mutex };
    std::unique_ptr< IEvent > curEvent = std::move(m_queue.front());
    m_queue.pop();
    return curEvent;
}

bool EventQueue::HasEvent() noexcept {
    std::scoped_lock lock { m_mutex };
    return !m_queue.empty();
}

EventQueue::Queue EventQueue::m_queue {};
std::mutex        EventQueue::m_mutex {};

END_NAMESPACE
