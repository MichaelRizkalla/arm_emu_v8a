#if !defined(INODE_H_INCLUDED_B5F0B0CA_9AA8_4F8E_B794_E507FA34BDD2)
    #define INODE_H_INCLUDED_B5F0B0CA_9AA8_4F8E_B794_E507FA34BDD2

    #include <API/Api.h>
    #include <Event/EventHandler.h>
    #include <Event/EventType.h>
    #include <Event/IEvent.h>
    #include <Window/Window.h>

namespace arm_emu {

    struct INode {
        virtual void          OnEvent(IEvent* const anEvent)                                 = 0;
        virtual void          OnRender()                                                     = 0;
        virtual Window* const GetWindow() const noexcept                                     = 0;
        virtual void          Subscribe(EventType eventType, EventHandler::Callback handler) = 0;
    };

} // namespace arm_emu

#endif // !defined(INODE_H_INCLUDED_B5F0B0CA_9AA8_4F8E_B794_E507FA34BDD2)
