#if !defined(IEVENT_H_INCLUDED_382E72E7_FB23_4F8B_9484_86465BB46336)
    #define IEVENT_H_INCLUDED_382E72E7_FB23_4F8B_9484_86465BB46336

    #include <Event/EventCategory.h>
    #include <Event/EventType.h>
    #include <cstdint>
    #include <functional>
    #include <type_traits>

namespace arm_emu {

    struct IEvent {
      public:
        IEvent() : m_isHandled(false) {
        }
        virtual ~IEvent() = default;

        virtual EventType                               GetEventType() const noexcept       = 0;
        virtual std::underlying_type_t< EventCategory > GetEventCategories() const noexcept = 0;

        bool IsInCategory(EventCategory category) {
            return static_cast< bool >(GetEventCategories() &
                                       static_cast< std::underlying_type_t< EventCategory > >(category));
        }
        void MarkAsHandled() noexcept {
            m_isHandled = true;
        }
        bool IsHandled() const noexcept {
            return m_isHandled;
        }

      private:
        bool m_isHandled;
    };

} // namespace arm_emu

#endif // !defined(IEVENT_H_INCLUDED_382E72E7_FB23_4F8B_9484_86465BB46336)
