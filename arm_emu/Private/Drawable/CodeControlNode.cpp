
#include <API/HiddenAPI.h>
#include <Drawable/CodeControlNode.h>
#include <Event/CPUEvent.h>
#include <Event/CompilerEvent.h>
#include <Event/ControlEvent.h>
#include <Event/EventQueue.h>
#include <Event/ProgramEvent.h>
#include <imgui.h>
#include <imgui_internal.h>

BEGIN_NAMESPACE

namespace {
    static bool prevLoad         = false;
    static bool show             = false;
    static bool isReadyToStepIn  = false;
    static bool isStepInFinished = false;
    static bool stepInResult     = false;
    static bool cpuExists        = false;
} // namespace

CodeControlNode::CodeControlNode(Window* const window) :
    m_window(window), m_handler(), m_objectFileData(nullptr), m_entryPointOffset(0) {
    m_handler.Subscribe(EventType::ShowProgramControls, [&](IEvent* const) {
        show            = true;
        isReadyToStepIn = false;
    });
    m_handler.Subscribe(EventType::HideProgramControls, [&](IEvent* const) {
        show            = false;
        isReadyToStepIn = false;
    });
    m_handler.Subscribe(EventType::CompilationSuccessful, [&](IEvent* const e) {
        CompilationSuccessEvent* ee = static_cast< CompilationSuccessEvent* const >(e);

        auto& result     = ee->GetResults();
        m_objectFileData = allocate_unique< std::pmr::vector< std::uint8_t > >(
            std::pmr::polymorphic_allocator< std::pmr::vector< std::uint8_t > > {},
            *(result.m_ELFFile.m_objectFileData.get()));
        m_entryPointOffset = result.m_ELFFile.m_entryPointOffset;
        isReadyToStepIn    = false;
    });
    m_handler.Subscribe(EventType::CPUCreationFailure, [&](IEvent* const e) {
        cpuExists       = false;
        isReadyToStepIn = false;
    });
    m_handler.Subscribe(EventType::CPUCreationSuccessful, [&](IEvent* const e) {
        cpuExists       = true;
        prevLoad        = false;
        isReadyToStepIn = false;
    });
    m_handler.Subscribe(EventType::LoadProgramFailure, [&](IEvent* const e) { isReadyToStepIn = false; });
    m_handler.Subscribe(EventType::StepInProgramAvailable, [&](IEvent* const e) {
        isReadyToStepIn  = true;
        isStepInFinished = false;
    });
    m_handler.Subscribe(EventType::StepInProgramFinished, [&](IEvent* const e) {
        StepInProgramFinishedEvent* ee = static_cast< StepInProgramFinishedEvent* const >(e);

        stepInResult     = ee->GetResult();
        isStepInFinished = true;
    });
}

void CodeControlNode::OnEvent(IEvent* const anEvent) {
    m_handler.Handle(anEvent);
}

void CodeControlNode::OnRender() {
    if (show && !isReadyToStepIn) {
        ImGui::SameLine();
        const auto load = ImGui::Button("Load");

        if (load) {
            if (cpuExists) {
                EventQueue::PostEvent(CreateEvent< LoadProgramEvent >(*m_objectFileData.get(), m_entryPointOffset));
                prevLoad = false;
            } else {
                ImGui::SameLine();
                ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, "Please create a CPU to load the program!");
                prevLoad = true;
            }
        }
        if (prevLoad && !cpuExists) {
            ImGui::SameLine();
            ImGui::TextColored({ 1.0f, 0.0f, 0.0f, 1.0f }, "Please create a CPU to load the program!");
        }
    } else if (isReadyToStepIn) {
        if (isStepInFinished) {
            ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
            ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
        }
        ImGui::SameLine();
        const auto stepIn = ImGui::Button("Step in");
        if (isStepInFinished) {
            ImGui::SameLine();
            if (stepInResult) {
                ImGui::TextColored({ 0.f, 1.f, 0.f, 1.f }, "Program returned successfully!");
            } else {
                ImGui::TextColored({ 1.f, 0.f, 0.f, 1.f }, "Program execution failed!");
            }
            ImGui::PopItemFlag();
            ImGui::PopStyleVar();
        }

        if (stepIn) {
            EventQueue::PostEvent(CreateEvent< StepInProgramEvent >());
        }
    }
}

Window* const CodeControlNode::GetWindow() const noexcept {
    return m_window;
}

void CodeControlNode::Subscribe(EventType eventType, EventHandler::Callback handler) {
    m_handler.Subscribe(eventType, handler);
}

END_NAMESPACE
