#if !defined(A64INSTRUCTIONMANAGER_H_INCLUDED_57014F19_76A7_4FC8_AB95_026CCF6E210F)
    #define A64INSTRUCTIONMANAGER_H_INCLUDED_57014F19_76A7_4FC8_AB95_026CCF6E210F

    #include <API/Api.h>
    #include <ProcessingUnit/A64Instruction/Instruction.h>
    #include <ProcessingUnit/A64InstructionSet/A64InstructionSet.h>
    #include <Utility/Bitset.h>
    #include <concepts>

BEGIN_NAMESPACE

struct A64InstructionManager {
    STATIC_CLASS(A64InstructionManager)

    enum class Tag
    {
        DecodeFields,

        Cond,

        Imm3,
        Imm5,
        Imm6,
        Imm9,
        Imm12,
        Imm16,
        Imm19,
        Imm26,
        Immhi,
        Immlo,
        Imms,
        Immr,

        UImm4,
        UImm6,

        Hw,

        Ra,
        Rd,
        Rn,
        Rm,
        Rt,

        Xd,
        Xn,

        Sh,

        CRm,
        CRn,

        Size,
        Option,
        Shift,
        NZCV,

        A,
        L,
        M,
        N,
        S,
        Z,

        Op1,
        Op2,
        Opc,

        O0,
    };

    [[nodiscard]] static Bitset Get(const Instruction& instruction, Tag tag) noexcept;

    [[nodiscard]] static A64DecodeGroup GetDecodeGroup(const Instruction& instruction) noexcept;
};

END_NAMESPACE

#endif // !defined(A64INSTRUCTIONMANAGER_H_INCLUDED_57014F19_76A7_4FC8_AB95_026CCF6E210F)
