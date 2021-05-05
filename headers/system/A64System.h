#pragma once
#include <functional>
#include <concepts>

#include <utilities.hpp>
#include <Registers.h>
#include <Memory.h>
//#include <A64Enums.h>
//#include <A64Structs.h>
#include <A64InstructionSet.h>
#include <A64Instructions.h>

#include <ALU.h>
#include <AArch64.h>

namespace arm_emu {

class System {
    friend struct ALU;
    friend struct AArch64;
    friend struct Cache;

  public:
    System() = delete;
    NULL_COPY_MOVE(System)
    template < std::size_t N >
    System(const std::uint64_t& ramSize, std::array< std::uint32_t, N > instructions) : ram_block(ramSize), prog_block(N), cache(this, 0) {
        prog_block.write_block(0, instructions);
        cache.fill_cache();
    }

    template < typename Ty >
    requires iteratable< Ty >&& std::same_as< typename Ty::value_type, std::uint32_t > System(const std::uint64_t& ramSize, Ty instructions) noexcept :
        ram_block(ramSize), prog_block(instructions.size()), cache(this, 0) {
        std::uint32_t i = 0;
        for (auto&& instruction : instructions) { prog_block.write_loc(i++, instruction); }
        cache.fill_cache();
    }

    ~System() = default;

    using LogSignature =
        std::function< void(System* /*system*/, const std::uint64_t /*PC*/, DecodeGroupA64, BaseInstruction /*current instruction*/,
                            GPRegisters* /*general purpose registers*/, SpecialRegisters* /*special purpose registers*/, SIMDRegisters* /*SIMD registers*/) >;

    void run() {
        setup_registers();
        while (instructions_remaining()) { fetch_and_execute(); }
    }
    void run(const LogSignature& logger) {
        setup_registers();
        while (instructions_remaining()) { fetch_and_execute(logger); }
    }
    void run(std::uint64_t&& expected) {
        setup_registers();
        while (instructions_remaining()) { fetch_and_execute(); }
        std::cout << "Value of R0: " << gp_registers.X(0).to_ullong() << '\n';
        std::cout << "Expected value: " << expected << '\n';
    }
    void run(const LogSignature& logger, std::uint64_t&& expected) {
        setup_registers();
        while (instructions_remaining()) { fetch_and_execute(logger); }
        std::cout << "Value of R0: " << gp_registers.X(0).to_ullong() << '\n';
        std::cout << "Expected value: " << expected << '\n';
    }

  private:
    inline auto write_nzcv(std::uint8_t nzcv) {
        sp_registers.NZCV.N((nzcv >> 3) & 0x01);
        sp_registers.NZCV.Z((nzcv >> 2) & 0x01);
        sp_registers.NZCV.C((nzcv >> 1) & 0x01);
        sp_registers.NZCV.V((nzcv >> 0) & 0x01);
    };
    inline auto write_nzcv(std::bitset< 4 > nzcv) {
        sp_registers.NZCV.N(nzcv[3]);
        sp_registers.NZCV.Z(nzcv[2]);
        sp_registers.NZCV.C(nzcv[1]);
        sp_registers.NZCV.V(nzcv[0]);
    }

    auto process(ReservedInstruction< ReservedGroupA64::UDP >&& instruction) {}

