
#include <API/HiddenAPI.h>
#include <Drawable/CodeInputNode.h>
#include <Event/CPUEvent.h>
#include <Event/CompilerEvent.h>
#include <Event/ControlEvent.h>
#include <Event/EventQueue.h>
#include <ImGuiFileDialog.h>
#include <imgui.h>

BEGIN_NAMESPACE

namespace {

    static bool compilationResult     = false;
    static bool showCompilationResult = false;
    static bool loadInputFromFile     = false;
    static bool saveInputToFile       = false;

    static constexpr const char* noCompilerLoaded = "No compiler loaded!";

    static constexpr const char* compilerLoadedFrom = "Compiler loaded from: ";

    static constexpr const char* defaultCode =
        "int main(){\n\tint x = 5;\n\n\tif (x == 7) {\n\t\treturn 0;\n\t} else if (x == 5) {\n\treturn "
        "5;\n\t}\n\treturn 0;\n}";

} // namespace

CodeInputNode::CodeInputNode(std::string header, Window* const window) :
    m_codeControlNode(window),
    m_header(std::move(header)),
    m_window(window),
    m_handler(),
    m_input(defaultCode),
    m_compilerPath(noCompilerLoaded) {
    m_handler.Subscribe(EventType::CompilationFailure, [&](IEvent* const) {
        showCompilationResult = true;
        compilationResult     = false;
    });
    m_handler.Subscribe(EventType::CompilationSuccessful, [&](IEvent* const) {
        showCompilationResult = true;
        compilationResult     = true;
    });
    m_handler.Subscribe(EventType::LoadProgram, [&](IEvent* const) {
        showCompilationResult = false;
    });
    m_handler.Subscribe(EventType::CompilerLoadingSuccessful, [&](IEvent* const e) {
        CompilerLoadingSuccessEvent* ee = static_cast< CompilerLoadingSuccessEvent* const >(e);

        m_compilerPath = compilerLoadedFrom + ee->GetCompilerPath().string();
    });
    m_handler.Subscribe(EventType::CompilerLoadingFailure, [&](IEvent* const e) {
        CompilerLoadingSuccessEvent* ee = static_cast< CompilerLoadingSuccessEvent* const >(e);

        m_compilerPath = noCompilerLoaded;
    });
    m_handler.Subscribe(EventType::LoadInputFromFile, [&](IEvent* const e) {
        loadInputFromFile = true;
    });
    m_handler.Subscribe(EventType::SaveInput, [&](IEvent* const e) {
        saveInputToFile = true;
    });
    m_handler.Subscribe(EventType::ClearInput, [&](IEvent* const) {
        m_input.clear();
    });
    m_handler.Subscribe(EventType::HideProgramControls, [&](IEvent* const) {
        showCompilationResult = false;
    });
}

void CodeInputNode::OnEvent(IEvent* const anEvent) {
    m_handler.Handle(anEvent);
    m_codeControlNode.OnEvent(anEvent);
}

void CodeInputNode::OnRender() {
    const auto dims = m_window->GetDimensions();
    ImGui::SetNextWindowPos({ dims.m_width * 0.f, dims.m_height * 0.05f });
    std::string newHeader = m_header + " - " + m_compilerPath;
    ImGui::Begin(newHeader.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

    if (m_input.size() - strlen(m_input.c_str()) < 256) {
        m_input.resize(m_input.size() + 512);
    }

    ImGui::BeginChild("Code", { dims.m_width * 0.6f, dims.m_height * 0.45f });
    {
        const auto                  availRegion    = ImGui::GetContentRegionAvail();
        [[maybe_unused]] const auto isInputChanged = ImGui::InputTextMultiline(
            m_header.c_str(), m_input.data(), m_input.size(), { availRegion.x * 1.0f, availRegion.y * 0.88f });

        if (isInputChanged) {
            compilationResult = true;
            EventQueue::PostEvent(CreateEvent< HideProgramControlsEvent >());
        }

        const auto compileCode = ImGui::Button("Compile!");
        if (compileCode) {
            compilationResult = true;
            EventQueue::PostEvent(CreateEvent< CompileCodeEvent >(m_input));
        }

        if (showCompilationResult && !compilationResult) {
            ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, "Compilation failed!");
        }

        m_codeControlNode.OnRender();

        if (showCompilationResult && compilationResult) {
            ImGui::TextColored({ 0.0f, 1.0f, 0.0f, 1.0f }, "Compilation successful!");
        }
    }
    ImGui::EndChild();

    if (loadInputFromFile) {
        ImGuiFileDialog::Instance()->OpenDialog("Open...", "Choose File", ".cpp,.h,.hpp", ".", "", 1, nullptr,
                                                ImGuiFileDialogFlags_DisableCreateDirectoryButton |
                                                    ImGuiFileDialogFlags_DontShowHiddenFiles);

        if (ImGuiFileDialog::Instance()->Display("Open...")) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                std::filesystem::path filePath { ImGuiFileDialog::Instance()->GetFilePathName() };
                if (!std::filesystem::exists(filePath)) {
                } else {
                    if (std::ifstream os(filePath, std::ios::ate); os.good()) {
                        const auto size = os.tellg();
                        os.seekg(0);
                        std::string input(size, '\0');
                        os.read(&input[0], size);
                        m_input = std::move(input);
                    }
                }
            }
            ImGuiFileDialog::Instance()->Close();
            loadInputFromFile = false;
        }
    }

    if (saveInputToFile) {
        ImGuiFileDialog::Instance()->OpenDialog("Save As...", "Choose File", ".cpp,.h,.hpp", ".", "", 1, nullptr,
                                                ImGuiFileDialogFlags_ConfirmOverwrite |
                                                    ImGuiFileDialogFlags_DontShowHiddenFiles);

        if (ImGuiFileDialog::Instance()->Display("Save As...")) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                std::filesystem::path filePath { ImGuiFileDialog::Instance()->GetFilePathName() };
                if (std::filesystem::exists(filePath)) {
                    std::filesystem::remove(filePath);
                }
                if (std::ofstream os(filePath); os.good()) {
                    os.write(m_input.data(), static_cast< std::streamsize >(std::strlen(m_input.c_str())));
                    os.flush();
                }
            }
            ImGuiFileDialog::Instance()->Close();
            saveInputToFile = false;
        }
    }

    ImGui::End();
}

Window* const CodeInputNode::GetWindow() const noexcept {
    return m_window;
}

void CodeInputNode::Subscribe(EventType eventType, EventHandler::Callback handler) {
    m_handler.Subscribe(eventType, handler);
}

END_NAMESPACE
