#if !defined(WINDOW_H_INCLUDED_54C73F7A_5B9B_471B_BC39_78EEDBC5C35C)
    #define WINDOW_H_INCLUDED_54C73F7A_5B9B_471B_BC39_78EEDBC5C35C

    #include <API/Api.h>
    #include <Event/EventHandler.h>
    #include <Event/EventType.h>
    #include <Event/IEvent.h>
    #include <GLFW/glfw3.h>
    #include <Window/WindowProperties.h>
    #include <atomic>
    #include <memory>
    #include <mutex>

namespace arm_emu {

    class [[nodiscard]] Window {
      public:
        enum class ClientAPI
        {
            NoAPI,
            OpenGL20,
            OpenGL32,
            Vulkan,
        };

        using EventCallbackSignature = EventHandler::Callback;

        struct WindowDimensions {
            std::uint32_t m_width;
            std::uint32_t m_height;
        };

        using OnWindowCloseCallback  = void (*)(GLFWwindow*);
        using OnWindowReSizeCallback = void (*)(GLFWwindow*, int, int);

        ~Window();

        /// @brief CreateWindow is reserved in Windows "WinUser.h" so I used Create
        /// Create only initializes a new Window object, call Init and Finalize to open and close the window
        [[nodiscard]] static std::unique_ptr< Window > Create();

        [[nodiscard]] const GLFWwindow* GetHandle() const noexcept;
        [[nodiscard]] GLFWwindow*       GetHandle() noexcept;

        [[nodiscard]] std::uint32_t GetWidth() const noexcept;
        [[nodiscard]] std::uint32_t GetHeight() const noexcept;

        [[nodiscard]] WindowDimensions GetDimensions() const noexcept;
        [[nodiscard]] const char*      GetTitle() const noexcept;
        [[nodiscard]] bool             HasVSync() const noexcept;
        void                           SetEventCallback(EventType type, EventCallbackSignature&& callback) noexcept;
        void                           SetWindowCallback(OnWindowCloseCallback callback) noexcept;
        void                           SetWindowCallback(OnWindowReSizeCallback callback) noexcept;
        void                           SetVSync(bool vsync) noexcept;

        void               OnEvent(IEvent* const anEvent);
        void               OnUpdate();
        [[nodiscard]] bool Init(const WindowProperties& windowProperties, ClientAPI clientAPI) noexcept;
        void               Finalize() noexcept;

      protected:
        Window();
        Window(Window&&)      = delete;
        Window(const Window&) = delete;
        Window& operator=(Window&&) = delete;
        Window& operator=(const Window&) = delete;

        GLFWwindow*      m_window;
        EventHandler     m_eventHandler;
        WindowProperties m_windowProperties;

        static std::atomic_bool     isGLFWInitialized;
        static std::atomic_uint32_t nWindowsInitialized;
        static std::mutex           mMutex;
    };

    namespace default_callbacks {

        // Default window callbacks
        static void onWindowClose(GLFWwindow* window);
        static void onWindowReSize(GLFWwindow* window, int width, int height);

    } // namespace default_callbacks

} // namespace arm_emu

#endif // !defined(WINDOW_H_INCLUDED_54C73F7A_5B9B_471B_BC39_78EEDBC5C35C)
