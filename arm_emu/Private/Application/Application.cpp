
#include <API/HiddenAPI.h>
#include <Application/Application.h>
#include <Compiler/Compiler.h>
#include <Drawable/AssemblyNode.h>
#include <Drawable/CPUSettingsNode.h>
#include <Drawable/CodeInputNode.h>
#include <Drawable/ProgramStateNode.h>
#include <Drawable/WindowNode.h>
#include <Event/AppEvent.h>
#include <Event/CPUEvent.h>
#include <Event/CompilerEvent.h>
#include <Event/ControlEvent.h>
#include <Event/EventQueue.h>
#include <Event/ProgramEvent.h>
#include <Memory/ProgramMemory.h>
#include <cstddef>
#include <memory_resource>
#include <thread>

#define IMGUI_IMPL_OPENGL_ES3
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include <DebugUtils/Log.h>

BEGIN_NAMESPACE

namespace {

    static int         defaultWidth { 1280 };
    static int         defaultHeight { 720 };
    static std::string defaultTitle { "arm_emu" };
    static bool        defaultVSync { false };

    void ResetDefaults() {
        defaultWidth = 1280;
        defaultWidth = 720;
        defaultTitle = "arm_emu";
        defaultVSync = false;
    }

} // namespace

IMPLEMENT_AS_SINGLETON(Application)

Application::Application() noexcept :
    m_eventHandler(), m_isRunning(false), m_window(nullptr), m_cpu(nullptr), m_compiler(nullptr), m_result(nullptr) {
    m_eventHandler.Subscribe(EventType::LoadCompiler, [&](IEvent* const e) {
        LoadCompilerEvent* ee = static_cast< LoadCompilerEvent* const >(e);

        auto& searchPaths = ee->GetSearchPaths();
        auto  compiler    = Compiler::FindCompiler(searchPaths);
        if (compiler.has_value()) {
            m_compiler = std::make_unique< Compiler >(std::move(compiler.value()));
            EventQueue::PostEvent(CreateEvent< CompilerLoadingSuccessEvent >(m_compiler->GetPath()));
        } else {
            EventQueue::PostEvent(CreateEvent< CompilerLoadingFailureEvent >());
        }
    });
    m_eventHandler.Subscribe(EventType::CompileCode, [&](IEvent* const e) {
        if (m_result) {
            EventQueue::PostEvent(CreateEvent< UnloadProgramEvent >());
        }
        if (m_compiler) {
            CompileCodeEvent* ee = static_cast< CompileCodeEvent* const >(e);

            auto result = m_compiler->Compile(ee->GetFile(), ee->GetLibraries());
            if (!result.has_value() || !result->m_ELFFile.m_validBinary) {
                EventQueue::PostEvent(CreateEvent< CompilationFailureEvent >(std::move(result.value())));
                return;
            }
            EventQueue::PostEvent(CreateEvent< ShowProgramControlsEvent >());
            EventQueue::PostEvent(CreateEvent< CompilationSuccessEvent >(std::move(result.value())));
        } else {
            EventQueue::PostEvent(CreateEvent< CompilationFailureEvent >(CompilationResult {}));
        }
    });
    m_eventHandler.Subscribe(EventType::CreateCPU, [&](IEvent* const e) {
        CreateCPUEvent* ee = static_cast< CreateCPUEvent* const >(e);

        try {
            m_cpu = SystemCreator::CreateCPU(ee->GetSystemSettings());
        } catch (cpu_creation_failure& ex) {
            EventQueue::PostEvent(CreateEvent< CPUCreationFailureEvent >(ex.what()));
        }
        if (m_cpu) {
            EventQueue::PostEvent(CreateEvent< CPUCreationSuccessEvent >(ee->GetSystemSettings()));
        } else {
            EventQueue::PostEvent(CreateEvent< CPUCreationFailureEvent >(""));
        }
    });
    m_eventHandler.Subscribe(EventType::Terminate, [&](IEvent* const) { Stop(); });
    m_eventHandler.Subscribe(EventType::LoadProgram, [&](IEvent* const e) {
        LoadProgramEvent* ee = static_cast< LoadProgramEvent* const >(e);

        auto& objectData = ee->GetObjectData();
        auto  entryPoint = ee->GetEntryPoint();

        try {
            auto program = CreateProgram(objectData, entryPoint);
            m_result     = std::make_unique< ControlledResult >(m_cpu->StepIn(std::move(program)));

            std::thread { [&]() {
                while (!m_result->CanStepIn())
                    ;
                EventQueue::PostEvent(CreateEvent< StepInProgramAvailableEvent >());
            } }.detach();

            EventQueue::PostEvent(CreateEvent< LoadProgramSuccessEvent >(m_result.get()));
        } catch (std::exception& e) {
            EventQueue::PostEvent(CreateEvent< LoadProgramFailureEvent >(std::string { e.what() }));
        }
    });
    m_eventHandler.Subscribe(EventType::UnloadProgram, [&](IEvent* const e) {
        if (m_result) {
            m_cpu->Stop();
            m_result = nullptr;
        }
    });
    m_eventHandler.Subscribe(EventType::StepInProgram, [&](IEvent* const e) {
        // TODO send an event (StepInUpdateState "?") to ProgramStateNode to update its internal old Frame
        if (m_result->CanStepIn()) {
            EventQueue::PostEvent(CreateEvent< UpdateProgramStateEvent >());
            std::thread { [&]() { m_result->StepIn(); } }.detach();
        } else {
            auto state = m_result->GetState();
            EventQueue::PostEvent(
                CreateEvent< StepInProgramFinishedEvent >(state == IResult::State::Ready ? true : false));
        }
    });

    Log::SetLogType(LogType::Instruction);
}