    auto process(DataProcessingImmediateInstruction< DataProcessingImmediateGroupA64::PC_RELATIVE_ADDRESSING >&& instruction) {
        auto Rd    = instruction.get_Rd();
        auto immhi = instruction.get_immhi(); // 19bits
        auto immlo = instruction.get_immlo(); // 2bits
        switch (instruction.getInstructionType()) {
            case DataProcessingImmediateA64::PCRelativeAddressing::ADR: { // P.879
                std::bitset< 64 > imm;
                std::bitset< 21 > temp_imm = concate< 19, 2 >(immhi, immlo);
                imm                        = SignExtend< 64 >(temp_imm);

                std::bitset< 64 > base     = gp_registers.PC();
                std::uint64_t     new_addr = base.to_ullong() + imm.to_ullong();
                gp_registers.write(Rd, new_addr);
            } break;
            case DataProcessingImmediateA64::PCRelativeAddressing::ADRP: { // P.880
                std::bitset< 64 >      imm;
                std::bitset< 21 + 12 > temp_imm = concate< 21, 12 >(concate< 19, 2 >(immhi, immlo), 0);
                imm                             = SignExtend< 64 >(temp_imm);

                std::bitset< 64 > base = gp_registers.PC();
                for (auto i = 0; i <= 11; i++) { base[i] = 0; }
                std::uint64_t new_addr = base.to_ullong() + imm.to_ullong();
                gp_registers.write(Rd, new_addr);
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    auto process(DataProcessingImmediateInstruction< DataProcessingImmediateGroupA64::ADD_SUBTRACT_IMMEDIATE >&& instruction) {
        auto              Rd    = instruction.get_Rd();
        auto              Rn    = instruction.get_Rn();
        std::bitset< 12 > imm12 = instruction.get_imm12();
        auto              shift = instruction.get_sh();
        switch (instruction.getInstructionType()) {
            case DataProcessingImmediateA64::AddSubtractImmediate::ADDi_32BIT: { // P.779 + 88
                constexpr auto          datasize = 32;
                std::bitset< datasize > imm;
                if (shift)
                    imm = ZeroExtend< datasize >(concate< 12, 12 >(imm12, 0));
                else
                    imm = ZeroExtend< datasize >(imm12);
                auto operand1    = Rn == 31 ? SP< datasize >(&sp_registers) : gp_registers.W(Rn);
                auto [result, _] = AddWithCarry(operand1, imm, 0);
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                if (Rd == 31)
                    SP(&sp_registers, result);
                else
                    gp_registers.write(Rd, result);
            } break;
            case DataProcessingImmediateA64::AddSubtractImmediate::ADDSi_32BIT: { // P.787 + 88
                constexpr auto          datasize = 32;
                std::bitset< datasize > imm;
                if (shift) {
                    imm = ZeroExtend< datasize >(concate< 12, 12 >(imm12, 0));
                } else
                    imm = ZeroExtend< datasize >(imm12);
                auto operand1       = Rn == 31 ? SP< datasize >(&sp_registers) : gp_registers.W(Rn);
                auto [result, nzcv] = AddWithCarry(operand1, imm, 0);
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(nzcv), std::uint8_t >);
                write_nzcv(nzcv);
                gp_registers.write(Rd, result);
            } break;
            case DataProcessingImmediateA64::AddSubtractImmediate::SUBi_32BIT: { // P.1333 + 88
                constexpr auto          datasize = 32;
                std::bitset< datasize > imm;
                if (shift)
                    imm = ZeroExtend< datasize >(concate< 12, 12 >(imm12, 0));
                else
                    imm = ZeroExtend< datasize >(imm12);
                auto operand1 = Rn == 31 ? SP< datasize >(&sp_registers) : gp_registers.W(Rn);
                auto operand2 = imm;
                operand2.flip();
                auto [result, _] = AddWithCarry(operand1, operand2, static_cast< std::uint32_t >(1));
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                if (Rd == 31)
                    SP(&sp_registers, result);
                else
                    gp_registers.write(Rd, result);
            } break;
            case DataProcessingImmediateA64::AddSubtractImmediate::SUBSi_32BIT: { // P.1343 + 88
                constexpr auto          datasize = 32;
                std::bitset< datasize > imm;
                if (shift)
                    imm = ZeroExtend< datasize >(concate< 12, 12 >(imm12, 0));
                else
                    imm = ZeroExtend< datasize >(imm12);
                auto operand1 = Rn == 31 ? SP< datasize >(&sp_registers) : gp_registers.W(Rn);
                auto operand2 = imm;
                operand2.flip();
                auto [result, nzcv] = AddWithCarry(operand1, operand2, static_cast< std::uint32_t >(1));
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(nzcv), std::uint8_t >);
                write_nzcv(nzcv);
                gp_registers.write(Rd, result);
            } break;
            case DataProcessingImmediateA64::AddSubtractImmediate::ADDi_64BIT: { // P.779 + 88
                constexpr auto          datasize = 64;
                std::bitset< datasize > imm;
                if (shift) {
                    imm = ZeroExtend< datasize >(concate< 12, 12 >(imm12, 0));
                } else
                    imm = ZeroExtend< datasize >(imm12);
                auto operand1    = Rn == 31 ? SP< datasize >(&sp_registers) : gp_registers.X(Rn);
                auto [result, _] = AddWithCarry(operand1, imm, 0);
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                if (Rd == 31)
                    SP(&sp_registers, result);
                else
                    gp_registers.write(Rd, result);
            } break;
            case DataProcessingImmediateA64::AddSubtractImmediate::ADDSi_64BIT: { // P.787 + 88
                constexpr auto          datasize = 64;
                std::bitset< datasize > imm;
                if (shift) {
                    imm = ZeroExtend< datasize >(concate< 12, 12 >(imm12, 0));
                } else
                    imm = ZeroExtend< datasize >(imm12);
                auto operand1       = Rn == 31 ? SP< datasize >(&sp_registers) : gp_registers.X(Rn);
                auto [result, nzcv] = AddWithCarry(operand1, imm, 0);
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(nzcv), std::uint8_t >);
                write_nzcv(nzcv);

                gp_registers.write(Rd, result);
            } break;
            case DataProcessingImmediateA64::AddSubtractImmediate::SUBi_64BIT: { // P.1333 + 88
                constexpr auto          datasize = 64;
                std::bitset< datasize > imm;
                if (shift) {
                    imm = ZeroExtend< datasize >(concate< 12, 12 >(imm12, 0));
                } else
                    imm = ZeroExtend< datasize >(imm12);
                auto operand1 = Rn == 31 ? SP< datasize >(&sp_registers) : gp_registers.X(Rn);
                auto operand2 = imm;
                operand2.flip();
                auto [result, _] = AddWithCarry(operand1, operand2, static_cast< std::uint64_t >(1));
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                if (Rd == 31)
                    SP(&sp_registers, result);
                else
                    gp_registers.write(Rd, result);
            } break;
            case DataProcessingImmediateA64::AddSubtractImmediate::SUBSi_64BIT: { // P.1343 + 88
                constexpr auto          datasize = 64;
                std::bitset< datasize > imm;
                if (shift) {
                    imm = ZeroExtend< datasize >(concate< 12, 12 >(imm12, 0));
                } else
                    imm = ZeroExtend< datasize >(imm12);
                auto operand1 = Rn == 31 ? SP< datasize >(&sp_registers) : gp_registers.X(Rn);
                auto operand2 = imm;
                operand2.flip();
                auto [result, nzcv] = AddWithCarry(operand1, imm, static_cast< std::uint64_t >(1));
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(nzcv), std::uint8_t >);
                write_nzcv(nzcv);

                gp_registers.write(Rd, result);
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    auto process(DataProcessingImmediateInstruction< DataProcessingImmediateGroupA64::ADD_SUBTRACT_IMMEDIATE_TAG >&& instruction) {
        /*auto Xd    = instruction.get_Xd();
        auto Xn    = instruction.get_Xn();
        auto uimm4 = instruction.get_uimm4();
        auto uimm6 = instruction.get_uimm6();*/
        switch (instruction.getInstructionType()) {
            case DataProcessingImmediateA64::AddSubtractImmediateTag::ADDG: {
                throw not_implemented_feature {};
                // if (!Query< SystemSettings, Features, Features::FEAT_MTE >::result()) { throw undefined_instruction {}; }
                /*std::bitset<64> offset = LSL<64>(ZeroExtend<64>(std::bitset<6>(uimm6)), ALU::LOG2_TAG_GRANULE);
                std::bitset<64> operand1 = gp_registers.X(Xn);
                std::bitset<4> start_tag = AArch64::AllocationTagFromAddress(operand1);
                std::bitset<16> exclude = feat_mte_register.GCR_EL1.Exclude();
                std::bitset<64> result;
                std::bitset<4> rtag;*/
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    auto process(DataProcessingImmediateInstruction< DataProcessingImmediateGroupA64::LOGICAL_IMMEDIATE >&& instruction) {
        auto Rd   = instruction.get_Rd();
        auto Rn   = instruction.get_Rn();
        auto imms = instruction.get_imms();
        auto immr = instruction.get_immr();
        auto N    = instruction.get_N();
        switch (instruction.getInstructionType()) {
            case DataProcessingImmediateA64::LogicalImmediate::AND_32BIT: { // P.881
                if (N != 0) throw undefined_behaviour {};
                constexpr auto datasize = 32;

                auto [imm, _] = DecodeBitMasks< datasize >(N, imms, immr, true);
                static_assert(std::is_same_v< decltype(imm), std::bitset< datasize > >);
                auto operand1 = gp_registers.W(Rn);
                auto result   = operand1 & imm;
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                if (Rd == 31)
                    SP(&sp_registers, result);
                else
                    gp_registers.write(Rd, result);
            } break;
            case DataProcessingImmediateA64::LogicalImmediate::ORR_32BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingImmediateA64::LogicalImmediate::EOR_32BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingImmediateA64::LogicalImmediate::ANDS_32BIT: { // P.885
                if (N == 0) throw undefined_behaviour {};
                constexpr auto datasize = 32;

                auto [imm, _] = DecodeBitMasks< datasize >(N, imms, immr, true);
                static_assert(std::is_same_v< decltype(imm), std::bitset< datasize > >);
                auto operand1 = gp_registers.W(Rn);
                auto result   = operand1 & imm;
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                std::bitset< 4 > nzcv = concate< 2, 2 >(concate< 1, 1 >(result[31] ? 1 : 0, result == 0 ? 1 : 0), 0);
                write_nzcv(nzcv);
                if (Rd == 31) // Documentation does not state SP as an option to Rd
                    SP(&sp_registers, result);
                else
                    gp_registers.write(Rd, result);
            } break;
            case DataProcessingImmediateA64::LogicalImmediate::AND_64BIT: { // P.881
                auto [imm, _] = DecodeBitMasks< 64 >(N, imms, immr, true);
                static_assert(std::is_same_v< decltype(imm), std::bitset< 64 > >);
                auto operand1 = gp_registers.X(Rn);
                auto result   = operand1 & imm;
                static_assert(std::is_same_v< decltype(result), std::bitset< 64 > >);
                if (Rd == 31)
                    SP(&sp_registers, result);
                else
                    gp_registers.write(Rd, result);
            } break;
            case DataProcessingImmediateA64::LogicalImmediate::ORR_64BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingImmediateA64::LogicalImmediate::EOR_64BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingImmediateA64::LogicalImmediate::ANDS_64BIT: { // // P.885
                constexpr auto datasize = 64;

                auto [imm, _] = DecodeBitMasks< datasize >(N, imms, immr, true);
                static_assert(std::is_same_v< decltype(imm), std::bitset< datasize > >);
                auto operand1 = gp_registers.X(Rn);
                auto result   = operand1 & imm;
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                std::bitset< 4 > nzcv = concate< 2, 2 >(concate< 1, 1 >(result[63] ? 1 : 0, result == 0 ? 1 : 0), 0);
                write_nzcv(nzcv);
                if (Rd == 31) // Documentation does not state SP as an option to Rd
                    SP(&sp_registers, result);
                else
                    gp_registers.write(Rd, result);
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    auto process(DataProcessingImmediateInstruction< DataProcessingImmediateGroupA64::MOVE_WIDE_IMMEDIATE >&& instruction) {
        auto Rd    = instruction.get_Rd();
        auto imm16 = instruction.get_imm16();
        auto hw    = instruction.get_hw();

        switch (instruction.getInstructionType()) {
            case DataProcessingImmediateA64::MoveWideImmediate::MOVN_32BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingImmediateA64::MoveWideImmediate::MOVZ_32BIT: { // P.1123 + 88
                if (hw >= 2) throw undefined_behaviour {};
                constexpr auto datasize = 32;

                std::uint8_t            pos = hw << 4;
                std::bitset< datasize > result(0);
                for (auto i = pos; i < pos + 16; i++) { result[i] = static_cast< bool >((static_cast< std::uint16_t >(1) << (i - pos)) & imm16); }
                gp_registers.write(Rd, result);
            } break;
            case DataProcessingImmediateA64::MoveWideImmediate::MOVK_32BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingImmediateA64::MoveWideImmediate::MOVN_64BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingImmediateA64::MoveWideImmediate::MOVZ_64BIT: { // P.1123 + 88
                constexpr auto datasize = 64;

                std::uint8_t            pos = hw << 4;
                std::bitset< datasize > result(0);
                for (auto i = pos; i < pos + 16; i++) { result[i] = static_cast< bool >((static_cast< std::uint16_t >(1) << (i - pos)) & imm16); }
                gp_registers.write(Rd, result);
            } break;
            case DataProcessingImmediateA64::MoveWideImmediate::MOVK_64BIT: {
                throw not_implemented_feature {};
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    auto process(DataProcessingImmediateInstruction< DataProcessingImmediateGroupA64::BITFIELD >&& instruction) {
        auto             Rd   = instruction.get_Rd();
        auto             Rn   = instruction.get_Rn();
        std::bitset< 6 > imms = instruction.get_imms();
        std::bitset< 6 > immr = instruction.get_immr();
        auto             N    = instruction.get_N();

        switch (instruction.getInstructionType()) {
            case DataProcessingImmediateA64::Bitfield::SBFM_32BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingImmediateA64::Bitfield::BFM_32BIT: { // P.822 + 88
                if (N != 0 || immr[5] != 0 || imms[5] != 0) throw undefined_behaviour {};
                constexpr auto datasize = 32;
                auto           R        = instruction.get_immr();

                auto [wmask, tmask] = DecodeBitMasks< datasize >(N, imms, immr, false);
                static_assert(std::is_same_v< decltype(wmask), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(tmask), std::bitset< datasize > >);

                std::bitset< datasize > dst = gp_registers.W(Rd);
                std::bitset< datasize > src = gp_registers.W(Rn);

                std::bitset< datasize > bot = (dst & ~wmask) | (ROR< datasize, datasize - 1 >(src, R) & wmask);

                gp_registers.write(Rd, (dst & ~tmask) | (bot & tmask));
            } break;
            case DataProcessingImmediateA64::Bitfield::UBFM_32BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingImmediateA64::Bitfield::SBFM_64BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingImmediateA64::Bitfield::BFM_64BIT: { // P.822 + 88
                if (N != 1) throw undefined_behaviour {};
                constexpr auto datasize = 64;
                auto           R        = instruction.get_immr();

                auto [wmask, tmask] = DecodeBitMasks< datasize >(N, imms, immr, false);
                static_assert(std::is_same_v< decltype(wmask), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(tmask), std::bitset< datasize > >);

                std::bitset< datasize > dst = gp_registers.X(Rd);
                std::bitset< datasize > src = gp_registers.X(Rn);

                std::bitset< datasize > bot = (dst & ~wmask) | (ROR< datasize, datasize - 1 >(src, R) & wmask);

                gp_registers.write(Rd, (dst & ~tmask) | (bot & tmask));
            } break;
            case DataProcessingImmediateA64::Bitfield::UBFM_64BIT: {
                throw not_implemented_feature {};
            } break;
            default:
                throw undefined_instruction {};
                break;
        }
    }
    auto process(DataProcessingImmediateInstruction< DataProcessingImmediateGroupA64::EXTRACT >&& instruction) {}

    auto process(Branch_Exception_SystemInstruction< Branch_Exception_SystemGroupA64::CONDITIONAL_BRANCHING >&& instruction) {
        auto cond  = instruction.get_cond();
        auto imm19 = instruction.get_imm19();

        switch (instruction.getInstructionType()) {
            case Branch_Exception_SystemA64::ConditionalBranching::BCond: { // P.816 + 88
                std::bitset< 64 > offset = (SignExtend< 64 >(concate< 19, 2 >(imm19, 0))).to_ullong() / sizeof(decltype(prog_block)::type);
                const auto        pc     = gp_registers.PC() - 1;
                if (ConditionHolds(cond, &sp_registers.NZCV)) BranchTo(&sp_registers, gp_registers.PC(), pc + offset.to_ullong(), BranchType::DIR);
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    auto process(Branch_Exception_SystemInstruction< Branch_Exception_SystemGroupA64::EXCEPTION_GENERATION >&& instruction) {
        auto imm16 = instruction.get_imm16();

        switch (instruction.getInstructionType()) {
            case Branch_Exception_SystemA64::ExceptionGeneration::SVC: {
                throw not_implemented_feature {};
            } break;
            case Branch_Exception_SystemA64::ExceptionGeneration::HVC: {
                throw not_implemented_feature {};
            } break;
            case Branch_Exception_SystemA64::ExceptionGeneration::SMC: {
                throw not_implemented_feature {};
            } break;
            case Branch_Exception_SystemA64::ExceptionGeneration::BRK: { // P.837 + 88
                // if (AArch64::HaveBTIExt()) always false
                // SetBTypeCompatible(TRUE);
                SoftwareBreakpoint(&sp_registers, gp_registers.PC(), imm16);
            } break;
            case Branch_Exception_SystemA64::ExceptionGeneration::HLT: {
                throw not_implemented_feature {};
            } break;
            case Branch_Exception_SystemA64::ExceptionGeneration::DCPS1: {
                throw not_implemented_feature {};
            } break;
            case Branch_Exception_SystemA64::ExceptionGeneration::DCPS2: {
                throw not_implemented_feature {};
            } break;
            case Branch_Exception_SystemA64::ExceptionGeneration::DCPS3: {
                throw not_implemented_feature {};
            } break;
        }
    }
    auto process(Branch_Exception_SystemInstruction< Branch_Exception_SystemGroupA64::PSTATE >&& instruction) {

        switch (instruction.getInstructionType()) {
            case Branch_Exception_SystemA64::PState::CFINV: {
                if (!HaveFlagManipulateExt()) throw undefined_instruction {};

                sp_registers.NZCV.C(static_cast< std::uint64_t >(sp_registers.NZCV.C()));
            } break;
            case Branch_Exception_SystemA64::PState::XAFLAG: {

            } break;
            case Branch_Exception_SystemA64::PState::AXFLAG: {
                if (!HaveFlagManipulateExt()) throw undefined_instruction {};

                bool Z = sp_registers.NZCV.Z() || sp_registers.NZCV.V();
                bool C = sp_registers.NZCV.C() && (!sp_registers.NZCV.V());

                sp_registers.NZCV.N(0);
                sp_registers.NZCV.Z(Z);
                sp_registers.NZCV.C(C);
                sp_registers.NZCV.V(0);
            } break;
            default:
                throw undefined_instruction {};
                break;
        }
    }
    auto process(Branch_Exception_SystemInstruction< Branch_Exception_SystemGroupA64::SYSTEM_INSTRUCTIONS >&& instruction) {
        auto Rt  = instruction.get_Rt();
        auto op2 = instruction.get_op2();
        auto CRm = instruction.get_CRm();
        auto CRn = instruction.get_CRn();
        auto op1 = instruction.get_op1();
        auto L   = instruction.get_L();

        switch (instruction.getInstructionType()) {
            case Branch_Exception_SystemA64::SystemInstruction::SYS: { // P.1465
                AArch64::CheckSystemAccess(&sp_registers, 0b01, op1, CRn, CRm, op2, Rt, L);

                std::uint32_t sys_op1 = static_cast< std::uint32_t >(op1);
                std::uint32_t sys_op2 = static_cast< std::uint32_t >(op2);
                std::uint32_t sys_crn = static_cast< std::uint32_t >(CRn);
                std::uint32_t sys_crm = static_cast< std::uint32_t >(CRm);

                SysInstr(1, sys_op1, sys_crn, sys_crm, sys_op2, gp_registers.X(Rt));

            } break;
            case Branch_Exception_SystemA64::SystemInstruction::SYSL: {
                throw not_implemented_feature {};
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    auto process(Branch_Exception_SystemInstruction< Branch_Exception_SystemGroupA64::UNCONDITIONAL_BRANCH_REGISTER >&& instruction) {
        auto Rn = instruction.get_Rn();
        auto Rm = instruction.get_Rm();
        auto A  = instruction.get_A();
        auto M  = instruction.get_M();
        auto Z  = instruction.get_Z();

        switch (instruction.getInstructionType()) {
            case Branch_Exception_SystemA64::UnconditionalBranchRegister::BR: { // P.834 + 88
                auto target = gp_registers.X(Rn);
                static_assert(std::is_same_v< decltype(target), std::bitset< 64 > >);

                BranchTo(&sp_registers, gp_registers.PC(), target.to_ullong(), BranchType::INDCALL);
            } break;
            case Branch_Exception_SystemA64::UnconditionalBranchRegister::BRAAZ: {
                throw not_implemented_feature {};
            } break;
            case Branch_Exception_SystemA64::UnconditionalBranchRegister::BRABZ: {
                throw not_implemented_feature {};
            } break;
            case Branch_Exception_SystemA64::UnconditionalBranchRegister::BLR: { // P.831 + 88
                auto target = gp_registers.X(Rn);
                static_assert(std::is_same_v< decltype(target), std::bitset< 64 > >);

                const auto pc = gp_registers.PC();
                gp_registers.write(30, pc + 4);
                BranchTo(&sp_registers, gp_registers.PC(), target.to_ullong(), BranchType::INDCALL);
            } break;
            case Branch_Exception_SystemA64::UnconditionalBranchRegister::BLRAAZ: {
                throw not_implemented_feature {};
            } break;
            case Branch_Exception_SystemA64::UnconditionalBranchRegister::BLRABZ: {
                throw not_implemented_feature {};
            } break;
            case Branch_Exception_SystemA64::UnconditionalBranchRegister::RET: {
                auto target = gp_registers.X(Rn);
                static_assert(std::is_same_v< decltype(target), std::bitset< 64 > >);
                BranchTo(&sp_registers, gp_registers.PC(), target.to_ullong(), BranchType::RET);
            } break;
            case Branch_Exception_SystemA64::UnconditionalBranchRegister::RETAA: {
                throw not_implemented_feature {};
            } break;
            case Branch_Exception_SystemA64::UnconditionalBranchRegister::RETAB: {
                throw not_implemented_feature {};
            } break;
            case Branch_Exception_SystemA64::UnconditionalBranchRegister::ERET: {
                throw not_implemented_feature {};
            } break;
            case Branch_Exception_SystemA64::UnconditionalBranchRegister::ERETAA: {
                throw not_implemented_feature {};
            } break;
            case Branch_Exception_SystemA64::UnconditionalBranchRegister::ERETAB: {
                throw not_implemented_feature {};
            } break;
            case Branch_Exception_SystemA64::UnconditionalBranchRegister::DRPS: {
                throw not_implemented_feature {};
            } break;
            case Branch_Exception_SystemA64::UnconditionalBranchRegister::BRAA: {
                throw not_implemented_feature {};
            } break;
            case Branch_Exception_SystemA64::UnconditionalBranchRegister::BRAB: {
                throw not_implemented_feature {};
            } break;
            case Branch_Exception_SystemA64::UnconditionalBranchRegister::BLRAA: {
                throw not_implemented_feature {};
            } break;
            case Branch_Exception_SystemA64::UnconditionalBranchRegister::BLRAB: {
                throw not_implemented_feature {};
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    auto process(Branch_Exception_SystemInstruction< Branch_Exception_SystemGroupA64::UNCONDITIONAL_BRANCH_IMMEDIATE >&& instruction) {
        auto imm26 = instruction.get_imm26();

        switch (instruction.getInstructionType()) {
            case Branch_Exception_SystemA64::UnconditionalBranchImmediate::B: { // P.817 + 88
                std::bitset< 64 > offset = (SignExtend< 64 >(concate< 26, 2 >(imm26, 0)).to_ullong() / sizeof(decltype(prog_block)::type));
                const auto        pc     = gp_registers.PC();
                BranchTo(&sp_registers, gp_registers.PC(), pc + offset.to_ullong(), BranchType::DIR);
            } break;
            case Branch_Exception_SystemA64::UnconditionalBranchImmediate::BL: { // P.830 + 88
                std::bitset< 64 > offset = (SignExtend< 64 >(concate< 26, 2 >(imm26, 0)).to_ullong() / sizeof(decltype(prog_block)::type));
                const auto        pc     = gp_registers.PC();
                gp_registers.write(30, pc + 4);
                BranchTo(&sp_registers, gp_registers.PC(), pc + offset.to_ullong(), BranchType::DIRCALL);
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    auto process(Branch_Exception_SystemInstruction< Branch_Exception_SystemGroupA64::COMPARE_AND_BRANCH_IMMEDIATE >&& instruction) {
        auto Rt    = instruction.get_Rt();
        auto imm19 = instruction.get_imm19();

        switch (instruction.getInstructionType()) {
            case Branch_Exception_SystemA64::CompareAndBranchImmediate::CBZ_32BIT: { // P.851 + 88
                constexpr auto datasize = 32;

                auto offset = SignExtend< 64 >(concate< 19, 2 >(imm19, 0));

                auto operand1 = gp_registers.W(Rt);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);

                const auto pc = gp_registers.PC();
                if (operand1 == 0) { BranchTo(&sp_registers, gp_registers.PC(), pc + offset.to_ullong(), BranchType::DIR); }
            } break;
            case Branch_Exception_SystemA64::CompareAndBranchImmediate::CBNZ_32BIT: { // P.850 + 88
                constexpr auto datasize = 32;

                auto offset = SignExtend< 64 >(concate< 19, 2 >(imm19, 0));

                auto operand1 = gp_registers.W(Rt);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);

                const auto pc = gp_registers.PC();
                if (operand1 != 0) { BranchTo(&sp_registers, gp_registers.PC(), pc + offset.to_ullong(), BranchType::DIR); }
            } break;
            case Branch_Exception_SystemA64::CompareAndBranchImmediate::CBZ_64BIT: { // P.851 + 88
                constexpr auto datasize = 64;

                auto offset = SignExtend< 64 >(concate< 19, 2 >(imm19, 0));

                auto operand1 = gp_registers.X(Rt);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);

                const auto pc = gp_registers.PC();
                if (operand1 == 0) { BranchTo(&sp_registers, gp_registers.PC(), pc + offset.to_ullong(), BranchType::DIR); }
            } break;
            case Branch_Exception_SystemA64::CompareAndBranchImmediate::CBNZ_64BIT: { // P.850 + 88
                constexpr auto datasize = 64;

                auto offset = SignExtend< 64 >(concate< 19, 2 >(imm19, 0));

                auto operand1 = gp_registers.X(Rt);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);

                const auto pc = gp_registers.PC();
                if (operand1 != 0) { BranchTo(&sp_registers, gp_registers.PC(), pc + offset.to_ullong(), BranchType::DIR); }
            } break;
            default:
                throw undefined_instruction {};
                break;
        }
    }

    auto process(LoadStoreInstruction< LoadStoreGroupA64::LOAD_STORE_REGISTER_IMMEDIATE_POST_INDEXED >&& instruction) {
        auto Rt   = instruction.get_Rt();
        auto Rn   = instruction.get_Rn();
        auto imm9 = instruction.get_imm9();
        auto size = instruction.get_size();
        (void)Rt;
        (void)Rn;
        (void)imm9;
        (void)size;
        switch (instruction.getInstructionType()) {
            case LoadStoreA64::LoadStoreRegisterImmediatePostIndexed::STRBi: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterImmediatePostIndexed::LDRBi: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterImmediatePostIndexed::LDRSBi_64BIT: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterImmediatePostIndexed::LDRSBi_32BIT: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterImmediatePostIndexed::STRi_8BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterImmediatePostIndexed::LDRi_8BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterImmediatePostIndexed::STRi_128BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterImmediatePostIndexed::LDRi_128BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterImmediatePostIndexed::STRHi: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterImmediatePostIndexed::LDRHi: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterImmediatePostIndexed::LDRSHi_64BIT: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterImmediatePostIndexed::LDRSHi_32BIT: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterImmediatePostIndexed::STRi_16BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterImmediatePostIndexed::LDRi_16BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterImmediatePostIndexed::STRi_32BIT: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterImmediatePostIndexed::LDRi_32BIT: {
                throw not_implemented_feature {};
                /* incomplete if (size == 0b10) throw undefined_behaviour {};
                 bool wback     = true;
                 bool postindex = true;

                 std::bitset< 64 > offset = SignExtend< 64 >(std::bitset< 9 > { imm9 });*/
            } break;
            case LoadStoreA64::LoadStoreRegisterImmediatePostIndexed::LDRSWi: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterImmediatePostIndexed::STRi_32BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterImmediatePostIndexed::LDRi_32BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterImmediatePostIndexed::STRi_64BIT: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterImmediatePostIndexed::LDRi_64BIT: {
                throw not_implemented_feature {};
                // if (size == 0b11) throw undefined_behaviour {};
            } break;
            case LoadStoreA64::LoadStoreRegisterImmediatePostIndexed::STRi_64BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterImmediatePostIndexed::LDRi_64BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    auto process(LoadStoreInstruction< LoadStoreGroupA64::LOAD_STORE_REGISTER_UNSIGNED_IMMEDIATE >&& instruction) {
        auto Rt    = instruction.get_Rt();
        auto Rn    = instruction.get_Rn();
        auto imm12 = instruction.get_imm12();
        auto size  = instruction.get_size();

        switch (instruction.getInstructionType()) {
            case LoadStoreA64::LoadStoreRegisterUnsignedImmediate::STRBi: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterUnsignedImmediate::LDRBi: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterUnsignedImmediate::LDRSBi_64BIT: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterUnsignedImmediate::LDRSBi_32BIT: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterUnsignedImmediate::STRi_8BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterUnsignedImmediate::LDRi_8BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterUnsignedImmediate::STRi_128BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterUnsignedImmediate::LDRi_128BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterUnsignedImmediate::STRHi: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterUnsignedImmediate::LDRHi: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterUnsignedImmediate::LDRSHi_64BIT: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterUnsignedImmediate::LDRSHi_32BIT: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterUnsignedImmediate::STRi_16BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterUnsignedImmediate::LDRi_16BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterUnsignedImmediate::STRi_32BIT: { // P.1261 + 88
                /**/
                if (size != 0b10) throw undefined_behaviour {};
                auto wback     = false;
                auto postindex = false;

                auto offset = LSL< 64 >(ZeroExtend< 64 >(std::bitset< 12 > { imm12 }), size);
                static_assert(std::is_same_v< decltype(offset), std::bitset< 64 > >);

                constexpr std::uint32_t datasize    = 8 << 0b10;
                bool                    tag_checked = wback || (Rn != 31);

                if (Query< SystemSettings, Features, Features::FEAT_MTE >::result()) {
                    (void)tag_checked;
                    // SetTagCheckedInstruction(tag_checked);
                }
                std::bitset< 64 >       address;
                std::bitset< datasize > data;

                bool rt_unknown = false;

                if (wback && Rn == Rt && Rn != 31) {
                    /*auto c = ALU::ConstrainUnpredictable();
                    if (c != Constraint::NONE && c != Constraint::UNKNOWN && c != Constraint::UNDEF && c != Constraint::NOP) throw undefined_behaviour {};
                    switch (c) {
                        case Constraint::NONE: {
                            rt_unknown = false;
                        } break;
                        case Constraint::UNKNOWN: {
                            rt_unknown = true;
                        } break;
                        case Constraint::UNDEF: {
                            throw undefined_behaviour {};
                        } break;
                        case Constraint::NOP: {
                            EndOfInstruction(); // return
                        } break;
                    }*/
                }

                if (Rn == 31) {
                    // checkSPAlignment();
                    address = SP< 64 >(&sp_registers);
                } else {
                    address = gp_registers.X(Rn);
                }

                if (!postindex) address = address.to_ullong() + offset.to_ullong();

                if (rt_unknown) {
                    throw undefined_behaviour {};
                    // data = std::bitset< datasize >(0);
                } else
                    data = gp_registers.W(Rt);
                // ALU::Mem[address, datasize / 8, AccType::NORMAL] = data;
                ram_block.write_loc(address.to_ullong(), data.to_ullong());

                if (wback) {
                    if (postindex)
                        address = address.to_ullong() + offset.to_ullong();
                    else if (Rn == 31)
                        SP(&sp_registers, address);
                    else
                        gp_registers.write(Rn, address);
                }
            } break;
            case LoadStoreA64::LoadStoreRegisterUnsignedImmediate::LDRi_32BIT: { // // P.997 + 88
                if (size != 0b10) throw undefined_behaviour {};
                bool wback     = false;
                bool postindex = false;

                auto offset = LSL< 64 >(ZeroExtend< 64 >(std::bitset< 12 > { imm12 }), size);
                static_assert(std::is_same_v< decltype(offset), std::bitset< 64 > >);

                constexpr std::uint32_t datasize = 8 << 0b10;
                if (Query< SystemSettings, Features, Features::FEAT_MTE >::result()) {
                    // SetTagCheckedInstruction(tag_checked);
                }
                std::bitset< 64 >       address;
                std::bitset< datasize > data;

                bool wb_unknown = false;

                if (wback && Rn == Rt && Rn != 31) {
                    /*auto c = ALU::ConstrainUnpredictable();
                    if (c != Constraint::NONE && c != Constraint::UNKNOWN && c != Constraint::UNDEF && c != Constraint::NOP) throw undefined_behaviour {};
                    switch (c) {
                    case Constraint::NONE: {
                    rt_unknown = false;
                    } break;
                    case Constraint::UNKNOWN: {
                    rt_unknown = true;
                    } break;
                    case Constraint::UNDEF: {
                    throw undefined_behaviour {};
                    } break;
                    case Constraint::NOP: {
                    EndOfInstruction(); // return
                    } break;
                    }*/
                }

                if (Rn == 31) {
                    // checkSPAlignment();
                    address = SP< 64 >(&sp_registers);
                } else
                    address = gp_registers.X(Rn);

                if (!postindex) address = address.to_ullong() + offset.to_ullong();

                // data = ALU::Mem[address, datasize / 8, AccType::NORMAL];
                data = ram_block[address.to_ullong()];
                gp_registers.write(Rt, data);

                if (wback) {
                    if (wb_unknown)
                        throw undefined_behaviour {};
                    else if (postindex)
                        address = address.to_ullong() + offset.to_ullong();

                    if (Rn == 31)
                        SP(&sp_registers, address);
                    else
                        gp_registers.write(Rn, address);
                }
            } break;
            case LoadStoreA64::LoadStoreRegisterUnsignedImmediate::LDRSWi: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterUnsignedImmediate::STRi_32BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterUnsignedImmediate::LDRi_32BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterUnsignedImmediate::STRi_64BIT: { // P.1261 + 88
                /**/
                if (size != 0b11) throw undefined_behaviour {};
                auto wback     = false;
                auto postindex = false;

                auto offset = LSL< 64 >(ZeroExtend< 64 >(std::bitset< 12 > { imm12 }), size);
                static_assert(std::is_same_v< decltype(offset), std::bitset< 64 > >);

                constexpr std::uint32_t datasize    = 8 << 0b11;
                bool                    tag_checked = wback || (Rn != 31);

                if (Query< SystemSettings, Features, Features::FEAT_MTE >::result()) {
                    (void)tag_checked;
                    // SetTagCheckedInstruction(tag_checked);
                }
                std::bitset< 64 >       address;
                std::bitset< datasize > data;

                bool rt_unknown = false;

                if (wback && Rn == Rt && Rn != 31) {
                    /*auto c = ALU::ConstrainUnpredictable();
                    if (c != Constraint::NONE && c != Constraint::UNKNOWN && c != Constraint::UNDEF && c != Constraint::NOP) throw undefined_behaviour {};
                    switch (c) {
                    case Constraint::NONE: {
                    rt_unknown = false;
                    } break;
                    case Constraint::UNKNOWN: {
                    rt_unknown = true;
                    } break;
                    case Constraint::UNDEF: {
                    throw undefined_behaviour {};
                    } break;
                    case Constraint::NOP: {
                    EndOfInstruction(); // return
                    } break;
                    }*/
                }

                if (Rn == 31) {
                    // checkSPAlignment();
                    address = SP< 64 >(&sp_registers);
                } else
                    address = gp_registers.X(Rn);

                if (!postindex) address = address.to_ullong() + offset.to_ullong();

                if (rt_unknown)
                    data = std::bitset< datasize >(0);
                else
                    data = gp_registers.X(Rt);
                // ALU::Mem[address, datasize / 8, AccType::NORMAL] = data;
                ram_block.write_loc(address.to_ullong(), data.to_ullong());

                if (wback) {
                    if (postindex)
                        address = address.to_ullong() + offset.to_ullong();
                    else if (Rn == 31)
                        SP(&sp_registers, address);
                    else
                        gp_registers.write(Rn, address);
                }
            } break;
            case LoadStoreA64::LoadStoreRegisterUnsignedImmediate::LDRi_64BIT: { // P.997 + 88
                if (size != 0b11) throw undefined_behaviour {};
                bool wback     = false;
                bool postindex = false;

                auto offset = LSL< 64 >(ZeroExtend< 64 >(std::bitset< 12 > { imm12 }), size);
                static_assert(std::is_same_v< decltype(offset), std::bitset< 64 > >);

                constexpr std::uint32_t datasize = 8 << 0b11;
                if (Query< SystemSettings, Features, Features::FEAT_MTE >::result()) {
                    // SetTagCheckedInstruction(tag_checked);
                }
                std::bitset< 64 >       address;
                std::bitset< datasize > data;

                bool wb_unknown = false;

                if (wback && Rn == Rt && Rn != 31) {
                    /*auto c = ALU::ConstrainUnpredictable();
                    if (c != Constraint::NONE && c != Constraint::UNKNOWN && c != Constraint::UNDEF && c != Constraint::NOP) throw undefined_behaviour {};
                    switch (c) {
                    case Constraint::NONE: {
                    rt_unknown = false;
                    } break;
                    case Constraint::UNKNOWN: {
                    rt_unknown = true;
                    } break;
                    case Constraint::UNDEF: {
                    throw undefined_behaviour {};
                    } break;
                    case Constraint::NOP: {
                    EndOfInstruction(); // return
                    } break;
                    }*/
                }

                if (Rn == 31) {
                    // checkSPAlignment();
                    address = SP< 64 >(&sp_registers);
                } else
                    address = gp_registers.X(Rn);

                if (!postindex) address = address.to_ullong() + offset.to_ullong();

                // data = ALU::Mem[address, datasize / 8, AccType::NORMAL];
                data = ram_block[address.to_ullong()];
                gp_registers.write(Rt, data);

                if (wback) {
                    if (wb_unknown)
                        throw undefined_behaviour {};
                    else if (postindex)
                        address = address.to_ullong() + offset.to_ullong();

                    if (Rn == 31)
                        SP(&sp_registers, address);
                    else
                        gp_registers.write(Rn, address);
                }
            } break;
            case LoadStoreA64::LoadStoreRegisterUnsignedImmediate::PRFMi: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterUnsignedImmediate::STRi_64BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            case LoadStoreA64::LoadStoreRegisterUnsignedImmediate::LDRi_64BIT_SIMD: {
                throw not_implemented_feature {};
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }

    auto process(DataProcessingRegisterInstruction< DataProcessingRegisterGroupA64::DATA_PROCESSING_TWO_SOURCE >&& instruction) {
        auto Rd  = instruction.get_Rd();
        auto Rn  = instruction.get_Rn();
        auto Rm  = instruction.get_Rm();
        auto op2 = instruction.get_op2();

        switch (instruction.getInstructionType()) {
            case DataProcessingRegisterA64::DataProcessingTwoSource::UDIV_32BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingTwoSource::SDIV_32BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingTwoSource::LSLV_32BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingTwoSource::LSRV_32BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingTwoSource::ASRV_32BIT: { // P.889
                constexpr auto datasize   = 32;
                auto           shift_type = DecodeShift(op2);

                auto operand2 = gp_registers.W(Rm);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);

                auto result =
                    ShiftReg< datasize, datasize - 1 >(&gp_registers, Rn, shift_type, static_cast< std::uint8_t >(std::uint64_t { operand2.to_ulong() } % datasize));
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                gp_registers.write(Rd, result);

            } break;
            case DataProcessingRegisterA64::DataProcessingTwoSource::RORV_32BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingTwoSource::CRC32B: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingTwoSource::CRC32H: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingTwoSource::CRC32W: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingTwoSource::CRC32CB: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingTwoSource::CRC32CH: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingTwoSource::CRC32CW: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingTwoSource::SUBP: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingTwoSource::UDIV_64BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingTwoSource::SDIV_64BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingTwoSource::IRG: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingTwoSource::GMI: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingTwoSource::LSLV_64BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingTwoSource::LSRV_64BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingTwoSource::ASRV_64BIT: { // P.889
                constexpr auto datasize   = 64;
                auto           shift_type = DecodeShift(op2);

                auto operand2 = gp_registers.X(Rm);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);

                auto result =
                    ShiftReg< datasize, datasize - 1 >(&gp_registers, Rn, shift_type, static_cast< std::uint8_t >(std::uint64_t { operand2.to_ulong() } % datasize));
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                gp_registers.write(Rd, result);
            } break;
            case DataProcessingRegisterA64::DataProcessingTwoSource::RORV_64BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingTwoSource::PACGA: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingTwoSource::CRC32X: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingTwoSource::CRC32CX: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingTwoSource::SUBPS: {
                throw not_implemented_feature {};
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    auto process(DataProcessingRegisterInstruction< DataProcessingRegisterGroupA64::DATA_PROCESSING_ONE_SOURCE >&& instruction) {
        auto Rd = instruction.get_Rd();
        auto Rn = instruction.get_Rn();
        auto Z  = instruction.get_Z();

        switch (instruction.getInstructionType()) {
            case DataProcessingRegisterA64::DataProcessingOneSource::RBIT_32BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingOneSource::REV16_32BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingOneSource::REV_32BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingOneSource::CLZ_32BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingOneSource::CLS_32BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingOneSource::RBIT_64BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingOneSource::REV16_64BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingOneSource::REV32: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingOneSource::REV_64BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingOneSource::CLZ_64BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingOneSource::CLS_64BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingOneSource::PACIA: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingOneSource::PACIB: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingOneSource::PACDA: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingOneSource::PACDB: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingOneSource::AUTIA: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingOneSource::AUTIB: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingOneSource::AUTDA: { // P.897
                auto source_is_sp = false;
                if (!HavePACExt()) throw undefined_instruction {};

                // Will always throw since instruction is ARMv8p3
                /*if (Rn == 31) source_is_sp = true;

                if (AArch64::HavePACExt()) {
                    if (source_is_sp) {
                        gp_registers.write(Rd, AArch64::AuthDA(gp_registers.X(Rd), ALU::SP<64>(&sp_registers), false));
                    }
                    else {
                        gp_registers.write(Rd, AArch64::AuthDA(gp_registers.X(Rd), gp_registers.X(Rn), false));
                    }
                }*/
            } break;
            case DataProcessingRegisterA64::DataProcessingOneSource::AUTDB: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingOneSource::PACIZA: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingOneSource::PACIZB: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingOneSource::PACDZA: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingOneSource::PACDZB: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingOneSource::AUTIZA: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingOneSource::AUTIZB: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingOneSource::AUTDZA: { // P.897
                if (!HavePACExt()) throw undefined_instruction {};
                // Will always throw since instruction is ARMv8p3
                /*auto source_is_sp = false;

                if (AArch64::HavePACExt()) {
                if (source_is_sp) {
                gp_registers.write(Rd, AArch64::AuthDA(gp_registers.X(Rd), ALU::SP<64>(&sp_registers), false));
                }
                else {
                gp_registers.write(Rd, AArch64::AuthDA(gp_registers.X(Rd), gp_registers.X(Rn), false));
                }
                }*/
            } break;
            case DataProcessingRegisterA64::DataProcessingOneSource::AUTDZB: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingOneSource::XPACI: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::DataProcessingOneSource::XPACD: {
                throw not_implemented_feature {};
            } break;
            default:
                throw undefined_instruction {};
                break;
        }
    }
    auto process(DataProcessingRegisterInstruction< DataProcessingRegisterGroupA64::ADD_SUBTRACT_CARRY >&& instruction) {
        auto Rd = instruction.get_Rd();
        auto Rn = instruction.get_Rn();
        auto Rm = instruction.get_Rm();
        switch (instruction.getInstructionType()) {
            case DataProcessingRegisterA64::AddSubtractCarry::ADC_32BIT: { // P.772 + 88
                constexpr auto datasize = 32;
                auto [result, _]        = AddWithCarry(gp_registers.W(Rn), gp_registers.W(Rm), static_cast< std::uint32_t >(sp_registers.NZCV.C()));
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                gp_registers.write(Rd, result);
            } break;
            case DataProcessingRegisterA64::AddSubtractCarry::ADCS_32BIT: { // P.774 + 88
                constexpr auto datasize = 32;
                auto [result, nzcv]     = AddWithCarry(gp_registers.W(Rn), gp_registers.W(Rm), static_cast< std::uint32_t >(sp_registers.NZCV.C()));
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(nzcv), std::uint8_t >);
                write_nzcv(nzcv);
                gp_registers.write(Rd, result);
            } break;
            case DataProcessingRegisterA64::AddSubtractCarry::SBC_32BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::AddSubtractCarry::SBCS_32BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::AddSubtractCarry::ADC_64BIT: { // P.772 + 88
                constexpr auto datasize = 64;
                auto [result, _]        = AddWithCarry(gp_registers.X(Rn), gp_registers.X(Rm), static_cast< std::uint64_t >(sp_registers.NZCV.C()));
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                gp_registers.write(Rd, result);
            } break;
            case DataProcessingRegisterA64::AddSubtractCarry::ADCS_64BIT: { // P.774 + 88
                constexpr auto datasize = 64;
                auto [result, nzcv]     = AddWithCarry(gp_registers.X(Rn), gp_registers.X(Rm), static_cast< std::uint64_t >(sp_registers.NZCV.C()));
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(nzcv), std::uint8_t >);
                write_nzcv(nzcv);
                gp_registers.write(Rd, result);
            } break;
            case DataProcessingRegisterA64::AddSubtractCarry::SBC_64BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::AddSubtractCarry::SBCS_64BIT: {
                throw not_implemented_feature {};
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    auto process(DataProcessingRegisterInstruction< DataProcessingRegisterGroupA64::ADD_SUBTRACT_EXTENDED_REGISTER >&& instruction) {
        auto Rd     = instruction.get_Rd();
        auto Rn     = instruction.get_Rn();
        auto Rm     = instruction.get_Rm();
        auto option = instruction.get_option();
        auto imm3   = instruction.get_imm3();
        switch (instruction.getInstructionType()) {
            case DataProcessingRegisterA64::AddSubtractExtendedRegister::ADD_32BIT_EXTENDED: { // P.776 + 88
                if (imm3 > 4) throw undefined_behaviour {};
                constexpr auto datasize = 32;
                auto           operand1 = Rn == 31 ? SP< datasize >(&sp_registers) : gp_registers.W(Rn);
                auto           operand2 = ExtendReg< datasize >(&gp_registers, Rm, static_cast< ExtendType >(option), imm3);
                auto [result, _]        = AddWithCarry(operand1, operand2, static_cast< std::uint8_t >(0));
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                if (Rn == 31)
                    SP(&sp_registers, result);
                else
                    gp_registers.write(Rd, result);
            } break;
            case DataProcessingRegisterA64::AddSubtractExtendedRegister::ADDS_32BIT_EXTENDED: { // P.784 + 88
                if (imm3 > 4) throw undefined_behaviour {};
                if (imm3 > 4) throw undefined_behaviour {};
                constexpr auto datasize = 32;
                auto           operand1 = Rn == 31 ? SP< datasize >(&sp_registers) : gp_registers.W(Rn);
                auto           operand2 = ExtendReg< datasize >(&gp_registers, Rm, static_cast< ExtendType >(option), imm3);
                auto [result, nzcv]     = AddWithCarry(operand1, operand2, static_cast< std::uint8_t >(0));
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(nzcv), std::uint8_t >);
                write_nzcv(nzcv);
                if (Rn == 31)
                    SP(&sp_registers, result);
                else
                    gp_registers.write(Rd, result);
            } break;
            case DataProcessingRegisterA64::AddSubtractExtendedRegister::SUB_32BIT_EXTENDED: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::AddSubtractExtendedRegister::SUBS_32BIT_EXTENDED: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::AddSubtractExtendedRegister::ADD_64BIT_EXTENDED: { // P.776 + 88
                if (imm3 > 4) throw undefined_behaviour {};
                constexpr auto datasize = 64;
                auto           operand1 = Rn == 31 ? SP< datasize >(&sp_registers) : gp_registers.X(Rn);
                auto           operand2 = ExtendReg< datasize >(&gp_registers, Rm, static_cast< ExtendType >(option), imm3);
                auto [result, _]        = AddWithCarry(operand1, operand2, static_cast< std::uint8_t >(0));
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                if (Rn == 31)
                    SP(&sp_registers, result);
                else
                    gp_registers.write(Rd, result);
            } break;
            case DataProcessingRegisterA64::AddSubtractExtendedRegister::ADDS_64BIT_EXTENDED: { // P.784 + 88
                if (imm3 > 4) throw undefined_behaviour {};
                constexpr auto datasize = 64;
                auto           operand1 = Rn == 31 ? SP< datasize >(&sp_registers) : gp_registers.X(Rn);
                auto           operand2 = ExtendReg< datasize >(&gp_registers, Rm, static_cast< ExtendType >(option), imm3);
                auto [result, nzcv]     = AddWithCarry(operand1, operand2, static_cast< std::uint8_t >(0));
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(nzcv), std::uint8_t >);
                write_nzcv(nzcv);
                if (Rn == 31)
                    SP(&sp_registers, result);
                else
                    gp_registers.write(Rd, result);
            } break;
            case DataProcessingRegisterA64::AddSubtractExtendedRegister::SUB_64BIT_EXTENDED: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::AddSubtractExtendedRegister::SUBS_64BIT_EXTENDED: {
                throw not_implemented_feature {};
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    auto process(DataProcessingRegisterInstruction< DataProcessingRegisterGroupA64::ADD_SUBTRACT_SHIFTED_REGISTER >&& instruction) {
        auto Rd    = instruction.get_Rd();
        auto Rn    = instruction.get_Rn();
        auto Rm    = instruction.get_Rm();
        auto shift = instruction.get_shift();
        auto imm6  = instruction.get_imm6();
        switch (instruction.getInstructionType()) {
            case DataProcessingRegisterA64::AddSubtractShiftedRegister::ADD_32BIT_SHIFTED: { // P.781 + 88
                if (shift == 3) throw undefined_behaviour {};
                if (static_cast< bool >(imm6 & 0b100000) == true) throw undefined_behaviour {};
                constexpr auto datasize   = 32;
                auto           shift_type = DecodeShift(shift);

                auto operand1 = gp_registers.W(Rn);
                auto operand2 = ShiftReg< datasize, datasize - 1 >(&gp_registers, Rm, shift_type, imm6);

                auto [result, _] = AddWithCarry(operand1, operand2, 0);

                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                gp_registers.write(Rd, result);
            } break;
            case DataProcessingRegisterA64::AddSubtractShiftedRegister::ADDS_32BIT_SHIFTED: {
                if (shift == 3) throw undefined_behaviour {};
                if (static_cast< bool >(imm6 & 0b100000) == true) throw undefined_behaviour {};
                constexpr auto datasize   = 32;
                auto           shift_type = DecodeShift(shift);

                auto operand1 = gp_registers.W(Rn);
                auto operand2 = ShiftReg< datasize, datasize - 1 >(&gp_registers, Rm, shift_type, imm6);

                auto [result, nzcv] = AddWithCarry(operand1, operand2, 0);

                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(nzcv), std::uint8_t >);
                write_nzcv(nzcv);
                gp_registers.write(Rd, result);
            } break;
            case DataProcessingRegisterA64::AddSubtractShiftedRegister::SUB_32BIT_SHIFTED: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::AddSubtractShiftedRegister::SUBS_32BIT_SHIFTED: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::AddSubtractShiftedRegister::ADD_64BIT_SHIFTED: { // P.781 + 88
                if (shift == 3) throw undefined_behaviour {};
                constexpr auto datasize   = 64;
                auto           shift_type = DecodeShift(shift);

                auto operand1 = gp_registers.X(Rn);
                auto operand2 = ShiftReg< datasize, datasize - 1 >(&gp_registers, Rm, shift_type, imm6);

                auto [result, _] = AddWithCarry(operand1, operand2, 0);

                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
            } break;
            case DataProcessingRegisterA64::AddSubtractShiftedRegister::ADDS_64BIT_SHIFTED: {
                if (shift == 3) throw undefined_behaviour {};
                constexpr auto datasize   = 64;
                auto           shift_type = DecodeShift(shift);

                auto operand1 = gp_registers.X(Rn);
                auto operand2 = ShiftReg< datasize, datasize - 1 >(&gp_registers, Rm, shift_type, imm6);

                auto [result, nzcv] = AddWithCarry(operand1, operand2, 0);

                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(nzcv), std::uint8_t >);
                write_nzcv(nzcv);
                gp_registers.write(Rd, result);
            } break;
            case DataProcessingRegisterA64::AddSubtractShiftedRegister::SUB_64BIT_SHIFTED: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::AddSubtractShiftedRegister::SUBS_64BIT_SHIFTED: {
                throw not_implemented_feature {};
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    auto process(DataProcessingRegisterInstruction< DataProcessingRegisterGroupA64::LOGICAL_SHIFTED_REGISTER >&& instruction) {
        auto Rd    = instruction.get_Rd();
        auto Rn    = instruction.get_Rn();
        auto Rm    = instruction.get_Rm();
        auto imm6  = instruction.get_imm6();
        auto N     = instruction.get_N();
        auto shift = instruction.get_shift();

        switch (instruction.getInstructionType()) {
            case DataProcessingRegisterA64::LogicalShiftedRegister::AND_32BIT_SHIFTED: { // P.795 + 88
                if (static_cast< bool >(imm6 & 0b100000) == true) throw undefined_behaviour {};
                constexpr auto datasize   = 32;
                auto           shift_type = DecodeShift(shift);

                auto operand1 = gp_registers.W(Rn);
                auto operand2 = ShiftReg< datasize, datasize - 1 >(&gp_registers, Rm, shift_type, imm6);

                auto result = operand1 & operand2;
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                gp_registers.write(Rd, result);
            } break;
            case DataProcessingRegisterA64::LogicalShiftedRegister::BIC_32BIT_SHIFTED: { // P.826 + 88
                constexpr auto datasize = 32;
                if (imm6 >= 32) throw undefined_instruction {};

                auto shift_type   = DecodeShift(shift);
                auto shift_amount = imm6;

                auto operand1 = gp_registers.W(Rn);
                auto operand2 = ShiftReg< datasize, datasize - 1 >(&gp_registers, Rm, shift_type, shift_amount);

                operand2.flip();
                auto result = operand1 & operand2;
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                gp_registers.write(Rd, result);
            } break;
            case DataProcessingRegisterA64::LogicalShiftedRegister::ORR_32BIT_SHIFTED: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::LogicalShiftedRegister::ORN_32BIT_SHIFTED: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::LogicalShiftedRegister::EOR_32BIT_SHIFTED: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::LogicalShiftedRegister::EON_32BIT_SHIFTED: { // P.914 + 88
                constexpr auto datasize = 32;
                if (imm6 >= 32) throw undefined_instruction {};

                auto shift_type   = DecodeShift(shift);
                auto shift_amount = imm6;

                auto operand1 = gp_registers.W(Rn);
                auto operand2 = ShiftReg< datasize, datasize - 1 >(&gp_registers, Rm, shift_type, shift_amount);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);

                operand2.flip();

                auto result = operand1 | operand2;
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                gp_registers.write(Rd, result);
            } break;
            case DataProcessingRegisterA64::LogicalShiftedRegister::ANDS_32BIT_SHIFTED: { // P.799 + 88
                if (static_cast< bool >(imm6 & 0b100000) == true) throw undefined_behaviour {};
                constexpr auto datasize   = 32;
                auto           shift_type = DecodeShift(shift);

                auto operand1 = gp_registers.W(Rn);
                auto operand2 = ShiftReg< datasize, datasize - 1 >(&gp_registers, Rm, shift_type, imm6);

                auto result = operand1 & operand2;
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                std::bitset< 4 > nzcv = concate< 2, 2 >(concate< 1, 1 >(result[31] ? 1 : 0, result == 0 ? 1 : 0), 0);
                write_nzcv(nzcv);
                gp_registers.write(Rd, result);
            } break;
            case DataProcessingRegisterA64::LogicalShiftedRegister::BICS_32BIT_SHIFTED: { // P.828 + 88
                constexpr auto datasize = 32;
                if (imm6 >= 32) throw undefined_instruction {};

                auto shift_type   = DecodeShift(shift);
                auto shift_amount = imm6;

                auto operand1 = gp_registers.W(Rn);
                auto operand2 = ShiftReg< datasize, datasize - 1 >(&gp_registers, Rm, shift_type, shift_amount);

                operand2.flip();
                auto result = operand1 & operand2;
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                bool val = result[datasize - 1];
                write_nzcv(concate< 1, 3 >(val, concate< 1, 2 >(result == 0, 0)));
                gp_registers.write(Rd, result);
            } break;
            case DataProcessingRegisterA64::LogicalShiftedRegister::AND_64BIT_SHIFTED: { // P.795 + 88
                auto           shift_type = DecodeShift(shift);
                constexpr auto datasize   = 64;

                auto operand1 = gp_registers.X(Rn);
                auto operand2 = ShiftReg< datasize, datasize - 1 >(&gp_registers, Rm, shift_type, imm6);

                auto result = operand1 & operand2;
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                gp_registers.write(Rd, result);
            } break;
            case DataProcessingRegisterA64::LogicalShiftedRegister::BIC_64BIT_SHIFTED: { // P.826 + 88
                constexpr auto datasize = 64;

                auto shift_type   = DecodeShift(shift);
                auto shift_amount = imm6;

                auto operand1 = gp_registers.X(Rn);
                auto operand2 = ShiftReg< datasize, datasize - 1 >(&gp_registers, Rm, shift_type, shift_amount);

                operand2.flip();
                auto result = operand1 & operand2;
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                gp_registers.write(Rd, result);
            } break;
            case DataProcessingRegisterA64::LogicalShiftedRegister::ORR_64BIT_SHIFTED: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::LogicalShiftedRegister::ORN_64BIT_SHIFTED: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::LogicalShiftedRegister::EOR_64BIT_SHIFTED: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::LogicalShiftedRegister::EON_64BIT_SHIFTED: { // P.914 + 88
                constexpr auto datasize = 64;

                auto shift_type   = DecodeShift(shift);
                auto shift_amount = imm6;

                auto operand1 = gp_registers.X(Rn);
                auto operand2 = ShiftReg< datasize, datasize - 1 >(&gp_registers, Rm, shift_type, shift_amount);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);

                operand2.flip();

                auto result = operand1 | operand2;
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                gp_registers.write(Rd, result);
            } break;
            case DataProcessingRegisterA64::LogicalShiftedRegister::ANDS_64BIT_SHIFTED: { // P.799 + 88
                auto           shift_type = DecodeShift(shift);
                constexpr auto datasize   = 64;

                auto operand1 = gp_registers.X(Rn);
                auto operand2 = ShiftReg< datasize, datasize - 1 >(&gp_registers, Rm, shift_type, imm6);

                auto result = operand1 & operand2;
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);
                std::bitset< 4 > nzcv = concate< 2, 2 >(concate< 1, 1 >(result[63] ? 1 : 0, result == 0 ? 1 : 0), 0);
                write_nzcv(nzcv);
                gp_registers.write(Rd, result);
            } break;
            case DataProcessingRegisterA64::LogicalShiftedRegister::BICS_64BIT_SHIFTED: { // P.828 + 88
                constexpr auto datasize = 64;

                auto shift_type   = DecodeShift(shift);
                auto shift_amount = imm6;

                auto operand1 = gp_registers.X(Rn);
                auto operand2 = ShiftReg< datasize, datasize - 1 >(&gp_registers, Rm, shift_type, shift_amount);

                operand2.flip();
                auto result = operand1 & operand2;
                static_assert(std::is_same_v< decltype(result), std::bitset< datasize > >);

                bool val = result[datasize - 1];
                write_nzcv(concate< 1, 3 >(val, concate< 1, 2 >(result == 0, 0)));
                gp_registers.write(Rd, result);
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    auto process(DataProcessingRegisterInstruction< DataProcessingRegisterGroupA64::CONDITIONAL_COMPARE_REGISTER >&& instruction) {
        auto nzcv = instruction.get_nzcv();
        auto Rn   = instruction.get_Rn();
        auto cond = instruction.get_cond();
        auto Rm   = instruction.get_Rm();

        switch (instruction.getInstructionType()) {
            case DataProcessingRegisterA64::ConditionalCompareRegister::CCMNr_32BIT: { // P.854 + 88
                constexpr auto datasize = 32;
                auto           flags    = nzcv;

                auto operand1 = gp_registers.W(Rn);
                auto operand2 = gp_registers.W(Rm);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);

                if (ConditionHolds(cond, &sp_registers.NZCV)) {
                    auto val = AddWithCarry(operand1, operand2, 0);
                    flags    = val.second;
                }

                write_nzcv(flags);
            } break;
            case DataProcessingRegisterA64::ConditionalCompareRegister::CCMPr_32BIT: { // P.858 + 88
                constexpr auto datasize = 32;

                auto flags = nzcv;

                auto operand1 = gp_registers.W(Rn);
                auto operand2 = gp_registers.W(Rm);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);

                if (ConditionHolds(cond, &sp_registers.NZCV)) {
                    operand2.flip();
                    auto val = AddWithCarry(operand1, operand2, 1);
                    flags    = val.second;
                }
                write_nzcv(flags);
            } break;
            case DataProcessingRegisterA64::ConditionalCompareRegister::CCMNr_64BIT: { // P.854 + 88
                constexpr auto datasize = 64;
                auto           flags    = nzcv;

                auto operand1 = gp_registers.X(Rn);
                auto operand2 = gp_registers.X(Rm);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);

                if (ConditionHolds(cond, &sp_registers.NZCV)) {
                    auto val = AddWithCarry(operand1, operand2, 0);
                    flags    = val.second;
                }

                write_nzcv(flags);
            } break;
            case DataProcessingRegisterA64::ConditionalCompareRegister::CCMPr_64BIT: { // P.858 + 88
                constexpr auto datasize = 64;

                auto flags = nzcv;

                auto operand1 = gp_registers.X(Rn);
                auto operand2 = gp_registers.X(Rm);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);

                if (ConditionHolds(cond, &sp_registers.NZCV)) {
                    operand2.flip();
                    auto val = AddWithCarry(operand1, operand2, 1);
                    flags    = val.second;
                }
                write_nzcv(flags);
            } break;
            default:
                throw undefined_instruction {};
                break;
        }
    }
    auto process(DataProcessingRegisterInstruction< DataProcessingRegisterGroupA64::CONDITIONAL_COMPARE_IMMEDIATE >&& instruction) {
        auto nzcv = instruction.get_nzcv();
        auto Rn   = instruction.get_Rn();
        auto cond = instruction.get_cond();
        auto imm5 = instruction.get_imm5();

        switch (instruction.getInstructionType()) {
            case DataProcessingRegisterA64::ConditionalCompareImmediate::CCMNi_32BIT: { // P.852 + 88
                constexpr auto datasize = 32;

                auto flags = nzcv;
                auto imm   = ZeroExtend< datasize, 5 >(imm5);

                auto operand1 = gp_registers.W(Rn);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);

                if (ConditionHolds(cond, &sp_registers.NZCV)) {
                    auto val = AddWithCarry(operand1, imm, 0);
                    flags    = val.second;
                }
                write_nzcv(flags);
            } break;
            case DataProcessingRegisterA64::ConditionalCompareImmediate::CCMPi_32BIT: { // P.856 + 88
                constexpr auto datasize = 32;

                auto flags = nzcv;
                auto imm   = ZeroExtend< datasize, 5 >(imm5);
                static_assert(std::is_same_v< decltype(imm), std::bitset< datasize > >);

                auto operand1 = gp_registers.W(Rn);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);
                auto operand2 = std::bitset< datasize > {};

                if (ConditionHolds(cond, &sp_registers.NZCV)) {
                    operand2 = ~imm;
                    auto val = AddWithCarry(operand1, operand2, 1);
                }
                write_nzcv(flags);
            } break;
            case DataProcessingRegisterA64::ConditionalCompareImmediate::CCMNi_64BIT: { // P.852 + 88
                constexpr auto datasize = 64;

                auto flags = nzcv;
                auto imm   = ZeroExtend< datasize, 5 >(imm5);

                auto operand1 = gp_registers.X(Rn);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);

                if (ConditionHolds(cond, &sp_registers.NZCV)) {
                    auto val = AddWithCarry(operand1, imm, 0);
                    flags    = val.second;
                }
                write_nzcv(flags);
            } break;
            case DataProcessingRegisterA64::ConditionalCompareImmediate::CCMPi_64BIT: { // P.856 + 88
                constexpr auto datasize = 64;

                auto flags = nzcv;
                auto imm   = ZeroExtend< datasize, 5 >(imm5);
                static_assert(std::is_same_v< decltype(imm), std::bitset< datasize > >);

                auto operand1 = gp_registers.X(Rn);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);
                auto operand2 = std::bitset< datasize > {};

                if (ConditionHolds(cond, &sp_registers.NZCV)) {
                    operand2 = ~imm;
                    auto val = AddWithCarry(operand1, operand2, 1);
                }
                write_nzcv(flags);
            } break;
            default:
                throw undefined_instruction {};
                break;
        }
    }
    auto process(DataProcessingRegisterInstruction< DataProcessingRegisterGroupA64::CONDITIONAL_SELECT >&& instruction) {
        auto Rd   = instruction.get_Rd();
        auto Rn   = instruction.get_Rn();
        auto Rm   = instruction.get_Rm();
        auto cond = instruction.get_cond();

        switch (instruction.getInstructionType()) {
            case DataProcessingRegisterA64::ConditionalSelect::CSEL_32BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::ConditionalSelect::CSINC_32BIT: { // P.896 + 88
                constexpr auto datasize = 32;
                auto           operand1 = gp_registers.W(Rn);
                auto           operand2 = gp_registers.W(Rm);

                std::bitset< datasize > result;

                if (ConditionHolds(cond, &sp_registers.NZCV)) {
                    result = operand1;
                } else {
                    result = operand2.to_ullong() + 1;
                }
                gp_registers.write(Rd, result);
            } break;
            case DataProcessingRegisterA64::ConditionalSelect::CSINV_32BIT: { // P.898 + 88
                constexpr auto datasize = 32;

                auto operand1 = gp_registers.W(Rn);
                auto operand2 = gp_registers.W(Rm);

                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);
                decltype(operand1) result;

                if (ConditionHolds(cond, &sp_registers.NZCV))
                    result = operand1;
                else
                    result = ~operand2;

                gp_registers.write(Rd, result);
            } break;
            case DataProcessingRegisterA64::ConditionalSelect::CSNEG_32BIT: { // P.900 + 88
                constexpr auto datasize = 32;

                auto operand1 = gp_registers.W(Rn);
                auto operand2 = gp_registers.W(Rm);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);
                decltype(operand1) result;

                if (ConditionHolds(cond, &sp_registers.NZCV)) {
                    result = operand1;
                } else {
                    get_type_t< datasize > res = static_cast< get_type_t< datasize > >((~operand2).to_ullong());
                    result                     = static_cast< get_type_t< datasize > >(res) + static_cast< get_type_t< datasize > >(1);
                }

                gp_registers.write(Rd, result);
            } break;
            case DataProcessingRegisterA64::ConditionalSelect::CSEL_64BIT: {
                throw not_implemented_feature {};
            } break;
            case DataProcessingRegisterA64::ConditionalSelect::CSINC_64BIT: { // P.896 + 88
                constexpr auto datasize = 64;
                auto           operand1 = gp_registers.X(Rn);
                auto           operand2 = gp_registers.X(Rm);

                std::bitset< datasize > result;

                if (ConditionHolds(cond, &sp_registers.NZCV)) {
                    result = operand1;
                } else {
                    result = operand2.to_ullong() + 1;
                }
                gp_registers.write(Rd, result);
            } break;
            case DataProcessingRegisterA64::ConditionalSelect::CSINV_64BIT: { // P.898 + 88
                constexpr auto datasize = 64;

                auto operand1 = gp_registers.X(Rn);
                auto operand2 = gp_registers.X(Rm);

                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);
                decltype(operand1) result;

                if (ConditionHolds(cond, &sp_registers.NZCV))
                    result = operand1;
                else
                    result = ~operand2;

                gp_registers.write(Rd, result);
            } break;
            case DataProcessingRegisterA64::ConditionalSelect::CSNEG_64BIT: { // P.900 + 88
                constexpr auto datasize = 64;

                auto operand1 = gp_registers.X(Rn);
                auto operand2 = gp_registers.X(Rm);
                static_assert(std::is_same_v< decltype(operand1), std::bitset< datasize > >);
                static_assert(std::is_same_v< decltype(operand2), std::bitset< datasize > >);
                decltype(operand1) result;

                if (ConditionHolds(cond, &sp_registers.NZCV)) {
                    result = operand1;
                } else {
                    get_type_t< datasize > res = static_cast< get_type_t< datasize > >((~operand2).to_ullong());
                    result                     = res + static_cast< get_type_t< datasize > >(1);
                }

                gp_registers.write(Rd, result);
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }

    auto decode(ReservedGroup&& instruction) {
        switch (instruction.getInstructionType()) {
            case ReservedGroupA64::UDP:
                throw not_implemented_feature {};
                // process(ReservedInstruction< ReservedGroupA64::UDP > { instruction });
                break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    auto decode(DataProcessingImmediateGroup&& instruction) {
        switch (instruction.getInstructionClass()) {
            case DataProcessingImmediateGroupA64::PC_RELATIVE_ADDRESSING:
                process(DataProcessingImmediateInstruction< DataProcessingImmediateGroupA64::PC_RELATIVE_ADDRESSING > { instruction });
                break;
            case DataProcessingImmediateGroupA64::ADD_SUBTRACT_IMMEDIATE:
                process(DataProcessingImmediateInstruction< DataProcessingImmediateGroupA64::ADD_SUBTRACT_IMMEDIATE > { instruction });
                break;
            case DataProcessingImmediateGroupA64::ADD_SUBTRACT_IMMEDIATE_TAG:
                process(DataProcessingImmediateInstruction< DataProcessingImmediateGroupA64::ADD_SUBTRACT_IMMEDIATE_TAG > { instruction });
                break;
            case DataProcessingImmediateGroupA64::LOGICAL_IMMEDIATE:
                process(DataProcessingImmediateInstruction< DataProcessingImmediateGroupA64::LOGICAL_IMMEDIATE > { instruction });
                break;
            case DataProcessingImmediateGroupA64::MOVE_WIDE_IMMEDIATE:
                process(DataProcessingImmediateInstruction< DataProcessingImmediateGroupA64::MOVE_WIDE_IMMEDIATE > { instruction });
                break;
            case DataProcessingImmediateGroupA64::BITFIELD:
                process(DataProcessingImmediateInstruction< DataProcessingImmediateGroupA64::BITFIELD > { instruction });
                break;
            case DataProcessingImmediateGroupA64::EXTRACT:
                throw not_implemented_feature {};
                // process(DataProcessingImmediateInstruction< DataProcessingImmediateGroupA64::EXTRACT > { instruction });
                break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    auto decode(Branch_Exception_SystemGroup&& instruction) {
        switch (instruction.getInstructionClass()) {
            case Branch_Exception_SystemGroupA64::CONDITIONAL_BRANCHING: {
                process(Branch_Exception_SystemInstruction< Branch_Exception_SystemGroupA64::CONDITIONAL_BRANCHING > { instruction });
            } break;
            case Branch_Exception_SystemGroupA64::EXCEPTION_GENERATION: {
                process(Branch_Exception_SystemInstruction< Branch_Exception_SystemGroupA64::EXCEPTION_GENERATION > { instruction });
            } break;
            case Branch_Exception_SystemGroupA64::HINTS: {
                throw not_implemented_feature {};
                // process(Branch_Exception_SystemInstruction< Branch_Exception_SystemGroupA64::HINTS > {instruction});
            } break;
            case Branch_Exception_SystemGroupA64::BARRIERS: {
                throw not_implemented_feature {};
                // process(Branch_Exception_SystemInstruction< Branch_Exception_SystemGroupA64::BARRIERS > {instruction});
            } break;
            case Branch_Exception_SystemGroupA64::PSTATE: {
                process(Branch_Exception_SystemInstruction< Branch_Exception_SystemGroupA64::PSTATE > { instruction });
            } break;
            case Branch_Exception_SystemGroupA64::SYSTEM_INSTRUCTIONS: {
                process(Branch_Exception_SystemInstruction< Branch_Exception_SystemGroupA64::SYSTEM_INSTRUCTIONS > { instruction });
            } break;
            case Branch_Exception_SystemGroupA64::SYSTEM_REGISTER_MOVE: {
                throw not_implemented_feature {};
                // process(Branch_Exception_SystemInstruction< Branch_Exception_SystemGroupA64::SYSTEM_REGISTER_MOVE > {instruction});
            } break;
            case Branch_Exception_SystemGroupA64::UNCONDITIONAL_BRANCH_REGISTER: {
                process(Branch_Exception_SystemInstruction< Branch_Exception_SystemGroupA64::UNCONDITIONAL_BRANCH_REGISTER > { instruction });
            } break;
            case Branch_Exception_SystemGroupA64::UNCONDITIONAL_BRANCH_IMMEDIATE: {
                process(Branch_Exception_SystemInstruction< Branch_Exception_SystemGroupA64::UNCONDITIONAL_BRANCH_IMMEDIATE > { instruction });
            } break;
            case Branch_Exception_SystemGroupA64::COMPARE_AND_BRANCH_IMMEDIATE: {
                process(Branch_Exception_SystemInstruction< Branch_Exception_SystemGroupA64::COMPARE_AND_BRANCH_IMMEDIATE > { instruction });
            } break;
            case Branch_Exception_SystemGroupA64::TEST_AND_BRANCH_IMMEDIATE: {
                throw not_implemented_feature {};
                // process(Branch_Exception_SystemInstruction< Branch_Exception_SystemGroupA64::TEST_AND_BRANCH_IMMEDIATE > {instruction});
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    auto decode(LoadStoreGroup&& instruction) {
        switch (instruction.getInstructionClass()) {
            case LoadStoreGroupA64::ADVANCED_SIMD_LOAD_STORE_MULTIPLE_STRUCTURES: {
                throw not_implemented_feature {};
                // process(LoadStoreInstruction< LoadStoreGroupA64::ADVANCED_SIMD_LOAD_STORE_MULTIPLE_STRUCTURES > {instruction});
            } break;
            case LoadStoreGroupA64::ADVANCED_SIMD_LOAD_STORE_MULTIPLE_STRUCTURES_POST_INDEXED: {
                throw not_implemented_feature {};
                // process(LoadStoreInstruction< LoadStoreGroupA64::ADVANCED_SIMD_LOAD_STORE_MULTIPLE_STRUCTURES_POST_INDEXED > {instruction});
            } break;
            case LoadStoreGroupA64::ADVANCED_SIMD_LOAD_STORE_SINGLE_STRUCTURE: {
                throw not_implemented_feature {};
                // process(LoadStoreInstruction< LoadStoreGroupA64::ADVANCED_SIMD_LOAD_STORE_SINGLE_STRUCTURE > {instruction});
            } break;
            case LoadStoreGroupA64::ADVANCED_SIMD_LOAD_STORE_SINGLE_STRUCTURE_POST_INDEXED: {
                throw not_implemented_feature {};
                // process(LoadStoreInstruction< LoadStoreGroupA64::ADVANCED_SIMD_LOAD_STORE_SINGLE_STRUCTURE_POST_INDEXED > {instruction});
            } break;
            case LoadStoreGroupA64::LOAD_STORE_MEMORY_TAG: {
                throw not_implemented_feature {};
                // process(LoadStoreInstruction< LoadStoreGroupA64::LOAD_STORE_MEMORY_TAG > {instruction});
            } break;
            case LoadStoreGroupA64::LOAD_STORE_EXCLUSIVE: {
                throw not_implemented_feature {};
                // process(LoadStoreInstruction< LoadStoreGroupA64::LOAD_STORE_EXCLUSIVE > {instruction});
            } break;
            case LoadStoreGroupA64::LDAPR_STLR_UNSCALED_IMMEDIATE: {
                throw not_implemented_feature {};
                // process(LoadStoreInstruction< LoadStoreGroupA64::LDAPR_STLR_UNSCALED_IMMEDIATE > {instruction});
            } break;
            case LoadStoreGroupA64::LOAD_REGISTER_LITERAL: {
                throw not_implemented_feature {};
                // process(LoadStoreInstruction< LoadStoreGroupA64::LOAD_REGISTER_LITERAL > {instruction});
            } break;
            case LoadStoreGroupA64::LOAD_STORE_NO_ALLOCATE_PAIR_OFFSET: {
                throw not_implemented_feature {};
                // process(LoadStoreInstruction< LoadStoreGroupA64::LOAD_STORE_NO_ALLOCATE_PAIR_OFFSET > {instruction});
            } break;
            case LoadStoreGroupA64::LOAD_STORE_REGISTER_PAIR_POST_INDEXED: {
                throw not_implemented_feature {};
                // process(LoadStoreInstruction< LoadStoreGroupA64::LOAD_STORE_REGISTER_PAIR_POST_INDEXED > {instruction});
            } break;
            case LoadStoreGroupA64::LOAD_STORE_REGISTER_PAIR_OFFSET: {
                throw not_implemented_feature {};
                // process(LoadStoreInstruction< LoadStoreGroupA64::LOAD_STORE_REGISTER_PAIR_OFFSET > {instruction});
            } break;
            case LoadStoreGroupA64::LOAD_STORE_REGISTER_PAIR_PRE_INDEXED: {
                throw not_implemented_feature {};
                // process(LoadStoreInstruction< LoadStoreGroupA64::LOAD_STORE_REGISTER_PAIR_PRE_INDEXED > {instruction});
            } break;
            case LoadStoreGroupA64::LOAD_STORE_REGISTER_UNSCALED_IMMEDIATE: {
                throw not_implemented_feature {};
                // process(LoadStoreInstruction< LoadStoreGroupA64::LOAD_STORE_REGISTER_UNSCALED_IMMEDIATE > {instruction});
            } break;
            case LoadStoreGroupA64::LOAD_STORE_REGISTER_IMMEDIATE_POST_INDEXED: {
                process(LoadStoreInstruction< LoadStoreGroupA64::LOAD_STORE_REGISTER_IMMEDIATE_POST_INDEXED > { instruction });
            } break;
            case LoadStoreGroupA64::LOAD_STORE_REGISTER_UNPRIVILEGED: {
                throw not_implemented_feature {};
                // process(LoadStoreInstruction< LoadStoreGroupA64::LOAD_STORE_REGISTER_UNPRIVILEGED > {instruction});
            } break;
            case LoadStoreGroupA64::LOAD_STORE_REGISTER_IMMEDIATE_PRE_INDEXED: {
                throw not_implemented_feature {};
                // process(LoadStoreInstruction< LoadStoreGroupA64::LOAD_STORE_REGISTER_IMMEDIATE_PRE_INDEXED > {instruction});
            } break;
            case LoadStoreGroupA64::ATOMIC_MEMORY_OPERATION: {
                throw not_implemented_feature {};
                // process(LoadStoreInstruction< LoadStoreGroupA64::ATOMIC_MEMORY_OPERATION > {instruction});
            } break;
            case LoadStoreGroupA64::LOAD_STORE_REGISTER_REGISTER_OFFSET: {
                throw not_implemented_feature {};
                // process(LoadStoreInstruction< LoadStoreGroupA64::LOAD_STORE_REGISTER_REGISTER_OFFSET > {instruction});
            } break;
            case LoadStoreGroupA64::LOAD_STORE_REGISTER_PAC: {
                throw not_implemented_feature {};
                // process(LoadStoreInstruction< LoadStoreGroupA64::LOAD_STORE_REGISTER_PAC > {instruction});
            } break;
            case LoadStoreGroupA64::LOAD_STORE_REGISTER_UNSIGNED_IMMEDIATE: {
                process(LoadStoreInstruction< LoadStoreGroupA64::LOAD_STORE_REGISTER_UNSIGNED_IMMEDIATE > { instruction });
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    auto decode(DataProcessingRegisterGroup&& instruction) {
        switch (instruction.getInstructionClass()) {
            case DataProcessingRegisterGroupA64::DATA_PROCESSING_TWO_SOURCE: {
                process(DataProcessingRegisterInstruction< DataProcessingRegisterGroupA64::DATA_PROCESSING_TWO_SOURCE > { instruction });
            } break;
            case DataProcessingRegisterGroupA64::DATA_PROCESSING_ONE_SOURCE: {
                process(DataProcessingRegisterInstruction< DataProcessingRegisterGroupA64::DATA_PROCESSING_ONE_SOURCE > { instruction });
            } break;
            case DataProcessingRegisterGroupA64::LOGICAL_SHIFTED_REGISTER: {
                process(DataProcessingRegisterInstruction< DataProcessingRegisterGroupA64::LOGICAL_SHIFTED_REGISTER > { instruction });
            } break;
            case DataProcessingRegisterGroupA64::ADD_SUBTRACT_SHIFTED_REGISTER: {
                process(DataProcessingRegisterInstruction< DataProcessingRegisterGroupA64::ADD_SUBTRACT_SHIFTED_REGISTER > { instruction });
            } break;
            case DataProcessingRegisterGroupA64::ADD_SUBTRACT_EXTENDED_REGISTER: {
                process(DataProcessingRegisterInstruction< DataProcessingRegisterGroupA64::ADD_SUBTRACT_EXTENDED_REGISTER > { instruction });
            } break;
            case DataProcessingRegisterGroupA64::ADD_SUBTRACT_CARRY: {
                process(DataProcessingRegisterInstruction< DataProcessingRegisterGroupA64::ADD_SUBTRACT_CARRY > { instruction });
            } break;
            case DataProcessingRegisterGroupA64::ROTATE_RIGHT_INTO_FLAGS: {
                throw not_implemented_feature {};
                // process(DataProcessingRegisterInstruction< DataProcessingRegisterGroupA64::ROTATE_RIGHT_INTO_FLAGS > { instruction });
            } break;
            case DataProcessingRegisterGroupA64::EVALUATE_INTO_FLAGS: {
                throw not_implemented_feature {};
                // process(DataProcessingRegisterInstruction< DataProcessingRegisterGroupA64::EVALUATE_INTO_FLAGS > { instruction });
            } break;
            case DataProcessingRegisterGroupA64::CONDITIONAL_COMPARE_REGISTER: {
                process(DataProcessingRegisterInstruction< DataProcessingRegisterGroupA64::CONDITIONAL_COMPARE_REGISTER > { instruction });
            } break;
            case DataProcessingRegisterGroupA64::CONDITIONAL_COMPARE_IMMEDIATE: {
                process(DataProcessingRegisterInstruction< DataProcessingRegisterGroupA64::CONDITIONAL_COMPARE_IMMEDIATE > { instruction });
            } break;
            case DataProcessingRegisterGroupA64::CONDITIONAL_SELECT: {
                process(DataProcessingRegisterInstruction< DataProcessingRegisterGroupA64::CONDITIONAL_SELECT > { instruction });
            } break;
            case DataProcessingRegisterGroupA64::DATA_PROCESSING_THREE_SOURCE: {
                throw not_implemented_feature {};
                // process(DataProcessingRegisterInstruction< DataProcessingRegisterGroupA64::DATA_PROCESSING_THREE_SOURCE > { instruction });
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    auto decode(DataProcessing_ScalarFloatingPoint_AdvancedSIMDGroup&& instruction) {
        switch (instruction.getInstructionClass()) {
            case DataProcessing_ScalarFloatingPoint_AdvancedSIMDGroupA64::CRYPTOGRAPHIC_AES: {
                throw not_implemented_feature {};
            } break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }

    auto decode(const BaseInstruction& instruction, DecodeGroupA64 type) {
        gp_registers.PC() += 1;
        switch (type) {
            case DecodeGroupA64::Reserved:
                decode(ReservedGroup { instruction });
                break;
            case DecodeGroupA64::ScalableVectorExtension: // not yet implemented
                throw not_implemented_feature {};
                break;
            case DecodeGroupA64::DataProcessingImmediate:
                decode(DataProcessingImmediateGroup { instruction });
                break;
            case DecodeGroupA64::Branch_Exception_System:
                decode(Branch_Exception_SystemGroup { instruction });
                break;
            case DecodeGroupA64::Load_Store:
                decode(LoadStoreGroup { instruction });
                break;
            case DecodeGroupA64::DataProcessingRegister:
                decode(DataProcessingRegisterGroup { instruction });
                break;
            case DecodeGroupA64::DataProcessing_ScalarFloatingPoint_AdvancedSIMD:
                decode(DataProcessing_ScalarFloatingPoint_AdvancedSIMDGroup { instruction });
                break;
            default: {
                throw undefined_instruction {};
            } break;
        }
    }
    auto decode(const BaseInstruction& instruction) const {
        switch (instruction.getDecodeGroup()) {
            case DecodeGroupA64::Reserved:
                return DecodeGroupA64::Reserved;
                break;
            case DecodeGroupA64::ScalableVectorExtension: // not yet implemented
                throw not_implemented_feature {};
                break;
            case DecodeGroupA64::DataProcessingImmediate:
                return DecodeGroupA64::DataProcessingImmediate;
                break;
            case DecodeGroupA64::Branch_Exception_System:
                return DecodeGroupA64::Branch_Exception_System;
                break;
            case DecodeGroupA64::Load_Store:
                return DecodeGroupA64::Load_Store;
                break;
            case DecodeGroupA64::DataProcessingRegister:
                return DecodeGroupA64::DataProcessingRegister;
                break;
            case DecodeGroupA64::DataProcessing_ScalarFloatingPoint_AdvancedSIMD:
                return DecodeGroupA64::DataProcessing_ScalarFloatingPoint_AdvancedSIMD;
                break;
            default:
                return DecodeGroupA64::Undefined;
                break;
        }
    }

    void setup_registers() noexcept {
        gp_registers.PC() = 0;
        gp_registers.SP() = ram_block.size() - 1ull;
        sp_registers.SP_EL0.Set(ram_block.size() - 1ull);
    }
    [[nodiscard]] auto read_instruction(const std::uint64_t& loc) const noexcept {
        if (loc < prog_block.size())
            return prog_block[loc];
        else
            return static_cast< decltype(prog_block)::type >(-1); // need to find an undefined value
    }
    [[nodiscard]] bool instructions_remaining() const noexcept { return (gp_registers.PC() < (prog_block.size())); }
    void               fetch_and_execute() const {
        const auto instruction = prog_block[gp_registers.PC()];
        decode(BaseInstruction { instruction });
    }
    void fetch_and_execute(const LogSignature& logger) {
        auto [instruction, type] = cache.fetch(gp_registers.PC());
        logger(this, gp_registers.PC(), type, instruction, &gp_registers, &sp_registers, nullptr);
        decode(instruction, type);
    }

    SIMDRegisters    fp_registers;
    GPRegisters      gp_registers;
    SpecialRegisters sp_registers;

    Ram< std::uint64_t > ram_block;
    Ram< std::uint32_t > prog_block;

    struct Cache {
        struct CacheElement {
            BaseInstruction instruction;
            DecodeGroupA64  type;

            // TODO check if these are necessary in current MSVC, which is why they were added
            CacheElement() : instruction(0), type(DecodeGroupA64::Undefined) {}
            CacheElement(const CacheElement&) noexcept = default;
            CacheElement(CacheElement&&) noexcept      = default;
            CacheElement& operator=(CacheElement&&) noexcept = default;
            CacheElement& operator=(const CacheElement&) noexcept = default;
            ~CacheElement()                                       = default;
        };

        Cache(const System* sys, const std::uint64_t& start_) noexcept : start(start_), sys_(sys), cache {} {}

        [[nodiscard]] CacheElement fetch(const std::uint64_t& loc) noexcept {
            if (loc > start + cache.size() || loc < start) {
                start = loc;
                fill_cache();
            }

            return cache[(loc - start)];
        }
        void fill_cache() noexcept {
            auto loc = start;
            for (auto& elem : cache) {
                elem.instruction = BaseInstruction { sys_->read_instruction(loc) };
                elem.type        = sys_->decode(elem.instruction);
                loc += 1;
            }
        }

      private:
        std::uint64_t start;
        const System* sys_;
        // 8 KB cache
        std::array< CacheElement, 8192 / (sizeof(decltype(prog_block)::type) * 8) > cache;
    };

    Cache cache;
};

} // namespace arm_emu