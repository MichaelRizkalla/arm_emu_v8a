#if !defined(TEXTINPUTNODE_H_INCLUDED_A046B290_C93F_420F_AEF5_1ED4322AD7DA)
    #define TEXTINPUTNODE_H_INCLUDED_A046B290_C93F_420F_AEF5_1ED4322AD7DA

    #include <API/Api.h>
    #include <Drawable/CodeControlNode.h>
    #include <Drawable/INode.h>
    #include <string>

namespace arm_emu {

    struct [[nodiscard]] CodeInputNode : public INode {
        CodeInputNode(std::string header, Window* const window);

        void               OnEvent(IEvent* const anEvent) override;
        void               OnRender() override;
        Window* const      GetWindow() const noexcept override;
        void               Subscribe(EventType eventType, EventHandler::Callback handler) override;

      private:
        CodeControlNode m_codeControlNode;
        std::string     m_header;
        std::string     m_input;
        Window* const   m_window;
        EventHandler    m_handler;

        std::string m_compilerPath;
    };

} // namespace arm_emu

#endif // !defined(TEXTINPUTNODE_H_INCLUDED_A046B290_C93F_420F_AEF5_1ED4322AD7DA)
