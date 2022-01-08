#if !defined(WINDOWNODE_H_INCLUDED_6DB2D892_6A92_4911_8D14_66F7B3BEEC21)
    #define WINDOWNODE_H_INCLUDED_6DB2D892_6A92_4911_8D14_66F7B3BEEC21

    #include <API/Api.h>
    #include <Drawable/INode.h>
    #include <Event/EventHandler.h>
    #include <Window/Window.h>

namespace arm_emu {

    struct [[nodiscard]] WindowNode : public INode {
        WindowNode(Window* const window);

        void          OnEvent(IEvent* const anEvent) override;
        void          OnRender() override;
        Window* const GetWindow() const noexcept override;
        void          Subscribe(EventType eventType, EventHandler::Callback handler) override;

      private:
        void CreateCPUPopUpMenu();
        void LoadCompilerPopUpMenu();

        bool ValidateEntry(const std::string& entry) const noexcept;

        bool AddTextEntry(const char* header, const char* label, char* data, std::size_t size);

        Window* const m_window;
        EventHandler  m_handler;

        std::string m_compilerPaths;
    };

} // namespace arm_emu

#endif // !defined(WINDOWNODE_H_INCLUDED_6DB2D892_6A92_4911_8D14_66F7B3BEEC21)
