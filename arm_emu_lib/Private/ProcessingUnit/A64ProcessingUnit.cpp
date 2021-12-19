
#include <Interrupt/Interrupt.h>
#include <ProcessingUnit/A64Instruction/Instruction.h>
#include <ProcessingUnit/A64InstructionManager/A64InstructionManager.h>
#include <ProcessingUnit/A64ProcessingUnit.h>
#include <ProcessingUnit/A64ProcessingUnitWatcher.h>
#include <ProcessingUnit/A64Registers/GeneralRegisters.h>
#include <ProcessingUnit/A64Registers/SystemRegisters.h>
#include <Program/ResultElement.h>
#include <Utility/StreamableEnum.h>
#include <atomic>
#include <condition_variable>
#include <queue>
#include <set>

BEGIN_NAMESPACE

namespace {
    static const std::pmr::vector< IProcessingUnit::Extension > supportedExtensions {};

    static constexpr auto executionState = IProcessingUnit::ArchitectureProfile::AArch64;

    static constexpr auto extensionVersion = IProcessingUnit::ExtensionVersion::Armv8p0;

    static constexpr auto instructionSet = IProcessingUnit::InstructionSet::A64;

    static const std::pmr::set< IProcessingUnit::Feature > supportedFeatures {};

    static const std::pmr::set< IProcessingUnit::ExceptionLevel > supportedExceptionLevels {
        IProcessingUnit::ExceptionLevel::EL0, IProcessingUnit::ExceptionLevel::EL1
    };

    static constexpr const char* instructionLogStatement = "Executing instruction {} as {} from {} group!";

} // namespace

struct A64ProcessState : public A64ProcessingUnit::ProcessState {
    // TODO: Rework RAM-Stack accesses
    // now processes shares same RAM, this will not work for multi-threaded apps.
    // current status of the project is loading all programs on single ProcessingUnit, so it won't cause any bugs
    // however, it's something to think about once multi-threading is brought up
  private:
    struct ProgramState {
        Program                        m_program;
        bool                           m_stepIn;
        std::weak_ptr< ResultElement > m_result;
    };

    decltype(auto) PC() {
        return m_gpRegisters.PC();
    }

    decltype(auto) SP() {
        return m_gpRegisters.SP();
    }

    decltype(auto) WSP() {
        return m_gpRegisters.WSP();
    }

    void WSP(std::uint32_t data) noexcept {
        m_gpRegisters.WSP(data);
    }

    void WSP(const std::bitset< 32 >& data) noexcept {
        m_gpRegisters.WSP(data);
    }

    std::bitset< 4 > NZCV() {
        std::bitset< 4 > NZCV { 0000 };
        NZCV.set(0, V[0]);
        NZCV.set(1, C[0]);
        NZCV.set(2, Z[0]);
        NZCV.set(3, N[0]);

        return NZCV;
    }

    void SetNZCV(const std::bitset< 4 >& nzcv) noexcept {
        N.set(0, nzcv[3]);
        Z.set(0, nzcv[2]);
        C.set(0, nzcv[1]);
        V.set(0, nzcv[0]);
    }

