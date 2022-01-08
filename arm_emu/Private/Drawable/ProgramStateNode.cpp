
#include <API/HiddenAPI.h>
#include <Drawable/ProgramStateNode.h>
#include <Event/CompilerEvent.h>
#include <Event/ProgramEvent.h>
#include <imgui.h>
#include <variant>

BEGIN_NAMESPACE

namespace {
    static bool hasCPU  = false;
    static bool hasProg = false;

    static bool show { false };

    static std::array< std::uint64_t, 33 > previousRegistersValues { 0 };
    static std::array< bool, 4 >           previousNZCV { false };
    static std::uint64_t                   prevPC { 0 };

    void ResetOldFrame() {
        previousRegistersValues.fill(0);
        previousNZCV.fill(false);
        prevPC = 0;
    }

    constexpr auto instructionSize = (sizeof(IMemory::DataUnit) / sizeof(CompilationResult::ObjectData::value_type));
} // namespace

ProgramStateNode::ProgramStateNode(Window* const window) : m_window(window), m_handler(), m_result() {
    m_handler.Subscribe(EventType::CompilationSuccessful, [&](IEvent* const e) {
        CompilationSuccessEvent* ee = static_cast< CompilationSuccessEvent* const >(e);

        m_compilationResult = ee->MoveResults();
        show                = true;
        m_result            = nullptr;
    });
    m_handler.Subscribe(EventType::HideProgramControls, [&](IEvent* const) {
        show     = false;
        m_result = nullptr;
    });
    m_handler.Subscribe(EventType::CPUCreationSuccessful, [&](IEvent* const) {
        hasCPU   = true;
        m_result = nullptr;
    });
    m_handler.Subscribe(EventType::CPUCreationFailure, [&](IEvent* const e) {
        hasCPU   = false;
        m_result = nullptr;
    });
    m_handler.Subscribe(EventType::LoadProgramFailure, [&](IEvent* const e) {
        hasProg  = false;
        m_result = nullptr;
    });
    m_handler.Subscribe(EventType::LoadProgramSuccessful, [&](IEvent* const e) {
        LoadProgramSuccessEvent* ee = static_cast< LoadProgramSuccessEvent* const >(e);

        m_result = ee->GetResult();
        hasProg  = true;
        ResetOldFrame();
    });
    m_handler.Subscribe(EventType::UpdateProgramState, [&](IEvent* const e) {
        if (show) {
            const auto frameData = m_result->GetResultFrame();

            for (std::size_t i = 0; i < 33; ++i) {
                switch (i) {
                    case 31: {
                        previousRegistersValues.at(i) = frameData.GetSP();
                    } break;
                    case 32: {
                        const auto pc = frameData.GetPC() * instructionSize;

                        previousRegistersValues.at(i) = pc;
                        prevPC                        = pc;
                    } break;
                    default: {
                        previousRegistersValues.at(i) = frameData.GetGPRegisterValue(i);
                    }
                }
            }

            previousNZCV =
                std::array< bool, 4 > { frameData.GetN(), frameData.GetZ(), frameData.GetC(), frameData.GetV() };
        }
    });
    m_handler.Subscribe(EventType::UnloadProgram, [&](IEvent* const e) {
        ResetOldFrame();
        m_result = nullptr;
        hasProg  = false;
    });
}

void ProgramStateNode::OnEvent(IEvent* const anEvent) {
    m_handler.Handle(anEvent);
}

// Credits: https://github.com/lefticus/cpp_box/blob/master/src/cpp_box.cpp
template < typename... Params >
inline void AddText(const bool enabled, const char* formatStr, Params&&... params) {
    if (!enabled) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
    }
    const std::string s     = fmt::vformat(formatStr, fmt::make_format_args(std::forward< Params >(params)...));
    const auto        begin = s.c_str();
    const auto        end   = begin + s.size();
    ImGui::TextUnformatted(begin, end);
    if (!enabled) {
        ImGui::PopStyleColor();
    }
}

