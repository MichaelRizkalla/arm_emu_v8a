#if !defined(EVENTHANDLER_H_INCLUDED_AA9E76D7_771F_4DA6_B140_9D15D6DD8B2E)
    #define EVENTHANDLER_H_INCLUDED_AA9E76D7_771F_4DA6_B140_9D15D6DD8B2E

    #include <Event/IEvent.h>
    #include <functional>
    #include <memory_resource>
    #include <unordered_map>

namespace arm_emu {

    struct EventHandler {
        using Callback = std::function< void(IEvent* const) >;
        void Handle(IEvent* const inEvent) {
            if (auto handler = m_handlers.find(inEvent->GetEventType()); handler != m_handlers.end()) {
                handler->second(inEvent);
            }
        }

        void Subscribe(EventType eventType, Callback handler) {
            m_handlers.emplace(eventType, handler);
        }

      private:
        std::pmr::unordered_map< EventType, std::function< void(IEvent* const) > > m_handlers;
    };

} // namespace arm_emu

#endif // !defined(EVENTHANDLER_H_INCLUDED_AA9E76D7_771F_4DA6_B140_9D15D6DD8B2E)
