
#include <API/HiddenAPI.h>
#include <Drawable/CPUSettingsNode.h>
#include <Event/CPUEvent.h>
#include <Event/EventQueue.h>
#include <imgui.h>

BEGIN_NAMESPACE

namespace {
    static bool show = false;
}

CPUSettingsNode::CPUSettingsNode(std::string header, Window* const window) :
    m_header(header), m_window(window), m_systemSettings() {
    m_handler.Subscribe(EventType::CPUCreationSuccessful, [&](IEvent* const e) {
        CPUCreationSuccessEvent* ee = static_cast< CPUCreationSuccessEvent* const >(e);
        m_systemSettings            = ee->GetSystemSettings();
        show                        = true;
    });
    m_handler.Subscribe(EventType::CPUCreationFailure, [&](IEvent* const e) {
        CPUCreationFailureEvent* ee = static_cast< CPUCreationFailureEvent* const >(e);
        m_systemSettings            = {};
        show                        = false;
    });
}

void CPUSettingsNode::OnEvent(IEvent* const anEvent) {
    m_handler.Handle(anEvent);
}

void CPUSettingsNode::OnRender() {
    if (show) {
        const auto dims = m_window->GetDimensions();
        ImGui::SetNextWindowPos({ dims.m_width * 0.78f, dims.m_height * 0.05f });
        ImGui::Begin(m_header.c_str(), nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::BeginChild("CPUSettings", { dims.m_width * 0.19f, dims.m_height * 0.2f }, true);
        {
            ImGui::Text("CPU settings");
            // ImGui::SameLine();
            ImGui::PushItemWidth(-175);
            (void)ImGui::Text("Core count: %d", m_systemSettings.nCores);
            (void)ImGui::Text("Thread count: %d", m_systemSettings.nThreadsPerCore);
            (void)ImGui::Text("L1 Cache size: %d Addresses", m_systemSettings.L1CacheSize);
            (void)ImGui::Text("L2 Cache size: %d Addresses", m_systemSettings.L2CacheSize);
            (void)ImGui::Text("L3 Cache size: %d Addresses", m_systemSettings.L3CacheSize);
            (void)ImGui::Text("Ram size: %d Addresses", m_systemSettings.RamSize);
            (void)ImGui::Text("Stack size: %d Addresses", m_systemSettings.StackSize);
            (void)ImGui::Text("Heap size: %d Addresses", m_systemSettings.HeapSize);
        }
        ImGui::EndChild();

        ImGui::End();
    }
}

Window* const CPUSettingsNode::GetWindow() const noexcept {
    return m_window;
}

void CPUSettingsNode::Subscribe(EventType eventType, EventHandler::Callback handler) {
    m_handler.Subscribe(eventType, std::move(handler));
}

END_NAMESPACE
