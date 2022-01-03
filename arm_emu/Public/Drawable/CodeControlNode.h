#if !defined(CODECONTROLNODE_H_INCLUDED_78D48582_10EE_476D_8361_0F65A33BAE64)
    #define CODECONTROLNODE_H_INCLUDED_78D48582_10EE_476D_8361_0F65A33BAE64

    #include <Drawable/INode.h>
    #include <string>

namespace arm_emu {

    struct [[nodiscard]] CodeControlNode : public INode {
        CodeControlNode(Window* const window);

        void          OnEvent(IEvent* const anEvent) override;
        void          OnRender() override;
        Window* const GetWindow() const noexcept override;
        void          Subscribe(EventType eventType, EventHandler::Callback handler) override;

      private:
        Window* const m_window;
        EventHandler  m_handler;

        std::uint64_t                                 m_entryPointOffset;
        UniqueRef< std::pmr::vector< std::uint8_t > > m_objectFileData;
    };

} // namespace arm_emu

#endif // !defined(CODECONTROLNODE_H_INCLUDED_78D48582_10EE_476D_8361_0F65A33BAE64)
