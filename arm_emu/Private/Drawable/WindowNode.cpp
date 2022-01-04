
#include <API/HiddenAPI.h>
#include <CPU/SystemSettings.h>
#include <Drawable/WindowNode.h>
#include <Event/AppEvent.h>
#include <Event/CPUEvent.h>
#include <Event/CompilerEvent.h>
#include <Event/EventQueue.h>
#include <Event/InputEvent.h>
#include <cstdlib>
#include <filesystem>
#include <imgui.h>

BEGIN_NAMESPACE

namespace {

    static std::string nCores {};
    static std::string nThreadsPerCore {};
    static std::string L1CacheSize {};
    static std::string L2CacheSize {};
    static std::string L3CacheSize {};
    static std::string ramSize {};
    static std::string stackSize {};
    static std::string heapSize {};
    static bool        isInvalid = false;

    void ResetValues() noexcept {
        nCores.reserve(32);
        nThreadsPerCore.reserve(32);
        L1CacheSize.reserve(32);
        L2CacheSize.reserve(32);
        L3CacheSize.reserve(32);
        ramSize.reserve(32);
        stackSize.reserve(32);
        heapSize.reserve(32);

        nCores          = "1";
        nThreadsPerCore = "1";
        L1CacheSize     = "1024";
        L2CacheSize     = "8192";
        L3CacheSize     = "16384";
        ramSize         = "10485760";
        stackSize       = "524288";
        heapSize        = "524288";
    }

    static bool cpuPopUpMenu       = false;
    static bool compilerPopUpMenu  = false;
    static bool cpuCreationFailed  = false;
    static bool compilerLoadFailed = false;

    static constexpr const char* cpuPopUpMenuString      = "Create CPU";
    static constexpr const char* compilerPopUpMenuString = "Load Compiler";

    static constexpr const char* cpuCreationFailedString     = "CPU creation failed!";
    static constexpr const char* compilerLoadingFailedString = "Compiler loading failed!";

    static std::string cpuFailureMessage = "";

} // namespace

WindowNode::WindowNode(Window* const window) :
    m_window(window),
    m_handler(),
    m_compilerPaths(
#ifdef ARMEMU_OS_WINDOWS
        R"(C:\msys64\usr\bin\clang++.exe)"
#elif defined(ARMEMU_OS_LINUX)
        "/usr/bin/clang++"
#else
#endif
    ) {
    ResetValues();
    m_handler.Subscribe(EventType::CompilerLoadingFailure, [&](IEvent* const) { compilerLoadFailed = true; });
    m_handler.Subscribe(EventType::CPUCreationFailure, [&](IEvent* const e) {
        CPUCreationFailureEvent* ee = static_cast< CPUCreationFailureEvent* const >(e);
        cpuFailureMessage           = ee->GetMessage();
        cpuCreationFailed           = true;
    });
}

void WindowNode::OnEvent(IEvent* const anEvent) {
    m_handler.Handle(anEvent);
}