    void Execute(Instruction&& instruction, DataProcessingImmediateGroup::PCRelativeAddressing instructionType) {
        const auto Rd    = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rd);
        auto       immhi = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Immhi); // 19bits
        auto       immlo = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Immlo); // 2bits

        switch (instructionType) {
            case DataProcessingImmediateGroup::PCRelativeAddressing::ADR: { // P.879
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingImmediateGroup::PCRelativeAddressing::ADR),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingImmediate));

                std::bitset< 64 > imm;
                std::bitset< 21 > temp_imm = Concat(immhi, immlo).ToULong();
                imm                        = SignExtend< 64 >(temp_imm);

                std::bitset< 64 > base     = PC();
                std::uint64_t     new_addr = base.to_ullong() + imm.to_ullong();
                m_gpRegisters.write(Rd, new_addr);
            } break;
            case DataProcessingImmediateGroup::PCRelativeAddressing::ADRP: { // P.880
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingImmediateGroup::PCRelativeAddressing::ADRP),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingImmediate));

                std::bitset< 64 >      imm;
                std::bitset< 21 + 12 > temp_imm = concate< 21, 12 >(Concat(immhi, immlo).ToULong(), 0);
                imm                             = SignExtend< 64 >(temp_imm);

                std::bitset< 64 > base = PC();
                for (auto i = 0; i <= 11; i++) {
                    base[i] = 0;
                }
                std::uint64_t new_addr = base.to_ullong() + imm.to_ullong();
                m_gpRegisters.write(Rd, new_addr);
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    void Execute(Instruction&& instruction, DataProcessingImmediateGroup::AddSubtractImmediate instructionType) {
        auto Rd    = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rd);
        auto Rn    = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rn);
        auto imm12 = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Imm12);
        auto shift = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Sh);

        switch (instructionType) {
            case DataProcessingImmediateGroup::AddSubtractImmediate::ADDi_32BIT: { // P.867
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingImmediateGroup::AddSubtractImmediate::ADDi_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingImmediate));

                constexpr auto datasize = 32;

                std::bitset< datasize > imm;
                if (shift) {
                    imm = ZeroExtend< datasize >(concate< 12, 12 >(imm12.ToULong(), 0));
                } else {
                    imm = ZeroExtend< datasize, 12 >(imm12.ToULong());
                }
                auto operand1    = Rn == 31 ? WSP() : m_gpRegisters.W(Rn);
                auto [result, _] = AddWithCarry(operand1, imm, 0);
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                if (Rd == 31) {
                    WSP(result);
                } else {
                    m_gpRegisters.write(Rd, result);
                }
            } break;
            case DataProcessingImmediateGroup::AddSubtractImmediate::ADDSi_32BIT: { // P.875
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingImmediateGroup::AddSubtractImmediate::ADDSi_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingImmediate));

                constexpr auto datasize = 32;

                std::bitset< datasize > imm;
                if (shift) {
                    imm = ZeroExtend< datasize >(concate< 12, 12 >(imm12.ToULong(), 0));
                } else {
                    imm = ZeroExtend< datasize, 12 >(imm12.ToULong());
                }
                auto operand1       = Rn == 31 ? WSP() : m_gpRegisters.W(Rn);
                auto [result, nzcv] = AddWithCarry(operand1, imm, 0);
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(nzcv), std::bitset< 4 > >);

                SetNZCV(nzcv);
                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingImmediateGroup::AddSubtractImmediate::SUBi_32BIT: { // P.1439
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingImmediateGroup::AddSubtractImmediate::SUBi_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingImmediate));

                constexpr auto datasize = 32;

                std::bitset< datasize > imm;
                if (shift) {
                    imm = ZeroExtend< datasize >(concate< 12, 12 >(imm12.ToULong(), 0));
                } else {
                    imm = ZeroExtend< datasize, 12 >(imm12.ToULong());
                }
                auto operand1 = Rn == 31 ? WSP() : m_gpRegisters.W(Rn);
                auto operand2 = imm;
                operand2.flip();
                auto [result, _] = AddWithCarry(operand1, operand2, static_cast< std::uint32_t >(1));
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                if (Rd == 31) {
                    WSP(result);
                } else {
                    m_gpRegisters.write(Rd, result);
                }
            } break;
            case DataProcessingImmediateGroup::AddSubtractImmediate::SUBSi_32BIT: { // P.1449
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingImmediateGroup::AddSubtractImmediate::SUBSi_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingImmediate));

                constexpr auto datasize = 32;

                std::bitset< datasize > imm;
                if (shift) {
                    imm = ZeroExtend< datasize >(concate< 12, 12 >(imm12.ToULong(), 0));
                } else {
                    imm = ZeroExtend< datasize, 12 >(imm12.ToULong());
                }
                auto operand1 = Rn == 31 ? WSP() : m_gpRegisters.W(Rn);
                auto operand2 = imm;
                operand2.flip();
                auto [result, nzcv] = AddWithCarry(operand1, operand2, static_cast< std::uint32_t >(1));
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(nzcv), std::bitset< 4 > >);

                SetNZCV(nzcv);
                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingImmediateGroup::AddSubtractImmediate::ADDi_64BIT: { // P.867
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingImmediateGroup::AddSubtractImmediate::ADDi_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingImmediate));

                constexpr auto datasize = 64;

                std::bitset< datasize > imm;
                if (shift) {
                    imm = ZeroExtend< datasize >(concate< 12, 12 >(imm12.ToULong(), 0));
                } else {
                    imm = ZeroExtend< datasize, 12 >(imm12.ToULong());
                }
                auto operand1    = Rn == 31 ? SP() : m_gpRegisters.X(Rn);
                auto [result, _] = AddWithCarry(operand1, imm, 0);
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                if (Rd == 31) {
                    SP() = result;
                } else {
                    m_gpRegisters.write(Rd, result);
                }
            } break;
            case DataProcessingImmediateGroup::AddSubtractImmediate::ADDSi_64BIT: { // P.875
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingImmediateGroup::AddSubtractImmediate::ADDSi_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingImmediate));

                constexpr auto datasize = 64;

                std::bitset< datasize > imm;
                if (shift) {
                    imm = ZeroExtend< datasize >(concate< 12, 12 >(imm12.ToULong(), 0));
                } else {
                    imm = ZeroExtend< datasize, 12 >(imm12.ToULong());
                }
                auto operand1 = Rn == 31 ? SP() : m_gpRegisters.X(Rn);

                auto [result, nzcv] = AddWithCarry(operand1, imm, 0);
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(nzcv), std::bitset< 4 > >);

                SetNZCV(nzcv);
                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingImmediateGroup::AddSubtractImmediate::SUBi_64BIT: { // P.1439
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingImmediateGroup::AddSubtractImmediate::SUBi_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingImmediate));

                constexpr auto datasize = 64;

                std::bitset< datasize > imm;
                if (shift) {
                    imm = ZeroExtend< datasize >(concate< 12, 12 >(imm12.ToULong(), 0));
                } else {
                    imm = ZeroExtend< datasize, 12 >(imm12.ToULong());
                }
                auto operand1 = Rn == 31 ? SP() : m_gpRegisters.X(Rn);
                auto operand2 = imm;
                operand2.flip();
                auto [result, _] = AddWithCarry(operand1, operand2, static_cast< std::uint64_t >(1));
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                if (Rd == 31) {
                    SP() = result;
                } else {
                    m_gpRegisters.write(Rd, result);
                }
            } break;
            case DataProcessingImmediateGroup::AddSubtractImmediate::SUBSi_64BIT: { // P.1449
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingImmediateGroup::AddSubtractImmediate::SUBSi_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingImmediate));

                constexpr auto datasize = 64;

                std::bitset< datasize > imm;
                if (shift) {
                    imm = ZeroExtend< datasize >(concate< 12, 12 >(imm12.ToULong(), 0));
                } else
                    imm = ZeroExtend< datasize, 12 >(imm12.ToULong());
                auto operand1 = Rn == 31 ? SP() : m_gpRegisters.X(Rn);
                auto operand2 = imm;
                operand2.flip();
                auto [result, nzcv] = AddWithCarry(operand1, imm, static_cast< std::uint64_t >(1));
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(nzcv), std::bitset< 4 > >);

                SetNZCV(nzcv);
                m_gpRegisters.write(Rd, result);
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    void Execute(Instruction&& instruction, DataProcessingImmediateGroup::AddSubtractImmediateTag instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&& instruction, DataProcessingImmediateGroup::LogicalImmediate instructionType) {
        auto Rd   = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rd);
        auto Rn   = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rn);
        auto imms = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Imms);
        auto immr = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Immr);
        auto N    = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::N);

        switch (instructionType) {
            case DataProcessingImmediateGroup::LogicalImmediate::AND_32BIT: { // P.881
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingImmediateGroup::LogicalImmediate::AND_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingImmediate));

                if (N != 0) {
                    throw undefined_behaviour {};
                }
                constexpr auto datasize = 32;

                auto [imm, _] = DecodeBitMasks< datasize >(static_cast< bool >(N), imms, immr, true);
                static_assert(std::is_same_v< decltype(imm), std::bitset< datasize > >);
                auto operand1 = m_gpRegisters.W(Rn);
                auto result   = operand1 & imm;
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                if (Rd == 31) {
                    WSP(result);
                } else {
                    m_gpRegisters.write(Rd, result);
                }
            } break;
            case DataProcessingImmediateGroup::LogicalImmediate::ORR_32BIT: { // P.1240
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingImmediateGroup::LogicalImmediate::ORR_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingImmediate));

                if (N != 0) {
                    throw undefined_behaviour {};
                }
                constexpr auto datasize = 32;

                auto [imm, _] = DecodeBitMasks< datasize >(static_cast< bool >(N), imms, immr, true);

                static_assert(std::is_same_v< decltype(imm), std::bitset< datasize > >);

                auto operand1 = m_gpRegisters.W(Rn);
                auto result   = operand1 | imm;

                if (Rd == 31) {
                    WSP(result);
                } else {
                    m_gpRegisters.write(Rd, result);
                }
            } break;
            case DataProcessingImmediateGroup::LogicalImmediate::EOR_32BIT: { // P.1006
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingImmediateGroup::LogicalImmediate::EOR_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingImmediate));

                if (N != 0) {
                    throw undefined_behaviour {};
                }
                constexpr auto datasize = 32;

                auto [imm, _] = DecodeBitMasks< datasize >(static_cast< bool >(N), imms, immr, true);

                static_assert(std::is_same_v< decltype(imm), std::bitset< datasize > >);

                auto operand1 = m_gpRegisters.W(Rn);
                auto result   = operand1 xor imm;

                if (Rd == 31) {
                    WSP(result);
                } else {
                    m_gpRegisters.write(Rd, result);
                }
            } break;
            case DataProcessingImmediateGroup::LogicalImmediate::ANDS_32BIT: { // P.885
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingImmediateGroup::LogicalImmediate::ANDS_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingImmediate));

                if (N == 0) {
                    throw undefined_behaviour {};
                }
                constexpr auto datasize = 32;

                auto [imm, _] = DecodeBitMasks< datasize >(static_cast< bool >(N), imms, immr, true);
                static_assert(std::is_same_v< decltype(imm), std::bitset< datasize > >);

                auto operand1 = m_gpRegisters.W(Rn);
                auto result   = operand1 & imm;
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                std::bitset< 4 > nzcv = concate< 2, 2 >(concate< 1, 1 >(result[31] ? 1 : 0, result == 0 ? 1 : 0), 0);
                SetNZCV(nzcv);
                if (Rd == 31) { // Documentation does not state SP as an option to Rd
                    WSP(result);
                } else {
                    m_gpRegisters.write(Rd, result);
                }
            } break;
            case DataProcessingImmediateGroup::LogicalImmediate::AND_64BIT: { // P.881
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingImmediateGroup::LogicalImmediate::AND_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingImmediate));

                constexpr auto datasize = 64;

                auto [imm, _] = DecodeBitMasks< datasize >(static_cast< bool >(N), imms, immr, true);

                static_assert(std::is_same_v< decltype(imm), std::bitset< datasize > >);

                std::bitset< datasize > operand1 = m_gpRegisters.X(Rn);
                auto                    result   = operand1 & imm;
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                if (Rd == 31) {
                    SP() = result;
                } else {
                    m_gpRegisters.write(Rd, result);
                }
            } break;
            case DataProcessingImmediateGroup::LogicalImmediate::ORR_64BIT: { // P.1240
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingImmediateGroup::LogicalImmediate::ORR_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingImmediate));

                constexpr auto datasize = 64;

                auto [imm, _] = DecodeBitMasks< datasize >(static_cast< bool >(N), imms, immr, true);

                static_assert(std::is_same_v< decltype(imm), std::bitset< datasize > >);

                std::bitset< datasize > operand1 = m_gpRegisters.X(Rn);
                auto                    result   = operand1 | imm;

                if (Rd == 31) {
                    SP() = result;
                } else {
                    m_gpRegisters.write(Rd, result);
                }
            } break;
            case DataProcessingImmediateGroup::LogicalImmediate::EOR_64BIT: { // P.1006
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingImmediateGroup::LogicalImmediate::EOR_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingImmediate));

                constexpr auto datasize = 64;

                auto [imm, _] = DecodeBitMasks< datasize >(static_cast< bool >(N), imms, immr, true);

                static_assert(std::is_same_v< decltype(imm), std::bitset< datasize > >);

                std::bitset< datasize > operand1 = m_gpRegisters.X(Rn);
                auto                    result   = operand1 xor imm;

                if (Rd == 31) {
                    SP() = result;
                } else {
                    m_gpRegisters.write(Rd, result);
                }
            } break;
            case DataProcessingImmediateGroup::LogicalImmediate::ANDS_64BIT: { // // P.885
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingImmediateGroup::LogicalImmediate::ANDS_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingImmediate));

                constexpr auto datasize = 64;

                auto [imm, _] = DecodeBitMasks< datasize >(static_cast< bool >(N), imms, immr, true);
                static_assert(std::is_same_v< decltype(imm), std::bitset< datasize > >);

                std::bitset< datasize > operand1 = m_gpRegisters.X(Rn);
                auto                    result   = operand1 & imm;

                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                std::bitset< 4 > nzcv = concate< 2, 2 >(concate< 1, 1 >(result[63] ? 1 : 0, result == 0 ? 1 : 0), 0);
                SetNZCV(nzcv);
                if (Rd == 31) { // Documentation does not state SP as an option to Rd
                    SP() = result;
                } else {
                    m_gpRegisters.write(Rd, result);
                }
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    void Execute(Instruction&& instruction, DataProcessingImmediateGroup::MoveWideImmediate instructionType) {
        auto Rd    = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rd);
        auto imm16 = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Imm16);
        auto hw    = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Hw);

        switch (instructionType) {
            case DataProcessingImmediateGroup::MoveWideImmediate::MOVN_32BIT: { // P.1215
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingImmediateGroup::MoveWideImmediate::MOVN_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingImmediate));

                if (hw.ToULong() >= 2) {
                    throw undefined_behaviour {};
                }
                constexpr auto datasize = 32;

                std::uint8_t            pos = hw.ToULong() << 4;
                std::bitset< datasize > result { 0 };
                auto                    imm16_ = imm16.ToULong();
                for (auto i = pos; i < pos + 16; i++) {
                    result[i] = static_cast< bool >((static_cast< std::uint16_t >(1) << (i - pos)) & imm16_);
                }
                result.flip();

                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingImmediateGroup::MoveWideImmediate::MOVZ_32BIT: { // P.1217
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingImmediateGroup::MoveWideImmediate::MOVZ_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingImmediate));

                if (hw.ToULong() >= 2) {
                    throw undefined_behaviour {};
                }
                constexpr auto datasize = 32;

                std::uint8_t            pos = hw.ToULong() << 4;
                std::bitset< datasize > result(0);
                auto                    imm16_ = imm16.ToULong();
                for (auto i = pos; i < pos + 16; i++) {
                    result[i] = static_cast< bool >((static_cast< std::uint16_t >(1) << (i - pos)) & imm16_);
                }
                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingImmediateGroup::MoveWideImmediate::MOVK_32BIT: { // P.1213
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingImmediateGroup::MoveWideImmediate::MOVK_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingImmediate));

                if (hw.ToULong() >= 2) {
                    throw undefined_behaviour {};
                }
                constexpr auto datasize = 32;

                std::uint8_t pos    = hw.ToULong() << 4;
                auto         result = m_gpRegisters.W(Rd);
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                auto imm16_ = imm16.ToULong();
                for (auto i = pos; i < pos + 16; i++) {
                    result[i] = static_cast< bool >((static_cast< std::uint16_t >(1) << (i - pos)) & imm16_);
                }
                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingImmediateGroup::MoveWideImmediate::MOVN_64BIT: { // P.1215
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingImmediateGroup::MoveWideImmediate::MOVN_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingImmediate));

                constexpr auto datasize = 64;

                std::uint8_t            pos = hw.ToULong() << 4;
                std::bitset< datasize > result { 0 };
                auto                    imm16_ = imm16.ToULLong();
                for (auto i = pos; i < pos + 16; i++) {
                    result[i] = static_cast< bool >((static_cast< std::uint16_t >(1) << (i - pos)) & imm16_);
                }
                result.flip();

                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingImmediateGroup::MoveWideImmediate::MOVZ_64BIT: { // P.1217
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingImmediateGroup::MoveWideImmediate::MOVZ_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingImmediate));

                constexpr auto datasize = 64;

                std::uint8_t            pos = hw.ToULong() << 4;
                std::bitset< datasize > result(0);
                auto                    imm16_ = imm16.ToULLong();
                for (auto i = pos; i < pos + 16; i++) {
                    result[i] = static_cast< bool >((static_cast< std::uint16_t >(1) << (i - pos)) & imm16_);
                }
                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingImmediateGroup::MoveWideImmediate::MOVK_64BIT: { // P.1213
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingImmediateGroup::MoveWideImmediate::MOVK_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingImmediate));

                constexpr auto datasize = 64;

                std::uint8_t pos    = hw.ToULong() << 4;
                auto         result = m_gpRegisters.X(Rd);
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                auto imm16_ = imm16.ToULLong();
                for (auto i = pos; i < pos + 16; i++) {
                    result[i] = static_cast< bool >((static_cast< std::uint16_t >(1) << (i - pos)) & imm16_);
                }
                m_gpRegisters.write(Rd, result);
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    void Execute(Instruction&& instruction, DataProcessingImmediateGroup::Bitfield instructionType) {
        auto Rd   = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rd);
        auto Rn   = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rn);
        auto imms = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Imms);
        auto immr = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Immr);
        auto N    = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::N);

        switch (instructionType) {
            case DataProcessingImmediateGroup::Bitfield::SBFM_32BIT: { // P.1288
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingImmediateGroup::Bitfield::SBFM_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingImmediate));

                constexpr auto datasize = 32;

                auto R = static_cast< int >(immr.ToULong());
                auto S = static_cast< int >(imms.ToULong());

                auto [wmask, tmask] = DecodeBitMasks< datasize >(static_cast< bool >(Rn), imms, immr, false);

                auto src = m_gpRegisters.W(Rn);
                static_assert(std::is_same_v< decltype(src), std::bitset< datasize > >);

                // perform bitfield move on low bits
                auto bot = ROR< datasize >(src, R) & wmask;

                // determine extension bits (sign, zero or dest register)
                auto top = Replicate< datasize, 1 >(std::bitset< 1 > { src[S] });

                // combine extension bits and result bits
                auto bot_AND_tmask = bot & tmask;
                tmask.flip();
                auto top_AND_NOT_tmask = top & tmask;

                m_gpRegisters.write(Rd, bot_AND_tmask | top_AND_NOT_tmask);
            } break;
            case DataProcessingImmediateGroup::Bitfield::BFM_32BIT: { // P.910
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingImmediateGroup::Bitfield::BFM_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingImmediate));

                if (N != 0 || immr[5] != 0 || imms[5] != 0)
                    throw undefined_behaviour {};
                constexpr auto datasize = 32;

                Bitset R = immr;

                auto [wmask, tmask] = DecodeBitMasks< datasize >(static_cast< bool >(N), imms, immr, false);
                static_assert(std::is_same_v< decltype(wmask), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(tmask), std::bitset< datasize > >);

                std::bitset< datasize > dst = m_gpRegisters.W(Rd);
                std::bitset< datasize > src = m_gpRegisters.W(Rn);

                std::bitset< datasize > bot = (dst & ~wmask) | (ROR< datasize >(src, R.ToULong()) & wmask);

                m_gpRegisters.write(Rd, (dst & ~tmask) | (bot & tmask));
            } break;
            case DataProcessingImmediateGroup::Bitfield::UBFM_32BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingImmediateGroup::Bitfield::SBFM_64BIT: { // P.1288
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingImmediateGroup::Bitfield::SBFM_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingImmediate));

                constexpr auto datasize = 64;

                auto R = static_cast< int >(immr.ToULong());
                auto S = static_cast< int >(imms.ToULong());

                auto [wmask, tmask] = DecodeBitMasks< datasize >(static_cast< bool >(Rn), imms, immr, false);

                auto src = m_gpRegisters.X(Rn);
                static_assert(std::is_same_v< decltype(src), std::bitset< datasize > >);

                // perform bitfield move on low bits
                auto bot = ROR< datasize >(src, R) & wmask;

                // determine extension bits (sign, zero or dest register)
                auto top = Replicate< datasize, 1 >(std::bitset< 1 > { src[S] });

                // combine extension bits and result bits
                auto bot_AND_tmask = bot & tmask;
                tmask.flip();
                auto top_AND_NOT_tmask = top & tmask;

                m_gpRegisters.write(Rd, bot_AND_tmask | top_AND_NOT_tmask);
            } break;
            case DataProcessingImmediateGroup::Bitfield::BFM_64BIT: { // P.910
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingImmediateGroup::Bitfield::BFM_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingImmediate));

                if (N != 1)
                    throw undefined_behaviour {};
                constexpr auto datasize = 64;

                Bitset R = immr;

                auto [wmask, tmask] = DecodeBitMasks< datasize >(static_cast< bool >(N), imms, immr, false);
                static_assert(std::is_same_v< decltype(wmask), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(tmask), std::bitset< datasize > >);

                std::bitset< datasize > dst = m_gpRegisters.X(Rd);
                std::bitset< datasize > src = m_gpRegisters.X(Rn);

                std::bitset< datasize > bot = (dst & ~wmask) | (ROR< datasize >(src, static_cast< bool >(R)) & wmask);

                m_gpRegisters.write(Rd, (dst & ~tmask) | (bot & tmask));
            } break;
            case DataProcessingImmediateGroup::Bitfield::UBFM_64BIT: {
                throw not_implemented_feature {};
            } break;
            default:
                throw undefined_instruction {};
                break;
        }
    }
    void Execute(Instruction&& instruction, DataProcessingImmediateGroup::Extract instructionType) {
        throw not_implemented_feature {};
    }

    void Execute(Instruction&& instruction, BranchExceptionSystemGroup::ConditionalBranching instructionType) {
        auto cond  = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Cond);
        auto imm19 = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Imm19);

        switch (instructionType) {
            case BranchExceptionSystemGroup::ConditionalBranching::BCond: { // P.904
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(BranchExceptionSystemGroup::ConditionalBranching::BCond),
                                  Enum::ToChar(A64DecodeGroup::BranchExceptionSystem));

                std::bitset< 64 > offset =
                    SignExtend< 64, 21 >(imm19.Concat(Bitset { 2, 0 }).ToULLong() / sizeof(IMemory::DataUnit));
                const auto pc = PC() - 1;
                if (ConditionHolds(cond, NZCV())) {
                    BranchTo(pc + offset.to_ullong(), IProcessingUnit::BranchType::Dir);
                }
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    void Execute(Instruction&& instruction, BranchExceptionSystemGroup::ExceptionGeneration instructionType) {
        auto imm16 = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Imm16);

        switch (instructionType) {
            case BranchExceptionSystemGroup::ExceptionGeneration::SVC: {
                throw not_implemented_feature {};
            } break;
            case BranchExceptionSystemGroup::ExceptionGeneration::HVC: {
                throw not_implemented_feature {};
            } break;
            case BranchExceptionSystemGroup::ExceptionGeneration::SMC: {
                throw not_implemented_feature {};
            } break;
            case BranchExceptionSystemGroup::ExceptionGeneration::BRK: { // P.925
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(BranchExceptionSystemGroup::ExceptionGeneration::BRK),
                                  Enum::ToChar(A64DecodeGroup::BranchExceptionSystem));

                // TODO
                // if (AArch64::HaveBTIExt()) always false
                // SetBTypeCompatible(TRUE);
                SoftwareBreakpoint(imm16.ToULong());
            } break;
            case BranchExceptionSystemGroup::ExceptionGeneration::HLT: {
                throw not_implemented_feature {};
            } break;
            case BranchExceptionSystemGroup::ExceptionGeneration::DCPS1: {
                throw not_implemented_feature {};
            } break;
            case BranchExceptionSystemGroup::ExceptionGeneration::DCPS2: {
                throw not_implemented_feature {};
            } break;
            case BranchExceptionSystemGroup::ExceptionGeneration::DCPS3: {
                throw not_implemented_feature {};
            } break;
        }
    }
    void Execute(Instruction&& instruction, BranchExceptionSystemGroup::Hints instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&& instruction, BranchExceptionSystemGroup::Barriers instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&& instruction, BranchExceptionSystemGroup::PState instructionType) {

        switch (instructionType) {
            case BranchExceptionSystemGroup::PState::CFINV: {
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(BranchExceptionSystemGroup::PState::CFINV),
                                  Enum::ToChar(A64DecodeGroup::BranchExceptionSystem));

                if (!HaveFlagManipulateExt()) {
                    throw unsupported_instruction {};
                }

                C = C;
            } break;
            case BranchExceptionSystemGroup::PState::XAFLAG: {

            } break;
            case BranchExceptionSystemGroup::PState::AXFLAG: {
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(BranchExceptionSystemGroup::PState::AXFLAG),
                                  Enum::ToChar(A64DecodeGroup::BranchExceptionSystem));

                if (!HaveFlagManipulateExt())
                    throw unsupported_instruction {};

                bool Z_ = (Z | V).any();
                bool C_ = (C & ~V).any();

                N = 0;
                Z = Z_;
                C = C_;
                V = 0;
            } break;
            default:
                throw undefined_instruction {};
                break;
        }
    }
    void Execute(Instruction&& instruction, BranchExceptionSystemGroup::SystemInstruction instructionType) {
        auto Rt  = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rt);
        auto op2 = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Op2);
        auto CRm = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::CRm);
        auto CRn = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::CRn);
        auto op1 = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Op1);
        auto L   = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::L);

        switch (instructionType) {
            case BranchExceptionSystemGroup::SystemInstruction::SYS: { // P.1465
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(BranchExceptionSystemGroup::SystemInstruction::SYS),
                                  Enum::ToChar(A64DecodeGroup::BranchExceptionSystem));

                AArch64CheckSystemAccess(0b01, op1.ToULong(), CRn.ToULong(), CRm.ToULong(), op2.ToULong(), Rt.ToULong(),
                                         L.ToULong());

                std::uint32_t sys_op1 = static_cast< std::uint32_t >(op1.ToULong());
                std::uint32_t sys_op2 = static_cast< std::uint32_t >(op2.ToULong());
                std::uint32_t sys_crn = static_cast< std::uint32_t >(CRn.ToULong());
                std::uint32_t sys_crm = static_cast< std::uint32_t >(CRm.ToULong());

                SysInstr(1, sys_op1, sys_crn, sys_crm, sys_op2, m_gpRegisters.X(Rt));
            } break;
            case BranchExceptionSystemGroup::SystemInstruction::SYSL: {
                throw not_implemented_feature {};
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    void Execute(Instruction&& instruction, BranchExceptionSystemGroup::SystemRegisterMove instructionType) {
        auto Rt  = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rt);
        auto op2 = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Op2);
        auto CRm = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::CRm);
        auto CRn = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::CRn);
        auto op1 = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Op1);
        auto o0  = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::O0);
        auto L   = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::L);

        switch (instructionType) {
            case BranchExceptionSystemGroup::SystemRegisterMove::MSRr: { // P.1223
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(BranchExceptionSystemGroup::SystemRegisterMove::MSRr),
                                  Enum::ToChar(A64DecodeGroup::BranchExceptionSystem));

                auto op0 = static_cast< std::uint8_t >(2) + static_cast< std::uint8_t >(o0);

                AArch64CheckSystemAccess(op0, op1.ToULong(), CRn.ToULong(), CRm.ToULong(), op2.ToULong(), Rt.ToULong(),
                                         L.ToULong());

                SysRegWrite(op0, op1.ToULong(), CRn.ToULong(), CRm.ToULong(), op2.ToULong(), m_gpRegisters.X(Rt));
            } break;
            case BranchExceptionSystemGroup::SystemRegisterMove::MRS: { // P.1219
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(BranchExceptionSystemGroup::SystemRegisterMove::MRS),
                                  Enum::ToChar(A64DecodeGroup::BranchExceptionSystem));

                auto op0 = static_cast< std::uint8_t >(2) + static_cast< std::uint8_t >(o0);

                m_gpRegisters.write(Rt, SysRegRead(op0, op1.ToULong(), CRn.ToULong(), CRm.ToULong(), op2.ToULong()));
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    void Execute(Instruction&& instruction, BranchExceptionSystemGroup::UnconditionalBranchRegister instructionType) {
        auto Rn = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rn);
        auto Rm = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rm);
        auto A  = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::A);
        auto M  = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::M);
        auto Z  = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Z);

        switch (instructionType) {
            case BranchExceptionSystemGroup::UnconditionalBranchRegister::BR: { // P.922
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(BranchExceptionSystemGroup::UnconditionalBranchRegister::BR),
                                  Enum::ToChar(A64DecodeGroup::BranchExceptionSystem));

                const auto& target = m_gpRegisters.X(Rn);
                static_assert(std::is_same_v< std::remove_cvref_t< decltype(target) >, std::bitset< 64 > >);

                BranchTo(target.to_ullong(), IProcessingUnit::BranchType::IndCall);
            } break;
            case BranchExceptionSystemGroup::UnconditionalBranchRegister::BRAAZ: {
                throw not_implemented_feature {};
            } break;
            case BranchExceptionSystemGroup::UnconditionalBranchRegister::BRABZ: {
                throw not_implemented_feature {};
            } break;
            case BranchExceptionSystemGroup::UnconditionalBranchRegister::BLR: { // P.919
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(BranchExceptionSystemGroup::UnconditionalBranchRegister::BLR),
                                  Enum::ToChar(A64DecodeGroup::BranchExceptionSystem));

                const auto& target = m_gpRegisters.X(Rn);
                static_assert(std::is_same_v< std::remove_cvref_t< decltype(target) >, std::bitset< 64 > >);

                const auto pc = PC();
                m_gpRegisters.write(30, pc + 4);
                BranchTo(target.to_ullong(), IProcessingUnit::BranchType::IndCall);
            } break;
            case BranchExceptionSystemGroup::UnconditionalBranchRegister::BLRAAZ: {
                throw not_implemented_feature {};
            } break;
            case BranchExceptionSystemGroup::UnconditionalBranchRegister::BLRABZ: {
                throw not_implemented_feature {};
            } break;
            case BranchExceptionSystemGroup::UnconditionalBranchRegister::RET: { // P.1265
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(BranchExceptionSystemGroup::UnconditionalBranchRegister::RET),
                                  Enum::ToChar(A64DecodeGroup::BranchExceptionSystem));

                const auto& target = m_gpRegisters.X(Rn);
                static_assert(std::is_same_v< std::remove_cvref_t< decltype(target) >, std::bitset< 64 > >);
                BranchTo(target.to_ullong(), IProcessingUnit::BranchType::Ret);
            } break;
            case BranchExceptionSystemGroup::UnconditionalBranchRegister::RETAA: { // P.1266
                /*if constexpr (HasArchVersion(ArchVersion::ARMv8p3)) {
                    throw not_implemented_feature {};
                } else {
                    throw unsupported_instruction {};
                }*/
            } break;
            case BranchExceptionSystemGroup::UnconditionalBranchRegister::RETAB: { // P.1266
                /*if constexpr (HasArchVersion(ArchVersion::ARMv8p3)) {
                    throw not_implemented_feature {};
                } else {
                    throw unsupported_instruction {};
                }*/
            } break;
            case BranchExceptionSystemGroup::UnconditionalBranchRegister::ERET: {
                throw not_implemented_feature {};
            } break;
            case BranchExceptionSystemGroup::UnconditionalBranchRegister::ERETAA: {
                throw not_implemented_feature {};
            } break;
            case BranchExceptionSystemGroup::UnconditionalBranchRegister::ERETAB: {
                throw not_implemented_feature {};
            } break;
            case BranchExceptionSystemGroup::UnconditionalBranchRegister::DRPS: {
                throw not_implemented_feature {};
            } break;
            case BranchExceptionSystemGroup::UnconditionalBranchRegister::BRAA: {
                throw not_implemented_feature {};
            } break;
            case BranchExceptionSystemGroup::UnconditionalBranchRegister::BRAB: {
                throw not_implemented_feature {};
            } break;
            case BranchExceptionSystemGroup::UnconditionalBranchRegister::BLRAA: {
                throw not_implemented_feature {};
            } break;
            case BranchExceptionSystemGroup::UnconditionalBranchRegister::BLRAB: {
                throw not_implemented_feature {};
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    void Execute(Instruction&& instruction, BranchExceptionSystemGroup::UnconditionalBranchImmediate instructionType) {
        auto imm26 = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Imm26);

        switch (instructionType) {
            case BranchExceptionSystemGroup::UnconditionalBranchImmediate::B: { // P.905
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(BranchExceptionSystemGroup::UnconditionalBranchImmediate::B),
                                  Enum::ToChar(A64DecodeGroup::BranchExceptionSystem));

                std::bitset< 64 > offset =
                    (SignExtend< 64 >(concate< 26, 2 >(imm26.ToULong(), 0)).to_ullong() / sizeof(IMemory::DataUnit));
                const auto pc = PC();
                BranchTo(pc + offset.to_ullong(), IProcessingUnit::BranchType::Dir);
            } break;
            case BranchExceptionSystemGroup::UnconditionalBranchImmediate::BL: { // P.918
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(BranchExceptionSystemGroup::UnconditionalBranchImmediate::BL),
                                  Enum::ToChar(A64DecodeGroup::BranchExceptionSystem));

                std::bitset< 64 > offset =
                    (SignExtend< 64 >(concate< 26, 2 >(imm26.ToULong(), 0)).to_ullong() / sizeof(IMemory::DataUnit));
                const auto pc = PC();
                m_gpRegisters.write(30, pc + 4);
                BranchTo(pc + offset.to_ullong(), IProcessingUnit::BranchType::DirCall);
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    void Execute(Instruction&& instruction, BranchExceptionSystemGroup::CompareAndBranchImmediate instructionType) {
        auto Rt    = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rt);
        auto imm19 = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Imm19);

        switch (instructionType) {
            case BranchExceptionSystemGroup::CompareAndBranchImmediate::CBZ_32BIT: { // P.939
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(BranchExceptionSystemGroup::CompareAndBranchImmediate::CBZ_32BIT),
                                  Enum::ToChar(A64DecodeGroup::BranchExceptionSystem));

                constexpr auto datasize = 32;

                auto offset = SignExtend< 64 >(concate< 19, 2 >(imm19.ToULong(), 0));

                auto operand1 = m_gpRegisters.W(Rt);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);

                const auto pc = PC();
                if (operand1 == 0) {
                    BranchTo(pc + offset.to_ullong(), IProcessingUnit::BranchType::Dir);
                }
            } break;
            case BranchExceptionSystemGroup::CompareAndBranchImmediate::CBNZ_32BIT: { // P.938
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(BranchExceptionSystemGroup::CompareAndBranchImmediate::CBNZ_32BIT),
                                  Enum::ToChar(A64DecodeGroup::BranchExceptionSystem));

                constexpr auto datasize = 32;

                auto offset = SignExtend< 64 >(concate< 19, 2 >(imm19.ToULong(), 0));

                auto operand1 = m_gpRegisters.W(Rt);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);

                const auto pc = PC();
                if (operand1 != 0) {
                    BranchTo(pc + offset.to_ullong(), IProcessingUnit::BranchType::Dir);
                }
            } break;
            case BranchExceptionSystemGroup::CompareAndBranchImmediate::CBZ_64BIT: { // P.939
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(BranchExceptionSystemGroup::CompareAndBranchImmediate::CBZ_64BIT),
                                  Enum::ToChar(A64DecodeGroup::BranchExceptionSystem));

                constexpr auto datasize = 64;

                auto offset = SignExtend< 64 >(concate< 19, 2 >(imm19.ToULong(), 0));

                auto operand1 = m_gpRegisters.X(Rt);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);

                const auto pc = PC();
                if (operand1 == 0) {
                    BranchTo(pc + offset.to_ullong(), IProcessingUnit::BranchType::Dir);
                }
            } break;
            case BranchExceptionSystemGroup::CompareAndBranchImmediate::CBNZ_64BIT: { // P.938
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(BranchExceptionSystemGroup::CompareAndBranchImmediate::CBNZ_64BIT),
                                  Enum::ToChar(A64DecodeGroup::BranchExceptionSystem));

                constexpr auto datasize = 64;

                auto offset = SignExtend< 64 >(concate< 19, 2 >(imm19.ToULong(), 0));

                auto operand1 = m_gpRegisters.X(Rt);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);

                const auto pc = PC();
                if (operand1 != 0) {
                    BranchTo(pc + offset.to_ullong(), IProcessingUnit::BranchType::Dir);
                }
            } break;
            default:
                throw undefined_instruction {};
                break;
        }
    }
    void Execute(Instruction&& instruction, BranchExceptionSystemGroup::TestAndBranchImmediate instructionType) {
        throw not_implemented_feature {};
    }

    void Execute(Instruction&& instruction, LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructures instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&&                                                      instruction,
                 LoadStoreGroup::AdvancedSIMDLoadStoreMultipleStructuresPostIndexed instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&& instruction, LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructure instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&&                                                   instruction,
                 LoadStoreGroup::AdvancedSIMDLoadStoreSingleStructurePostIndexed instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&& instruction, LoadStoreGroup::LoadStoreMemoryTag instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&& instruction, LoadStoreGroup::LoadStoreExclusive instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&& instruction, LoadStoreGroup::LdaprStlrUnscaledImmediate instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&& instruction, LoadStoreGroup::LoadRegisterLiteral instructionType) {
        auto Rt    = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rt);
        auto imm19 = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Imm19);
        (void)Rt;
        (void)imm19;

        switch (instructionType) {
            case LoadStoreGroup::LoadRegisterLiteral::LDR_32BIT_LITERAL: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadRegisterLiteral::LDR_32BIT_LITERAL_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadRegisterLiteral::LDR_64BIT_LITERAL: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadRegisterLiteral::LDR_64BIT_LITERAL_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadRegisterLiteral::LDRSW_LITERAL: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadRegisterLiteral::LDR_128BIT_LITERAL_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadRegisterLiteral::PRFM_LITERAL: { // P.1255
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(LoadStoreGroup::LoadRegisterLiteral::PRFM_LITERAL),
                                  Enum::ToChar(A64DecodeGroup::LoadStore));
                // No-op
                // This system emulation is depending on O(1) memory access, no operation is needed to pre-fetch
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    void Execute(Instruction&& instruction, LoadStoreGroup::LoadStoreNoAllocatePairOffset instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&& instruction, LoadStoreGroup::LoadStoreRegisterPairPostIndexed instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&& instruction, LoadStoreGroup::LoadStoreRegisterPairOffset instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&& instruction, LoadStoreGroup::LoadStoreRegisterPairPreIndexed instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&& instruction, LoadStoreGroup::LoadStoreRegisterUnscaledImmediate instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&& instruction, LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed instructionType) {
        auto Rt   = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rt);
        auto Rn   = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rn);
        auto imm9 = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Imm9);
        auto size = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Size);
        (void)Rt;
        (void)Rn;
        (void)imm9;
        (void)size;

        switch (instructionType) {
            case LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::STRBi: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::LDRBi: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::LDRSBi_64BIT: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::LDRSBi_32BIT: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::STRi_8BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::LDRi_8BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::STRi_128BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::LDRi_128BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::STRHi: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::LDRHi: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::LDRSHi_64BIT: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::LDRSHi_32BIT: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::STRi_16BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::LDRi_16BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::STRi_32BIT: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::LDRi_32BIT: {
                throw not_implemented_feature {};
                /* incomplete if (size == 0b10) throw undefined_behaviour {};
                 bool wback     = true;
                 bool postindex = true;

                 std::bitset< 64 > offset = SignExtend< 64 >(std::bitset< 9 > { imm9 });*/
            } break;
            case LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::LDRSWi: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::STRi_32BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::LDRi_32BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::STRi_64BIT: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::LDRi_64BIT: {
                throw not_implemented_feature {};
                // if (size == 0b11) throw undefined_behaviour {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::STRi_64BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterImmediatePostIndexed::LDRi_64BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    void Execute(Instruction&& instruction, LoadStoreGroup::LoadStoreRegisterUnprivileged instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&& instruction, LoadStoreGroup::LoadStoreRegisterImmediatePreIndexed instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&& instruction, LoadStoreGroup::AtomicMemoryOperation instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&& instruction, LoadStoreGroup::LoadStoreRegisterRegisterOffset instructionType) {
        auto Rt     = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rt);
        auto Rn     = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rn);
        auto S      = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::S);
        auto option = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Option);
        auto Rm     = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rm);
        auto size   = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Size);

        switch (instructionType) {
            case LoadStoreGroup::LoadStoreRegisterRegisterOffset::STRBr_EXTENDED: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterRegisterOffset::STRBr_SHIFTED: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterRegisterOffset::LDRBr_EXTENDED: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterRegisterOffset::LDRBr_SHIFTED: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterRegisterOffset::LDRSBr_64BIT_EXTENDED: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterRegisterOffset::LDRSBr_64BIT_SHIFTED: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterRegisterOffset::LDRSBr_32BIT_EXTENDED: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterRegisterOffset::LDRSBr_32BIT_SHIFTED: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterRegisterOffset::STRr_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterRegisterOffset::LDRr_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterRegisterOffset::STRHr: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterRegisterOffset::LDRHr: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterRegisterOffset::LDRSHr_64BIT: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterRegisterOffset::LDRSHr_32BIT: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterRegisterOffset::STRr_32BIT: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterRegisterOffset::LDRr_32BIT: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterRegisterOffset::LDRSWr: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterRegisterOffset::STRr_64BIT: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterRegisterOffset::LDRr_64BIT: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterRegisterOffset::PRFMr: { // P.1257
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(LoadStoreGroup::LoadStoreRegisterRegisterOffset::PRFMr),
                                  Enum::ToChar(A64DecodeGroup::LoadStore));
                // No-op
                // This system emulation is depending on O(1) memory access, no operation is needed to pre-fetch
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    void Execute(Instruction&& instruction, LoadStoreGroup::LoadStoreRegisterPAC instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&& instruction, LoadStoreGroup::LoadStoreRegisterUnsignedImmediate instructionType) {
        auto Rt    = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rt);
        auto Rn    = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rn);
        auto imm12 = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Imm12);
        auto size  = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Size);

        switch (instructionType) {
            case LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::STRBi: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::LDRBi: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::LDRSBi_64BIT: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::LDRSBi_32BIT: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::STRi_8BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::LDRi_8BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::STRi_128BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::LDRi_128BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::STRHi: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::LDRHi: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::LDRSHi_64BIT: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::LDRSHi_32BIT: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::STRi_16BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::LDRi_16BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::STRi_32BIT: { // P.1261 + 88
                // TODO
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::STRi_32BIT),
                                  Enum::ToChar(A64DecodeGroup::LoadStore));

                if (size != 0b10) {
                    throw undefined_behaviour {};
                }
                auto wback     = false;
                auto postindex = false;

                auto offset = LSL< 64 >(ZeroExtend< 64 >(std::bitset< 12 > { imm12.ToULong() }), size.ToULong());
                static_assert(std::is_same_v< decltype(offset), std::bitset< 64 > >);

                constexpr std::uint32_t datasize    = 8 << 0b10;
                bool                    tag_checked = wback || (Rn != 31);

                if (supportedFeatures.find(IProcessingUnit::Feature::FEAT_MTE) != supportedFeatures.end()) {
                    (void)tag_checked;
                    // SetTagCheckedInstruction(tag_checked);
                }
                std::bitset< 64 >       address;
                std::bitset< datasize > data;

                bool rt_unknown = false;

                if (wback && Rn == Rt && Rn != 31) {
                    /*auto c = ALU::ConstrainUnpredictable();
                    if (c != Constraint::NONE && c != Constraint::UNKNOWN && c != Constraint::UNDEF && c !=
                    Constraint::NOP) throw undefined_behaviour {}; switch (c) { case Constraint::NONE: { rt_unknown =
                    false; } break; case Constraint::UNKNOWN: { rt_unknown = true; } break; case Constraint::UNDEF: {
                            throw undefined_behaviour {};
                        } break;
                        case Constraint::NOP: {
                            EndOfInstruction(); // return
                        } break;
                    }*/
                }

                if (Rn == 31) {
                    // checkSPAlignment();
                    address = SP();
                } else {
                    address = m_gpRegisters.X(Rn);
                }

                if (!postindex) {
                    address = address.to_ullong() + offset.to_ullong();
                }

                if (rt_unknown) {
                    throw undefined_behaviour {};
                    // data = std::bitset< datasize >(0);
                } else {
                    data = m_gpRegisters.W(Rt);
                }
                // ALU::Mem[address, datasize / 8, AccType::NORMAL] = data;
                m_upStreamMemory->Write(address.to_ullong(), data.to_ullong());

                if (wback) {
                    if (postindex) {
                        address = address.to_ullong() + offset.to_ullong();
                    } else if (Rn == 31) {
                        SP() = address;
                    } else {
                        m_gpRegisters.write(Rn, address);
                    }
                }
            } break;
            case LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::LDRi_32BIT: { // // P.997 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::LDRi_32BIT),
                                  Enum::ToChar(A64DecodeGroup::LoadStore));

                if (size != 0b10) {
                    throw undefined_behaviour {};
                }
                bool wback     = false;
                bool postindex = false;

                auto offset = LSL< 64 >(ZeroExtend< 64 >(std::bitset< 12 > { imm12.ToULong() }), size.ToULong());
                static_assert(std::is_same_v< decltype(offset), std::bitset< 64 > >);

                constexpr std::uint32_t datasize = 8 << 0b10;
                if (supportedFeatures.find(IProcessingUnit::Feature::FEAT_MTE) != supportedFeatures.end()) {
                    // SetTagCheckedInstruction(tag_checked);
                }
                std::bitset< 64 >       address;
                std::bitset< datasize > data;

                bool wb_unknown = false;

                if (wback && Rn == Rt && Rn != 31) {
                    /*auto c = ALU::ConstrainUnpredictable();
                    if (c != Constraint::NONE && c != Constraint::UNKNOWN && c != Constraint::UNDEF && c !=
                    Constraint::NOP) throw undefined_behaviour {}; switch (c) { case Constraint::NONE: { rt_unknown =
                    false; } break; case Constraint::UNKNOWN: { rt_unknown = true; } break; case Constraint::UNDEF: {
                    throw undefined_behaviour {};
                    } break;
                    case Constraint::NOP: {
                    EndOfInstruction(); // return
                    } break;
                    }*/
                }

                if (Rn == 31) {
                    // checkSPAlignment();
                    address = SP();
                } else {
                    address = m_gpRegisters.X(Rn);
                }

                if (!postindex) {
                    address = address.to_ullong() + offset.to_ullong();
                }

                // data = ALU::Mem[address, datasize / 8, AccType::NORMAL];
                data = m_upStreamMemory->Read(address.to_ullong());
                m_gpRegisters.write(Rt, data);

                if (wback) {
                    if (wb_unknown) {
                        throw undefined_behaviour {};
                    } else if (postindex) {
                        address = address.to_ullong() + offset.to_ullong();
                    }

                    if (Rn == 31) {
                        SP() = address;
                    } else {
                        m_gpRegisters.write(Rn, address);
                    }
                }
            } break;
            case LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::LDRSWi: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::STRi_32BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::LDRi_32BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::STRi_64BIT: { // P.1261 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::STRi_64BIT),
                                  Enum::ToChar(A64DecodeGroup::LoadStore));

                /**/
                if (size != 0b11) {
                    throw undefined_behaviour {};
                }
                auto wback     = false;
                auto postindex = false;

                auto offset = LSL< 64 >(ZeroExtend< 64 >(std::bitset< 12 > { imm12.ToULong() }), size.ToULong());
                static_assert(std::is_same_v< decltype(offset), std::bitset< 64 > >);

                constexpr std::uint32_t datasize    = 8 << 0b11;
                bool                    tag_checked = wback || (Rn != 31);

                if (supportedFeatures.find(IProcessingUnit::Feature::FEAT_MTE) != supportedFeatures.end()) {
                    (void)tag_checked;
                    // SetTagCheckedInstruction(tag_checked);
                }
                std::bitset< 64 >       address;
                std::bitset< datasize > data;

                bool rt_unknown = false;

                if (wback && Rn == Rt && Rn != 31) {
                    /*auto c = ALU::ConstrainUnpredictable();
                    if (c != Constraint::NONE && c != Constraint::UNKNOWN && c != Constraint::UNDEF && c !=
                    Constraint::NOP) throw undefined_behaviour {}; switch (c) { case Constraint::NONE: { rt_unknown =
                    false; } break; case Constraint::UNKNOWN: { rt_unknown = true; } break; case Constraint::UNDEF: {
                    throw undefined_behaviour {};
                    } break;
                    case Constraint::NOP: {
                    EndOfInstruction(); // return
                    } break;
                    }*/
                }

                if (Rn == 31) {
                    // checkSPAlignment();
                    address = SP();
                } else {
                    address = m_gpRegisters.X(Rn);
                }

                if (!postindex) {
                    address = address.to_ullong() + offset.to_ullong();
                }

                if (rt_unknown) {
                    data = std::bitset< datasize >(0);
                } else {
                    data = m_gpRegisters.X(Rt);
                }
                // ALU::Mem[address, datasize / 8, AccType::NORMAL] = data;
                m_upStreamMemory->Write(address.to_ullong(), data.to_ullong());

                if (wback) {
                    if (postindex) {
                        address = address.to_ullong() + offset.to_ullong();
                    } else if (Rn == 31) {
                        SP() = address;
                    } else {
                        m_gpRegisters.write(Rn, address);
                    }
                }
            } break;
            case LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::LDRi_64BIT: { // P.997 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::LDRi_64BIT),
                                  Enum::ToChar(A64DecodeGroup::LoadStore));

                if (size != 0b11) {
                    throw undefined_behaviour {};
                }
                bool wback     = false;
                bool postindex = false;

                auto offset = LSL< 64 >(ZeroExtend< 64 >(std::bitset< 12 > { imm12.ToULong() }), size.ToULong());
                static_assert(std::is_same_v< decltype(offset), std::bitset< 64 > >);

                constexpr std::uint32_t datasize = 8 << 0b11;
                if (supportedFeatures.find(IProcessingUnit::Feature::FEAT_MTE) != supportedFeatures.end()) {
                    // SetTagCheckedInstruction(tag_checked);
                }
                std::bitset< 64 >       address;
                std::bitset< datasize > data;

                bool wb_unknown = false;

                if (wback && Rn == Rt && Rn != 31) {
                    /*auto c = ALU::ConstrainUnpredictable();
                    if (c != Constraint::NONE && c != Constraint::UNKNOWN && c != Constraint::UNDEF && c !=
                    Constraint::NOP) throw undefined_behaviour {}; switch (c) { case Constraint::NONE: { rt_unknown =
                    false; } break; case Constraint::UNKNOWN: { rt_unknown = true; } break; case Constraint::UNDEF: {
                    throw undefined_behaviour {};
                    } break;
                    case Constraint::NOP: {
                    EndOfInstruction(); // return
                    } break;
                    }*/
                }

                if (Rn == 31) {
                    // checkSPAlignment();
                    address = SP();
                } else {
                    address = m_gpRegisters.X(Rn);
                }

                if (!postindex) {
                    address = address.to_ullong() + offset.to_ullong();
                }

                // data = ALU::Mem[address, datasize / 8, AccType::NORMAL];
                data = m_upStreamMemory->Read(address.to_ullong());
                m_gpRegisters.write(Rt, data);

                if (wback) {
                    if (wb_unknown) {
                        throw undefined_behaviour {};
                    } else if (postindex) {
                        address = address.to_ullong() + offset.to_ullong();
                    }

                    if (Rn == 31) {
                        SP() = address;
                    } else {
                        m_gpRegisters.write(Rn, address);
                    }
                }
            } break;
            case LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::PRFMi: { // P.1253
                // No-op
                // This system emulation is depending on O(1) memory access, no operation is needed to pre-fetch
            } break;
            case LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::STRi_64BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreGroup::LoadStoreRegisterUnsignedImmediate::LDRi_64BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }

    void Execute(Instruction&& instruction, DataProcessingRegisterGroup::DataProcessingTwoSource instructionType) {
        auto Rd  = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rd);
        auto Rn  = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rn);
        auto Rm  = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rm);
        auto op2 = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Op2);

        switch (instructionType) {
            case DataProcessingRegisterGroup::DataProcessingTwoSource::UDIV_32BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::DataProcessingTwoSource::SDIV_32BIT: { // P.1293
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::DataProcessingTwoSource::SDIV_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 32;

                auto               operand1 = m_gpRegisters.W(Rn);
                decltype(operand1) operand2 = m_gpRegisters.W(Rm);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);

                std::int64_t result {};

                if (!operand2.any()) {
                    result = 0;
                } else {
                    result = RoundTowardsZero(
                        static_cast< long double >(std::bit_cast< std::int64_t >(operand1.to_ullong())) /
                        static_cast< long double >(std::bit_cast< std::int64_t >(operand2.to_ullong())));
                }

                m_gpRegisters.write(Rd, std::bit_cast< std::uint64_t >(result));
            } break;
            case DataProcessingRegisterGroup::DataProcessingTwoSource::LSLV_32BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::DataProcessingTwoSource::LSRV_32BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::DataProcessingTwoSource::ASRV_32BIT: { // P.889
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::DataProcessingTwoSource::ASRV_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 32;

                auto shift_type = DecodeShift(op2);

                auto operand2 = m_gpRegisters.W(Rm);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);

                auto result = ShiftReg< datasize >(
                    Rn, shift_type, static_cast< std::uint8_t >(std::uint64_t { operand2.to_ulong() } % datasize));
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                m_gpRegisters.write(Rd, result);

            } break;
            case DataProcessingRegisterGroup::DataProcessingTwoSource::RORV_32BIT: { // P.1279
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::DataProcessingTwoSource::RORV_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 32;

                auto shift_type = DecodeShift(op2);

                auto operand2 = m_gpRegisters.W(Rm);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);

                auto result = ShiftReg< datasize >(
                    Rn, shift_type, static_cast< std::uint8_t >(std::uint64_t { operand2.to_ulong() } % datasize));
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::DataProcessingTwoSource::CRC32B: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::DataProcessingTwoSource::CRC32H: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::DataProcessingTwoSource::CRC32W: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::DataProcessingTwoSource::CRC32CB: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::DataProcessingTwoSource::CRC32CH: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::DataProcessingTwoSource::CRC32CW: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::DataProcessingTwoSource::SUBP: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::DataProcessingTwoSource::UDIV_64BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::DataProcessingTwoSource::SDIV_64BIT: { // P.1293
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::DataProcessingTwoSource::SDIV_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 64;

                auto               operand1 = m_gpRegisters.X(Rn);
                decltype(operand1) operand2 = m_gpRegisters.X(Rm);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);

                std::int64_t result {};

                if (!operand2.any()) {
                    result = 0;
                } else {
                    result = RoundTowardsZero(
                        static_cast< long double >(std::bit_cast< std::int64_t >(operand1.to_ullong())) /
                        static_cast< long double >(std::bit_cast< std::int64_t >(operand2.to_ullong())));
                }

                m_gpRegisters.write(Rd, std::bit_cast< std::uint64_t >(result));
            } break;
            case DataProcessingRegisterGroup::DataProcessingTwoSource::IRG: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::DataProcessingTwoSource::GMI: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::DataProcessingTwoSource::LSLV_64BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::DataProcessingTwoSource::LSRV_64BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::DataProcessingTwoSource::ASRV_64BIT: { // P.889
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::DataProcessingTwoSource::ASRV_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize   = 64;
                auto           shift_type = DecodeShift(op2);

                auto operand2 = m_gpRegisters.X(Rm);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);

                auto result = ShiftReg< datasize >(
                    Rn, shift_type, static_cast< std::uint8_t >(std::uint64_t { operand2.to_ulong() } % datasize));
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::DataProcessingTwoSource::RORV_64BIT: { // P.1279
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::DataProcessingTwoSource::RORV_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 64;

                auto shift_type = DecodeShift(op2);

                auto operand2 = m_gpRegisters.X(Rm);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);

                auto result = ShiftReg< datasize >(
                    Rn, shift_type, static_cast< std::uint8_t >(std::uint64_t { operand2.to_ulong() } % datasize));
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::DataProcessingTwoSource::PACGA: { // P.1246
                if (HasArchVersion(IProcessingUnit::ExtensionVersion::Armv8p3)) {
                    throw unsupported_instruction {};
                } else {
                    throw not_implemented_feature {};
                }
            } break;
            case DataProcessingRegisterGroup::DataProcessingTwoSource::CRC32X: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::DataProcessingTwoSource::CRC32CX: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::DataProcessingTwoSource::SUBPS: {
                throw not_implemented_feature {};
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    void Execute(Instruction&& instruction, DataProcessingRegisterGroup::DataProcessingOneSource instructionType) {
        auto Rd  = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rd);
        auto Rn  = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rn);
        auto Z   = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Z);
        auto opc = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Opc);

        switch (instructionType) {
            case DataProcessingRegisterGroup::DataProcessingOneSource::RBIT_32BIT: { // P.1263
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::DataProcessingOneSource::RBIT_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 32;

                auto operand = m_gpRegisters.W(Rn);
                auto result  = decltype(operand) {};
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                for (auto i = 0; i < datasize; ++i) {
                    result[datasize - 1 - i] = operand[i];
                }
                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::DataProcessingOneSource::REV16_32BIT: { // P.1269
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::DataProcessingOneSource::REV16_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 32;

                int container_size {};
                switch (opc.ToULong()) {
                    case 0: {
                        Unreachable();
                    } break;
                    case 1: {
                        container_size = 16;
                    } break;
                    case 2: {
                        container_size = 32;
                    } break;
                    case 3: {
                        throw undefined_behaviour {};
                    } break;
                    default: {
                        throw undefined_behaviour {};
                    }
                }
                auto operand = m_gpRegisters.W(Rn);
                auto result  = decltype(operand) {};
                static_assert(std::is_same_v< decltype(operand), std::bitset< datasize > >);

                auto containers             = datasize / container_size;
                auto elements_per_container = container_size / 8;
                int  index                  = 0;
                int  rev_index;
                for (auto c = 0; c < containers; ++c) {
                    rev_index = index + ((elements_per_container - 1) * 8);
                    for (auto e = 0; e < elements_per_container; ++e) {
                        result[rev_index]                            = operand[index];
                        result[static_cast< size_t >(rev_index) + 1] = operand[static_cast< size_t >(index) + 1];
                        result[static_cast< size_t >(rev_index) + 2] = operand[static_cast< size_t >(index) + 2];
                        result[static_cast< size_t >(rev_index) + 3] = operand[static_cast< size_t >(index) + 3];
                        result[static_cast< size_t >(rev_index) + 4] = operand[static_cast< size_t >(index) + 4];
                        result[static_cast< size_t >(rev_index) + 5] = operand[static_cast< size_t >(index) + 5];
                        result[static_cast< size_t >(rev_index) + 6] = operand[static_cast< size_t >(index) + 6];
                        result[static_cast< size_t >(rev_index) + 7] = operand[static_cast< size_t >(index) + 7];

                        index     = index + 8;
                        rev_index = rev_index - 8;
                    }
                }

                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::DataProcessingOneSource::REV_32BIT: { // P.1267
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::DataProcessingOneSource::REV_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 32;

                int container_size {};
                switch (opc.ToULong()) {
                    case 0: {
                        Unreachable();
                    } break;
                    case 1: {
                        container_size = 16;
                    } break;
                    case 2: {
                        container_size = 32;
                    } break;
                    case 3: {
                        throw undefined_behaviour {};
                    } break;
                    default: {
                        throw undefined_behaviour {};
                    }
                }
                auto operand = m_gpRegisters.W(Rn);
                auto result  = decltype(operand) {};
                static_assert(std::is_same_v< decltype(operand), std::bitset< datasize > >);

                auto containers             = datasize / container_size;
                auto elements_per_container = container_size / 8;
                int  index                  = 0;
                int  rev_index;
                for (auto c = 0; c < containers; ++c) {
                    rev_index = index + ((elements_per_container - 1) * 8);
                    for (auto e = 0; e < elements_per_container; ++e) {
                        result[rev_index]                            = operand[index];
                        result[static_cast< size_t >(rev_index) + 1] = operand[static_cast< size_t >(index) + 1];
                        result[static_cast< size_t >(rev_index) + 2] = operand[static_cast< size_t >(index) + 2];
                        result[static_cast< size_t >(rev_index) + 3] = operand[static_cast< size_t >(index) + 3];
                        result[static_cast< size_t >(rev_index) + 4] = operand[static_cast< size_t >(index) + 4];
                        result[static_cast< size_t >(rev_index) + 5] = operand[static_cast< size_t >(index) + 5];
                        result[static_cast< size_t >(rev_index) + 6] = operand[static_cast< size_t >(index) + 6];
                        result[static_cast< size_t >(rev_index) + 7] = operand[static_cast< size_t >(index) + 7];

                        index     = index + 8;
                        rev_index = rev_index - 8;
                    }
                }

                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::DataProcessingOneSource::CLZ_32BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::DataProcessingOneSource::CLS_32BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::DataProcessingOneSource::RBIT_64BIT: { // P.1263
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::DataProcessingOneSource::RBIT_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 64;

                auto operand = m_gpRegisters.X(Rn);
                auto result  = decltype(operand) {};
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                for (auto i = 0; i < datasize; ++i) {
                    result[datasize - 1 - i] = operand[i];
                }
                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::DataProcessingOneSource::REV16_64BIT: { // P.1269
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::DataProcessingOneSource::REV16_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 64;

                int container_size {};
                switch (opc.ToULong()) {
                    case 0: {
                        Unreachable();
                    } break;
                    case 1: {
                        container_size = 16;
                    } break;
                    case 2: {
                        container_size = 32;
                    } break;
                    case 3: {
                        container_size = 64;
                    } break;
                    default: {
                        throw undefined_behaviour {};
                    }
                }
                auto operand = m_gpRegisters.X(Rn);
                auto result  = decltype(operand) {};
                static_assert(std::is_same_v< decltype(operand), std::bitset< datasize > >);

                auto containers             = datasize / container_size;
                auto elements_per_container = container_size / 8;
                int  index                  = 0;
                int  rev_index              = 0;
                for (auto c = 0; c < containers; ++c) {
                    rev_index = index + ((elements_per_container - 1) * 8);
                    for (auto e = 0; e < elements_per_container; ++e) {
                        result[rev_index]                            = operand[index];
                        result[static_cast< size_t >(rev_index) + 1] = operand[static_cast< size_t >(index) + 1];
                        result[static_cast< size_t >(rev_index) + 2] = operand[static_cast< size_t >(index) + 2];
                        result[static_cast< size_t >(rev_index) + 3] = operand[static_cast< size_t >(index) + 3];
                        result[static_cast< size_t >(rev_index) + 4] = operand[static_cast< size_t >(index) + 4];
                        result[static_cast< size_t >(rev_index) + 5] = operand[static_cast< size_t >(index) + 5];
                        result[static_cast< size_t >(rev_index) + 6] = operand[static_cast< size_t >(index) + 6];
                        result[static_cast< size_t >(rev_index) + 7] = operand[static_cast< size_t >(index) + 7];

                        index     = index + 8;
                        rev_index = rev_index - 8;
                    }
                }

                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::DataProcessingOneSource::REV32: { // P.1271
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::DataProcessingOneSource::REV32),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 64;

                int container_size {};
                switch (opc.ToULong()) {
                    case 0: {
                        Unreachable();
                    } break;
                    case 1: {
                        container_size = 16;
                    } break;
                    case 2: {
                        container_size = 32;
                    } break;
                    case 3: {
                        container_size = 64;
                    } break;
                    default: {
                        throw undefined_behaviour {};
                    }
                }
                auto operand = m_gpRegisters.X(Rn);
                auto result  = decltype(operand) {};
                static_assert(std::is_same_v< decltype(operand), std::bitset< datasize > >);

                auto containers             = datasize / container_size;
                auto elements_per_container = container_size / 8;
                int  index                  = 0;
                int  rev_index              = 0;
                for (auto c = 0; c < containers; ++c) {
                    rev_index = index + ((elements_per_container - 1) * 8);
                    for (auto e = 0; e < elements_per_container; ++e) {
                        result[rev_index]                            = operand[index];
                        result[static_cast< size_t >(rev_index) + 1] = operand[static_cast< size_t >(index) + 1];
                        result[static_cast< size_t >(rev_index) + 2] = operand[static_cast< size_t >(index) + 2];
                        result[static_cast< size_t >(rev_index) + 3] = operand[static_cast< size_t >(index) + 3];
                        result[static_cast< size_t >(rev_index) + 4] = operand[static_cast< size_t >(index) + 4];
                        result[static_cast< size_t >(rev_index) + 5] = operand[static_cast< size_t >(index) + 5];
                        result[static_cast< size_t >(rev_index) + 6] = operand[static_cast< size_t >(index) + 6];
                        result[static_cast< size_t >(rev_index) + 7] = operand[static_cast< size_t >(index) + 7];

                        index     = index + 8;
                        rev_index = rev_index - 8;
                    }
                }

                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::DataProcessingOneSource::REV_64BIT: { // P.1267
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::DataProcessingOneSource::REV_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 64;

                int container_size {};
                switch (opc.ToULong()) {
                    case 0: {
                        Unreachable();
                    } break;
                    case 1: {
                        container_size = 16;
                    } break;
                    case 2: {
                        container_size = 32;
                    } break;
                    case 3: {
                        container_size = 64;
                    } break;
                    default: {
                        throw undefined_behaviour {};
                    }
                }
                auto operand = m_gpRegisters.X(Rn);
                auto result  = decltype(operand) {};
                static_assert(std::is_same_v< decltype(operand), std::bitset< datasize > >);

                auto containers             = datasize / container_size;
                auto elements_per_container = container_size / 8;
                int  index                  = 0;
                int  rev_index              = 0;
                for (auto c = 0; c < containers; ++c) {
                    rev_index = index + ((elements_per_container - 1) * 8);
                    for (auto e = 0; e < elements_per_container; ++e) {
                        result[rev_index]                            = operand[index];
                        result[static_cast< size_t >(rev_index) + 1] = operand[static_cast< size_t >(index) + 1];
                        result[static_cast< size_t >(rev_index) + 2] = operand[static_cast< size_t >(index) + 2];
                        result[static_cast< size_t >(rev_index) + 3] = operand[static_cast< size_t >(index) + 3];
                        result[static_cast< size_t >(rev_index) + 4] = operand[static_cast< size_t >(index) + 4];
                        result[static_cast< size_t >(rev_index) + 5] = operand[static_cast< size_t >(index) + 5];
                        result[static_cast< size_t >(rev_index) + 6] = operand[static_cast< size_t >(index) + 6];
                        result[static_cast< size_t >(rev_index) + 7] = operand[static_cast< size_t >(index) + 7];

                        index     = index + 8;
                        rev_index = rev_index - 8;
                    }
                }

                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::DataProcessingOneSource::CLZ_64BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::DataProcessingOneSource::CLS_64BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::DataProcessingOneSource::PACIA: { // P.1247
                if (HasArchVersion(IProcessingUnit::ExtensionVersion::Armv8p3)) {
                    throw unsupported_instruction {};
                } else {
                    throw not_implemented_feature {};
                }
            } break;
            case DataProcessingRegisterGroup::DataProcessingOneSource::PACIB: { // P.1250
                if (HasArchVersion(IProcessingUnit::ExtensionVersion::Armv8p3)) {
                    throw unsupported_instruction {};
                } else {
                    throw not_implemented_feature {};
                }
            } break;
            case DataProcessingRegisterGroup::DataProcessingOneSource::PACDA: { // P.1244
                if (HasArchVersion(IProcessingUnit::ExtensionVersion::Armv8p3)) {
                    throw unsupported_instruction {};
                } else {
                    throw not_implemented_feature {};
                }
            } break;
            case DataProcessingRegisterGroup::DataProcessingOneSource::PACDB: { // P.1245
                if (HasArchVersion(IProcessingUnit::ExtensionVersion::Armv8p3)) {
                    throw unsupported_instruction {};
                } else {
                    throw not_implemented_feature {};
                }
            } break;
            case DataProcessingRegisterGroup::DataProcessingOneSource::AUTIA: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::DataProcessingOneSource::AUTIB: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::DataProcessingOneSource::AUTDA: { // P.897
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::DataProcessingOneSource::AUTDA),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                auto source_is_sp = false;
                if (!HavePACExt())
                    throw unsupported_instruction {};

                // Will always throw since instruction is ARMv8p3
                /*if (Rn == 31) source_is_sp = true;

                if (AArch64::HavePACExt()) {
                    if (source_is_sp) {
                        m_gpRegisters.write(Rd, AArch64::AuthDA(m_gpRegisters.X(Rd), ALU::SP<64>(&sp_registers),
                false));
                    }
                    else {
                        m_gpRegisters.write(Rd, AArch64::AuthDA(m_gpRegisters.X(Rd), m_gpRegisters.X(Rn), false));
                    }
                }*/
            } break;
            case DataProcessingRegisterGroup::DataProcessingOneSource::AUTDB: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::DataProcessingOneSource::PACIZA: { // P.1247
                if (HasArchVersion(IProcessingUnit::ExtensionVersion::Armv8p3)) {
                    throw unsupported_instruction {};
                } else {
                    throw not_implemented_feature {};
                }
            } break;
            case DataProcessingRegisterGroup::DataProcessingOneSource::PACIZB: { // P.1250
                if (HasArchVersion(IProcessingUnit::ExtensionVersion::Armv8p3)) {
                    throw unsupported_instruction {};
                } else {
                    throw not_implemented_feature {};
                }
            } break;
            case DataProcessingRegisterGroup::DataProcessingOneSource::PACDZA: { // P.1244
                if (HasArchVersion(IProcessingUnit::ExtensionVersion::Armv8p3)) {
                    throw unsupported_instruction {};
                } else {
                    throw not_implemented_feature {};
                }
            } break;
            case DataProcessingRegisterGroup::DataProcessingOneSource::PACDZB: { // P.1245
                if (HasArchVersion(IProcessingUnit::ExtensionVersion::Armv8p3)) {
                    throw unsupported_instruction {};
                } else {
                    throw not_implemented_feature {};
                }
            } break;
            case DataProcessingRegisterGroup::DataProcessingOneSource::AUTIZA: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::DataProcessingOneSource::AUTIZB: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::DataProcessingOneSource::AUTDZA: { // P.897
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::DataProcessingOneSource::AUTDZA),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                if (!HavePACExt()) {
                    throw unsupported_instruction {};
                } else { // Will always throw since instruction is ARMv8p3
                    /*auto source_is_sp = false;

                    if (HavePACExt()) {
                        if (source_is_sp) {
                            m_gpRegisters.write(Rd, AuthDA(m_gpRegisters.X(Rd), ALU::SP< 64 >(&sp_registers), false));
                        } else {
                            m_gpRegisters.write(Rd, AuthDA(m_gpRegisters.X(Rd), m_gpRegisters.X(Rn), false));
                        }
                    }*/
                }
            } break;
            case DataProcessingRegisterGroup::DataProcessingOneSource::AUTDZB: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::DataProcessingOneSource::XPACI: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::DataProcessingOneSource::XPACD: {
                throw not_implemented_feature {};
            } break;
            default:
                throw undefined_instruction {};
                break;
        }
    }
    void Execute(Instruction&& instruction, DataProcessingRegisterGroup::LogicalShiftedRegister instructionType) {
        auto Rd    = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rd);
        auto Rn    = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rn);
        auto Rm    = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rm);
        auto imm6  = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Imm6);
        auto N     = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::N);
        auto shift = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Shift);

        switch (instructionType) {
            case DataProcessingRegisterGroup::LogicalShiftedRegister::AND_32BIT_SHIFTED: { // P.795 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::LogicalShiftedRegister::AND_32BIT_SHIFTED),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                if (static_cast< bool >(imm6.ToULong() & 0b100000) == true)
                    throw undefined_behaviour {};
                constexpr auto datasize   = 32;
                auto           shift_type = DecodeShift(shift);

                auto operand1 = m_gpRegisters.W(Rn);
                auto operand2 = ShiftReg< datasize >(Rm, shift_type, imm6.ToULong());

                auto result = operand1 & operand2;
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::LogicalShiftedRegister::BIC_32BIT_SHIFTED: { // P.826 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::LogicalShiftedRegister::BIC_32BIT_SHIFTED),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 32;
                if (imm6.ToULong() >= 32)
                    throw undefined_instruction {};

                auto shift_type   = DecodeShift(shift);
                auto shift_amount = imm6.ToULong();

                auto operand1 = m_gpRegisters.W(Rn);
                auto operand2 = ShiftReg< datasize >(Rm, shift_type, shift_amount);

                operand2.flip();
                auto result = operand1 & operand2;
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::LogicalShiftedRegister::ORR_32BIT_SHIFTED: { // P.1242
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::LogicalShiftedRegister::ORR_32BIT_SHIFTED),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 32;
                if (imm6.ToULong() >= 32)
                    throw undefined_instruction {};

                auto shift_type   = DecodeShift(shift);
                auto shift_amount = imm6.ToULong();

                auto operand1 = m_gpRegisters.W(Rn);
                auto operand2 = ShiftReg< datasize >(Rm, shift_type, shift_amount);

                auto result = operand1 | operand2;
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                m_gpRegisters.write(Rd, result);

            } break;
            case DataProcessingRegisterGroup::LogicalShiftedRegister::ORN_32BIT_SHIFTED: { // P.1238
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::LogicalShiftedRegister::ORN_32BIT_SHIFTED),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 32;
                if (imm6.ToULong() >= 32)
                    throw undefined_instruction {};

                auto shift_type   = DecodeShift(shift);
                auto shift_amount = imm6.ToULong();

                auto operand1 = m_gpRegisters.W(Rn);
                auto operand2 = ShiftReg< datasize >(Rm, shift_type, shift_amount);
                operand2.flip();

                auto result = operand1 | operand2;
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::LogicalShiftedRegister::EOR_32BIT_SHIFTED: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::LogicalShiftedRegister::EON_32BIT_SHIFTED: { // P.914 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::LogicalShiftedRegister::EON_32BIT_SHIFTED),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 32;
                if (imm6.ToULong() >= 32)
                    throw undefined_instruction {};

                auto shift_type   = DecodeShift(shift);
                auto shift_amount = imm6.ToULong();

                auto operand1 = m_gpRegisters.W(Rn);
                auto operand2 = ShiftReg< datasize >(Rm, shift_type, shift_amount);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);

                operand2.flip();

                auto result = operand1 | operand2;
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::LogicalShiftedRegister::ANDS_32BIT_SHIFTED: { // P.799 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::LogicalShiftedRegister::ANDS_32BIT_SHIFTED),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                if (static_cast< bool >(imm6.ToULong() & 0b100000) == true)
                    throw undefined_behaviour {};
                constexpr auto datasize   = 32;
                auto           shift_type = DecodeShift(shift);

                auto operand1 = m_gpRegisters.W(Rn);
                auto operand2 = ShiftReg< datasize >(Rm, shift_type, imm6.ToULong());

                auto result = operand1 & operand2;
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                std::bitset< 4 > nzcv = concate< 2, 2 >(concate< 1, 1 >(result[31] ? 1 : 0, result == 0 ? 1 : 0), 0);
                SetNZCV(nzcv);
                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::LogicalShiftedRegister::BICS_32BIT_SHIFTED: { // P.828 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::LogicalShiftedRegister::BICS_32BIT_SHIFTED),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 32;
                if (imm6.ToULong() >= 32)
                    throw undefined_instruction {};

                auto shift_type   = DecodeShift(shift);
                auto shift_amount = imm6.ToULong();

                auto operand1 = m_gpRegisters.W(Rn);
                auto operand2 = ShiftReg< datasize >(Rm, shift_type, shift_amount);

                operand2.flip();
                auto result = operand1 & operand2;
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                bool val = result[datasize - 1];
                SetNZCV(concate< 1, 3 >(val, concate< 1, 2 >(result == 0, 0)));
                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::LogicalShiftedRegister::AND_64BIT_SHIFTED: { // P.795 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::LogicalShiftedRegister::AND_64BIT_SHIFTED),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                auto           shift_type = DecodeShift(shift);
                constexpr auto datasize   = 64;

                auto operand1 = m_gpRegisters.X(Rn);
                auto operand2 = ShiftReg< datasize >(Rm, shift_type, imm6.ToULong());

                auto result = operand1 & operand2;
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::LogicalShiftedRegister::BIC_64BIT_SHIFTED: { // P.826 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::LogicalShiftedRegister::BIC_64BIT_SHIFTED),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 64;

                auto shift_type   = DecodeShift(shift);
                auto shift_amount = imm6;

                auto operand1 = m_gpRegisters.X(Rn);
                auto operand2 = ShiftReg< datasize >(Rm, shift_type, shift_amount.ToULong());

                operand2.flip();
                auto result = operand1 & operand2;
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::LogicalShiftedRegister::ORR_64BIT_SHIFTED: { // P.1242
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::LogicalShiftedRegister::ORR_64BIT_SHIFTED),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 64;

                auto shift_type   = DecodeShift(shift);
                auto shift_amount = imm6.ToULong();

                auto operand1 = m_gpRegisters.X(Rn);
                auto operand2 = ShiftReg< datasize >(Rm, shift_type, shift_amount);

                auto result = operand1 | operand2;
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::LogicalShiftedRegister::ORN_64BIT_SHIFTED: { // P.1238
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::LogicalShiftedRegister::ORN_64BIT_SHIFTED),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 64;

                auto shift_type   = DecodeShift(shift);
                auto shift_amount = imm6.ToULong();

                auto operand1 = m_gpRegisters.X(Rn);
                auto operand2 = ShiftReg< datasize >(Rm, shift_type, shift_amount);
                operand2.flip();

                auto result = operand1 | operand2;
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::LogicalShiftedRegister::EOR_64BIT_SHIFTED: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::LogicalShiftedRegister::EON_64BIT_SHIFTED: { // P.914 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::LogicalShiftedRegister::EON_64BIT_SHIFTED),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 64;

                auto shift_type   = DecodeShift(shift);
                auto shift_amount = imm6.ToULong();

                auto operand1 = m_gpRegisters.X(Rn);
                auto operand2 = ShiftReg< datasize >(Rm, shift_type, shift_amount);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);

                operand2.flip();

                auto result = operand1 | operand2;
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::LogicalShiftedRegister::ANDS_64BIT_SHIFTED: { // P.799 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::LogicalShiftedRegister::ANDS_64BIT_SHIFTED),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                auto           shift_type = DecodeShift(shift);
                constexpr auto datasize   = 64;

                auto operand1 = m_gpRegisters.X(Rn);
                auto operand2 = ShiftReg< datasize >(Rm, shift_type, imm6.ToULong());

                auto result = operand1 & operand2;
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                std::bitset< 4 > nzcv = concate< 2, 2 >(concate< 1, 1 >(result[63] ? 1 : 0, result == 0 ? 1 : 0), 0);
                SetNZCV(nzcv);
                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::LogicalShiftedRegister::BICS_64BIT_SHIFTED: { // P.828 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::LogicalShiftedRegister::BICS_64BIT_SHIFTED),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 64;

                auto shift_type   = DecodeShift(shift);
                auto shift_amount = imm6.ToULong();

                auto operand1 = m_gpRegisters.X(Rn);
                auto operand2 = ShiftReg< datasize >(Rm, shift_type, shift_amount);

                operand2.flip();
                auto result = operand1 & operand2;
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                bool val = result[datasize - 1];
                SetNZCV(concate< 1, 3 >(val, concate< 1, 2 >(result == 0, 0)));
                m_gpRegisters.write(Rd, result);
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    void Execute(Instruction&& instruction, DataProcessingRegisterGroup::AddSubtractShiftedRegister instructionType) {
        auto Rd    = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rd);
        auto Rn    = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rn);
        auto Rm    = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rm);
        auto shift = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Shift);
        auto imm6  = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Imm6);

        switch (instructionType) {
            case DataProcessingRegisterGroup::AddSubtractShiftedRegister::ADD_32BIT_SHIFTED: { // P.781 + 88
                m_debugObject.Log(
                    LogType::Instruction, instructionLogStatement, std::bitset< 32 > { instruction.Get() }.to_string(),
                    Enum::ToChar(DataProcessingRegisterGroup::AddSubtractShiftedRegister::ADD_32BIT_SHIFTED),
                    Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                if (shift == 3) {
                    throw undefined_behaviour {};
                }
                if (static_cast< bool >(imm6.ToULong() & 0b100000) == true) {
                    throw undefined_behaviour {};
                }
                constexpr auto datasize   = 32;
                auto           shift_type = DecodeShift(shift);

                auto operand1 = m_gpRegisters.W(Rn);
                auto operand2 = ShiftReg< datasize >(Rm, shift_type, imm6.ToULong());

                auto [result, _] = AddWithCarry(operand1, operand2, 0);

                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::AddSubtractShiftedRegister::ADDS_32BIT_SHIFTED: {
                m_debugObject.Log(
                    LogType::Instruction, instructionLogStatement, std::bitset< 32 > { instruction.Get() }.to_string(),
                    Enum::ToChar(DataProcessingRegisterGroup::AddSubtractShiftedRegister::ADDS_32BIT_SHIFTED),
                    Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                if (shift == 3) {
                    throw undefined_behaviour {};
                }
                if (static_cast< bool >(imm6.ToULong() & 0b100000) == true) {
                    throw undefined_behaviour {};
                }
                constexpr auto datasize   = 32;
                auto           shift_type = DecodeShift(shift);

                auto operand1 = m_gpRegisters.W(Rn);
                auto operand2 = ShiftReg< datasize >(Rm, shift_type, imm6.ToULong());

                auto [result, nzcv] = AddWithCarry(operand1, operand2, 0);

                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(nzcv), std::bitset< 4 > >);
                SetNZCV(nzcv);
                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::AddSubtractShiftedRegister::SUB_32BIT_SHIFTED: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::AddSubtractShiftedRegister::SUBS_32BIT_SHIFTED: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::AddSubtractShiftedRegister::ADD_64BIT_SHIFTED: { // P.781 + 88
                m_debugObject.Log(
                    LogType::Instruction, instructionLogStatement, std::bitset< 32 > { instruction.Get() }.to_string(),
                    Enum::ToChar(DataProcessingRegisterGroup::AddSubtractShiftedRegister::ADD_64BIT_SHIFTED),
                    Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                if (shift == 3) {
                    throw undefined_behaviour {};
                }
                constexpr auto datasize   = 64;
                auto           shift_type = DecodeShift(shift);

                auto operand1 = m_gpRegisters.X(Rn);
                auto operand2 = ShiftReg< datasize >(Rm, shift_type, imm6.ToULong());

                auto [result, _] = AddWithCarry(operand1, operand2, 0);

                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
            } break;
            case DataProcessingRegisterGroup::AddSubtractShiftedRegister::ADDS_64BIT_SHIFTED: {
                m_debugObject.Log(
                    LogType::Instruction, instructionLogStatement, std::bitset< 32 > { instruction.Get() }.to_string(),
                    Enum::ToChar(DataProcessingRegisterGroup::AddSubtractShiftedRegister::ADDS_64BIT_SHIFTED),
                    Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                if (shift == 3) {
                    throw undefined_behaviour {};
                }
                constexpr auto datasize   = 64;
                auto           shift_type = DecodeShift(shift);

                auto operand1 = m_gpRegisters.X(Rn);
                auto operand2 = ShiftReg< datasize >(Rm, shift_type, imm6.ToULong());

                auto [result, nzcv] = AddWithCarry(operand1, operand2, 0);

                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(nzcv), std::bitset< 4 > >);
                SetNZCV(nzcv);
                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::AddSubtractShiftedRegister::SUB_64BIT_SHIFTED: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::AddSubtractShiftedRegister::SUBS_64BIT_SHIFTED: {
                throw not_implemented_feature {};
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    void Execute(Instruction&& instruction, DataProcessingRegisterGroup::AddSubtractExtendedRegister instructionType) {
        auto Rd     = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rd);
        auto Rn     = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rn);
        auto Rm     = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rm);
        auto option = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Option);
        auto imm3   = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Imm3);

        switch (instructionType) {
            case DataProcessingRegisterGroup::AddSubtractExtendedRegister::ADD_32BIT_EXTENDED: { // P.776 + 88
                m_debugObject.Log(
                    LogType::Instruction, instructionLogStatement, std::bitset< 32 > { instruction.Get() }.to_string(),
                    Enum::ToChar(DataProcessingRegisterGroup::AddSubtractExtendedRegister::ADD_32BIT_EXTENDED),
                    Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                if (imm3.ToULong() > 4) {
                    throw undefined_behaviour {};
                }
                constexpr auto datasize = 32;
                auto           operand1 = Rn == 31 ? WSP() : m_gpRegisters.W(Rn);
                auto operand2 = ExtendReg< datasize >(Rm, static_cast< IProcessingUnit::ExtendType >(option.ToULong()),
                                                      imm3.ToULong());
                auto [result, _] = AddWithCarry(operand1, operand2, static_cast< std::uint8_t >(0));
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                if (Rn == 31) {
                    WSP(result);
                } else {
                    m_gpRegisters.write(Rd, result);
                }
            } break;
            case DataProcessingRegisterGroup::AddSubtractExtendedRegister::ADDS_32BIT_EXTENDED: { // P.784 + 88
                m_debugObject.Log(
                    LogType::Instruction, instructionLogStatement, std::bitset< 32 > { instruction.Get() }.to_string(),
                    Enum::ToChar(DataProcessingRegisterGroup::AddSubtractExtendedRegister::ADDS_32BIT_EXTENDED),
                    Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                if (imm3.ToULong() > 4) {
                    throw undefined_behaviour {};
                }
                if (imm3.ToULong() > 4) {
                    throw undefined_behaviour {};
                }
                constexpr auto datasize = 32;
                auto           operand1 = Rn == 31 ? WSP() : m_gpRegisters.W(Rn);
                auto operand2 = ExtendReg< datasize >(Rm, static_cast< IProcessingUnit::ExtendType >(option.ToULong()),
                                                      imm3.ToULong());
                auto [result, nzcv] = AddWithCarry(operand1, operand2, static_cast< std::uint8_t >(0));
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(nzcv), std::bitset< 4 > >);
                SetNZCV(nzcv);
                if (Rn == 31) {
                    WSP(result);
                } else {
                    m_gpRegisters.write(Rd, result);
                }
            } break;
            case DataProcessingRegisterGroup::AddSubtractExtendedRegister::SUB_32BIT_EXTENDED: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::AddSubtractExtendedRegister::SUBS_32BIT_EXTENDED: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::AddSubtractExtendedRegister::ADD_64BIT_EXTENDED: { // P.776 + 88
                m_debugObject.Log(
                    LogType::Instruction, instructionLogStatement, std::bitset< 32 > { instruction.Get() }.to_string(),
                    Enum::ToChar(DataProcessingRegisterGroup::AddSubtractExtendedRegister::ADD_64BIT_EXTENDED),
                    Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                if (imm3.ToULong() > 4) {
                    throw undefined_behaviour {};
                }
                constexpr auto datasize = 64;
                auto           operand1 = Rn == 31 ? SP() : m_gpRegisters.X(Rn);
                auto operand2 = ExtendReg< datasize >(Rm, static_cast< IProcessingUnit::ExtendType >(option.ToULong()),
                                                      imm3.ToULong());
                auto [result, _] = AddWithCarry(operand1, operand2, static_cast< std::uint8_t >(0));
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                if (Rn == 31) {
                    SP() = result;
                } else {
                    m_gpRegisters.write(Rd, result);
                }
            } break;
            case DataProcessingRegisterGroup::AddSubtractExtendedRegister::ADDS_64BIT_EXTENDED: { // P.784 + 88
                m_debugObject.Log(
                    LogType::Instruction, instructionLogStatement, std::bitset< 32 > { instruction.Get() }.to_string(),
                    Enum::ToChar(DataProcessingRegisterGroup::AddSubtractExtendedRegister::ADDS_64BIT_EXTENDED),
                    Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                if (imm3.ToULong() > 4) {
                    throw undefined_behaviour {};
                }
                constexpr auto datasize = 64;
                auto           operand1 = Rn == 31 ? SP() : m_gpRegisters.X(Rn);
                auto operand2 = ExtendReg< datasize >(Rm, static_cast< IProcessingUnit::ExtendType >(option.ToULong()),
                                                      imm3.ToULong());
                auto [result, nzcv] = AddWithCarry(operand1, operand2, static_cast< std::uint8_t >(0));
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(nzcv), std::bitset< 4 > >);
                SetNZCV(nzcv);
                if (Rn == 31) {
                    SP() = result;
                } else {
                    m_gpRegisters.write(Rd, result);
                }
            } break;
            case DataProcessingRegisterGroup::AddSubtractExtendedRegister::SUB_64BIT_EXTENDED: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::AddSubtractExtendedRegister::SUBS_64BIT_EXTENDED: {
                throw not_implemented_feature {};
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    void Execute(Instruction&& instruction, DataProcessingRegisterGroup::AddSubtractCarry instructionType) {
        auto Rd = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rd);
        auto Rn = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rn);
        auto Rm = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rm);

        switch (instructionType) {
            case DataProcessingRegisterGroup::AddSubtractCarry::ADC_32BIT: { // P.772 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::AddSubtractCarry::ADC_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 32;
                auto [result, _] =
                    AddWithCarry(m_gpRegisters.W(Rn), m_gpRegisters.W(Rm), static_cast< std::uint32_t >(C[0]));
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::AddSubtractCarry::ADCS_32BIT: { // P.774 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::AddSubtractCarry::ADCS_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 32;
                auto [result, nzcv] =
                    AddWithCarry(m_gpRegisters.W(Rn), m_gpRegisters.W(Rm), static_cast< std::uint32_t >(C[0]));
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(nzcv), std::bitset< 4 > >);
                SetNZCV(nzcv);
                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::AddSubtractCarry::SBC_32BIT: { // P.1282
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::AddSubtractCarry::SBC_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 32;

                auto               operand1 = m_gpRegisters.W(Rn);
                decltype(operand1) operand2 = m_gpRegisters.W(Rm);

                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);
                operand2.flip();

                auto [result, _] = AddWithCarry(operand1, operand2, C[0]);
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::AddSubtractCarry::SBCS_32BIT: { // P.1284
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::AddSubtractCarry::SBCS_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 32;

                auto               operand1 = m_gpRegisters.W(Rn);
                decltype(operand1) operand2 = m_gpRegisters.W(Rm);

                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);
                operand2.flip();

                auto [result, nzcv] = AddWithCarry(operand1, operand2, C[0]);
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(nzcv), std::bitset< 4 > >);
                SetNZCV(nzcv);
                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::AddSubtractCarry::ADC_64BIT: { // P.772 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::AddSubtractCarry::ADC_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 64;
                auto [result, _] =
                    AddWithCarry(m_gpRegisters.X(Rn), m_gpRegisters.X(Rm), static_cast< std::uint64_t >(C[0]));
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::AddSubtractCarry::ADCS_64BIT: { // P.774 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::AddSubtractCarry::ADCS_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 64;
                auto [result, nzcv] =
                    AddWithCarry(m_gpRegisters.X(Rn), m_gpRegisters.X(Rm), static_cast< std::uint64_t >(C[0]));
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(nzcv), std::bitset< 4 > >);
                SetNZCV(nzcv);
                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::AddSubtractCarry::SBC_64BIT: { // P.1282
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::AddSubtractCarry::SBC_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 64;

                auto               operand1 = m_gpRegisters.X(Rn);
                decltype(operand1) operand2 = m_gpRegisters.X(Rm);

                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);
                operand2.flip();

                auto [result, _] = AddWithCarry(operand1, operand2, C[0]);
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::AddSubtractCarry::SBCS_64BIT: { // P.1284
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::AddSubtractCarry::SBCS_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 64;

                auto               operand1 = m_gpRegisters.X(Rn);
                decltype(operand1) operand2 = m_gpRegisters.X(Rm);

                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);
                operand2.flip();

                auto [result, nzcv] = AddWithCarry(operand1, operand2, C[0]);
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(nzcv), std::bitset< 4 > >);
                SetNZCV(nzcv);
                m_gpRegisters.write(Rd, result);
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    void Execute(Instruction&& instruction, DataProcessingRegisterGroup::RotateRightIntoFlags instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&& instruction, DataProcessingRegisterGroup::EvaluateIntoFlags instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&& instruction, DataProcessingRegisterGroup::ConditionalCompareRegister instructionType) {
        auto nzcv = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::NZCV);
        auto Rn   = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rn);
        auto cond = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Cond);
        auto Rm   = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rm);

        switch (instructionType) {
            case DataProcessingRegisterGroup::ConditionalCompareRegister::CCMNr_32BIT: { // P.854 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::ConditionalCompareRegister::CCMNr_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto   datasize = 32;
                std::bitset< 4 > flags    = nzcv.ToULong();

                auto operand1 = m_gpRegisters.W(Rn);
                auto operand2 = m_gpRegisters.W(Rm);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);

                if (ConditionHolds(cond, NZCV())) {
                    auto [_, val] = AddWithCarry(operand1, operand2, 0);
                    flags         = val;
                }

                SetNZCV(flags);
            } break;
            case DataProcessingRegisterGroup::ConditionalCompareRegister::CCMPr_32BIT: { // P.858 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::ConditionalCompareRegister::CCMPr_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 32;

                std::bitset< 4 > flags = nzcv.ToULong();

                auto operand1 = m_gpRegisters.W(Rn);
                auto operand2 = m_gpRegisters.W(Rm);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);

                if (ConditionHolds(cond, NZCV())) {
                    operand2.flip();
                    auto [_, val] = AddWithCarry(operand1, operand2, 1);
                    flags         = val;
                }
                SetNZCV(flags);
            } break;
            case DataProcessingRegisterGroup::ConditionalCompareRegister::CCMNr_64BIT: { // P.854 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::ConditionalCompareRegister::CCMNr_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto   datasize = 64;
                std::bitset< 4 > flags    = nzcv.ToULong();

                auto operand1 = m_gpRegisters.X(Rn);
                auto operand2 = m_gpRegisters.X(Rm);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);

                if (ConditionHolds(cond, NZCV())) {
                    auto [_, val] = AddWithCarry(operand1, operand2, 0);
                    flags         = val;
                }

                SetNZCV(flags);
            } break;
            case DataProcessingRegisterGroup::ConditionalCompareRegister::CCMPr_64BIT: { // P.858 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::ConditionalCompareRegister::CCMPr_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 64;

                std::bitset< 4 > flags = nzcv.ToULong();

                auto operand1 = m_gpRegisters.X(Rn);
                auto operand2 = m_gpRegisters.X(Rm);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);

                if (ConditionHolds(cond, NZCV())) {
                    operand2.flip();
                    auto [_, val] = AddWithCarry(operand1, operand2, 1);
                    flags         = val;
                }
                SetNZCV(flags);
            } break;
            default:
                throw undefined_instruction {};
                break;
        }
    }
    void Execute(Instruction&& instruction, DataProcessingRegisterGroup::ConditionalCompareImmediate instructionType) {
        auto nzcv = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::NZCV);
        auto Rn   = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rn);
        auto cond = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Cond);
        auto imm5 = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Imm5);

        switch (instructionType) {
            case DataProcessingRegisterGroup::ConditionalCompareImmediate::CCMNi_32BIT: { // P.852 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::ConditionalCompareImmediate::CCMNi_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 32;

                std::bitset< 4 > flags = nzcv.ToULong();
                auto             imm   = ZeroExtend< datasize, 5 >(imm5.ToULong());

                auto operand1 = m_gpRegisters.W(Rn);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);

                if (ConditionHolds(cond, NZCV())) {
                    auto [_, val] = AddWithCarry(operand1, imm, 0);
                    flags         = val;
                }
                SetNZCV(flags);
            } break;
            case DataProcessingRegisterGroup::ConditionalCompareImmediate::CCMPi_32BIT: { // P.856 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::ConditionalCompareImmediate::CCMPi_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 32;

                std::bitset< 4 > flags = nzcv.ToULong();
                auto             imm   = ZeroExtend< datasize, 5 >(imm5.ToULong());
                static_assert(std::is_same_v< decltype(imm), std::bitset< datasize > >);

                auto operand1 = m_gpRegisters.W(Rn);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);
                auto operand2 = std::bitset< datasize > {};

                if (ConditionHolds(cond, NZCV())) {
                    operand2 = ~imm;
                    (void)AddWithCarry(operand1, operand2, 1); // TODO: Check
                }
                SetNZCV(flags);
            } break;
            case DataProcessingRegisterGroup::ConditionalCompareImmediate::CCMNi_64BIT: { // P.852 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::ConditionalCompareImmediate::CCMNi_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 64;

                std::bitset< 4 > flags = nzcv.ToULong();
                auto             imm   = ZeroExtend< datasize, 5 >(imm5.ToULong());

                auto operand1 = m_gpRegisters.X(Rn);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);

                if (ConditionHolds(cond, NZCV())) {
                    auto [_, val] = AddWithCarry(operand1, imm, 0);
                    flags         = val;
                }
                SetNZCV(flags);
            } break;
            case DataProcessingRegisterGroup::ConditionalCompareImmediate::CCMPi_64BIT: { // P.856 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::ConditionalCompareImmediate::CCMPi_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 64;

                std::bitset< 4 > flags = nzcv.ToULong();
                auto             imm   = ZeroExtend< datasize, 5 >(imm5.ToULong());
                static_assert(std::is_same_v< decltype(imm), std::bitset< datasize > >);

                auto operand1 = m_gpRegisters.X(Rn);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);
                auto operand2 = std::bitset< datasize > {};

                if (ConditionHolds(cond, NZCV())) {
                    operand2 = ~imm;
                    (void)AddWithCarry(operand1, operand2, 1); // TODO: Check
                }
                SetNZCV(flags);
            } break;
            default:
                throw undefined_instruction {};
                break;
        }
    }
    void Execute(Instruction&& instruction, DataProcessingRegisterGroup::ConditionalSelect instructionType) {
        auto Rd   = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rd);
        auto Rn   = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rn);
        auto Rm   = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rm);
        auto cond = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Cond);

        switch (instructionType) {
            case DataProcessingRegisterGroup::ConditionalSelect::CSEL_32BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::ConditionalSelect::CSINC_32BIT: { // P.896 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::ConditionalSelect::CSINC_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 32;
                auto           operand1 = m_gpRegisters.W(Rn);
                auto           operand2 = m_gpRegisters.W(Rm);

                std::bitset< datasize > result;

                if (ConditionHolds(cond, NZCV())) {
                    result = operand1;
                } else {
                    result = operand2.to_ullong() + 1;
                }
                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::ConditionalSelect::CSINV_32BIT: { // P.898 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::ConditionalSelect::CSINV_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 32;

                auto operand1 = m_gpRegisters.W(Rn);
                auto operand2 = m_gpRegisters.W(Rm);

                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);
                decltype(operand1) result;

                if (ConditionHolds(cond, NZCV()))
                    result = operand1;
                else
                    result = ~operand2;

                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::ConditionalSelect::CSNEG_32BIT: { // P.900 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::ConditionalSelect::CSNEG_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 32;

                auto operand1 = m_gpRegisters.W(Rn);
                auto operand2 = m_gpRegisters.W(Rm);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);
                decltype(operand1) result;

                if (ConditionHolds(cond, NZCV())) {
                    result = operand1;
                } else {
                    get_type_t< datasize > res = static_cast< get_type_t< datasize > >((~operand2).to_ullong());
                    result = static_cast< get_type_t< datasize > >(res) + static_cast< get_type_t< datasize > >(1);
                }

                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::ConditionalSelect::CSEL_64BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::ConditionalSelect::CSINC_64BIT: { // P.896 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::ConditionalSelect::CSINC_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 64;
                auto           operand1 = m_gpRegisters.X(Rn);
                auto           operand2 = m_gpRegisters.X(Rm);

                std::bitset< datasize > result;

                if (ConditionHolds(cond, NZCV())) {
                    result = operand1;
                } else {
                    result = operand2.to_ullong() + 1;
                }
                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::ConditionalSelect::CSINV_64BIT: { // P.898 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::ConditionalSelect::CSINV_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 64;

                auto operand1 = m_gpRegisters.X(Rn);
                auto operand2 = m_gpRegisters.X(Rm);

                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);
                decltype(operand1) result;

                if (ConditionHolds(cond, NZCV()))
                    result = operand1;
                else
                    result = ~operand2;

                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::ConditionalSelect::CSNEG_64BIT: { // P.900 + 88
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::ConditionalSelect::CSNEG_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 64;

                auto operand1 = m_gpRegisters.X(Rn);
                auto operand2 = m_gpRegisters.X(Rm);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);
                decltype(operand1) result;

                if (ConditionHolds(cond, NZCV())) {
                    result = operand1;
                } else {
                    get_type_t< datasize > res = static_cast< get_type_t< datasize > >((~operand2).to_ullong());
                    result                     = res + static_cast< get_type_t< datasize > >(1);
                }

                m_gpRegisters.write(Rd, result);
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    void Execute(Instruction&& instruction, DataProcessingRegisterGroup::DataProcessingThreeSource instructionType) {
        auto Rm = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rm);
        auto Ra = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Ra);
        auto Rn = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rn);
        auto Rd = A64InstructionManager::Get(instruction, A64InstructionManager::Tag::Rd);

        switch (instructionType) {
            case DataProcessingRegisterGroup::DataProcessingThreeSource::MADD_32BIT: { // P.1200
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::DataProcessingThreeSource::MADD_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 32;

                auto operand1 = m_gpRegisters.W(Rn);
                auto operand2 = m_gpRegisters.W(Rm);
                auto operand3 = m_gpRegisters.W(Ra);

                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(operand3), std::bitset< datasize > >);

                auto result =
                    std::bitset< datasize > { operand3.to_ullong() + (operand1.to_ullong() * operand2.to_ullong()) };

                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::DataProcessingThreeSource::MSUB_32BIT: { // P.1224
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::DataProcessingThreeSource::MSUB_32BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 32;

                auto operand1 = m_gpRegisters.W(Rn);
                auto operand2 = m_gpRegisters.W(Rm);
                auto operand3 = m_gpRegisters.W(Ra);

                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(operand3), std::bitset< datasize > >);

                auto result =
                    std::bitset< datasize > { operand3.to_ullong() - (operand1.to_ullong() * operand2.to_ullong()) };

                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::DataProcessingThreeSource::MADD_64BIT: { // P.1200
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::DataProcessingThreeSource::MADD_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 64;

                auto operand1 = m_gpRegisters.X(Rn);
                auto operand2 = m_gpRegisters.X(Rm);
                auto operand3 = m_gpRegisters.X(Ra);

                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(operand3), std::bitset< datasize > >);

                auto result =
                    std::bitset< datasize > { operand3.to_ullong() + (operand1.to_ullong() * operand2.to_ullong()) };

                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::DataProcessingThreeSource::MSUB_64BIT: { // P.1224
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::DataProcessingThreeSource::MSUB_64BIT),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                constexpr auto datasize = 64;

                auto operand1 = m_gpRegisters.X(Rn);
                auto operand2 = m_gpRegisters.X(Rm);
                auto operand3 = m_gpRegisters.X(Ra);

                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(operand3), std::bitset< datasize > >);

                auto result =
                    std::bitset< datasize > { operand3.to_ullong() - (operand1.to_ullong() * operand2.to_ullong()) };

                m_gpRegisters.write(Rd, result);
            } break;
            case DataProcessingRegisterGroup::DataProcessingThreeSource::SMADDL: { // P.1297
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::DataProcessingThreeSource::SMADDL),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                std::bitset< 32 > operand1 = m_gpRegisters.W(Rn);
                std::bitset< 32 > operand2 = m_gpRegisters.W(Rm);
                std::bitset< 64 > operand3 = m_gpRegisters.X(Ra);

                std::int64_t result = std::bit_cast< std::int64_t >(operand3.to_ullong()) +
                                      (std::bit_cast< std::int64_t >(operand1.to_ullong()) *
                                       std::bit_cast< std::int64_t >(operand2.to_ullong()));

                m_gpRegisters.write(Rd, std::bit_cast< std::uint64_t >(result));
            } break;
            case DataProcessingRegisterGroup::DataProcessingThreeSource::SMSUBL: { // P.1301
                m_debugObject.Log(LogType::Instruction, instructionLogStatement,
                                  std::bitset< 32 > { instruction.Get() }.to_string(),
                                  Enum::ToChar(DataProcessingRegisterGroup::DataProcessingThreeSource::SMSUBL),
                                  Enum::ToChar(A64DecodeGroup::DataProcessingRegister));

                std::bitset< 32 > operand1 = m_gpRegisters.W(Rn);
                std::bitset< 32 > operand2 = m_gpRegisters.W(Rm);
                std::bitset< 64 > operand3 = m_gpRegisters.X(Ra);

                std::int64_t result = std::bit_cast< std::int64_t >(operand3.to_ullong()) -
                                      (std::bit_cast< std::int64_t >(operand1.to_ullong()) *
                                       std::bit_cast< std::int64_t >(operand2.to_ullong()));

                m_gpRegisters.write(Rd, std::bit_cast< std::uint64_t >(result));
            } break;
            case DataProcessingRegisterGroup::DataProcessingThreeSource::SMULH: { // P.1303
                std::bitset< 64 > operand1 = m_gpRegisters.X(Rn);
                std::bitset< 64 > operand2 = m_gpRegisters.X(Rm);

                // bitset multiply to 128
            } break;
            case DataProcessingRegisterGroup::DataProcessingThreeSource::UMADDL: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::DataProcessingThreeSource::UMSUBL: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterGroup::DataProcessingThreeSource::UMULH: {
                throw not_implemented_feature {};
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }

    void Execute(Instruction&&                                                        instruction,
                 DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicAES instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&&                                                                     instruction,
                 DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterSHA instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&&                                                                   instruction,
                 DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicTwoRegisterSHA instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&&                                                              instruction,
                 DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarCopy instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&&                                                                       instruction,
                 DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSameFP16 instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&& instruction,
                 DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneousFP16
                     instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(
        Instruction&&                                                                             instruction,
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSameExtraction instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&& instruction,
                 DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarTwoRegisterMiscellaneous
                     instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&&                                                                  instruction,
                 DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarPairwise instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&&                                                                        instruction,
                 DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeDifferent instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&&                                                                   instruction,
                 DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarThreeSame instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(
        Instruction&&                                                                          instruction,
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarShiftByImmediate instructionType) {
        throw not_implemented_feature {};
    }
    void
        Execute(Instruction&&                                                                         instruction,
                DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDScalarXIndexedElement instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&&                                                               instruction,
                 DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTableLookup instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&&                                                           instruction,
                 DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDPermute instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&&                                                           instruction,
                 DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDExtract instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&&                                                        instruction,
                 DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDCopy instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&&                                                                 instruction,
                 DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSameFP16 instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(
        Instruction&&                                                                                instruction,
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneousFP16 instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(
        Instruction&&                                                                          instruction,
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeRegisterExtension instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(
        Instruction&&                                                                            instruction,
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDTwoRegisterMiscellaneous instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&&                                                               instruction,
                 DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDAcrossLanes instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&&                                                                  instruction,
                 DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeDifferent instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&&                                                             instruction,
                 DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDThreeSame instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&&                                                                     instruction,
                 DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDModifiedImmediate instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&&                                                                    instruction,
                 DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDShiftByImmediate instructionType) {
        throw not_implemented_feature {};
    }
    void
        Execute(Instruction&&                                                                         instruction,
                DataProcessingScalarFloatingPointAdvancedSIMDGroup::AdvancedSIMDVectorXIndexedElement instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&&                                                                      instruction,
                 DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterIMM2 instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&&                                                                        instruction,
                 DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicThreeRegisterSHA512 instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&&                                                                 instruction,
                 DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicFourRegister instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&& instruction, DataProcessingScalarFloatingPointAdvancedSIMDGroup::XAR instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&&                                                                      instruction,
                 DataProcessingScalarFloatingPointAdvancedSIMDGroup::CryptographicTwoRegisterSHA512 instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(
        Instruction&&                                                                            instruction,
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndFixedPoint instructionType) {
        throw not_implemented_feature {};
    }
    void
        Execute(Instruction&&                                                                         instruction,
                DataProcessingScalarFloatingPointAdvancedSIMDGroup::ConversionFloatingPointAndInteger instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(
        Instruction&&                                                                            instruction,
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingOneSource instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&&                                                            instruction,
                 DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointCompare instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&&                                                              instruction,
                 DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointImmediate instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&&                                                                       instruction,
                 DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointConditionalCompare instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(
        Instruction&&                                                                            instruction,
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingTwoSource instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(Instruction&&                                                                      instruction,
                 DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointConditionalSelect instructionType) {
        throw not_implemented_feature {};
    }
    void Execute(
        Instruction&&                                                                              instruction,
        DataProcessingScalarFloatingPointAdvancedSIMDGroup::FloatingPointDataProcessingThreeSource instructionType) {
        throw not_implemented_feature {};
    }

    void Execute(Instruction&& instruction, A64ReservedGroup instructionClass) {
        switch (instructionClass) {
            case A64ReservedGroup::UDP:
                throw not_implemented_feature {};
                break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    void Execute(Instruction&& instruction, A64DataProcessingImmediateGroup instructionClass) {
        switch (instructionClass) {
            case A64DataProcessingImmediateGroup::PC_RELATIVE_ADDRESSING: {
                const auto instructionType =
                    DataProcessingImmediateGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingImmediateGroup::PC_RELATIVE_ADDRESSING >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingImmediateGroup::ADD_SUBTRACT_IMMEDIATE: {
                const auto instructionType =
                    DataProcessingImmediateGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingImmediateGroup::ADD_SUBTRACT_IMMEDIATE >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingImmediateGroup::ADD_SUBTRACT_IMMEDIATE_TAG: {
                const auto instructionType =
                    DataProcessingImmediateGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingImmediateGroup::ADD_SUBTRACT_IMMEDIATE_TAG >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingImmediateGroup::LOGICAL_IMMEDIATE: {
                const auto instructionType =
                    DataProcessingImmediateGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingImmediateGroup::LOGICAL_IMMEDIATE >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingImmediateGroup::MOVE_WIDE_IMMEDIATE: {
                const auto instructionType =
                    DataProcessingImmediateGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingImmediateGroup::MOVE_WIDE_IMMEDIATE >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingImmediateGroup::BITFIELD: {
                const auto instructionType =
                    DataProcessingImmediateGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingImmediateGroup::BITFIELD >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingImmediateGroup::EXTRACT: {
                const auto instructionType =
                    DataProcessingImmediateGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingImmediateGroup::BITFIELD >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    void Execute(Instruction&& instruction, A64BranchExceptionSystemGroup instructionClass) {
        switch (instructionClass) {
            case A64BranchExceptionSystemGroup::CONDITIONAL_BRANCHING: {
                const auto instructionType =
                    BranchExceptionSystemGroup::GetInstance()
                        .GetInstructionType< A64BranchExceptionSystemGroup::CONDITIONAL_BRANCHING >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64BranchExceptionSystemGroup::EXCEPTION_GENERATION: {
                const auto instructionType =
                    BranchExceptionSystemGroup::GetInstance()
                        .GetInstructionType< A64BranchExceptionSystemGroup::EXCEPTION_GENERATION >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64BranchExceptionSystemGroup::HINTS: {
                const auto instructionType =
                    BranchExceptionSystemGroup::GetInstance()
                        .GetInstructionType< A64BranchExceptionSystemGroup::HINTS >(instruction);
                Execute(std::move(instruction), instructionType);
                throw not_implemented_feature {};
            } break;
            case A64BranchExceptionSystemGroup::BARRIERS: {
                const auto instructionType =
                    BranchExceptionSystemGroup::GetInstance()
                        .GetInstructionType< A64BranchExceptionSystemGroup::BARRIERS >(instruction);
                Execute(std::move(instruction), instructionType);
                throw not_implemented_feature {};
            } break;
            case A64BranchExceptionSystemGroup::PSTATE: {
                const auto instructionType =
                    BranchExceptionSystemGroup::GetInstance()
                        .GetInstructionType< A64BranchExceptionSystemGroup::PSTATE >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64BranchExceptionSystemGroup::SYSTEM_INSTRUCTIONS: {
                const auto instructionType =
                    BranchExceptionSystemGroup::GetInstance()
                        .GetInstructionType< A64BranchExceptionSystemGroup::SYSTEM_INSTRUCTIONS >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64BranchExceptionSystemGroup::SYSTEM_REGISTER_MOVE: {
                const auto instructionType =
                    BranchExceptionSystemGroup::GetInstance()
                        .GetInstructionType< A64BranchExceptionSystemGroup::SYSTEM_REGISTER_MOVE >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64BranchExceptionSystemGroup::UNCONDITIONAL_BRANCH_REGISTER: {
                const auto instructionType =
                    BranchExceptionSystemGroup::GetInstance()
                        .GetInstructionType< A64BranchExceptionSystemGroup::UNCONDITIONAL_BRANCH_REGISTER >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64BranchExceptionSystemGroup::UNCONDITIONAL_BRANCH_IMMEDIATE: {
                const auto instructionType =
                    BranchExceptionSystemGroup::GetInstance()
                        .GetInstructionType< A64BranchExceptionSystemGroup::UNCONDITIONAL_BRANCH_IMMEDIATE >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64BranchExceptionSystemGroup::COMPARE_AND_BRANCH_IMMEDIATE: {
                const auto instructionType =
                    BranchExceptionSystemGroup::GetInstance()
                        .GetInstructionType< A64BranchExceptionSystemGroup::COMPARE_AND_BRANCH_IMMEDIATE >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64BranchExceptionSystemGroup::TEST_AND_BRANCH_IMMEDIATE: {
                const auto instructionType =
                    BranchExceptionSystemGroup::GetInstance()
                        .GetInstructionType< A64BranchExceptionSystemGroup::TEST_AND_BRANCH_IMMEDIATE >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    void Execute(Instruction&& instruction, A64LoadStoreGroup instructionClass) {
        switch (instructionClass) {
            case A64LoadStoreGroup::ADVANCED_SIMD_LOAD_STORE_MULTIPLE_STRUCTURES: {
                const auto instructionType =
                    LoadStoreGroup::GetInstance()
                        .GetInstructionType< A64LoadStoreGroup::ADVANCED_SIMD_LOAD_STORE_MULTIPLE_STRUCTURES >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64LoadStoreGroup::ADVANCED_SIMD_LOAD_STORE_MULTIPLE_STRUCTURES_POST_INDEXED: {
                const auto instructionType =
                    LoadStoreGroup::GetInstance()
                        .GetInstructionType<
                            A64LoadStoreGroup::ADVANCED_SIMD_LOAD_STORE_MULTIPLE_STRUCTURES_POST_INDEXED >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64LoadStoreGroup::ADVANCED_SIMD_LOAD_STORE_SINGLE_STRUCTURE: {
                const auto instructionType =
                    LoadStoreGroup::GetInstance()
                        .GetInstructionType< A64LoadStoreGroup::ADVANCED_SIMD_LOAD_STORE_SINGLE_STRUCTURE >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64LoadStoreGroup::ADVANCED_SIMD_LOAD_STORE_SINGLE_STRUCTURE_POST_INDEXED: {
                const auto instructionType =
                    LoadStoreGroup::GetInstance()
                        .GetInstructionType<
                            A64LoadStoreGroup::ADVANCED_SIMD_LOAD_STORE_SINGLE_STRUCTURE_POST_INDEXED >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64LoadStoreGroup::LOAD_STORE_MEMORY_TAG: {
                const auto instructionType =
                    LoadStoreGroup::GetInstance().GetInstructionType< A64LoadStoreGroup::LOAD_STORE_MEMORY_TAG >(
                        instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64LoadStoreGroup::LOAD_STORE_EXCLUSIVE: {
                const auto instructionType =
                    LoadStoreGroup::GetInstance().GetInstructionType< A64LoadStoreGroup::LOAD_STORE_EXCLUSIVE >(
                        instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64LoadStoreGroup::LDAPR_STLR_UNSCALED_IMMEDIATE: {
                const auto instructionType =
                    LoadStoreGroup::GetInstance()
                        .GetInstructionType< A64LoadStoreGroup::LDAPR_STLR_UNSCALED_IMMEDIATE >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64LoadStoreGroup::LOAD_REGISTER_LITERAL: {
                const auto instructionType =
                    LoadStoreGroup::GetInstance().GetInstructionType< A64LoadStoreGroup::LOAD_REGISTER_LITERAL >(
                        instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64LoadStoreGroup::LOAD_STORE_NO_ALLOCATE_PAIR_OFFSET: {
                const auto instructionType =
                    LoadStoreGroup::GetInstance()
                        .GetInstructionType< A64LoadStoreGroup::LOAD_STORE_NO_ALLOCATE_PAIR_OFFSET >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64LoadStoreGroup::LOAD_STORE_REGISTER_PAIR_POST_INDEXED: {
                const auto instructionType =
                    LoadStoreGroup::GetInstance()
                        .GetInstructionType< A64LoadStoreGroup::LOAD_STORE_REGISTER_PAIR_POST_INDEXED >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64LoadStoreGroup::LOAD_STORE_REGISTER_PAIR_OFFSET: {
                const auto instructionType =
                    LoadStoreGroup::GetInstance()
                        .GetInstructionType< A64LoadStoreGroup::LOAD_STORE_REGISTER_PAIR_OFFSET >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64LoadStoreGroup::LOAD_STORE_REGISTER_PAIR_PRE_INDEXED: {
                const auto instructionType =
                    LoadStoreGroup::GetInstance()
                        .GetInstructionType< A64LoadStoreGroup::LOAD_STORE_REGISTER_PAIR_PRE_INDEXED >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64LoadStoreGroup::LOAD_STORE_REGISTER_UNSCALED_IMMEDIATE: {
                const auto instructionType =
                    LoadStoreGroup::GetInstance()
                        .GetInstructionType< A64LoadStoreGroup::LOAD_STORE_REGISTER_UNSCALED_IMMEDIATE >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64LoadStoreGroup::LOAD_STORE_REGISTER_IMMEDIATE_POST_INDEXED: {
                const auto instructionType =
                    LoadStoreGroup::GetInstance()
                        .GetInstructionType< A64LoadStoreGroup::LOAD_STORE_REGISTER_IMMEDIATE_POST_INDEXED >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64LoadStoreGroup::LOAD_STORE_REGISTER_UNPRIVILEGED: {
                const auto instructionType =
                    LoadStoreGroup::GetInstance()
                        .GetInstructionType< A64LoadStoreGroup::LOAD_STORE_REGISTER_UNPRIVILEGED >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64LoadStoreGroup::LOAD_STORE_REGISTER_IMMEDIATE_PRE_INDEXED: {
                const auto instructionType =
                    LoadStoreGroup::GetInstance()
                        .GetInstructionType< A64LoadStoreGroup::LOAD_STORE_REGISTER_IMMEDIATE_PRE_INDEXED >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64LoadStoreGroup::ATOMIC_MEMORY_OPERATION: {
                const auto instructionType =
                    LoadStoreGroup::GetInstance().GetInstructionType< A64LoadStoreGroup::ATOMIC_MEMORY_OPERATION >(
                        instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64LoadStoreGroup::LOAD_STORE_REGISTER_REGISTER_OFFSET: {
                const auto instructionType =
                    LoadStoreGroup::GetInstance()
                        .GetInstructionType< A64LoadStoreGroup::LOAD_STORE_REGISTER_REGISTER_OFFSET >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64LoadStoreGroup::LOAD_STORE_REGISTER_PAC: {
                const auto instructionType =
                    LoadStoreGroup::GetInstance().GetInstructionType< A64LoadStoreGroup::LOAD_STORE_REGISTER_PAC >(
                        instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64LoadStoreGroup::LOAD_STORE_REGISTER_UNSIGNED_IMMEDIATE: {
                const auto instructionType =
                    LoadStoreGroup::GetInstance()
                        .GetInstructionType< A64LoadStoreGroup::LOAD_STORE_REGISTER_UNSIGNED_IMMEDIATE >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    void Execute(Instruction&& instruction, A64DataProcessingRegisterGroup instructionClass) {
        switch (instructionClass) {
            case A64DataProcessingRegisterGroup::DATA_PROCESSING_TWO_SOURCE: {
                const auto instructionType =
                    DataProcessingRegisterGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingRegisterGroup::DATA_PROCESSING_TWO_SOURCE >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingRegisterGroup::DATA_PROCESSING_ONE_SOURCE: {
                const auto instructionType =
                    DataProcessingRegisterGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingRegisterGroup::DATA_PROCESSING_ONE_SOURCE >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingRegisterGroup::LOGICAL_SHIFTED_REGISTER: {
                const auto instructionType =
                    DataProcessingRegisterGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingRegisterGroup::LOGICAL_SHIFTED_REGISTER >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingRegisterGroup::ADD_SUBTRACT_SHIFTED_REGISTER: {
                const auto instructionType =
                    DataProcessingRegisterGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingRegisterGroup::ADD_SUBTRACT_SHIFTED_REGISTER >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingRegisterGroup::ADD_SUBTRACT_EXTENDED_REGISTER: {
                const auto instructionType =
                    DataProcessingRegisterGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingRegisterGroup::ADD_SUBTRACT_EXTENDED_REGISTER >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingRegisterGroup::ADD_SUBTRACT_CARRY: {
                const auto instructionType =
                    DataProcessingRegisterGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingRegisterGroup::ADD_SUBTRACT_CARRY >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingRegisterGroup::ROTATE_RIGHT_INTO_FLAGS: {
                const auto instructionType =
                    DataProcessingRegisterGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingRegisterGroup::ROTATE_RIGHT_INTO_FLAGS >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingRegisterGroup::EVALUATE_INTO_FLAGS: {
                const auto instructionType =
                    DataProcessingRegisterGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingRegisterGroup::EVALUATE_INTO_FLAGS >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingRegisterGroup::CONDITIONAL_COMPARE_REGISTER: {
                const auto instructionType =
                    DataProcessingRegisterGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingRegisterGroup::CONDITIONAL_COMPARE_REGISTER >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingRegisterGroup::CONDITIONAL_COMPARE_IMMEDIATE: {
                const auto instructionType =
                    DataProcessingRegisterGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingRegisterGroup::CONDITIONAL_COMPARE_IMMEDIATE >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingRegisterGroup::CONDITIONAL_SELECT: {
                const auto instructionType =
                    DataProcessingRegisterGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingRegisterGroup::CONDITIONAL_SELECT >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingRegisterGroup::DATA_PROCESSING_THREE_SOURCE: {
                const auto instructionType =
                    DataProcessingRegisterGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingRegisterGroup::DATA_PROCESSING_THREE_SOURCE >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    void Execute(Instruction&& instruction, A64DataProcessingScalarFloatingPointAdvancedSIMDGroup instructionClass) {
        switch (instructionClass) {
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::CRYPTOGRAPHIC_AES: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::CRYPTOGRAPHIC_AES >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::CRYPTOHRAPHIC_THREE_REGISTER_SHA: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType<
                            A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::CRYPTOHRAPHIC_THREE_REGISTER_SHA >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::CRYPTOGRAPHIC_TWO_REGISTER_SHA: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType<
                            A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::CRYPTOGRAPHIC_TWO_REGISTER_SHA >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_SCALAR_COPY: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType<
                            A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_SCALAR_COPY >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMG_SCALAR_THREE_SAME_FP16: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::
                                                 ADVANCED_SIMG_SCALAR_THREE_SAME_FP16 >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::
                ADVANCED_SIMD_SCALAR_TWO_REGISTER_MISCELLANEOUS_FP16: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::
                                                 ADVANCED_SIMD_SCALAR_TWO_REGISTER_MISCELLANEOUS_FP16 >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_SCALAR_THREE_SAME_EXTRACTION: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::
                                                 ADVANCED_SIMD_SCALAR_THREE_SAME_EXTRACTION >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::
                ADVANCED_SIMD_SCALAR_TWO_REGISTER_MISCELLANEOUS: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::
                                                 ADVANCED_SIMD_SCALAR_TWO_REGISTER_MISCELLANEOUS >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_SCALAR_PAIRWISE: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType<
                            A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_SCALAR_PAIRWISE >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_SCALAR_THREE_DIFFERENT: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::
                                                 ADVANCED_SIMD_SCALAR_THREE_DIFFERENT >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_SCALAR_THREE_SAME: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType<
                            A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_SCALAR_THREE_SAME >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_SCALAR_SHIFT_BY_IMMEDIATE: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::
                                                 ADVANCED_SIMD_SCALAR_SHIFT_BY_IMMEDIATE >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_SCALAR_X_INDEXED_ELEMENT: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::
                                                 ADVANCED_SIMD_SCALAR_X_INDEXED_ELEMENT >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_TABLE_LOOKUP: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType<
                            A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_TABLE_LOOKUP >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_PERMUTE: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType<
                            A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_PERMUTE >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_EXTRACT: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType<
                            A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_EXTRACT >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_COPY: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType<
                            A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_COPY >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_THREE_SAME_FP16: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType<
                            A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_THREE_SAME_FP16 >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_TWO_REGISTER_MISCELLANEOUS_FP16: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::
                                                 ADVANCED_SIMD_TWO_REGISTER_MISCELLANEOUS_FP16 >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_THREE_REGISTER_EXTENSION: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::
                                                 ADVANCED_SIMD_THREE_REGISTER_EXTENSION >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_TWO_REGISTER_MISCELLANEOUS: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::
                                                 ADVANCED_SIMD_TWO_REGISTER_MISCELLANEOUS >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_ACROSS_LANES: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType<
                            A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_ACROSS_LANES >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_THREE_DIFFERENT: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType<
                            A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_THREE_DIFFERENT >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_THREE_SAME: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType<
                            A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_THREE_SAME >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_MODIFIED_IMMEDIATE: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType<
                            A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_MODIFIED_IMMEDIATE >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_SHIFT_BY_IMMEDIATE: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType<
                            A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_SHIFT_BY_IMMEDIATE >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::ADVANCED_SIMD_VECTOR_X_INDEXED_ELEMENT: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::
                                                 ADVANCED_SIMD_VECTOR_X_INDEXED_ELEMENT >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::CRYPTOGRAPHIC_THREE_REGISTER_IMM2: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType<
                            A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::CRYPTOGRAPHIC_THREE_REGISTER_IMM2 >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::CRYPTOGRAPHIC_THREE_REGISTER_SHA512: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::
                                                 CRYPTOGRAPHIC_THREE_REGISTER_SHA512 >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::CRYPTOGRAPHIC_FOUR_REGISTER: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType<
                            A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::CRYPTOGRAPHIC_FOUR_REGISTER >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::XAR: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::XAR >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::CRYPTOGRAPHIC_TWO_REGISTER_SHA512: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType<
                            A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::CRYPTOGRAPHIC_TWO_REGISTER_SHA512 >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::CONVERSION_FLOATING_POINT_AND_FIXED_POINT: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::
                                                 CONVERSION_FLOATING_POINT_AND_FIXED_POINT >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::CONVERSION_FLOATING_POINT_AND_INTEGER: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::
                                                 CONVERSION_FLOATING_POINT_AND_INTEGER >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::FLOATING_POINT_DATA_PROCESSING_ONE_SOURCE: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::
                                                 FLOATING_POINT_DATA_PROCESSING_ONE_SOURCE >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::FLOATING_POINT_COMPARE: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType<
                            A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::FLOATING_POINT_COMPARE >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::FLOATING_POINT_IMMEDIATE: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType<
                            A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::FLOATING_POINT_IMMEDIATE >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::FLOATING_POINT_CONDITIONAL_COMPARE: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType<
                            A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::FLOATING_POINT_CONDITIONAL_COMPARE >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::FLOATING_POINT_DATA_PROCESSING_TWO_SOURCE: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::
                                                 FLOATING_POINT_DATA_PROCESSING_TWO_SOURCE >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::FLOATING_POINT_CONDITIONAL_SELECT: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType<
                            A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::FLOATING_POINT_CONDITIONAL_SELECT >(
                            instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            case A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::FLOATING_POINT_DATA_PROCESSING_THREE_SOURCE: {
                const auto instructionType =
                    DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance()
                        .GetInstructionType< A64DataProcessingScalarFloatingPointAdvancedSIMDGroup::
                                                 FLOATING_POINT_DATA_PROCESSING_THREE_SOURCE >(instruction);
                Execute(std::move(instruction), instructionType);
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }

    void Execute(Instruction&& instruction, A64DecodeGroup decodeGroup) {
        if (decodeGroup == A64DecodeGroup::Reserved) {
            const auto instructionType = ReservedGroup::GetInstance().GetInstructionClass(instruction);
            Execute(std::move(instruction), instructionType);
        } else if (decodeGroup == A64DecodeGroup::ScalableVectorExtension) {
            throw not_implemented_feature {};
        } else if (decodeGroup == A64DecodeGroup::DataProcessingImmediate) {
            const auto instructionType = DataProcessingImmediateGroup::GetInstance().GetInstructionClass(instruction);
            Execute(std::move(instruction), instructionType);
        } else if (decodeGroup == A64DecodeGroup::BranchExceptionSystem) {
            const auto instructionType = BranchExceptionSystemGroup::GetInstance().GetInstructionClass(instruction);
            Execute(std::move(instruction), instructionType);
        } else if (decodeGroup == A64DecodeGroup::LoadStore) {
            const auto instructionType = LoadStoreGroup::GetInstance().GetInstructionClass(instruction);
            Execute(std::move(instruction), instructionType);
        } else if (decodeGroup == A64DecodeGroup::DataProcessingRegister) {
            const auto instructionType = DataProcessingRegisterGroup::GetInstance().GetInstructionClass(instruction);
            Execute(std::move(instruction), instructionType);
        } else if (decodeGroup == A64DecodeGroup::DataProcessingScalarFloatingPointAdvancedSIMD) {
            const auto instructionType =
                DataProcessingScalarFloatingPointAdvancedSIMDGroup::GetInstance().GetInstructionClass(instruction);
            Execute(std::move(instruction), instructionType);
        } else {
            throw undefined_instruction {};
        }
    }

  public:
    A64ProcessState(Object* logger, A64ProcessingUnitWatcher& watcher, ICacheMemory* upStreamMemory,
                    UniqueRef< IMemory > stackMemory) :
        m_status(IProcessingUnit::ProcessStatus::Idle),
        m_upStreamMemory(upStreamMemory),
        m_stackMemory(std::move(stackMemory)),
        m_programs(),
        m_watcher(watcher),
        m_debugObject(*logger),
        m_programMutex(),
        m_stopRunningInterrupt(nullptr),
        m_gpRegisters(*this) {
    }

    Result SetProgram(Program program) {
        constexpr bool isStepInAllowed = false;
        m_debugObject.LogTrace(LogType::Other, "Adding program {}!",
                               static_cast< const void* const >(program.GetProgram()));
        auto resultElement =
            std::allocate_shared< ResultElement >(std::pmr::polymorphic_allocator< ResultElement > {}, isStepInAllowed);
        {
            std::unique_lock lock { m_programMutex };
            m_programs.emplace(std::move(program), isStepInAllowed, resultElement->weak_from_this());
        }
        return Result { std::move(resultElement) };
    }

    ControlledResult SetProgramToStepIn(Program program) {
        constexpr bool isStepInAllowed = true;
        m_debugObject.LogTrace(LogType::Other, "Adding program {} to step in!",
                               static_cast< const void* const >(program.GetProgram()));
        auto resultElement =
            std::allocate_shared< ResultElement >(std::pmr::polymorphic_allocator< ResultElement > {}, isStepInAllowed);
        {
            std::unique_lock lock { m_programMutex };
            m_programs.emplace(std::move(program), isStepInAllowed, resultElement->weak_from_this());
        }
        return ControlledResult { std::move(resultElement) };
    }

    ICacheMemory const* GetUpStreamMemory() const noexcept {
        return m_upStreamMemory;
    }

    IMemory const* GetStackMemory() const noexcept {
        return m_stackMemory.get();
    }

    IMemory const* GetCurrentProgramMemory() const noexcept {
        return m_programs.size() > 0 ? m_programs.front().m_program.GetProgram() : nullptr;
    }

    IProcessingUnit::ProcessStatus GetStatus() const noexcept {
        return m_status.load(std::memory_order_seq_cst);
    }

    void Run(Interrupt interrupt, std::condition_variable& runProcessCondVar,
             std::condition_variable_any& stepInCondVar) {
        if (m_programs.size() == 0) {
            m_debugObject.Log(LogType::Other, "Run(): no programs found to run!");
            return;
        }

        m_debugObject.Log(LogType::Other, "ProcessingUnit::Run() is running!");

        auto                             currentProgramMemory = m_programs.front().m_program.GetProgram();
        ProgramSize                      currentProgramSize   = m_programs.front().m_program.GetProgramSize();
        bool                             doStepIn             = m_programs.front().m_stepIn;
        std::shared_ptr< ResultElement > currentResult        = m_programs.front().m_result.lock();

        m_stopRunningInterrupt = interrupt;
        m_status.store(IProcessingUnit::ProcessStatus::Running, std::memory_order_seq_cst);
        Interrupt                    stepInDoneInterrupt {};
        std::condition_variable_any* stepInDoneCondVar { nullptr };
        if (doStepIn && currentResult) {
            if (!stepInDoneInterrupt) {
                stepInDoneInterrupt = CreateInterrupt();
            }
            stepInDoneCondVar = std::addressof(currentResult->StepInSetup(stepInCondVar, stepInDoneInterrupt));
        }

        while (currentProgramMemory && !m_stopRunningInterrupt->IsTriggered()) {
            m_debugObject.Log(LogType::Other, "ProcessingUnit::Run() is starting program {} in {} mode!",
                              static_cast< const void* >(currentProgramMemory), doStepIn ? "StepIn" : "Run");

            if (currentResult) {
                currentResult->Signal(doStepIn ? IResult::State::Waiting : IResult::State::Running);
            }

            auto& memory = currentProgramMemory;
            bool  setup  = true;
            while (!m_stopRunningInterrupt->IsTriggered() &&
                   (!doStepIn || (doStepIn && currentResult) /* If result is destroyed then ignore the program */)) {
                // TODO: implement program handle
                if (setup) {
                    SetupRegisters();
                    setup = false;
                }

                if (doStepIn) {
                    if (currentResult) {
                        currentResult->StoreGPRegisters(m_gpRegisters.ReadBulk());
                        currentResult->StorePC(PC());
                        // TODO: Store more vars
                        std::mutex       localMutex {};
                        std::unique_lock localLock { localMutex };
                        auto             stepInInterrupt = currentResult->GetStepInInterrupt();
                        stepInDoneInterrupt->Trigger();
                        stepInDoneCondVar->notify_one();
                        currentResult->Signal(IResult::State::StepInMode);
                        stepInCondVar.wait(localLock, [=]() {
                            return stepInInterrupt->IsTriggered() || m_stopRunningInterrupt->IsTriggered();
                        });
                    } else {
                        break;
                    }
                }

                if (PC() >= currentProgramSize) {
                    if (doStepIn && currentResult) {
                        currentResult->SignalStepInValidity(false);
                        stepInDoneInterrupt->Trigger();
                        stepInDoneCondVar->notify_one();
                    }
                    break; // Program ended
                }

                // Fetch
                auto instruction = Instruction { memory->Read(PC()) };

                // Decode
                const auto decodeGroup = A64InstructionManager::GetDecodeGroup(instruction);
                PC() += 1;

                // Execute
                try {
                    Execute(std::move(instruction), decodeGroup);
                } catch (undefined_instruction) {
                    m_debugObject.Log(LogType::Other, "Program {} is using an undefined instruction!",
                                      static_cast< const void* >(currentProgramMemory));
                    ExitProgramWithInterrupt(static_cast< const void* >(currentProgramMemory));
                } catch (undefined_behaviour) {
                    m_debugObject.Log(LogType::Other, "Program {} caused undefined behaviour!",
                                      static_cast< const void* >(currentProgramMemory));
                    ExitProgramWithInterrupt(static_cast< const void* >(currentProgramMemory));
                } catch (undefined_register_access) {
                    m_debugObject.Log(LogType::Other, "Program {} triggered undefined register access!",
                                      static_cast< const void* >(currentProgramMemory));
                    ExitProgramWithInterrupt(static_cast< const void* >(currentProgramMemory));
                } catch (not_implemented_feature) {
                    m_debugObject.Log(LogType::Other, "Program {} is running using a non-implemented feature!",
                                      static_cast< const void* >(currentProgramMemory));
                    ExitProgramWithInterrupt(static_cast< const void* >(currentProgramMemory));
                }
            }

            if (m_stopRunningInterrupt->IsTriggered()) {
                m_status.store(IProcessingUnit::ProcessStatus::Interrupted, std::memory_order_seq_cst);
                if (currentResult) {
                    currentResult->StoreGPRegisters(m_gpRegisters.ReadBulk());
                    currentResult->Signal(Result::State::Interrupted);
                }

                m_watcher.RecordProcessInterrupted();
                if (doStepIn) {
                    currentResult->StepInFinalize();
                }
                return;
            }

            // Program completed without interrupt
            if (!doStepIn || (doStepIn && currentResult)) {
                m_watcher.RecordProcessHandled();
            }
            if (doStepIn && currentResult) {
                currentResult->StepInFinalize();
            }
            if (currentResult) {
                currentResult->StoreGPRegisters(m_gpRegisters.ReadBulk());
                currentResult->Signal(Result::State::Ready);
                if (doStepIn) {
                    // TODO: Store more variables
                }
            }
            m_debugObject.Log(LogType::Other, "ProcessingUnit::Run() finished executing program {}!",
                              static_cast< const void* >(currentProgramMemory));
            {
                std::unique_lock lock { m_programMutex };
                m_programs.pop();
            }
            if (m_programs.size() > 0) {
                auto                             currentProgramMemory = m_programs.front().m_program.GetProgram();
                ProgramSize                      currentProgramSize   = m_programs.front().m_program.GetProgramSize();
                bool                             doStepIn             = m_programs.front().m_stepIn;
                std::shared_ptr< ResultElement > currentResult        = m_programs.front().m_result.lock();
            } else {
                currentProgramMemory = nullptr;
                currentProgramSize   = 0;
                doStepIn             = false;
                currentResult        = nullptr;
            }
        }

        if (interrupt->IsTriggered()) {
            m_status.store(IProcessingUnit::ProcessStatus::Interrupted, std::memory_order_seq_cst);
            if (currentResult) {
                currentResult->StoreGPRegisters(m_gpRegisters.ReadBulk());
                currentResult->Signal(Result::State::Interrupted);
                if (doStepIn) {
                    // TODO: Store more variables
                }
            }

            m_watcher.RecordProcessInterrupted();
            return;
        }

        m_status.store(IProcessingUnit::ProcessStatus::Idle, std::memory_order_seq_cst);
        runProcessCondVar.notify_one();
        m_stopRunningInterrupt = nullptr;
        m_debugObject.Log(LogType::Other, "ProcessingUnit::Run() finished!");
    }

    void Reset() noexcept {
        auto status = m_status.load(std::memory_order_seq_cst);
        assert(status != IProcessingUnit::ProcessStatus::Running && "Impossible to reset a running process state!");

        if (status == IProcessingUnit::ProcessStatus::Interrupted &&
            m_programs.size() > 0 /* Interrupted a running a program */) {
            {
                std::unique_lock lock { m_programMutex };
                m_programs.pop();
            }
            m_watcher.RecordProcessHandled();
        }
        m_upStreamMemory->ClearCache();
        m_status.store(IProcessingUnit::ProcessStatus::Idle, std::memory_order_seq_cst);
    }

  private:
#include <ProcessingUnit/InstructionCodeImpl/AArch64Operations/AArch64Operations.h>
#include <ProcessingUnit/InstructionCodeImpl/SharedOperations/SharedOperations.h>

    void SetupRegisters() {
        PC() = 0;
        SP() = m_stackMemory->Size() - 1;
    }

    void ExitProgramWithInterrupt(const void* programAddress) const {
        m_debugObject.LogTrace(LogType::Other, "Exiting execution of program {} with interrupt signal!",
                               programAddress);
        m_stopRunningInterrupt->Trigger(); // At this point, the interrupt exists
    }

    std::atomic< IProcessingUnit::ProcessStatus >               m_status;
    ICacheMemory*                                               m_upStreamMemory;
    UniqueRef< IMemory >                                        m_stackMemory;
    std::queue< ProgramState, std::pmr::deque< ProgramState > > m_programs;
    std::mutex                                                  m_programMutex;
    A64ProcessingUnitWatcher&                                   m_watcher;
    Object&                                                     m_debugObject;

    // Registers ?
    Interrupt       m_stopRunningInterrupt;
    GPRegisters     m_gpRegisters;
    SystemRegisters m_sysRegisters;
};

class A64ProcessingUnit::Impl final {
  public:
    Impl(Object* logger, ICacheMemory* upStreamMemory, UniqueRef< IMemory > stackMemory) :
        m_processState(logger, m_watcher, upStreamMemory, std::move(stackMemory)),
        m_watcher(),
        m_runProcessMutex(),
        m_runProcessCondVar(),
        m_stepInProcessCondVar(),
        m_runProcessInterrupt(CreateInterrupt()),
        m_endProcessInterrupt(CreateInterrupt()),
        m_cleanUp(CreateInterrupt()),
        m_debugObject(*logger),
        m_runningThread([&]() { this->InternalRun(); }) {
        m_debugObject.Log(LogType::Construction, "Processing unit construction succeeded");
    }

    DELETE_COPY_CLASS(Impl)
    DEFAULT_MOVE_CLASS(Impl)

    ~Impl() {
        m_cleanUp->Trigger();
        m_runProcessCondVar.notify_one();

        if (m_runningThread.joinable()) {
            m_runningThread.join();
        }
        m_debugObject.Log(LogType::Destruction, "Processing unit destruction succeeded");
    }

    ICacheMemory const* GetUpStreamMemory() const noexcept {
        return m_processState.GetUpStreamMemory();
    }

    IMemory const* GetStackMemory() const noexcept {
        return m_processState.GetStackMemory();
    }

    IMemory const* GetCurrentProgramMemory() const noexcept {
        return m_processState.GetCurrentProgramMemory();
    }

    A64ProcessingUnit::ProcessStatus GetStatus() const noexcept {
        return m_processState.GetStatus();
    }

    const IProcessingUnitWatcher& GetProcessingUnitWatcher() const noexcept {
        return m_watcher;
    }

    const A64ProcessingUnit::ProcessState* const GetCurrentProcessState() const noexcept {
        return &m_processState;
    }

    Result Run(Program program) {
        return RequestRes< Result >(std::move(program));
    }

    ControlledResult StepIn(Program program) {
        return RequestRes< ControlledResult >(std::move(program));
    }

    void Stop() {
        if (m_processState.GetStatus() == ProcessStatus::Running) {
            m_endProcessInterrupt->Trigger();
            m_stepInProcessCondVar.notify_all();
        }
    }

    void ResetProcessState() noexcept {
        m_processState.Reset();
        m_runProcessCondVar.notify_one();
    }

  private:
    void InternalRun() {
        while (!m_cleanUp->IsTriggered()) {
            std::unique_lock lock(m_runProcessMutex);
            m_runProcessCondVar.wait(
                lock, [=]() { return m_runProcessInterrupt->IsTriggered() || m_cleanUp->IsTriggered(); });

            if (m_cleanUp->IsTriggered()) {
                break; // Cleanup triggered
            }

            m_processState.Run(m_endProcessInterrupt, m_runProcessCondVar, m_stepInProcessCondVar);

            if (m_endProcessInterrupt->IsTriggered()) {
                m_endProcessInterrupt->Reset();
            }
            if (m_runProcessInterrupt->IsTriggered()) {
                m_runProcessInterrupt->Reset();
            }
            ResetProcessState();
        }
    }

    template < class Res >
    Res RequestRes(Program program) {
        if constexpr (std::same_as< Res, ControlledResult >)
            m_debugObject.LogTrace(LogType::Other, "Stepping into program {} requested!",
                                   static_cast< const void* const >(program.GetProgram()));
        else if constexpr (std::same_as< Res, Result >)
            m_debugObject.LogTrace(LogType::Other, "Running program {} requested!",
                                   static_cast< const void* const >(program.GetProgram()));
        else
            auto throw_1 = ARM_EMU_EXCEPTION;

        auto processStatus = m_processState.GetStatus();
        if (processStatus == ProcessStatus::Running) {
            if constexpr (std::same_as< Res, ControlledResult >)
                return m_processState.SetProgramToStepIn(std::move(program));
            else if constexpr (std::same_as< Res, Result >)
                return m_processState.SetProgram(std::move(program));
        }

        if (processStatus == ProcessStatus::Interrupted) {
            m_debugObject.LogTrace(LogType::Other,
                                   "Process state is interrupt, waiting for idle status to add the program {}!",
                                   static_cast< const void* const >(program.GetProgram()));
            std::unique_lock lock(m_runProcessMutex);
            m_runProcessCondVar.wait(lock, [&]() { return m_processState.GetStatus() == ProcessStatus::Idle; });
        }

        if constexpr (std::same_as< Res, ControlledResult >) {
            auto result = m_processState.SetProgramToStepIn(std::move(program));
            m_runProcessInterrupt->Trigger();
            m_runProcessCondVar.notify_one();
            return result;
        } else if constexpr (std::same_as< Res, Result >) {
            auto result = m_processState.SetProgram(std::move(program));
            m_runProcessInterrupt->Trigger();
            m_runProcessCondVar.notify_one();
            return result;
        }
    }

    A64ProcessState             m_processState;
    A64ProcessingUnitWatcher    m_watcher;
    std::mutex                  m_runProcessMutex;
    std::condition_variable     m_runProcessCondVar;
    std::condition_variable_any m_stepInProcessCondVar;
    Interrupt                   m_runProcessInterrupt;
    Interrupt                   m_endProcessInterrupt;
    Interrupt                   m_cleanUp;
    Object&                     m_debugObject;
    std::thread                 m_runningThread;
};

template < class ImplDetail >
UniqueRef< A64ProcessingUnit::Impl > A64ProcessingUnit::ConstructProcessingUnit(ICacheMemory*        upStreamMemory,
                                                                                UniqueRef< IMemory > stackMemory,
                                                                                ImplDetail           myself) {
    myself->Log(LogType::Construction, "Constructing A64ProcessingUnit, with upStreamMemory: {}",
                static_cast< void* >(upStreamMemory));
    std::pmr::polymorphic_allocator< A64ProcessingUnit::Impl > alloc {};

    return allocate_unique< A64ProcessingUnit::Impl >(alloc, myself, upStreamMemory, std::move(stackMemory));
}

A64ProcessingUnit::A64ProcessingUnit(std::string name, ICacheMemory* upStreamMemory, UniqueRef< IMemory > stackMemory) :
    IProcessingUnit(std::move(name)),
    m_processingUnit(ConstructProcessingUnit(upStreamMemory, std::move(stackMemory), this)) {
}

A64ProcessingUnit::A64ProcessingUnit(ICacheMemory* upStreamMemory, UniqueRef< IMemory > stackMemory) :
    IProcessingUnit(Default_name),
    m_processingUnit(ConstructProcessingUnit(upStreamMemory, std::move(stackMemory), this)) {
}

A64ProcessingUnit::A64ProcessingUnit(A64ProcessingUnit&&) noexcept = default;

A64ProcessingUnit& A64ProcessingUnit::operator=(A64ProcessingUnit&&) noexcept = default;

A64ProcessingUnit::~A64ProcessingUnit() = default;

A64ProcessingUnit::ArchitectureProfile A64ProcessingUnit::GetExecutionState() const noexcept {
    return executionState;
}

A64ProcessingUnit::InstructionSet A64ProcessingUnit::GetInstructionSet() const noexcept {
    return instructionSet;
}

A64ProcessingUnit::ExtensionVersion A64ProcessingUnit::GetCurrentExtensionVersion() const noexcept {
    return extensionVersion;
}

const std::pmr::vector< A64ProcessingUnit::Extension >& A64ProcessingUnit::GetSupportedExtensions() const noexcept {
    return supportedExtensions;
}

bool A64ProcessingUnit::IsFeatureSupported(Feature feature) const noexcept {
    return supportedFeatures.find(feature) != supportedFeatures.end();
}

bool A64ProcessingUnit::IsExceptionSupported(ExceptionLevel exceptionLevel) const noexcept {
    return supportedExceptionLevels.find(exceptionLevel) != supportedExceptionLevels.end();
}

ICacheMemory const* A64ProcessingUnit::GetUpStreamMemory() const noexcept {
    return m_processingUnit->GetUpStreamMemory();
}

IMemory const* A64ProcessingUnit::GetStackMemory() const noexcept {
    return m_processingUnit->GetStackMemory();
}

IMemory const* A64ProcessingUnit::GetCurrentProgramMemory() const noexcept {
    return m_processingUnit->GetCurrentProgramMemory();
}

A64ProcessingUnit::ProcessStatus A64ProcessingUnit::GetStatus() const noexcept {
    return m_processingUnit->GetStatus();
}

const IProcessingUnitWatcher& A64ProcessingUnit::GetProcessingUnitWatcher() const noexcept {
    return m_processingUnit->GetProcessingUnitWatcher();
}

const A64ProcessingUnit::ProcessState* const A64ProcessingUnit::GetCurrentProcessState() const noexcept {
    return m_processingUnit->GetCurrentProcessState();
}

Result A64ProcessingUnit::Run(Program program) {
    return m_processingUnit->Run(std::move(program));
}

ControlledResult A64ProcessingUnit::StepIn(Program program) {
    return m_processingUnit->StepIn(std::move(program));
}

void A64ProcessingUnit::Stop() {
    m_processingUnit->Stop();
}

void A64ProcessingUnit::ResetProcessState() noexcept {
    m_processingUnit->ResetProcessState();
}

END_NAMESPACE