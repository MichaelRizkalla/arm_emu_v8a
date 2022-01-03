#if !defined(ASMREGISTRSNODE_H_INCLUDED_BD58C9E9_B1D6_4F9A_B298_E58449288168)
    #define ASMREGISTRSNODE_H_INCLUDED_BD58C9E9_B1D6_4F9A_B298_E58449288168

    #include <API/Api.h>
    #include <Drawable/INode.h>
    #include <string>

namespace arm_emu {

    struct [[nodiscard]] AssemblyNode : public INode {
        AssemblyNode(Window* const window);

        void          OnEvent(IEvent* const anEvent) override;
        void          OnRender() override;
        Window* const GetWindow() const noexcept override;
        void          Subscribe(EventType eventType, EventHandler::Callback handler) override;

      private:
        Window* const m_window;
        EventHandler  m_handler;

        bool        m_show;
        std::string m_result;
    };

} // namespace arm_emu

#endif // !defined(ASMREGISTRSNODE_H_INCLUDED_BD58C9E9_B1D6_4F9A_B298_E58449288168)
