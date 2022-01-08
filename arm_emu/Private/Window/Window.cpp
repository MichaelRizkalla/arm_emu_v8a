
#include <API/HiddenAPI.h>
#include <Event/AppEvent.h>
#include <Event/EventQueue.h>
#include <Window/Window.h>

BEGIN_NAMESPACE

std::atomic_bool     Window::isGLFWInitialized { false };
std::atomic_uint32_t Window::nWindowsInitialized { 0 };
std::mutex           Window::mMutex {};

Window::Window() : m_window(nullptr), m_windowProperties({ 0, 0, nullptr, 0, &m_eventHandler }), m_eventHandler() {
}

Window::~Window() {
    Finalize();
};

std::unique_ptr< Window > Window::Create() {
    struct WindowMaker : public Window {
        WindowMaker()  = default;
        ~WindowMaker() = default;
    };

    return std::make_unique< WindowMaker >();
}

const GLFWwindow* Window::GetHandle() const noexcept {
    return reinterpret_cast< const GLFWwindow* >(m_window);
}

GLFWwindow* Window::GetHandle() noexcept {
    return m_window;
}

// Get and set window props
std::uint32_t Window::GetWidth() const noexcept {
    WindowProperties& data = *reinterpret_cast< WindowProperties* >(glfwGetWindowUserPointer(m_window));
    return static_cast< std::uint32_t >(data.m_width);
}

std::uint32_t Window::GetHeight() const noexcept {
    WindowProperties& data = *reinterpret_cast< WindowProperties* >(glfwGetWindowUserPointer(m_window));
    return static_cast< std::uint32_t >(data.m_height);
}

Window::WindowDimensions Window::GetDimensions() const noexcept {
    WindowProperties& data = *reinterpret_cast< WindowProperties* >(glfwGetWindowUserPointer(m_window));
    return WindowDimensions { static_cast< std::uint32_t >(data.m_width), static_cast< std::uint32_t >(data.m_height) };
}

const char* Window::GetTitle() const noexcept {
    WindowProperties& data = *reinterpret_cast< WindowProperties* >(glfwGetWindowUserPointer(m_window));
    return data.m_title;
}

bool Window::HasVSync() const noexcept {
    return m_windowProperties.m_vSync;
}

void Window::SetEventCallback(EventType type, EventCallbackSignature&& callback) noexcept {
    m_eventHandler.Subscribe(type, std::move(callback));
}

void Window::SetWindowCallback(OnWindowCloseCallback callback) noexcept {
    glfwSetWindowCloseCallback(m_window, callback);
}

void Window::SetWindowCallback(OnWindowReSizeCallback callback) noexcept {
    glfwSetWindowSizeCallback(m_window, callback);
}

void Window::SetVSync(bool vsync) noexcept {
    if (vsync)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);

    m_windowProperties.m_vSync = vsync;
}

void Window::OnEvent(IEvent* const anEvent) {
    m_eventHandler.Handle(anEvent);
}

void Window::OnUpdate() {
    glfwPollEvents();
}

bool Window::Init(const WindowProperties& windowProperties, ClientAPI clientAPI) noexcept {
    if (clientAPI == ClientAPI::NoAPI || clientAPI == ClientAPI::OpenGL20 || clientAPI == ClientAPI::Vulkan) {
        return false; /* Not implemented */
    }

    m_windowProperties = windowProperties;

    mMutex.lock();
    if (!isGLFWInitialized.load()) {
        if (!glfwInit()) {
            mMutex.unlock();
            return false;
        }
        isGLFWInitialized.store(true);
    }
    mMutex.unlock();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    m_window =
        glfwCreateWindow(static_cast< int >(m_windowProperties.m_width),
                         static_cast< int >(m_windowProperties.m_height), m_windowProperties.m_title, nullptr, nullptr);
    if (!m_window) {
        return false;
    }
    nWindowsInitialized++;
    glfwSetWindowUserPointer(m_window, &m_windowProperties);

    // Set callbacks
    SetWindowCallback(default_callbacks::onWindowClose);
    SetWindowCallback(default_callbacks::onWindowReSize);

    return true;
}

void Window::Finalize() noexcept {
    if (m_window) {
        glfwDestroyWindow(m_window);

        nWindowsInitialized--;

        if (!nWindowsInitialized.load()) {
            glfwTerminate();
        }
    }
}

namespace default_callbacks {

    void onWindowClose(GLFWwindow* window) {
        EventQueue::PostEvent(CreateEvent< TerminateEvent >());
    }

    void onWindowReSize(GLFWwindow* window, int width, int height) {
        WindowProperties& data = *reinterpret_cast< WindowProperties* >(glfwGetWindowUserPointer(window));
        data.m_width           = width;
        data.m_height          = height;
    }

} // namespace default_callbacks

END_NAMESPACE
