#if !defined(APPLICATION_H_INCLUDED_B59BE8F2_10C9_4C6E_918B_6E0147A78C25)
    #define APPLICATION_H_INCLUDED_B59BE8F2_10C9_4C6E_918B_6E0147A78C25

    #include <API/Api.h>
    #include <CPU/SystemCreator.h>
    #include <Compiler/Compiler.h>
    #include <Drawable/INode.h>
    #include <Program/Program.h>
    #include <Utility/Singleton.h>
    #include <Utility/UniqueRef.h>
    #include <Window/Window.h>
    #include <exception>
    #include <vector>

namespace arm_emu {

    class Compiler;

    class failed_to_create_window : public std::exception {
      public:
        [[nodiscard]] virtual char const* what() const noexcept override {
            return "Failed to create a window";
        }
    };

    class [[nodiscard]] Application {
      public:
        ~Application() noexcept;
        DECLARE_AS_SINGLETON(Application, true)

        void ChangeTitle(std::string newTitle) const;
        void ChangeDimensions(int width, int height) const;
        void Run();
        void Stop() noexcept;

        Program CreateProgram(const CompilationResult::ObjectData& objectData, Program::EntryPoint entryPoint) const;

      protected:
        Application() noexcept;

      private:
        EventHandler                           m_eventHandler;
        std::atomic_bool                       m_isRunning;
        std::pmr::vector< UniqueRef< INode > > m_sceneNodes;
        UniqueRef< Window >                    m_window;
        UniqueRef< ICPU >                      m_cpu;
        UniqueRef< Compiler >                  m_compiler;
        UniqueRef< ControlledResult >          m_result;
    };

} // namespace arm_emu

#endif // !defined(APPLICATION_H_INCLUDED_B59BE8F2_10C9_4C6E_918B_6E0147A78C25)
