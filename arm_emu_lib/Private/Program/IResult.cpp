
#include <ProcessingUnit/A64Registers/GeneralRegisters.h>
#include <Program/IResult.h>
#include <Program/IResultImpl.h>
#include <cassert>

BEGIN_NAMESPACE

IResult::ResultFrame::Impl::Impl() :
    m_registers(), m_PC(0), m_SP(0), m_N(false), m_C(false), m_Z(false), m_V(false), m_processMemory() {
}

IResult::ResultFrame::Impl::Impl(GPRegisters::Arch64Registers registers, std::uint64_t PC, std::uint64_t SP, bool N,
                                 bool C, bool Z, bool V, std::pmr::vector< IMemory::DataUnit > processMemory) :
    m_registers(registers),
    m_PC(PC),
    m_SP(SP),
    m_N(N),
    m_C(C),
    m_Z(Z),
    m_V(V),
    m_processMemory(std::move(processMemory)) {
}

std::uint64_t IResult::ResultFrame::Impl::GetGPRegisterValue(std::uint8_t registerLocation) const {
    assert(registerLocation < 31 && registerLocation >= 0);
    return m_registers[registerLocation].to_ullong();
}

std::uint64_t IResult::ResultFrame::Impl::GetPC() const noexcept {
    return m_PC;
}

std::uint64_t IResult::ResultFrame::Impl::GetSP() const noexcept {
    return m_SP;
}

bool IResult::ResultFrame::Impl::GetN() const noexcept {
    return m_N;
}

bool IResult::ResultFrame::Impl::GetZ() const noexcept {
    return m_Z;
}

bool IResult::ResultFrame::Impl::GetC() const noexcept {
    return m_C;
}

bool IResult::ResultFrame::Impl::GetV() const noexcept {
    return m_V;
}

const std::pmr::vector< IMemory::DataUnit >& IResult::ResultFrame::Impl::GetProcessMemory() const noexcept {
    return m_processMemory;
}

IResult::ResultFrame::ResultFrame(UniqueRef< Impl > frame) : m_frame(std::move(frame)) {
}

IResult::ResultFrame::ResultFrame() : m_frame(nullptr) {
}

std::uint64_t IResult::ResultFrame::GetGPRegisterValue(std::uint8_t registerLocation) const {
    assert(registerLocation < 31 && registerLocation >= 0);
    return m_frame->GetGPRegisterValue(registerLocation);
}

std::uint64_t IResult::ResultFrame::GetPC() const noexcept {
    return m_frame->GetPC();
}

std::uint64_t IResult::ResultFrame::GetSP() const noexcept {
    return m_frame->GetSP();
}

bool IResult::ResultFrame::GetN() const noexcept {
    return m_frame->GetN();
}

bool IResult::ResultFrame::GetZ() const noexcept {
    return m_frame->GetZ();
}

bool IResult::ResultFrame::GetC() const noexcept {
    return m_frame->GetC();
}

bool IResult::ResultFrame::GetV() const noexcept {
    return m_frame->GetV();
}

const std::pmr::vector< IMemory::DataUnit >& IResult::ResultFrame::GetProcessMemory() const noexcept {
    return m_frame->GetProcessMemory();
}

END_NAMESPACE
