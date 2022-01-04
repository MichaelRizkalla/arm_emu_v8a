
#include <API/HiddenAPI.h>
#include <Drawable/AssemblyNode.h>
#include <Event/CompilerEvent.h>
#include <imgui.h>

BEGIN_NAMESPACE

AssemblyNode::AssemblyNode(Window* const window) : m_window(window), m_handler(), m_result(), m_show(false) {
    m_handler.Subscribe(EventType::CompilationSuccessful, [&](IEvent* const e) {
        CompilationSuccessEvent* ee = static_cast< CompilationSuccessEvent* const >(e);

        m_result = ee->GetResults().m_assembly;
        m_show   = true;
    });
    m_handler.Subscribe(EventType::HideProgramControls, [&](IEvent* const) { m_show = false; });
}

void AssemblyNode::OnEvent(IEvent* const anEvent) {
    m_handler.Handle(anEvent);
}

void AssemblyNode::OnRender() {
    if (m_show) {
        const auto dims = m_window->GetDimensions();
        ImGui::SetNextWindowPos({ dims.m_width * 0.615f, dims.m_height * 0.1f });
        ImGui::Begin("Main assembly", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::BeginChild("AsmData", { dims.m_width * 0.365f, dims.m_height * 0.4f });
        {
            ImGui::InputTextMultiline("asm", m_result.data(), m_result.size(), ImGui::GetContentRegionAvail(),
                                      ImGuiInputTextFlags_ReadOnly);
        }
        ImGui::EndChild();

        ImGui::End();
    }
}

Window* const AssemblyNode::GetWindow() const noexcept {
    return m_window;
}

void AssemblyNode::Subscribe(EventType eventType, EventHandler::Callback handler) {
    m_handler.Subscribe(eventType, handler);
}

END_NAMESPACE
