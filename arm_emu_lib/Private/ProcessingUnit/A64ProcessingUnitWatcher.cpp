
#include <ProcessingUnit/A64ProcessingUnitWatcher.h>
#include <cassert>
#include <exception>

BEGIN_NAMESPACE

A64ProcessingUnitWatcher::A64ProcessingUnitWatcher() :
    m_branchExceptionSystemInstructions(0),
    m_dataProcessingImmediateInstructions(0),
    m_dataProcessingRegisterInstructions(0),
    m_dataProcessingScalarFloatingPointAdvancedSIMDInstructions(0),
    m_loadStoreInstructions(0),
    m_reservedInstructions(0),
    m_scalableVectorExtension(0),
    m_processCount(0),
    m_interruptCount(0) {
}

void A64ProcessingUnitWatcher::RecordInstructionHandled(InstructionType type) noexcept {
    switch (type) {
        case InstructionType::BranchExceptionSystem:
            ++m_branchExceptionSystemInstructions;
            break;
        case InstructionType::DataProcessingImmediate:
            ++m_dataProcessingImmediateInstructions;
            break;
        case InstructionType::DataProcessingRegister:
            ++m_dataProcessingRegisterInstructions;
            break;
        case InstructionType::DataProcessingScalarFloatingPointAdvancedSIMD:
            ++m_dataProcessingScalarFloatingPointAdvancedSIMDInstructions;
            break;
        case InstructionType::LoadStore:
            ++m_loadStoreInstructions;
            break;
        case InstructionType::Reserved:
            ++m_reservedInstructions;
            break;
        case InstructionType::ScalableVectorExtension:
            ++m_scalableVectorExtension;
            break;
        default:
            assert(false && "Could be should be unreachable!");
            std::terminate();
    }
}

void A64ProcessingUnitWatcher::RecordProcessHandled() noexcept {
    ++m_processCount;
}

void A64ProcessingUnitWatcher::RecordProcessInterrupted() noexcept {
    ++m_interruptCount;
}

std::size_t A64ProcessingUnitWatcher::GetInstructionCountFor(InstructionType type) const noexcept {
    switch (type) {
        case InstructionType::BranchExceptionSystem:
            return m_branchExceptionSystemInstructions;
            break;
        case InstructionType::DataProcessingImmediate:
            return m_dataProcessingImmediateInstructions;
            break;
        case InstructionType::DataProcessingRegister:
            return m_dataProcessingRegisterInstructions;
            break;
        case InstructionType::DataProcessingScalarFloatingPointAdvancedSIMD:
            return m_dataProcessingScalarFloatingPointAdvancedSIMDInstructions;
            break;
        case InstructionType::LoadStore:
            return m_loadStoreInstructions;
            break;
        case InstructionType::Reserved:
            return m_reservedInstructions;
            break;
        case InstructionType::ScalableVectorExtension:
            return m_scalableVectorExtension;
            break;
        default:
            assert(false && "Could be should be unreachable!");
            std::terminate();
    }
}

std::size_t A64ProcessingUnitWatcher::GetProcessCount() const noexcept {
    return m_processCount;
}

std::size_t A64ProcessingUnitWatcher::GetInterruptCount() const noexcept {
    return m_interruptCount;
}

END_NAMESPACE