void ProgramStateNode::OnRender() {
    if (show) {
        const auto dims = m_window->GetDimensions();
        ImGui::SetNextWindowPos({ dims.m_width * 0.0f, dims.m_height * 0.55f });
        ImGui::Begin("CPU and memory state", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        if (m_result) {
            {
                const auto frameData = m_result->GetResultFrame();

                if (ImGui::CollapsingHeader("Source                            ",
                                            ImGuiTreeNodeFlags_CollapsingHeader)) {

                    const auto pc          = frameData.GetPC() * instructionSize;
                    const auto objectLoc   = pc;
                    const auto currentLine = m_compilationResult.m_locations[objectLoc].m_lineNumber;

                    ImGui::BeginChild("Active Source", { dims.m_width * 0.4f, 150 });
                    std::size_t lineEnd   = 0;
                    std::size_t lineBegin = 0;
                    int         lineNum   = 1;
                    while (lineEnd != std::string::npos) {
                        lineBegin       = std::exchange(lineEnd, m_compilationResult.m_source.find('\n', lineEnd));
                        const auto line = m_compilationResult.m_source.substr(lineBegin, lineEnd - lineBegin);
                        const auto isCurrentLine = lineNum == currentLine;
                        AddText(isCurrentLine, "{:4}: {}", lineNum, line);
                        if (isCurrentLine) {
                            ImGui::SetScrollHereY();
                        }
                        if (lineEnd != std::string::npos) {
                            ++lineEnd;
                        }
                        ++lineNum;
                    }
                    ImGui::EndChild();
                }

                if (ImGui::CollapsingHeader("Memory                            ",
                                            ImGuiTreeNodeFlags_CollapsingHeader)) {
                    const auto  sp            = frameData.GetSP();
                    const auto& processMemory = frameData.GetProcessMemory();

                    const std::uint32_t stackStart = processMemory.size() - 1;
                    const auto          pcEnd      = frameData.GetPC() == std::numeric_limits< std::uint64_t >::max();
                    auto                pc         = frameData.GetPC() * instructionSize;
                    if (pcEnd) {
                        pc = prevPC;
                    }
                    const std::uint32_t pcStart = pc < 5 * 4 ? 0 : pc - 5 * 4;

                    AddText(true, "Stack Pointer (SP)     Next Instruction");

                    if (processMemory.size() > 0) {
                        for (std::uint32_t idx = 0; idx < 44; idx += 4) {
                            const auto spLoc = stackStart - idx;
                            AddText(spLoc != sp, "{:08x}: {:08x}    ", spLoc, processMemory.at(spLoc));
                            ImGui::SameLine();
                            const auto pcLoc = pcStart + idx;
                            const auto word  = m_compilationResult.m_ELFFile.m_objectFileData->at(pcLoc);
                            AddText(pcLoc == pc, "{:08x}: {:08x} {}", pcLoc, word,
                                    m_compilationResult.m_locations[pcLoc].m_disassembly.c_str());
                        }
                    }
                }

                if (ImGui::CollapsingHeader("Registers                         ",
                                            ImGuiTreeNodeFlags_CollapsingHeader)) {
                    for (std::size_t i = 0; i < 33; ++i) {
                        switch (i) {
                            case 31:
                                AddText(true, "SP ");
                                break;
                            case 32:
                                AddText(true, "PC ");
                                break;
                            default:
                                AddText(true, "R{:<2}", i);
                        }
                        ImGui::SameLine();
                        switch (i) {
                            case 31: {
                                auto value = frameData.GetSP();
                                AddText(value != previousRegistersValues.at(i), "{:08x}", value);
                            } break;
                            case 32: {
                                auto value = frameData.GetPC() * instructionSize;
                                AddText(value != previousRegistersValues.at(i), "{:08x}", value);
                            } break;
                            default: {
                                auto value = frameData.GetGPRegisterValue(i);
                                if (i == 8) {
                                    int x = 5;
                                }
                                AddText(value != previousRegistersValues.at(i), "{:08x}", value);
                            }
                        }
                        if (i != 7 && i != 15 && i != 23 && i != 30 && i != 31 && i != 32) {
                            ImGui::SameLine();
                        }
                    }

                    const auto NZCV = std::array< bool, 4 > { frameData.GetN(), frameData.GetZ(), frameData.GetC(),
                                                              frameData.GetV() };

                    AddText(true, "NZCV ");
                    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 2, 0 });
                    for (std::size_t bit = 0; bit < 4; ++bit) {
                        ImGui::SameLine();
                        const auto newBit = NZCV.at(bit);
                        const auto oldBit = previousNZCV.at(bit);
                        AddText(newBit != oldBit, "{:d}", NZCV.at(bit));
                    }
                    ImGui::PopStyleVar();
                }
            }
        } else {
            if (!hasCPU) {
                ImGui::TextColored({ 1.0f, 0.f, 0.f, 1.0f }, "Create a CPU and load the program into it!");
            } else if (!hasProg) {
                ImGui::TextColored({ 1.0f, 0.f, 0.f, 1.0f }, "Failed to load the program!");
            } else {
                ImGui::Text("Waiting for the program to be loaded into CPU!");
            }
        }
        ImGui::End();
    }
}

Window* const ProgramStateNode::GetWindow() const noexcept {
    return m_window;
}

void ProgramStateNode::Subscribe(EventType eventType, EventHandler::Callback handler) {
    m_handler.Subscribe(eventType, handler);
}

END_NAMESPACE
