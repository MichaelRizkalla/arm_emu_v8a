#if !defined(EVENTQUEUE_H_INCLUDED_18E4057D_DDE7_4F4B_A173_B1EA3F179C59)
    #define EVENTQUEUE_H_INCLUDED_18E4057D_DDE7_4F4B_A173_B1EA3F179C59

    #include <API/Api.h>
    #include <API/HiddenAPI.h>
    #include <Event/IEvent.h>
    #include <Utility/UniqueRef.h>
    #include <memory_resource>
    #include <mutex>
    #include <queue>

BEGIN_NAMESPACE

template < class TEventType, class... Args >
UniqueRef< IEvent > CreateEvent(Args&&... args) {
    return allocate_unique< IEvent, TEventType >(std::pmr::polymorphic_allocator< TEventType > {},
                                                 std::forward< Args >(args)...);
}

struct EventQueue {
    using Queue = std::queue< UniqueRef< IEvent >, std::pmr::deque< UniqueRef< IEvent > > >;

    static void   PostEvent(UniqueRef< IEvent > anEvent);
    static Queue& GetQueue() noexcept;

  private:
    static Queue      m_queue;
    static std::mutex m_mutex;
};

END_NAMESPACE

#endif // !defined(EVENTQUEUE_H_INCLUDED_18E4057D_DDE7_4F4B_A173_B1EA3F179C59)