Application::~Application() noexcept {
    ResetDefaults();
}

void Application::ChangeTitle(std::string newTitle) const {
    defaultTitle = std::move(newTitle);
}

void Application::ChangeDimensions(int width, int height) const {
    defaultWidth  = width;
    defaultHeight = height;
}

static const char* ImGui_ImplGlfw_GetClipboardText(void* user_data) {
    return glfwGetClipboardString((GLFWwindow*)user_data);
}

static void ImGui_ImplGlfw_SetClipboardText(void* user_data, const char* text) {
    glfwSetClipboardString((GLFWwindow*)user_data, text);
}

void Application::Run() {
    m_isRunning = true;
    m_window    = Window::Create();

    WindowProperties props {};
    props.m_height = defaultHeight;
    props.m_width  = defaultWidth;
    props.m_title  = defaultTitle.c_str();
    props.m_vSync  = defaultVSync;

    if (!m_window->Init(props, Window::ClientAPI::OpenGL32)) {
        throw failed_to_create_window {};
    }

    auto windowHandle = m_window->GetHandle();
    glfwMakeContextCurrent(windowHandle);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io           = ImGui::GetIO();
    io.SetClipboardTextFn = ImGui_ImplGlfw_SetClipboardText;
    io.GetClipboardTextFn = ImGui_ImplGlfw_GetClipboardText;

    ImGui::StyleColorsDark();

    const char* glsl_version = "#version 430";
    ImGui_ImplGlfw_InitForOpenGL(windowHandle, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Create Scene nodes
    auto codeInputNode    = CodeInputNode { "C++ Code", m_window.get() };
    auto windowNode       = WindowNode { m_window.get() };
    auto cpuSettingsNode  = CPUSettingsNode { "CPU Settings", m_window.get() };
    auto assemblyNode     = AssemblyNode { m_window.get() };
    auto programStateNode = ProgramStateNode { m_window.get() };

    std::vector< INode* > m_sceneNodes {};
    m_sceneNodes.push_back(&windowNode);
    m_sceneNodes.push_back(&codeInputNode);
    m_sceneNodes.push_back(&cpuSettingsNode);
    m_sceneNodes.push_back(&assemblyNode);
    m_sceneNodes.push_back(&programStateNode);

    while (m_isRunning && !glfwWindowShouldClose(windowHandle)) {
        m_window->OnUpdate();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Handle global events
        while (EventQueue::HasEvent()) {
            auto anEvent = EventQueue::PopEvent();
            m_eventHandler.Handle(anEvent.get());
            for (auto& node : m_sceneNodes) {
                node->OnEvent(anEvent.get());
            }
            m_window->OnEvent(anEvent.get());
        }

        for (auto& node : m_sceneNodes) {
            node->OnRender();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(windowHandle);
        glfwSwapBuffers(windowHandle);
    }

    if (ImGui::GetCurrentContext()) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}

void Application::Stop() noexcept {
    m_isRunning = false;
}

Program Application::CreateProgram(const CompilationResult::ObjectData& objectData,
                                   Program::EntryPoint                  entryPoint) const {
    const IMemory::Address programSize =
        objectData.size() / (sizeof(IMemory::DataUnit) / sizeof(CompilationResult::ObjectData::value_type));

    auto memory        = allocate_unique< IMemory, ProgramMemory >(std::pmr::polymorphic_allocator< ProgramMemory > {},
                                                            "Program", programSize);
    auto programMemory = static_cast< ProgramMemory* >(memory.get());

    const auto data = reinterpret_cast< const IMemory::DataUnit* >(objectData.data());

    programMemory->WriteBlock(0, { programSize, data });

    return Program { std::move(memory),
                     entryPoint / (sizeof(IMemory::DataUnit) / sizeof(CompilationResult::ObjectData::value_type)) };
}

END_NAMESPACE