void WindowNode::OnRender() {
    static bool               dockspaceOpen           = true;
    static bool               optFullscreenPersistant = true;
    static ImGuiDockNodeFlags dockspaceFlags          = ImGuiDockNodeFlags_None;

    bool optFullscreen = optFullscreenPersistant;

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (optFullscreen) {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        windowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                       ImGuiWindowFlags_NoMove;
        windowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    if (dockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
        windowFlags |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", &dockspaceOpen, windowFlags);
    ImGui::PopStyleVar();

    if (optFullscreen) {
        ImGui::PopStyleVar(2);
    }

    // DockSpace
    ImGuiIO&    io          = ImGui::GetIO();
    ImGuiStyle& style       = ImGui::GetStyle();
    float       minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x   = 370.0f;

    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspaceId = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), dockspaceFlags);
    }

    style.WindowMinSize.x = minWinSizeX;

    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New", "Ctrl+N")) {
                EventQueue::PostEvent(CreateEvent< ClearInputEvent >());
            }

            if (ImGui::MenuItem("Open...", "Ctrl+O")) {
                EventQueue::PostEvent(CreateEvent< LoadInputFromFileEvent >());
            }

            if (ImGui::MenuItem("Save Code As...", "Ctrl+Shift+S")) {
                EventQueue::PostEvent(CreateEvent< SaveInputEvent >());
            }

            if (ImGui::MenuItem("Exit")) {
                EventQueue::PostEvent(CreateEvent< TerminateEvent >());
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit")) {
            if (ImGui::MenuItem("Create CPU", "Ctrl+P")) {
                cpuPopUpMenu = true;
            }

            if (ImGui::MenuItem("Load Compiler", "Ctrl+L")) {
                compilerPopUpMenu = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if (cpuPopUpMenu) {
        auto dims = m_window->GetDimensions();
        ImGui::SetNextWindowPos({ (dims.m_width / 2.f) - ((dims.m_width * 0.2f) / 2.0f),
                                  (dims.m_height / 2.f) - ((dims.m_height * 0.3f) / 2.0f) });

        ImGui::OpenPopup(cpuPopUpMenuString);
        CreateCPUPopUpMenu();
    }
    if (compilerPopUpMenu) {
        auto dims = m_window->GetDimensions();
        ImGui::SetNextWindowPos({ (dims.m_width / 2.f) - ((dims.m_width * 0.3f) / 2.0f),
                                  (dims.m_height / 2.f) - ((dims.m_height * 0.24f) / 2.0f) });
        ImGui::SetNextWindowSize({ dims.m_width * 0.3f, dims.m_height * 0.24f });

        ImGui::OpenPopup(compilerPopUpMenuString);
        LoadCompilerPopUpMenu();
    }
    if (cpuCreationFailed) {
        ImGui::OpenPopup(cpuCreationFailedString);
    }
    if (compilerLoadFailed) {
        ImGui::OpenPopup(compilerLoadingFailedString);
    }

    if (ImGui::BeginPopupModal(cpuCreationFailedString, nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, "Failed to create CPU!");
        ImGui::Text("Potential error message: ");
        ImGui::SameLine();
        ImGui::Text("%s", cpuFailureMessage.c_str());

        if (ImGui::Button("Ok")) {
            cpuCreationFailed = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Exit")) {
            EventQueue::PostEvent(CreateEvent< TerminateEvent >());
        }
        ImGui::EndPopup();
    }
    if (ImGui::BeginPopupModal(compilerLoadingFailedString, nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, "Couldn't find a compiler!");

        if (ImGui::Button("Ok")) {
            compilerLoadFailed = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Exit")) {
            EventQueue::PostEvent(CreateEvent< TerminateEvent >());
        }

        ImGui::EndPopup();
    }

    ImGui::End();
}

Window* const WindowNode::GetWindow() const noexcept {
    return m_window;
}

void WindowNode::Subscribe(EventType eventType, EventHandler::Callback handler) {
    m_handler.Subscribe(eventType, handler);
}

void WindowNode::CreateCPUPopUpMenu() {
    if (ImGui::BeginPopupModal(cpuPopUpMenuString, nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Enter CPU settings");

        (void)AddTextEntry("Core#:", "##Core", nCores.data(), nCores.capacity());
        (void)AddTextEntry("Thread#:", "##Thread", nThreadsPerCore.data(), nThreadsPerCore.capacity());
        (void)AddTextEntry("L1 Cache size:", "##L1Cache", L1CacheSize.data(), L1CacheSize.capacity());
        (void)AddTextEntry("L2 Cache size:", "##L2Cache", L2CacheSize.data(), L2CacheSize.capacity());
        (void)AddTextEntry("L3 Cache size:", "##L3Cache", L3CacheSize.data(), L3CacheSize.capacity());
        (void)AddTextEntry("Ram size:", "##Ram", ramSize.data(), ramSize.capacity());
        (void)AddTextEntry("Stack size:", "##Stack", stackSize.data(), stackSize.capacity());
        (void)AddTextEntry("Heap size:", "##Heap", heapSize.data(), heapSize.capacity());

        if (ImGui::Button("Create")) {
            if (ValidateEntry(nCores) && ValidateEntry(nThreadsPerCore) && ValidateEntry(L1CacheSize) &&
                ValidateEntry(L2CacheSize) && ValidateEntry(L3CacheSize) && ValidateEntry(ramSize) &&
                ValidateEntry(stackSize) && ValidateEntry(heapSize)) {

                SystemSettings systemSettings {};
                systemSettings.nCores          = static_cast< std::uint8_t >(std::stoul(nCores));
                systemSettings.nThreadsPerCore = static_cast< std::uint8_t >(std::stoul(nThreadsPerCore));
                systemSettings.L1CacheSize     = static_cast< std::uint64_t >(std::stoul(L1CacheSize));
                systemSettings.L2CacheSize     = static_cast< std::uint64_t >(std::stoul(L2CacheSize));
                systemSettings.L3CacheSize     = static_cast< std::uint64_t >(std::stoul(L3CacheSize));
                systemSettings.RamSize         = static_cast< std::uint64_t >(std::stoul(ramSize));
                systemSettings.StackSize       = static_cast< std::uint64_t >(std::stoul(stackSize));
                systemSettings.HeapSize        = static_cast< std::uint64_t >(std::stoul(heapSize));

                EventQueue::PostEvent(CreateEvent< CreateCPUEvent >(systemSettings));

                cpuPopUpMenu = false;
                isInvalid    = false;
                ResetValues();
                ImGui::CloseCurrentPopup();
            } else {
                isInvalid = true;
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            cpuPopUpMenu = false;
            isInvalid    = false;
            ResetValues();
            ImGui::CloseCurrentPopup();
        }
        if (isInvalid) {
            ImGui::SameLine();
            ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, "Invalid CPU settings!");
        }
        ImGui::EndPopup();
    }
}

void WindowNode::LoadCompilerPopUpMenu() {
    if (ImGui::BeginPopupModal(compilerPopUpMenuString, nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        std::pmr::vector< std::filesystem::path > m_paths {};

        if (m_compilerPaths.size() - std::strlen(m_compilerPaths.c_str()) < 256) {
            m_compilerPaths.resize(m_compilerPaths.size() + 256);
        }

        ImGui::Text(R"(Enter potential compiler paths separated by \n:)");
        const auto availRegion = ImGui::GetContentRegionAvail();
        (void)ImGui::InputTextMultiline("##CompilerPath", m_compilerPaths.data(), m_compilerPaths.capacity(),
                                        { availRegion.x, availRegion.y * 0.80f });

        if (ImGui::Button("Load")) {
            std::string       aPath {};
            std::stringstream ss {};
            ss << m_compilerPaths;
            while (std::getline(ss, aPath, '\n')) {
                aPath.resize(std::strlen(aPath.c_str()));
                m_paths.emplace_back(aPath);
            }

            EventQueue::PostEvent(CreateEvent< LoadCompilerEvent >(std::move(m_paths)));

            compilerPopUpMenu = false;
            m_compilerPaths.clear();
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            compilerPopUpMenu = false;
            m_compilerPaths.clear();
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

bool WindowNode::ValidateEntry(const std::string& entry) const noexcept {
    bool nonZeroFound = false;
    for (const auto& ch : entry) {
        if (ch == '\0') {
            return nonZeroFound;
        }
        if (ch != '0' && ch >= '1' && ch <= '9') {
            nonZeroFound = true;
        } else if (ch != '0') {
            return false;
        }
    }
    return nonZeroFound;
}

bool WindowNode::AddTextEntry(const char* header, const char* label, char* data, std::size_t size) {
    ImGui::Text("%s", header);
    ImGui::SameLine();
    ImGui::PushItemWidth(150);
    return ImGui::InputText(label, data, size, ImGuiInputTextFlags_EnterReturnsTrue);
}

END_NAMESPACE
