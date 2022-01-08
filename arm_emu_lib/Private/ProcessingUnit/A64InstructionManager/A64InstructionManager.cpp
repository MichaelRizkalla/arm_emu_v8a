
#include <ProcessingUnit/A64InstructionManager/A64InstructionManager.h>

BEGIN_NAMESPACE

Bitset A64InstructionManager::Get(const Instruction& instruction, Tag tag) noexcept {
    switch (tag) {
        case Tag::DecodeFields: {
            return Bitset { 8, static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b1111) &
                                                           (instruction.Get() >> 25)) };
        } break;
        case Tag::Cond: {
            return Bitset { 4, static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b1111) &
                                                           (instruction.Get() >> 0)) };
        } break;
        case Tag::Imm3: {
            return Bitset { 3, static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b111) &
                                                           (instruction.Get() >> 10)) };
        } break;
        case Tag::Imm5: {
            return Bitset { 5, static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b11111) &
                                                           (instruction.Get() >> 16)) };
        } break;
        case Tag::Imm6: {
            return Bitset { 6, static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b111111) &
                                                           (instruction.Get() >> 10)) };
        } break;
        case Tag::Imm9: {
            return Bitset { 9, static_cast< std::uint16_t >(static_cast< std::uint32_t >(0x1FF) &
                                                            (instruction.Get() >> 12)) };
        } break;
        case Tag::Imm12: {
            return Bitset { 12, static_cast< std::uint16_t >(static_cast< std::uint32_t >(0xFFF) &
                                                             (instruction.Get() >> 10)) };
        } break;
        case Tag::Imm16: {
            return Bitset { 16, static_cast< std::uint16_t >(static_cast< std::uint32_t >(0xFFFF) &
                                                             (instruction.Get() >> 5)) };
        } break;
        case Tag::Imm19: {
            return Bitset { 19, static_cast< std::uint32_t >(static_cast< std::uint32_t >(0x7FFFF) &
                                                             (instruction.Get() >> 5)) };
        } break;
        case Tag::Imm26: {
            return Bitset { 26, static_cast< std::uint32_t >(static_cast< std::uint32_t >(0x3FFFFFF) &
                                                             (instruction.Get() >> 0)) };
        } break;
        case Tag::Immhi: {
            return Bitset { 19, static_cast< std::uint32_t >(static_cast< std::uint32_t >(0x7FFFF) &
                                                             (instruction.Get() >> 5)) };
        } break;
        case Tag::Immlo: {
            return Bitset { 2, static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b11) &
                                                           (instruction.Get() >> 29)) };
        } break;
        case Tag::Imms: {
            return Bitset { 6, static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b111111) &
                                                           (instruction.Get() >> 10)) };
        } break;
        case Tag::Immr: {
            return Bitset { 6, static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b111111) &
                                                           (instruction.Get() >> 16)) };
        } break;
        case Tag::UImm4: {
            return Bitset { 4, static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b1111) &
                                                           (instruction.Get() >> 10)) };
        } break;
        case Tag::UImm6: {
            return Bitset { 6, static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b111111) &
                                                           (instruction.Get() >> 16)) };
        } break;
        case Tag::Hw: {
            return Bitset { 2, static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b11) &
                                                           (instruction.Get() >> 21)) };
        } break;
        case Tag::Ra: {
            return Bitset { 5, static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b11111) &
                                                           (instruction.Get() >> 10)) };
        } break;
        case Tag::Rd: {
            return Bitset { 5, static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b11111) &
                                                           (instruction.Get() >> 0)) };
        } break;
        case Tag::Rm: {
            return Bitset { 5, static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b11111) &
                                                           (instruction.Get() >> 0)) };
        } break;
        case Tag::Rn: {
            return Bitset { 5, static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b11111) &
                                                           (instruction.Get() >> 5)) };
        } break;
        case Tag::Rt: {
            return Bitset { 5, static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b11111) &
                                                           (instruction.Get() >> 0)) };
        } break;
        case Tag::Xd: {
            return Bitset { 5, static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b11111) &
                                                           (instruction.Get() >> 0)) };
        } break;
        case Tag::Xn: {
            return Bitset { 5, static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b11111) &
                                                           (instruction.Get() >> 5)) };
        } break;
        case Tag::Sh: {
            return Bitset { 1, instruction.ReadBit(22) };
        } break;
        case Tag::CRm: {
            return Bitset { 4, static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b1111) &
                                                           (instruction.Get() >> 8)) };
        } break;
        case Tag::CRn: {
            return Bitset { 4, static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b1111) &
                                                           (instruction.Get() >> 12)) };
        } break;
        case Tag::Size: {
            return Bitset { 2, static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b11) &
                                                           (instruction.Get() >> 30)) };
        } break;
        case Tag::Option: {
            return Bitset { 3, static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b111) &
                                                           (instruction.Get() >> 13)) };
        } break;
        case Tag::Shift: {
            return Bitset { 2, static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b11) &
                                                           (instruction.Get() >> 22)) };
        } break;
        case Tag::NZCV: {
            return Bitset { 4, static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b1111) &
                                                           (instruction.Get() >> 0)) };
        } break;
        case Tag::A: {
            return Bitset { 1, instruction.ReadBit(11) };
        } break;
        case Tag::L: {
            return Bitset { 1, instruction.ReadBit(21) };
        } break;
        case Tag::M: {
            return Bitset { 1, instruction.ReadBit(10) };
        } break;
        case Tag::N: {
            return Bitset { 1, instruction.ReadBit(22) };
        } break;
        case Tag::S: {
            return Bitset { 1, instruction.ReadBit(12) };
        } break;
        case Tag::Z: {
            return Bitset { 1, instruction.ReadBit(24) };
        } break;
        case Tag::Op1: {
            return Bitset { 3, static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b111) &
                                                           (instruction.Get() >> 16)) };
        } break;
        case Tag::Op2: {
            return Bitset { 3, static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b111) &
                                                           (instruction.Get() >> 16)) };
        } break;
        case Tag::Opc: {
            return Bitset { 2, static_cast< std::uint8_t >(static_cast< std::uint32_t >(0b11) &
                                                           (instruction.Get() >> 10)) };
        } break;
        case Tag::O0: {
            return Bitset { 1, instruction.ReadBit(19) };
        } break;
        default:
            assert(false && "This code path should not be reachable!");
            std::terminate();
    }
}

A64DecodeGroup A64InstructionManager::GetDecodeGroup(const Instruction& instruction) noexcept {
    auto bits = Get(instruction, Tag::DecodeFields);
    assert(bits.Size() == 8);
    return A64DecodeGroupTable.Lookup(static_cast< std::uint8_t >(bits.ToULong()));
}

END_NAMESPACE
