#if !defined(PROGRAMSTATENODE_H_INCLUDED_85E21080_9839_4CAE_8716_BFA6209D8268)
    #define PROGRAMSTATENODE_H_INCLUDED_85E21080_9839_4CAE_8716_BFA6209D8268

    #include <Compiler/Compiler.h>
    #include <Drawable/INode.h>
    #include <Program/IResult.h>
    #include <string>

namespace arm_emu {

    struct [[nodiscard]] ProgramStateNode : public INode {
        ProgramStateNode(Window* const window);

        void          OnEvent(IEvent* const anEvent) override;
        void          OnRender() override;
        Window* const GetWindow() const noexcept override;
        void          Subscribe(EventType eventType, EventHandler::Callback handler) override;

      private:
        Window* const m_window;
        EventHandler  m_handler;

        CompilationResult m_compilationResult;
        IResult*          m_result;
    };

} // namespace arm_emu

#endif // !defined(PROGRAMSTATENODE_H_INCLUDED_85E21080_9839_4CAE_8716_BFA6209D8268)
