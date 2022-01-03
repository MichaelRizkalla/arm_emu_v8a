#if !defined(CPUSETTINGSNODE_H_INCLUDED_A3131280_BDFF_429F_A0F9_AD36983C2682)
    #define CPUSETTINGSNODE_H_INCLUDED_A3131280_BDFF_429F_A0F9_AD36983C2682

    #include <CPU/SystemSettings.h>
    #include <Drawable/INode.h>
    #include <Event/EventHandler.h>
    #include <Window/Window.h>

namespace arm_emu {

    struct [[nodiscard]] CPUSettingsNode : public INode {
        CPUSettingsNode(std::string header, Window* const window);

        void          OnEvent(IEvent* const anEvent) override;
        void          OnRender() override;
        Window* const GetWindow() const noexcept override;
        void          Subscribe(EventType eventType, EventHandler::Callback handler) override;

      private:
        std::string   m_header;
        Window* const m_window;
        EventHandler  m_handler;

        SystemSettings m_systemSettings;
    };

} // namespace arm_emu

#endif // !defined(CPUSETTINGSNODE_H_INCLUDED_A3131280_BDFF_429F_A0F9_AD36983C2682)
