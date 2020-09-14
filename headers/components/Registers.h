// General Purpose Registers based on C1.2.5

#pragma once
#include <array>
#include <bitset>
#include <stdexcept>
#include <exception>
#include <variant>
#include <bitset>
#include <algorithm>
#include <A64Enums.h>
#include <A64SystemSettings.h>

#include <utilities.hpp>

namespace arm_emu {

struct GPRegisters {
    // 64-bit aliases
    [[nodiscard]] constexpr auto& SP() noexcept { return registers[31]; }
    [[nodiscard]] constexpr auto& XZR() noexcept { return registers[31]; }
    [[nodiscard]] constexpr auto& SP() const noexcept { return registers[31]; }
    [[nodiscard]] constexpr auto& XZR() const noexcept { return registers[31]; }
    [[nodiscard]] constexpr auto  X(const std::uint8_t& loc) const { return registers[loc]; }

    [[nodiscard]] constexpr auto& PC() { return PC_; }
    [[nodiscard]] constexpr auto& PC() const { return PC_; }

    // 32-bit aliases
    [[nodiscard]] auto& WSP() noexcept {
        registers[31] &= 0xFFFFFFFF;
        return registers[31];
    }
    [[nodiscard]] auto& WZR() noexcept {
        registers[31] &= 0xFFFFFFFF;
        return registers[31];
    }
    [[nodiscard]] inline auto W(const std::uint8_t& loc) {
        auto return_value = registers[loc].to_ullong() & 0xFFFFFFFF;
        return std::bitset< 32 >(static_cast< std::uint32_t >(return_value));
    }

    template < InstructionArch Arch >
    [[nodiscard]] inline constexpr auto read(const std::uint8_t& loc) {
        if constexpr (Arch == InstructionArch::ARM32) {
            auto return_value = registers[loc].to_ulong() & 0xFFFFFFFF;
            return std::bitset< 32 >(static_cast< std::uint32_t >(return_value));
        } else if constexpr (Arch == InstructionArch::ARM64) {
            return registers[loc];
        }
    }

    constexpr auto write(const std::uint8_t& loc, const std::uint64_t& data) { registers[loc] = data; }
    constexpr auto write(const std::uint8_t& loc, const std::bitset< 64 >& data) { registers[loc] = data; }
    constexpr auto write(const std::uint8_t& loc, const std::uint32_t& data) { registers[loc] = static_cast< std::uint64_t >(data); }
    auto           write(const std::uint8_t& loc, const std::bitset< 32 >& data) { registers[loc] = data.to_ullong(); }

    GPRegisters()  = default;
    ~GPRegisters() = default;

    GPRegisters(const GPRegisters&) = delete;
    GPRegisters(GPRegisters&&)      = delete;
    GPRegisters& operator=(const GPRegisters&) = delete;
    GPRegisters& operator=(GPRegisters&&) = delete;

    // TODO: constructor
  private:
    // TODO: register 30's role as a link on procedure calls.
    // R0 - R30 (31 registers)
    // R31 - SP
    // R32 - PC
    std::array< std::bitset< 64 >, 32 > registers;
    std::uint64_t                       PC_ { 0 };
};

struct SIMDRegisters {

    constexpr static std::size_t B_size    = 8;
    constexpr static std::size_t H_size    = 16;
    constexpr static std::size_t S_size    = 32;
    constexpr static std::size_t D_size    = 64;
    constexpr static std::size_t Q_size    = 128;
    constexpr static std::size_t Half_size = 64;
    constexpr static std::size_t Full_size = 128;

    [[nodiscard]] constexpr auto B(const std::uint8_t& loc) {
        std::uint8_t regVal = 0, mask = 1;
        for (std::size_t i = 0; i < B_size; i++) {
            if (registers[loc][i]) regVal |= mask;
            mask <<= 1;
        }
        return regVal;
    }
    [[nodiscard]] constexpr auto H(const std::uint8_t& loc) {
        std::uint16_t regVal = 0, mask = 1;
        for (std::size_t i = 0; i < H_size; i++) {
            if (registers[loc][i]) regVal |= mask;
            mask <<= 1;
        }
        return regVal;
    }
    [[nodiscard]] constexpr auto S(const std::uint8_t& loc) {
        std::uint32_t regVal = 0, mask = 1;
        for (std::size_t i = 0; i < S_size; i++) {
            if (registers[loc][i]) regVal |= mask;
            mask <<= 1;
        }
        return regVal;
    }
    [[nodiscard]] constexpr auto D(const std::uint8_t& loc) {
        std::uint64_t regVal = 0, mask = 1;
        for (std::size_t i = 0; i < H_size; i++) {
            if (registers[loc][i]) regVal |= mask;
            mask <<= 1;
        }
        return regVal;
    }
    [[nodiscard]] constexpr auto Q(const std::uint8_t& loc) { return registers[loc]; }
    [[nodiscard]] constexpr auto Q_LSB(const std::uint8_t& loc) {
        std::uint64_t regVal = 0, mask = 1;
        for (std::size_t i = 0; i < Q_size / 2; i++) {
            if (registers[loc][i]) regVal |= mask;
            mask <<= 1;
        }
        return regVal;
    }
    [[nodiscard]] constexpr auto Q_MSB(const std::uint8_t& loc) {
        std::uint64_t regVal = 0, mask = 1;
        for (std::size_t i = Q_size / 2; i < Q_size; i++) {
            if (registers[loc][i]) regVal |= mask;
            mask <<= 1;
        }
        return regVal;
    }

    // If the element size in bits multiplied by the number of lanes does not equal 128,
    // then the upper 64 bits of the register are ignored on a read and cleared to zero on a write
    [[nodiscard]] constexpr auto V8B(const std::uint8_t& loc) {
        std::uint64_t regVal = 0, mask = 1;
        for (std::size_t i = 0; i < Half_size; i++) {
            if (registers[loc][i]) regVal |= mask;
            mask <<= 1;
        }
        return regVal;
    }
    [[nodiscard]] constexpr auto V16B(const std::uint8_t& loc) { return registers[loc]; }
    [[nodiscard]] constexpr auto V4H(const std::uint8_t& loc) {
        std::uint64_t regVal = 0, mask = 1;
        for (std::size_t i = 0; i < Half_size; i++) {
            if (registers[loc][i]) regVal |= mask;
            mask <<= 1;
        }
        return regVal;
    }
    [[nodiscard]] constexpr auto V8H(const std::uint8_t& loc) { return registers[loc]; }
    [[nodiscard]] constexpr auto V2S(const std::uint8_t& loc) {
        std::uint64_t regVal = 0, mask = 1;
        for (std::size_t i = 0; i < Half_size; i++) {
            if (registers[loc][i]) regVal |= mask;
            mask <<= 1;
        }
        return regVal;
    }
    [[nodiscard]] constexpr auto V4S(const std::uint8_t& loc) { return registers[loc]; }
    [[nodiscard]] constexpr auto V1D(const std::uint8_t& loc) {
        std::uint64_t regVal = 0, mask = 1;
        for (std::size_t i = 0; i < Half_size; i++) {
            if (registers[loc][i]) regVal |= mask;
            mask <<= 1;
        }
        return regVal;
    }
    [[nodiscard]] constexpr auto V2D(const std::uint8_t& loc) { return registers[loc]; }

    void write(const std::uint8_t& loc, const std::uint8_t& data) {
        for (std::size_t i = 0; i < B_size; i++) { registers[loc].reset(i); }
        registers[loc] |= data;
    }
    void write(const std::uint8_t& loc, const std::uint16_t& data) {
        for (std::size_t i = 0; i < H_size; i++) { registers[loc].reset(i); }
        registers[loc] |= data;
    }
    void write(const std::uint8_t& loc, const std::uint32_t& data) {
        for (std::size_t i = 0; i < S_size; i++) { registers[loc].reset(i); }
        registers[loc] |= data;
    }
    void write(const std::uint8_t& loc, const std::uint64_t& data) {
        for (std::size_t i = 0; i < D_size; i++) { registers[loc].reset(i); }
        registers[loc] |= data;
    }
    void write(const std::uint8_t& loc, const std::uint64_t& LSBData, const std::uint64_t& MSBData) {
        registers[loc].reset();
        registers[loc] |= MSBData;
        registers[loc] <<= Half_size;
        registers[loc] |= LSBData;
    }

    void write_VB(const std::uint8_t& loc, const std::uint8_t& lane, const std::uint8_t& data) {
        const std::size_t start = lane * B_size;
        const std::size_t end   = start + B_size;
        for (std::size_t i = start; i < end; i++) { registers[loc].reset(i); }
        std::bitset< 128 > writeable = 0;
        writeable |= data;
        writeable <<= (lane * B_size);
        registers[loc] |= writeable;
    }
    void write_VH(const std::uint8_t& loc, const std::uint8_t& lane, const std::uint16_t& data) {
        const std::size_t start = lane * H_size;
        const std::size_t end   = start + H_size;
        for (std::size_t i = start; i < end; i++) { registers[loc].reset(i); }
        std::bitset< 128 > writeable = 0;
        writeable |= data;
        writeable <<= (lane * H_size);
        registers[loc] |= writeable;
    }
    void write_VS(const std::uint8_t& loc, const std::uint8_t& lane, const std::uint32_t& data) {
        const std::size_t start = lane * S_size;
        const std::size_t end   = start + S_size;
        for (std::size_t i = start; i < end; i++) { registers[loc].reset(i); }
        std::bitset< 128 > writeable = 0;
        writeable |= data;
        writeable <<= (lane * S_size);
        registers[loc] |= writeable;
    }
    void write_VD(const std::uint8_t& loc, const std::uint8_t& lane, const std::uint64_t& data) {
        const std::size_t start = lane * D_size;
        const std::size_t end   = start + D_size;
        for (std::size_t i = start; i < end; i++) { registers[loc].reset(i); }
        std::bitset< 128 > writeable = 0;
        writeable |= data;
        writeable <<= (lane * D_size);
        registers[loc] |= writeable;
    }

    constexpr SIMDRegisters() = default;
    ~SIMDRegisters()          = default;

    SIMDRegisters(const SIMDRegisters&) = delete;
    SIMDRegisters(SIMDRegisters&&)      = delete;
    SIMDRegisters& operator=(const SIMDRegisters&) = delete;
    SIMDRegisters& operator=(SIMDRegisters&&) = delete;

    // TODO: constructor

  private:
    // 128 127 .... 3 2 1
    std::array< std::bitset< 128 >, 32 > registers;
};
using FPRegisters = SIMDRegisters;

enum class Status : std::uint32_t {
    UNKNOWN,
    UNDEFINED,
    ACTIVE,
};

struct SpecialRegister {
    [[nodiscard]] constexpr auto operator=(const std::uint64_t& rhs) noexcept { value = rhs; }

    [[nodiscard]] friend constexpr auto operator&(const SpecialRegister& lhs, const std::uint64_t& rhs) noexcept { return lhs.value & rhs; }
    [[nodiscard]] friend constexpr auto operator|(const SpecialRegister& lhs, const std::uint64_t& rhs) noexcept { return lhs.value | rhs; }
    [[nodiscard]] friend constexpr auto operator&=(const SpecialRegister& lhs, const std::uint64_t& rhs) noexcept { return lhs.value & rhs; }
    [[nodiscard]] friend constexpr auto operator|=(const SpecialRegister& lhs, const std::uint64_t& rhs) noexcept { return lhs.value | rhs; }
    [[nodiscard]] friend constexpr auto operator==(const SpecialRegister& lhs, const std::uint64_t& rhs) noexcept { return lhs.value == rhs; }
    [[nodiscard]] friend constexpr auto operator!=(const SpecialRegister& lhs, const std::uint64_t& rhs) noexcept { return lhs.value != rhs; }
    [[nodiscard]] friend constexpr auto operator>>(const SpecialRegister& lhs, const std::uint64_t& rhs) noexcept { return lhs.value >> rhs; }
    [[nodiscard]] friend constexpr auto operator<<(const SpecialRegister& lhs, const std::uint64_t& rhs) noexcept { return lhs.value << rhs; }
    [[nodiscard]] friend constexpr auto operator>>=(const SpecialRegister& lhs, const std::uint64_t& rhs) noexcept { return lhs.value >> rhs; }
    [[nodiscard]] friend constexpr auto operator<<=(const SpecialRegister& lhs, const std::uint64_t& rhs) noexcept { return lhs.value << rhs; }

    ~SpecialRegister() = default;
    NULL_COPY_MOVE(SpecialRegister)

  protected:
    constexpr SpecialRegister() : value(0), status_(Status::ACTIVE), bitStatus_({ Status::ACTIVE }) {
        std::for_each(bitStatus_.begin(), bitStatus_.end(), [](auto& e) { e = Status::ACTIVE; });
    };
    constexpr explicit SpecialRegister(const std::uint64_t& val) : value(val), status_(Status::ACTIVE), bitStatus_({ Status::ACTIVE }) {
        std::for_each(bitStatus_.begin(), bitStatus_.end(), [](auto& e) { e = Status::ACTIVE; });
    };
    constexpr explicit SpecialRegister(const std::uint64_t& val, Status status) : value(val), status_(status), bitStatus_({ Status::ACTIVE }) {
        std::for_each(bitStatus_.begin(), bitStatus_.end(), [](auto& e) { e = Status::ACTIVE; });
    };

    std::uint64_t            value;
    Status                   status_;
    std::array< Status, 64 > bitStatus_;
};
struct CurrentELRegister final : public SpecialRegister {
    // C5.2.1 CurrentEL, Current Exception Level
    [[nodiscard]] constexpr auto& Value() const noexcept { return value; }
    [[nodiscard]] constexpr auto  EL() const noexcept { return static_cast< ExceptionLevel >((value & 0x00000000'0000000C) >> 2); }
    constexpr auto                EL(const ExceptionLevel& el) noexcept { value = static_cast< std::uint64_t >(el) << 2; }

    // CurrentELRegister() = default;
    constexpr CurrentELRegister() : SpecialRegister() {}
    constexpr explicit CurrentELRegister(const std::uint64_t& val) : SpecialRegister(val) {}
    constexpr explicit CurrentELRegister(const std::uint64_t& val, Status status) : SpecialRegister(val, status) {}
    ~CurrentELRegister() = default;

    NULL_COPY_MOVE(CurrentELRegister)
};
struct DAIFRegister final : public SpecialRegister {
    //////// values are wrong
    // C5.2.2 DAIF, Interrupt Mask Bits
    [[nodiscard]] constexpr auto& Value() const noexcept { return value; }
    constexpr auto                Set() noexcept { value |= 0x00000000'000003C0; }
    constexpr auto                Clear() noexcept { value &= 0xFFFFFC3F; }
    [[nodiscard]] constexpr auto  D() const noexcept { return (value & 0x00000000'00000200); }
    [[nodiscard]] constexpr auto  A() const noexcept { return (value & 0x00000000'00000100); }
    [[nodiscard]] constexpr auto  I() const noexcept { return (value & 0x00000000'00000080); }
    [[nodiscard]] constexpr auto  F() const noexcept { return (value & 0x00000000'00000040); }
    constexpr auto                D(const bool& val) noexcept { value |= (static_cast< std::uint64_t >(val) << 9); }
    constexpr auto                A(const bool& val) noexcept { value |= (static_cast< std::uint64_t >(val) << 8); }
    constexpr auto                I(const bool& val) noexcept { value |= (static_cast< std::uint64_t >(val) << 7); }
    constexpr auto                F(const bool& val) noexcept { value |= (static_cast< std::uint64_t >(val) << 6); }

    constexpr DAIFRegister() : SpecialRegister() {}
    constexpr explicit DAIFRegister(const std::uint64_t& val) : SpecialRegister(val) {}
    constexpr explicit DAIFRegister(const std::uint64_t& val, Status status) : SpecialRegister(val, status) {}
    ~DAIFRegister() = default;

    NULL_COPY_MOVE(DAIFRegister)
};
struct DITRegister final : public SpecialRegister {
    // C5.2.3 DIT, Data Independent Timing
    // This register is present only when FEAT_DIT is implemented. Otherwise, direct accesses to DIT are
    // UNDEFINED
    [[nodiscard]] auto Value() const {
        if (Query< SystemSettings, Features, Features::FEAT_DIT >::result()) return value;
        throw undefined_register_access {};
    }
    constexpr auto     Reset() noexcept { value = 0x0000000000000000; }
    [[nodiscard]] auto DIT() const {
        if (Query< SystemSettings, Features, Features::FEAT_DIT >::result()) return static_cast< bool >(value >> 24);
        throw undefined_register_access {};
    }
    constexpr auto DIT_Reset() noexcept { value &= 0xFFFFFFFFFEFFFFFF; }

    constexpr DITRegister() : SpecialRegister() {}
    constexpr explicit DITRegister(const std::uint64_t& val) : SpecialRegister(val) {}
    constexpr explicit DITRegister(const std::uint64_t& val, Status status) : SpecialRegister(val, status) {}
    ~DITRegister() = default;

    NULL_COPY_MOVE(DITRegister)
};
struct ELREL1Register : public SpecialRegister {
    // C5.2.4 ELR_EL1, Exception Link Register (EL1)
    [[nodiscard]] constexpr auto Value() {
        if (status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        status_ = Status::UNKNOWN;
        return value;
    }
    constexpr auto Set(const std::uint64_t& addr) noexcept {
        status_ = Status::ACTIVE;
        value   = addr;
    }
    constexpr auto Reset() noexcept { status_ = Status::UNKNOWN; }

    constexpr ELREL1Register() : SpecialRegister() {}
    constexpr explicit ELREL1Register(const std::uint64_t& val) : SpecialRegister(val) {}
    constexpr explicit ELREL1Register(const std::uint64_t& val, Status status) : SpecialRegister(val, status) {}
    ~ELREL1Register() = default;

    NULL_COPY_MOVE(ELREL1Register)
};
struct ELREL2Register final : public SpecialRegister {
    // C5.2.5 ELR_EL2, Exception Link Register (EL2)
    [[nodiscard]] constexpr auto Value() {
        if (status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        status_ = Status::UNKNOWN;
        return value;
    }
    constexpr auto Set(const std::uint64_t& addr) noexcept {
        status_ = Status::ACTIVE;
        value   = addr;
    }
    constexpr auto Reset() noexcept { status_ = Status::UNKNOWN; }

    constexpr ELREL2Register() : SpecialRegister() {}
    constexpr explicit ELREL2Register(const std::uint64_t& val) : SpecialRegister(val) {}
    constexpr explicit ELREL2Register(const std::uint64_t& val, Status status) : SpecialRegister(val, status) {}
    ~ELREL2Register() = default;

    NULL_COPY_MOVE(ELREL2Register)
};
struct ELREL3Register final : public SpecialRegister {
    // C5.2.6 ELR_EL3, Exception Link Register (EL3)
    [[nodiscard]] auto Value() {
        if (!Query< SystemSettings, Features, Features::EL3 >::result()) throw undefined_register_access {};
        if (status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        status_ = Status::UNKNOWN;
        return value;
    }
    constexpr auto Set(const std::uint64_t& addr) noexcept {
        status_ = Status::ACTIVE;
        value   = addr;
    }
    constexpr auto Reset() noexcept { status_ = Status::UNKNOWN; }

    constexpr ELREL3Register() : SpecialRegister() {}
    constexpr explicit ELREL3Register(const std::uint64_t& val) : SpecialRegister(val) {}
    constexpr explicit ELREL3Register(const std::uint64_t& val, Status status) : SpecialRegister(val, status) {}
    ~ELREL3Register() = default;

    NULL_COPY_MOVE(ELREL3Register)
};
struct FPCRRegister final : public SpecialRegister {
    // C5.2.7 FPCR, Floating-point Control Register
    [[nodiscard]] constexpr auto& Value() const { return value; }
    constexpr auto                Reset() noexcept { status_ = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto AHP() const {
        if (bitStatus_[26] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 26);
    }
    constexpr auto AHP(const std::uint64_t& data) {
        bitStatus_[26] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 26);
    }
    constexpr auto AHP_Reset() { bitStatus_[26] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto DN() const {
        if (bitStatus_[25] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 25);
    }
    constexpr auto DN(const std::uint64_t& data) {
        bitStatus_[25] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 25);
    }
    constexpr auto DN_Reset() { bitStatus_[25] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto FZ() const {
        if (bitStatus_[24] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 24);
    }
    constexpr auto FZ(const std::uint64_t& data) {
        bitStatus_[25] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 24);
    }
    constexpr auto FZ_Reset() { bitStatus_[24] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto RMode() const {
        if (bitStatus_[22] == Status::UNKNOWN || bitStatus_[23] == Status::UNKNOWN || status_ == Status::UNKNOWN)
            throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< RoundingMode >(static_cast< std::uint64_t >(3) & (value >> 22));
    }
    constexpr auto RMode(const std::uint64_t& data) {
        bitStatus_[22] = Status::ACTIVE;
        bitStatus_[23] = Status::ACTIVE;
        if (3 < data) throw std::invalid_argument("This field can only hold 0, 1, 2, 3");
        value |= (data << 22);
    }
    constexpr auto RMode(const RoundingMode& data) {
        bitStatus_[22] = Status::ACTIVE;
        bitStatus_[23] = Status::ACTIVE;
        if (3 < static_cast< std::uint64_t >(data)) throw std::invalid_argument("This field can only hold 0, 1, 2, 3");
        value |= (static_cast< std::uint64_t >(data) << 22);
    }
    constexpr auto RMode_Reset() {
        bitStatus_[22] = Status::UNKNOWN;
        bitStatus_[23] = Status::UNKNOWN;
    }

    // Stride // Ingored in AARCH64

    [[nodiscard]] auto FZ16() const {
        if (!Query< SystemSettings, Features, Features::FEAT_FP16 >::result()) throw undefined_register_access {};
        if (bitStatus_[19] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 19);
    }
    auto FZ16(const std::uint64_t& data) {
        if (!Query< SystemSettings, Features, Features::FEAT_FP16 >::result()) throw undefined_register_access {};
        bitStatus_[19] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 19);
    }
    constexpr auto FZ16_Reset() { bitStatus_[19] = Status::UNKNOWN; }

    // Len // Ignored in AARCH64

    [[nodiscard]] constexpr auto IDE() const {
        if (bitStatus_[15] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 15);
    }
    constexpr auto IDE(const std::uint64_t& data) {
        bitStatus_[15] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 15);
    }
    constexpr auto IDE_Reset() { bitStatus_[15] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto IXE() const {
        if (bitStatus_[12] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 12);
    }
    constexpr auto IXE(const std::uint64_t& data) {
        bitStatus_[12] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 12);
    }
    constexpr auto IXE_Reset() { bitStatus_[12] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto UFE() const {
        if (bitStatus_[11] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 11);
    }
    constexpr auto UFE(const std::uint64_t& data) {
        bitStatus_[11] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 11);
    }
    constexpr auto UFE_Reset() { bitStatus_[11] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto OFE() const {
        if (bitStatus_[10] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 10);
    }
    constexpr auto OFE(const std::uint64_t& data) {
        bitStatus_[10] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 10);
    }
    constexpr auto OFE_Reset() { bitStatus_[10] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto DZE() const {
        if (bitStatus_[9] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 9);
    }
    constexpr auto ZDE(const std::uint64_t& data) {
        bitStatus_[9] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 9);
    }
    constexpr auto DZE_Reset() { bitStatus_[9] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto IOE() const {
        if (bitStatus_[8] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 8);
    }
    constexpr auto IOE(const std::uint64_t& data) {
        bitStatus_[8] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 8);
    }
    constexpr auto IOE_Reset() { bitStatus_[8] = Status::UNKNOWN; }

    constexpr FPCRRegister() : SpecialRegister() {}
    constexpr explicit FPCRRegister(const std::uint64_t& val) : SpecialRegister(val) {}
    constexpr explicit FPCRRegister(const std::uint64_t& val, Status status) : SpecialRegister(val, status) {}
    ~FPCRRegister() = default;

    NULL_COPY_MOVE(FPCRRegister)
};
struct FPSRRegister final : public SpecialRegister {
    // C5.2.8 FPSR, Floating-point Status Register
    [[nodiscard]] constexpr auto& Value() const { return value; }
    constexpr auto                Reset() noexcept { status_ = Status::UNKNOWN; }

    // N // Not supported in AARCH64
    // Z // Not supported in AARCH64
    // C // Not supported in AARCH64
    // V // Not supported in AARCH64

    [[nodiscard]] constexpr auto QC() const {
        if (bitStatus_[27] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 27);
    }
    constexpr auto QC(const std::uint64_t& data) {
        bitStatus_[27] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 27);
    }
    constexpr auto QC_Reset() { bitStatus_[27] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto IDC() const {
        if (bitStatus_[7] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 7);
    }
    constexpr auto IDC(const std::uint64_t& data) {
        bitStatus_[7] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 7);
    }
    constexpr auto IDC_Reset() { bitStatus_[7] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto IXC() const {
        if (bitStatus_[4] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 4);
    }
    constexpr auto IXC(const std::uint64_t& data) {
        bitStatus_[4] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 4);
    }
    constexpr auto IXC_Reset() { bitStatus_[4] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto UFC() const {
        if (bitStatus_[3] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 3);
    }
    constexpr auto UFC(const std::uint64_t& data) {
        bitStatus_[3] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 3);
    }
    constexpr auto UFC_Reset() { bitStatus_[3] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto OFC() const {
        if (bitStatus_[2] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 2);
    }
    constexpr auto OFC(const std::uint64_t& data) {
        bitStatus_[2] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 2);
    }
    constexpr auto OFC_Reset() { bitStatus_[2] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto DZC() const {
        if (bitStatus_[1] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 1);
    }
    constexpr auto DZC(const std::uint64_t& data) {
        bitStatus_[1] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 1);
    }
    constexpr auto DZC_Reset() { bitStatus_[1] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto IOC() const {
        if (bitStatus_[0] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 0);
    }
    constexpr auto IOC(const std::uint64_t& data) {
        bitStatus_[0] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 0);
    }
    constexpr auto IOC_Reset() { bitStatus_[0] = Status::UNKNOWN; }

    constexpr FPSRRegister() : SpecialRegister() {}
    constexpr explicit FPSRRegister(const std::uint64_t& val) : SpecialRegister(val) {}
    constexpr explicit FPSRRegister(const std::uint64_t& val, Status status) : SpecialRegister(val, status) {}
    ~FPSRRegister() = default;

    NULL_COPY_MOVE(FPSRRegister)
};
struct NZCVRegister final : public SpecialRegister {
    // C5.2.9 NZCV, Condition Flags
    [[nodiscard]] constexpr auto N() const noexcept { return static_cast< bool >(static_cast< std::uint64_t >(1) & (value >> 31)); }
    constexpr auto               N(const std::uint64_t& data) {
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 31);
    }

    [[nodiscard]] constexpr auto Z() const noexcept { return static_cast< bool >(static_cast< std::uint64_t >(1) & (value >> 30)); }
    constexpr auto               Z(const std::uint64_t& data) {
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 30);
    }

    [[nodiscard]] constexpr auto C() const noexcept { return static_cast< bool >(static_cast< std::uint64_t >(1) & (value >> 29)); }
    constexpr auto               C(const std::uint64_t& data) {
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 29);
    }

    [[nodiscard]] constexpr auto V() const noexcept { return static_cast< bool >(static_cast< std::uint64_t >(1) & (value >> 28)); }
    constexpr auto               V(const std::uint64_t& data) {
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 28);
    }

    constexpr NZCVRegister() : SpecialRegister() {}
    constexpr explicit NZCVRegister(const std::uint64_t& val) : SpecialRegister(val) {}
    constexpr explicit NZCVRegister(const std::uint64_t& val, Status status) : SpecialRegister(val, status) {}
    ~NZCVRegister() = default;

    NULL_COPY_MOVE(NZCVRegister)
};
struct PANRegister final : public SpecialRegister {
    // C5.2.10 PAN, Privileged Access Never
    [[nodiscard]] auto Value() const {
        if (Query< SystemSettings, Features, Features::FEAT_PAN >::result()) return static_cast< bool >(value >> 22);
        throw undefined_register_access {};
    }
    constexpr auto     Reset() noexcept { value = 0x00000000; }
    [[nodiscard]] auto PAN() const { return Value(); }

    constexpr PANRegister() : SpecialRegister() {}
    constexpr explicit PANRegister(const std::uint64_t& val) : SpecialRegister(val) {}
    constexpr explicit PANRegister(const std::uint64_t& val, Status status) : SpecialRegister(val, status) {}
    ~PANRegister() = default;

    NULL_COPY_MOVE(PANRegister)
};
struct SPEL0Register final : public SpecialRegister {
    // C5.2.11 SP_EL0, Stack Pointer (EL0)
    [[nodiscard]] constexpr auto Value() {
        if (status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return value;
    }
    constexpr auto Set(const std::uint64_t& addr) noexcept {
        status_ = Status::ACTIVE;
        value   = addr;
    }
    constexpr auto Reset() noexcept { status_ = Status::UNKNOWN; }

    constexpr SPEL0Register() : SpecialRegister() {}
    constexpr explicit SPEL0Register(const std::uint64_t& val) : SpecialRegister(val) {}
    constexpr explicit SPEL0Register(const std::uint64_t& val, Status status) : SpecialRegister(val, status) {}
    ~SPEL0Register() = default;

    NULL_COPY_MOVE(SPEL0Register)
};
struct SPEL1Register final : public SpecialRegister {
    // C5.2.12 SP_EL1, Stack Pointer (EL1)
    [[nodiscard]] constexpr auto Value() {
        if (status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return value;
    }
    constexpr auto Set(const std::uint64_t& addr) noexcept {
        status_ = Status::ACTIVE;
        value   = addr;
    }
    constexpr auto Reset() noexcept { status_ = Status::UNKNOWN; }

    constexpr SPEL1Register() : SpecialRegister() {}
    constexpr explicit SPEL1Register(const std::uint64_t& val) : SpecialRegister(val) {}
    constexpr explicit SPEL1Register(const std::uint64_t& val, Status status) : SpecialRegister(val, status) {}
    ~SPEL1Register() = default;

    NULL_COPY_MOVE(SPEL1Register)
};
struct SPEL2Register final : public SpecialRegister {
    // C5.2.13 SP_EL2, Stack Pointer (EL2)
    [[nodiscard]] constexpr auto Value() {
        if (status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return value;
    }
    constexpr auto Set(const std::uint64_t& addr) noexcept {
        status_ = Status::ACTIVE;
        value   = addr;
    }
    constexpr auto Reset() noexcept { status_ = Status::UNKNOWN; }

    constexpr SPEL2Register() : SpecialRegister() {}
    constexpr explicit SPEL2Register(const std::uint64_t& val) : SpecialRegister(val) {}
    constexpr explicit SPEL2Register(const std::uint64_t& val, Status status) : SpecialRegister(val, status) {}
    ~SPEL2Register() = default;

    NULL_COPY_MOVE(SPEL2Register)
};
struct SPEL3Register final : public SpecialRegister {
    // C5.2.14 SP_EL3, Stack Pointer (EL3)
    [[nodiscard]] constexpr auto Value() {
        if (status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return value;
    }
    constexpr auto Set(const std::uint64_t& addr) noexcept {
        status_ = Status::ACTIVE;
        value   = addr;
    }
    constexpr auto Reset() noexcept { status_ = Status::UNKNOWN; }

    constexpr SPEL3Register() : SpecialRegister() {}
    constexpr explicit SPEL3Register(const std::uint64_t& val) : SpecialRegister(val) {}
    constexpr explicit SPEL3Register(const std::uint64_t& val, Status status) : SpecialRegister(val, status) {}
    ~SPEL3Register() = default;

    NULL_COPY_MOVE(SPEL3Register)
};
struct SPSelRegister final : public SpecialRegister {
    // C5.2.15 SPSel, Stack Pointer Select
    [[nodiscard]] constexpr auto Value() const noexcept { return static_cast< std::uint64_t >(1) & (value >> 0); }
    constexpr auto               SP() const noexcept { return Value(); }
    constexpr auto               SP(const std::uint64_t& data) {
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 0);
    }
    constexpr auto Reset() { value = 0x00000000'00000001; }

    constexpr SPSelRegister() : SpecialRegister() {}
    constexpr explicit SPSelRegister(const std::uint64_t& val) : SpecialRegister(val) {}
    constexpr explicit SPSelRegister(const std::uint64_t& val, Status status) : SpecialRegister(val, status) {}
    ~SPSelRegister() = default;

    NULL_COPY_MOVE(SPSelRegister)
};
struct SPSRabtRegister final : public SpecialRegister {
    // C5.2.16 SPSR_abt, Saved Program Status Register (Abort mode)
    [[nodiscard]] constexpr auto Value() const {
        if (status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return value;
    }
    constexpr auto Reset() { status_ = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto N() const {
        if (bitStatus_[31] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 31);
    }
    constexpr auto N(const std::uint64_t& data) {
        bitStatus_[31] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 31);
    }
    constexpr auto N_Reset() { bitStatus_[31] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto Z() const {
        if (bitStatus_[30] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 30);
    }
    constexpr auto Z(const std::uint64_t& data) {
        bitStatus_[30] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 30);
    }
    constexpr auto Z_Reset() { bitStatus_[30] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto C() const {
        if (bitStatus_[29] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 29);
    }
    constexpr auto C(const std::uint64_t& data) {
        bitStatus_[29] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 29);
    }
    constexpr auto C_Reset() { bitStatus_[29] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto V() const {
        if (bitStatus_[28] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 28);
    }
    constexpr auto V(const std::uint64_t& data) {
        bitStatus_[28] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 28);
    }
    constexpr auto V_Reset() { bitStatus_[28] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto Q() const {
        if (bitStatus_[27] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 27);
    }
    constexpr auto Q(const std::uint64_t& data) {
        bitStatus_[27] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 27);
    }
    constexpr auto Q_Reset() { bitStatus_[27] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto IT() const {
        if (bitStatus_[10] == Status::UNKNOWN || bitStatus_[11] == Status::UNKNOWN || bitStatus_[12] == Status::UNKNOWN || bitStatus_[13] == Status::UNKNOWN ||
            bitStatus_[14] == Status::UNKNOWN || bitStatus_[15] == Status::UNKNOWN || bitStatus_[25] == Status::UNKNOWN || bitStatus_[26] == Status::UNKNOWN ||
            status_ == Status::UNKNOWN)
            throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        std::uint8_t out_value = 0x00;
        out_value |= static_cast< std::uint8_t >(3) & static_cast< std::uint8_t >(value >> 25);
        out_value |= static_cast< std::uint8_t >(0xFC) & static_cast< std::uint8_t >(value >> 8);
        return static_cast< std::uint64_t >(out_value);
    }
    constexpr auto IT(const std::uint64_t& data) {
        bitStatus_[10] = Status::ACTIVE;
        bitStatus_[11] = Status::ACTIVE;
        bitStatus_[12] = Status::ACTIVE;
        bitStatus_[13] = Status::ACTIVE;
        bitStatus_[14] = Status::ACTIVE;
        bitStatus_[15] = Status::ACTIVE;
        bitStatus_[25] = Status::ACTIVE;
        bitStatus_[26] = Status::ACTIVE;
        if (255 < data) throw std::invalid_argument("This field can only hold values up to 255");
        value |= ((data & 0x00000000'00000003) << 25);
        value |= ((data & 0x00000000'000000FC) << (10 - 2));
    }
    constexpr auto IT_Reset() {
        bitStatus_[10] = Status::UNKNOWN;
        bitStatus_[11] = Status::UNKNOWN;
        bitStatus_[12] = Status::UNKNOWN;
        bitStatus_[13] = Status::UNKNOWN;
        bitStatus_[14] = Status::UNKNOWN;
        bitStatus_[15] = Status::UNKNOWN;
        bitStatus_[25] = Status::UNKNOWN;
        bitStatus_[26] = Status::UNKNOWN;
    }

    // J // NOT USED IN AARCH64

    [[nodiscard]] auto SSBS() const {
        if (Query< SystemSettings, Features, Features::FEAT_SSBS >::result()) {
            if (bitStatus_[23] == Status::UNKNOWN || status_ == Status::UNKNOWN)
                throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
            else
                return static_cast< std::uint64_t >(1) & (value >> 23);
        }
        throw undefined_register_access {};
    }
    auto SSBS(const std::uint64_t& data) {
        if (Query< SystemSettings, Features, Features::FEAT_SSBS >::result()) {
            [[unlikely]] if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
            else {
                bitStatus_[23] = Status::ACTIVE;
                value |= data << 23;
            }
        }
        else
            throw undefined_register_access {};
    }
    constexpr auto SSBS_Reset() { bitStatus_[23] = Status::UNKNOWN; }

    [[nodiscard]] auto PAN() const {
        if (Query< SystemSettings, Features, Features::FEAT_PAN >::result()){
            if (bitStatus_[22] == Status::UNKNOWN || status_ == Status::UNKNOWN)
                throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
            else
                return static_cast< std::uint64_t >(1) & (value >> 22);
        }
        throw undefined_register_access {};
    }
    auto PAN(const std::uint64_t& data) {
        if (Query< SystemSettings, Features, Features::FEAT_PAN >::result()){
            [[unlikely]] if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
            else {
                bitStatus_[22] = Status::ACTIVE;
                value |= data << 22;
            }
        }
        else
            throw undefined_register_access {};
    }
    constexpr auto PAN_Reset() { bitStatus_[22] = Status::UNKNOWN; }

    [[nodiscard]] auto DIT() const {
        if (Query< SystemSettings, Features, Features::FEAT_DIT >::result()){
            if (bitStatus_[21] == Status::UNKNOWN || status_ == Status::UNKNOWN)
                throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
            else
                return static_cast< std::uint64_t >(1) & (value >> 21);
        }
        throw undefined_register_access {};
    }
    auto DIT(const std::uint64_t& data) {
        if (Query< SystemSettings, Features, Features::FEAT_DIT >::result()){
            [[unlikely]] if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
            else {
                bitStatus_[21] = Status::ACTIVE;
                value |= data << 21;
            }
        }
        else
            throw undefined_register_access {};
    }
    constexpr auto DIT_Reset() { bitStatus_[21] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto IL() const {
        if (bitStatus_[20] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 20);
    }
    constexpr auto IL(const std::uint64_t& data) {
        bitStatus_[20] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 20);
    }
    constexpr auto IL_Reset() { bitStatus_[20] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto GE() const {
        if (bitStatus_[16] == Status::UNKNOWN || bitStatus_[17] == Status::UNKNOWN || bitStatus_[18] == Status::UNKNOWN || bitStatus_[19] == Status::UNKNOWN ||
            status_ == Status::UNKNOWN)
            throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(15) & (value >> 16);
    }
    constexpr auto GE(const std::uint64_t& data) {
        bitStatus_[16] = Status::ACTIVE;
        bitStatus_[17] = Status::ACTIVE;
        bitStatus_[18] = Status::ACTIVE;
        bitStatus_[19] = Status::ACTIVE;
        if (15 < data) throw std::invalid_argument("This field can only hold values up to 15");
        value |= (data << 16);
    }
    constexpr auto GE_Reset() {
        bitStatus_[16] = Status::UNKNOWN;
        bitStatus_[17] = Status::UNKNOWN;
        bitStatus_[18] = Status::UNKNOWN;
        bitStatus_[19] = Status::UNKNOWN;
    }

    [[nodiscard]] constexpr auto E() const {
        if (bitStatus_[9] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 9);
    }
    constexpr auto E(const std::uint64_t& data) {
        bitStatus_[9] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 9);
    }
    constexpr auto E() { bitStatus_[9] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto A() const {
        if (bitStatus_[8] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 8);
    }
    constexpr auto A(const std::uint64_t& data) {
        bitStatus_[8] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 8);
    }
    constexpr auto A() { bitStatus_[8] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto I() const {
        if (bitStatus_[7] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 7);
    }
    constexpr auto I(const std::uint64_t& data) {
        bitStatus_[7] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 7);
    }
    constexpr auto I() { bitStatus_[7] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto F() const {
        if (bitStatus_[6] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 6);
    }
    constexpr auto F(const std::uint64_t& data) {
        bitStatus_[6] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 6);
    }
    constexpr auto F() { bitStatus_[6] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto T() const {
        if (bitStatus_[5] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 5);
    }
    constexpr auto T(const std::uint64_t& data) {
        bitStatus_[5] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 5);
    }
    constexpr auto T() { bitStatus_[5] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto M() const {
        if (bitStatus_[0] == Status::UNKNOWN || bitStatus_[1] == Status::UNKNOWN || bitStatus_[2] == Status::UNKNOWN || bitStatus_[3] == Status::UNKNOWN ||
            bitStatus_[4] == Status::UNKNOWN || status_ == Status::UNKNOWN)
            throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< AbortMode >(static_cast< std::uint64_t >(0X1F) & (value >> 0));
    }
    constexpr auto M(const std::uint64_t& data) {
        bitStatus_[0] = Status::ACTIVE;
        bitStatus_[1] = Status::ACTIVE;
        bitStatus_[2] = Status::ACTIVE;
        bitStatus_[3] = Status::ACTIVE;
        bitStatus_[4] = Status::ACTIVE;
        if (31 < data) throw std::invalid_argument("This field can only hold values up to 31");
        value |= (data << 0);
    }
    constexpr auto M(AbortMode data) {
        bitStatus_[0] = Status::ACTIVE;
        bitStatus_[1] = Status::ACTIVE;
        bitStatus_[2] = Status::ACTIVE;
        bitStatus_[3] = Status::ACTIVE;
        bitStatus_[4] = Status::ACTIVE;
        auto dat_val  = static_cast< std::uint64_t >(data);
        if (31 < dat_val) throw std::invalid_argument("This field can only hold values up to 31");
        value |= (dat_val << 0);
    }
    constexpr auto M_Reset() {
        bitStatus_[0] = Status::UNKNOWN;
        bitStatus_[1] = Status::UNKNOWN;
        bitStatus_[2] = Status::UNKNOWN;
        bitStatus_[3] = Status::UNKNOWN;
        bitStatus_[4] = Status::UNKNOWN;
    }

    constexpr SPSRabtRegister() : SpecialRegister() {}
    constexpr explicit SPSRabtRegister(const std::uint64_t& val) : SpecialRegister(val) {}
    constexpr explicit SPSRabtRegister(const std::uint64_t& val, Status status) : SpecialRegister(val, status) {}
    ~SPSRabtRegister() = default;

    NULL_COPY_MOVE(SPSRabtRegister)
};
struct SPSREL1Register final : public SpecialRegister {
    // C5.2.17 SPSR_EL1, Saved Program Status Register (EL1)
    [[nodiscard]] constexpr auto Value() const {
        if (status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return value;
    }
    constexpr auto Reset() { status_ = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto N() const {
        if (bitStatus_[31] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 31);
    }
    constexpr auto N(const std::uint64_t& data) {
        bitStatus_[31] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 31);
    }
    constexpr auto N_Reset() { bitStatus_[31] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto Z() const {
        if (bitStatus_[30] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 30);
    }
    constexpr auto Z(const std::uint64_t& data) {
        bitStatus_[30] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 30);
    }
    constexpr auto Z_Reset() { bitStatus_[30] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto C() const {
        if (bitStatus_[29] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 29);
    }
    constexpr auto C(const std::uint64_t& data) {
        bitStatus_[29] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 29);
    }
    constexpr auto C_Reset() { bitStatus_[29] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto V() const {
        if (bitStatus_[28] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 28);
    }
    constexpr auto V(const std::uint64_t& data) {
        bitStatus_[28] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 28);
    }
    constexpr auto V_Reset() { bitStatus_[28] = Status::UNKNOWN; }

    [[nodiscard]] auto TCO() const {
        if (Query< SystemSettings, Features, Features::FEAT_MTE >::result()){
            if (bitStatus_[25] == Status::UNKNOWN || status_ == Status::UNKNOWN)
                throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
            else
                return static_cast< std::uint64_t >(1) & (value >> 25);
        }
        throw undefined_register_access {};
    }
    auto TCO(const std::uint64_t& data) {
        if (Query< SystemSettings, Features, Features::FEAT_SSBS >::result()){
            [[unlikely]] if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
            else {
                bitStatus_[25] = Status::ACTIVE;
                value |= data << 25;
            }
        }
        else
            throw undefined_register_access {};
    }
    constexpr auto TCO_Reset() { bitStatus_[25] = Status::UNKNOWN; }

    [[nodiscard]] auto DIT() const {
        if (Query< SystemSettings, Features, Features::FEAT_DIT >::result()){
            if (bitStatus_[24] == Status::UNKNOWN || status_ == Status::UNKNOWN)
                throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
            else
                return static_cast< std::uint64_t >(1) & (value >> 24);
        }
        throw undefined_register_access {};
    }
    auto DIT(const std::uint64_t& data) {
        if (Query< SystemSettings, Features, Features::FEAT_DIT >::result()){
            [[unlikely]] if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
            else {
                bitStatus_[24] = Status::ACTIVE;
                value |= data << 24;
            }   
        }
        else
            throw undefined_register_access {};
    }
    constexpr auto DIT_Reset() { bitStatus_[24] = Status::UNKNOWN; }

    [[nodiscard]] auto UAO() const {
        if (Query< SystemSettings, Features, Features::FEAT_UAO >::result()){
            if (bitStatus_[23] == Status::UNKNOWN || status_ == Status::UNKNOWN)
                throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
            else
                return static_cast< std::uint64_t >(1) & (value >> 23);
        }
        throw undefined_register_access {};
    }
    auto UAO(const std::uint64_t& data) {
        if (Query< SystemSettings, Features, Features::FEAT_PAN >::result()){
            [[unlikely]] if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
            else {
                bitStatus_[23] = Status::ACTIVE;
                value |= data << 23;
            }
        }
        else
            throw undefined_register_access {};
    }
    constexpr auto UAO_Reset() { bitStatus_[23] = Status::UNKNOWN; }

    [[nodiscard]] auto PAN() const {
        if (Query< SystemSettings, Features, Features::FEAT_PAN >::result()){
            if (bitStatus_[22] == Status::UNKNOWN || status_ == Status::UNKNOWN)
                throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
            else
                return static_cast< std::uint64_t >(1) & (value >> 22);
        }
        throw undefined_register_access {};
    }
    auto PAN(const std::uint64_t& data) {
        if (Query< SystemSettings, Features, Features::FEAT_PAN >::result()){
            [[unlikely]] if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
            else {
                bitStatus_[22] = Status::ACTIVE;
                value |= data << 22;
            }
        }
        else
            throw undefined_register_access {};
    }
    constexpr auto PAN_Reset() { bitStatus_[22] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto SS() const {
        if (bitStatus_[21] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 21);
    }
    constexpr auto S(const std::uint64_t& data) {
        bitStatus_[21] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 21);
    }
    constexpr auto SS_Reset() { bitStatus_[21] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto IL() const {
        if (bitStatus_[20] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 20);
    }
    constexpr auto IL(const std::uint64_t& data) {
        bitStatus_[20] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 20);
    }
    constexpr auto IL_Reset() { bitStatus_[20] = Status::UNKNOWN; }

    [[nodiscard]] auto SSBS() const {
        if (Query< SystemSettings, Features, Features::FEAT_SSBS >::result()){
            if (bitStatus_[12] == Status::UNKNOWN || status_ == Status::UNKNOWN)
                throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
            else
                return static_cast< std::uint64_t >(1) & (value >> 12);
        }
        throw undefined_register_access {};
    }
    auto SSBS(const std::uint64_t& data) {
        if (Query< SystemSettings, Features, Features::FEAT_SSBS >::result()){
            [[unlikely]] if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
            else {
                bitStatus_[12] = Status::ACTIVE;
                value |= data << 12;
            }
        }
        else
            throw undefined_register_access {};
    }
    constexpr auto SSBS_Reset() { bitStatus_[12] = Status::UNKNOWN; }

    [[nodiscard]] auto BTYPE() const {
        if (Query< SystemSettings, Features, Features::FEAT_BTI >::result()){
            if (bitStatus_[12] == Status::UNKNOWN || status_ == Status::UNKNOWN)
                throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
            else
                return static_cast< std::uint64_t >(3) & (value >> 10);
        }
        throw undefined_register_access {};
    }
    auto BTYPE(const std::uint64_t& data) {
        if (Query< SystemSettings, Features, Features::FEAT_SSBS >::result()){
            [[unlikely]] if (3 < data) throw std::invalid_argument("A bit can only hold up to 3");
            else {
                bitStatus_[10] = Status::ACTIVE;
                bitStatus_[11] = Status::ACTIVE;
                value |= data << 10;
            }
        }
        else
            throw undefined_register_access {};
    }
    constexpr auto BTYPE_Reset() {
        bitStatus_[10] = Status::UNKNOWN;
        bitStatus_[11] = Status::UNKNOWN;
    }

    [[nodiscard]] constexpr auto D() const {
        if (bitStatus_[9] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 9);
    }
    constexpr auto D(const std::uint64_t& data) {
        bitStatus_[9] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 9);
    }
    constexpr auto D_Reset() { bitStatus_[9] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto A() const {
        if (bitStatus_[8] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 8);
    }
    constexpr auto A(const std::uint64_t& data) {
        bitStatus_[8] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 8);
    }
    constexpr auto A_Reset() { bitStatus_[8] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto I() const {
        if (bitStatus_[7] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 7);
    }
    constexpr auto I(const std::uint64_t& data) {
        bitStatus_[7] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 7);
    }
    constexpr auto I_Reset() { bitStatus_[7] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto F() const {
        if (bitStatus_[6] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 6);
    }
    constexpr auto F(const std::uint64_t& data) {
        bitStatus_[6] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 6);
    }
    constexpr auto F_Reset() { bitStatus_[6] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto M() const {
        if (bitStatus_[0] == Status::UNKNOWN || bitStatus_[1] == Status::UNKNOWN || bitStatus_[2] == Status::UNKNOWN || bitStatus_[3] == Status::UNKNOWN ||
            bitStatus_[4] == Status::UNKNOWN || status_ == Status::UNKNOWN)
            throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< AbortMode >(static_cast< std::uint64_t >(0X1F) & (value >> 0));
    }
    constexpr auto M(const std::uint64_t& data) {
        bitStatus_[0] = Status::ACTIVE;
        bitStatus_[1] = Status::ACTIVE;
        bitStatus_[2] = Status::ACTIVE;
        bitStatus_[3] = Status::ACTIVE;
        bitStatus_[4] = Status::ACTIVE;
        if (31 < data) throw std::invalid_argument("This field can only hold values up to 31");
        value |= (data << 0);
    }
    constexpr auto M(A64SelectedStackPointer data) {
        bitStatus_[0] = Status::ACTIVE;
        bitStatus_[1] = Status::ACTIVE;
        bitStatus_[2] = Status::ACTIVE;
        bitStatus_[3] = Status::ACTIVE;
        bitStatus_[4] = Status::ACTIVE;
        auto dat_val  = static_cast< std::uint64_t >(data);
        if (31 < dat_val) throw std::invalid_argument("This field can only hold values up to 31");
        value |= (dat_val << 0);
    }
    constexpr auto M_Reset() {
        bitStatus_[0] = Status::UNKNOWN;
        bitStatus_[1] = Status::UNKNOWN;
        bitStatus_[2] = Status::UNKNOWN;
        bitStatus_[3] = Status::UNKNOWN;
        bitStatus_[4] = Status::UNKNOWN;
    }

    constexpr SPSREL1Register() : SpecialRegister() {}
    constexpr explicit SPSREL1Register(const std::uint64_t& val) : SpecialRegister(val) {}
    constexpr explicit SPSREL1Register(const std::uint64_t& val, Status status) : SpecialRegister(val, status) {}
    ~SPSREL1Register() = default;

    NULL_COPY_MOVE(SPSREL1Register)
};
struct SPSREL2Register final : public SpecialRegister {
    // C5.2.18 SPSR_EL2, Saved Program Status Register (EL2)
    [[nodiscard]] constexpr auto Value() const {
        if (status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return value;
    }
    constexpr auto Reset() { status_ = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto N() const {
        if (bitStatus_[31] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 31);
    }
    constexpr auto N(const std::uint64_t& data) {
        bitStatus_[31] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 31);
    }
    constexpr auto N_Reset() { bitStatus_[31] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto Z() const {
        if (bitStatus_[30] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 30);
    }
    constexpr auto Z(const std::uint64_t& data) {
        bitStatus_[30] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 30);
    }
    constexpr auto Z_Reset() { bitStatus_[30] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto C() const {
        if (bitStatus_[29] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 29);
    }
    constexpr auto C(const std::uint64_t& data) {
        bitStatus_[29] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 29);
    }
    constexpr auto C_Reset() { bitStatus_[29] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto V() const {
        if (bitStatus_[28] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 28);
    }
    constexpr auto V(const std::uint64_t& data) {
        bitStatus_[28] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 28);
    }
    constexpr auto V_Reset() { bitStatus_[28] = Status::UNKNOWN; }

    [[nodiscard]] auto TCO() const {
        if (Query< SystemSettings, Features, Features::FEAT_MTE >::result()){
            if (bitStatus_[25] == Status::UNKNOWN || status_ == Status::UNKNOWN)
                throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
            else
                return static_cast< std::uint64_t >(1) & (value >> 25);
        }
        throw undefined_register_access {};
    }
    auto TCO(const std::uint64_t& data) {
        if (Query< SystemSettings, Features, Features::FEAT_SSBS >::result()){
            [[unlikely]] if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
            else {
                bitStatus_[25] = Status::ACTIVE;
                value |= data << 25;
            }
        }
        else
            throw undefined_register_access {};
    }
    constexpr auto TCO_Reset() { bitStatus_[25] = Status::UNKNOWN; }

    [[nodiscard]] auto DIT() const {
        if (Query< SystemSettings, Features, Features::FEAT_DIT >::result()){
            if (bitStatus_[24] == Status::UNKNOWN || status_ == Status::UNKNOWN)
                throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
            else
                return static_cast< std::uint64_t >(1) & (value >> 24);
        }
        throw undefined_register_access {};
    }
    auto DIT(const std::uint64_t& data) {
        if (Query< SystemSettings, Features, Features::FEAT_DIT >::result()){
            [[unlikely]] if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
            else {
                bitStatus_[24] = Status::ACTIVE;
                value |= data << 24;
            }
        }
        else
            throw undefined_register_access {};
    }
    constexpr auto DIT_Reset() { bitStatus_[24] = Status::UNKNOWN; }

    [[nodiscard]] auto UAO() const {
        if (Query< SystemSettings, Features, Features::FEAT_UAO >::result()){
            if (bitStatus_[23] == Status::UNKNOWN || status_ == Status::UNKNOWN)
                throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
            else
                return static_cast< std::uint64_t >(1) & (value >> 23);
        }
        throw undefined_register_access {};
    }
    auto UAO(const std::uint64_t& data) {
        if (Query< SystemSettings, Features, Features::FEAT_PAN >::result()){
            [[unlikely]] if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
            else {
                bitStatus_[23] = Status::ACTIVE;
                value |= data << 23;
            }
        }
        else
            throw undefined_register_access {};
    }
    constexpr auto UAO_Reset() { bitStatus_[23] = Status::UNKNOWN; }

    [[nodiscard]] auto PAN() const {
        if (Query< SystemSettings, Features, Features::FEAT_PAN >::result()){
            if (bitStatus_[22] == Status::UNKNOWN || status_ == Status::UNKNOWN)
                throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
            else
                return static_cast< std::uint64_t >(1) & (value >> 22);
        }
        throw undefined_register_access {};
    }
    auto PAN(const std::uint64_t& data) {
        if (Query< SystemSettings, Features, Features::FEAT_PAN >::result()){
            [[unlikely]] if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
            else {
                bitStatus_[22] = Status::ACTIVE;
                value |= data << 22;
            }
        }
        else
            throw undefined_register_access {};
    }
    constexpr auto PAN_Reset() { bitStatus_[22] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto SS() const {
        if (bitStatus_[21] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 21);
    }
    constexpr auto S(const std::uint64_t& data) {
        bitStatus_[21] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 21);
    }
    constexpr auto SS_Reset() { bitStatus_[21] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto IL() const {
        if (bitStatus_[20] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 20);
    }
    constexpr auto IL(const std::uint64_t& data) {
        bitStatus_[20] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 20);
    }
    constexpr auto IL_Reset() { bitStatus_[20] = Status::UNKNOWN; }

    [[nodiscard]] auto SSBS() const {
        if (Query< SystemSettings, Features, Features::FEAT_SSBS >::result()){
            if (bitStatus_[12] == Status::UNKNOWN || status_ == Status::UNKNOWN)
                throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
            else
                return static_cast< std::uint64_t >(1) & (value >> 12);
        }
        throw undefined_register_access {};
    }
    auto SSBS(const std::uint64_t& data) {
        if (Query< SystemSettings, Features, Features::FEAT_SSBS >::result()){
            [[unlikely]] if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
            else {
                bitStatus_[12] = Status::ACTIVE;
                value |= data << 12;
            }
        }
        else
            throw undefined_register_access {};
    }
    constexpr auto SSBS_Reset() { bitStatus_[12] = Status::UNKNOWN; }

    [[nodiscard]] auto BTYPE() const {
        if (Query< SystemSettings, Features, Features::FEAT_BTI >::result()){
            if (bitStatus_[12] == Status::UNKNOWN || status_ == Status::UNKNOWN)
                throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
            else
                return static_cast< std::uint64_t >(3) & (value >> 10);
        }
        throw undefined_register_access {};
    }
    auto BTYPE(const std::uint64_t& data) {
        if (Query< SystemSettings, Features, Features::FEAT_SSBS >::result()){
            [[unlikely]] if (3 < data) throw std::invalid_argument("A bit can only hold up to 3");
            else {
                bitStatus_[10] = Status::ACTIVE;
                bitStatus_[11] = Status::ACTIVE;
                value |= data << 10;
            }
        }
        else
            throw undefined_register_access {};
    }
    constexpr auto BTYPE_Reset() {
        bitStatus_[10] = Status::UNKNOWN;
        bitStatus_[11] = Status::UNKNOWN;
    }

    [[nodiscard]] constexpr auto D() const {
        if (bitStatus_[9] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 9);
    }
    constexpr auto D(const std::uint64_t& data) {
        bitStatus_[9] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 9);
    }
    constexpr auto D_Reset() { bitStatus_[9] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto A() const {
        if (bitStatus_[8] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 8);
    }
    constexpr auto A(const std::uint64_t& data) {
        bitStatus_[8] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 8);
    }
    constexpr auto A_Reset() { bitStatus_[8] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto I() const {
        if (bitStatus_[7] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 7);
    }
    constexpr auto I(const std::uint64_t& data) {
        bitStatus_[7] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 7);
    }
    constexpr auto I_Reset() { bitStatus_[7] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto F() const {
        if (bitStatus_[6] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 6);
    }
    constexpr auto F(const std::uint64_t& data) {
        bitStatus_[6] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 6);
    }
    constexpr auto F_Reset() { bitStatus_[6] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto M() const {
        if (bitStatus_[0] == Status::UNKNOWN || bitStatus_[1] == Status::UNKNOWN || bitStatus_[2] == Status::UNKNOWN || bitStatus_[3] == Status::UNKNOWN ||
            bitStatus_[4] == Status::UNKNOWN || status_ == Status::UNKNOWN)
            throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< AbortMode >(static_cast< std::uint64_t >(0X1F) & (value >> 0));
    }
    constexpr auto M(const std::uint64_t& data) {
        bitStatus_[0] = Status::ACTIVE;
        bitStatus_[1] = Status::ACTIVE;
        bitStatus_[2] = Status::ACTIVE;
        bitStatus_[3] = Status::ACTIVE;
        bitStatus_[4] = Status::ACTIVE;
        if (31 < data) throw std::invalid_argument("This field can only hold values up to 31");
        value |= (data << 0);
    }
    constexpr auto M(A64SelectedStackPointer data) {
        bitStatus_[0] = Status::ACTIVE;
        bitStatus_[1] = Status::ACTIVE;
        bitStatus_[2] = Status::ACTIVE;
        bitStatus_[3] = Status::ACTIVE;
        bitStatus_[4] = Status::ACTIVE;
        auto dat_val  = static_cast< std::uint64_t >(data);
        if (31 < dat_val) throw std::invalid_argument("This field can only hold values up to 31");
        value |= (dat_val << 0);
    }
    constexpr auto M_Reset() {
        bitStatus_[0] = Status::UNKNOWN;
        bitStatus_[1] = Status::UNKNOWN;
        bitStatus_[2] = Status::UNKNOWN;
        bitStatus_[3] = Status::UNKNOWN;
        bitStatus_[4] = Status::UNKNOWN;
    }

    constexpr SPSREL2Register() : SpecialRegister() {}
    constexpr explicit SPSREL2Register(const std::uint64_t& val) : SpecialRegister(val) {}
    constexpr explicit SPSREL2Register(const std::uint64_t& val, Status status) : SpecialRegister(val, status) {}
    ~SPSREL2Register() = default;

    NULL_COPY_MOVE(SPSREL2Register)
};
struct SPSREL3Register final : public SpecialRegister {
    // C5.2.19 SPSR_EL3, Saved Program Status Register (EL3)
    [[nodiscard]] constexpr auto Value() const {
        if (status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return value;
    }
    constexpr auto Reset() { status_ = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto N() const {
        if (bitStatus_[31] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 31);
    }
    constexpr auto N(const std::uint64_t& data) {
        bitStatus_[31] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 31);
    }
    constexpr auto N_Reset() { bitStatus_[31] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto Z() const {
        if (bitStatus_[30] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 30);
    }
    constexpr auto Z(const std::uint64_t& data) {
        bitStatus_[30] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 30);
    }
    constexpr auto Z_Reset() { bitStatus_[30] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto C() const {
        if (bitStatus_[29] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 29);
    }
    constexpr auto C(const std::uint64_t& data) {
        bitStatus_[29] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 29);
    }
    constexpr auto C_Reset() { bitStatus_[29] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto V() const {
        if (bitStatus_[28] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 28);
    }
    constexpr auto V(const std::uint64_t& data) {
        bitStatus_[28] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 28);
    }
    constexpr auto V_Reset() { bitStatus_[28] = Status::UNKNOWN; }

    [[nodiscard]] auto TCO() const {
        if (Query< SystemSettings, Features, Features::FEAT_MTE >::result()){
            if (bitStatus_[25] == Status::UNKNOWN || status_ == Status::UNKNOWN)
                throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
            else
                return static_cast< std::uint64_t >(1) & (value >> 25);
        }
        throw undefined_register_access {};
    }
    auto TCO(const std::uint64_t& data) {
        if (Query< SystemSettings, Features, Features::FEAT_SSBS >::result()){
            [[unlikely]] if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
            else {
                bitStatus_[25] = Status::ACTIVE;
                value |= data << 25;
            }
        }
        else
            throw undefined_register_access {};
    }
    constexpr auto TCO_Reset() { bitStatus_[25] = Status::UNKNOWN; }

    [[nodiscard]] auto DIT() const {
        if (Query< SystemSettings, Features, Features::FEAT_DIT >::result()){
            if (bitStatus_[24] == Status::UNKNOWN || status_ == Status::UNKNOWN)
                throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
            else
                return static_cast< std::uint64_t >(1) & (value >> 24);
        }
        throw undefined_register_access {};
    }
    auto DIT(const std::uint64_t& data) {
        if (Query< SystemSettings, Features, Features::FEAT_DIT >::result()){
            [[unlikely]] if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
            else {
                bitStatus_[24] = Status::ACTIVE;
                value |= data << 24;
            }
        }
        else
            throw undefined_register_access {};
    }
    constexpr auto DIT_Reset() { bitStatus_[24] = Status::UNKNOWN; }

    [[nodiscard]] auto UAO() const {
        if (Query< SystemSettings, Features, Features::FEAT_UAO >::result()){
            if (bitStatus_[23] == Status::UNKNOWN || status_ == Status::UNKNOWN)
                throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
            else
                return static_cast< std::uint64_t >(1) & (value >> 23);
        }
        throw undefined_register_access {};
    }
    auto UAO(const std::uint64_t& data) {
        if (Query< SystemSettings, Features, Features::FEAT_PAN >::result()){
            [[unlikely]] if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
            else {
                bitStatus_[23] = Status::ACTIVE;
                value |= data << 23;
            }
        }
        else
            throw undefined_register_access {};
    }
    constexpr auto UAO_Reset() { bitStatus_[23] = Status::UNKNOWN; }

    [[nodiscard]] auto PAN() const {
        if (Query< SystemSettings, Features, Features::FEAT_PAN >::result()){
            if (bitStatus_[22] == Status::UNKNOWN || status_ == Status::UNKNOWN)
                throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
            else
                return static_cast< std::uint64_t >(1) & (value >> 22);
        }
        throw undefined_register_access {};
    }
    auto PAN(const std::uint64_t& data) {
        if (Query< SystemSettings, Features, Features::FEAT_PAN >::result()){
            [[unlikely]] if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
            else {
                bitStatus_[22] = Status::ACTIVE;
                value |= data << 22;
            }
        }
        else
            throw undefined_register_access {};
    }
    constexpr auto PAN_Reset() { bitStatus_[22] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto SS() const {
        if (bitStatus_[21] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 21);
    }
    constexpr auto S(const std::uint64_t& data) {
        bitStatus_[21] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 21);
    }
    constexpr auto SS_Reset() { bitStatus_[21] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto IL() const {
        if (bitStatus_[20] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 20);
    }
    constexpr auto IL(const std::uint64_t& data) {
        bitStatus_[20] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 20);
    }
    constexpr auto IL_Reset() { bitStatus_[20] = Status::UNKNOWN; }

    [[nodiscard]] auto SSBS() const {
        if (Query< SystemSettings, Features, Features::FEAT_SSBS >::result()){
            if (bitStatus_[12] == Status::UNKNOWN || status_ == Status::UNKNOWN)
                throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
            else
                return static_cast< std::uint64_t >(1) & (value >> 12);
        }
        throw undefined_register_access {};
    }
    auto SSBS(const std::uint64_t& data) {
        if (Query< SystemSettings, Features, Features::FEAT_SSBS >::result()){
            [[unlikely]] if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
            else {
                bitStatus_[12] = Status::ACTIVE;
                value |= data << 12;
            }
        }
        else
            throw undefined_register_access {};
    }
    constexpr auto SSBS_Reset() { bitStatus_[12] = Status::UNKNOWN; }

    [[nodiscard]] auto BTYPE() const {
        if (Query< SystemSettings, Features, Features::FEAT_BTI >::result()){
            if (bitStatus_[12] == Status::UNKNOWN || status_ == Status::UNKNOWN)
                throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
            else
                return static_cast< std::uint64_t >(3) & (value >> 10);
        }
        throw undefined_register_access {};
    }
    auto BTYPE(const std::uint64_t& data) {
        if (Query< SystemSettings, Features, Features::FEAT_SSBS >::result()){
            [[unlikely]] if (3 < data) throw std::invalid_argument("A bit can only hold up to 3");
            else {
                bitStatus_[10] = Status::ACTIVE;
                bitStatus_[11] = Status::ACTIVE;
                value |= data << 10;
            }
        }
        else
            throw undefined_register_access {};
    }
    constexpr auto BTYPE_Reset() {
        bitStatus_[10] = Status::UNKNOWN;
        bitStatus_[11] = Status::UNKNOWN;
    }

    [[nodiscard]] constexpr auto D() const {
        if (bitStatus_[9] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 9);
    }
    constexpr auto D(const std::uint64_t& data) {
        bitStatus_[9] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 9);
    }
    constexpr auto D_Reset() { bitStatus_[9] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto A() const {
        if (bitStatus_[8] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 8);
    }
    constexpr auto A(const std::uint64_t& data) {
        bitStatus_[8] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 8);
    }
    constexpr auto A_Reset() { bitStatus_[8] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto I() const {
        if (bitStatus_[7] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 7);
    }
    constexpr auto I(const std::uint64_t& data) {
        bitStatus_[7] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 7);
    }
    constexpr auto I_Reset() { bitStatus_[7] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto F() const {
        if (bitStatus_[6] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 6);
    }
    constexpr auto F(const std::uint64_t& data) {
        bitStatus_[6] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 6);
    }
    constexpr auto F_Reset() { bitStatus_[6] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto M() const {
        if (bitStatus_[0] == Status::UNKNOWN || bitStatus_[1] == Status::UNKNOWN || bitStatus_[2] == Status::UNKNOWN || bitStatus_[3] == Status::UNKNOWN ||
            bitStatus_[4] == Status::UNKNOWN || status_ == Status::UNKNOWN)
            throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< AbortMode >(static_cast< std::uint64_t >(0X1F) & (value >> 0));
    }
    constexpr auto M(const std::uint64_t& data) {
        bitStatus_[0] = Status::ACTIVE;
        bitStatus_[1] = Status::ACTIVE;
        bitStatus_[2] = Status::ACTIVE;
        bitStatus_[3] = Status::ACTIVE;
        bitStatus_[4] = Status::ACTIVE;
        if (31 < data) throw std::invalid_argument("This field can only hold values up to 31");
        value |= (data << 0);
    }
    constexpr auto M(A64SelectedStackPointer data) {
        bitStatus_[0] = Status::ACTIVE;
        bitStatus_[1] = Status::ACTIVE;
        bitStatus_[2] = Status::ACTIVE;
        bitStatus_[3] = Status::ACTIVE;
        bitStatus_[4] = Status::ACTIVE;
        auto dat_val  = static_cast< std::uint64_t >(data);
        if (31 < dat_val) throw std::invalid_argument("This field can only hold values up to 31");
        value |= (dat_val << 0);
    }
    constexpr auto M_Reset() {
        bitStatus_[0] = Status::UNKNOWN;
        bitStatus_[1] = Status::UNKNOWN;
        bitStatus_[2] = Status::UNKNOWN;
        bitStatus_[3] = Status::UNKNOWN;
        bitStatus_[4] = Status::UNKNOWN;
    }

    constexpr SPSREL3Register() : SpecialRegister() {}
    constexpr explicit SPSREL3Register(const std::uint64_t& val) : SpecialRegister(val) {}
    constexpr explicit SPSREL3Register(const std::uint64_t& val, Status status) : SpecialRegister(val, status) {}
    ~SPSREL3Register() = default;

    NULL_COPY_MOVE(SPSREL3Register)
};
struct SPSRfiqRegister final : public SpecialRegister {
    // C5.2.20 SPSR_fiq, Saved Program Status Register (FIQ mode)
    [[nodiscard]] constexpr auto Value() const {
        if (status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return value;
    }
    constexpr auto Reset() { status_ = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto N() const {
        if (bitStatus_[31] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 31);
    }
    constexpr auto N(const std::uint64_t& data) {
        bitStatus_[31] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 31);
    }
    constexpr auto N_Reset() { bitStatus_[31] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto Z() const {
        if (bitStatus_[30] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 30);
    }
    constexpr auto Z(const std::uint64_t& data) {
        bitStatus_[30] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 30);
    }
    constexpr auto Z_Reset() { bitStatus_[30] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto C() const {
        if (bitStatus_[29] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 29);
    }
    constexpr auto C(const std::uint64_t& data) {
        bitStatus_[29] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 29);
    }
    constexpr auto C_Reset() { bitStatus_[29] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto V() const {
        if (bitStatus_[28] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 28);
    }
    constexpr auto V(const std::uint64_t& data) {
        bitStatus_[28] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 28);
    }
    constexpr auto V_Reset() { bitStatus_[28] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto Q() const {
        if (bitStatus_[27] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 27);
    }
    constexpr auto Q(const std::uint64_t& data) {
        bitStatus_[27] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 27);
    }
    constexpr auto Q_Reset() { bitStatus_[27] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto IT() const {
        if (bitStatus_[10] == Status::UNKNOWN || bitStatus_[11] == Status::UNKNOWN || bitStatus_[12] == Status::UNKNOWN || bitStatus_[13] == Status::UNKNOWN ||
            bitStatus_[14] == Status::UNKNOWN || bitStatus_[15] == Status::UNKNOWN || bitStatus_[25] == Status::UNKNOWN || bitStatus_[26] == Status::UNKNOWN ||
            status_ == Status::UNKNOWN)
            throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        std::uint8_t out_value = 0x00;
        out_value |= static_cast< std::uint8_t >(3) & static_cast< std::uint8_t >(value >> 25);
        out_value |= static_cast< std::uint8_t >(0xFC) & static_cast< std::uint8_t >(value >> 8);
        return static_cast< std::uint64_t >(out_value);
    }
    constexpr auto IT(const std::uint64_t& data) {
        bitStatus_[10] = Status::ACTIVE;
        bitStatus_[11] = Status::ACTIVE;
        bitStatus_[12] = Status::ACTIVE;
        bitStatus_[13] = Status::ACTIVE;
        bitStatus_[14] = Status::ACTIVE;
        bitStatus_[15] = Status::ACTIVE;
        bitStatus_[25] = Status::ACTIVE;
        bitStatus_[26] = Status::ACTIVE;
        if (255 < data) throw std::invalid_argument("This field can only hold values up to 255");
        value |= ((data & 0x00000000'00000003) << 25);
        value |= ((data & 0x00000000'000000FC) << (10 - 2));
    }
    constexpr auto IT_Reset() {
        bitStatus_[10] = Status::UNKNOWN;
        bitStatus_[11] = Status::UNKNOWN;
        bitStatus_[12] = Status::UNKNOWN;
        bitStatus_[13] = Status::UNKNOWN;
        bitStatus_[14] = Status::UNKNOWN;
        bitStatus_[15] = Status::UNKNOWN;
        bitStatus_[25] = Status::UNKNOWN;
        bitStatus_[26] = Status::UNKNOWN;
    }

    // J // NOT USED IN AARCH64

    [[nodiscard]] auto SSBS() const {
        if (Query< SystemSettings, Features, Features::FEAT_SSBS >::result()){
            if (bitStatus_[23] == Status::UNKNOWN || status_ == Status::UNKNOWN)
                throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
            else
                return static_cast< std::uint64_t >(1) & (value >> 23);
        }
        throw undefined_register_access {};
    }
    auto SSBS(const std::uint64_t& data) {
        if (Query< SystemSettings, Features, Features::FEAT_SSBS >::result()){
            [[unlikely]] if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
            else {
                bitStatus_[23] = Status::ACTIVE;
                value |= data << 23;
            }
        }
        else
            throw undefined_register_access {};
    }
    constexpr auto SSBS_Reset() { bitStatus_[23] = Status::UNKNOWN; }

    [[nodiscard]] auto PAN() const {
        if (Query< SystemSettings, Features, Features::FEAT_PAN >::result()){
            if (bitStatus_[22] == Status::UNKNOWN || status_ == Status::UNKNOWN)
                throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
            else
                return static_cast< std::uint64_t >(1) & (value >> 22);
        }
        throw undefined_register_access {};
    }
    auto PAN(const std::uint64_t& data) {
        if (Query< SystemSettings, Features, Features::FEAT_PAN >::result()){
            [[unlikely]] if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
            else {
                bitStatus_[22] = Status::ACTIVE;
                value |= data << 22;
            }
        }
        else
            throw undefined_register_access {};
    }
    constexpr auto PAN_Reset() { bitStatus_[22] = Status::UNKNOWN; }

    [[nodiscard]] auto DIT() const {
        if (Query< SystemSettings, Features, Features::FEAT_DIT >::result()){
            if (bitStatus_[21] == Status::UNKNOWN || status_ == Status::UNKNOWN)
                throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
            else
                return static_cast< std::uint64_t >(1) & (value >> 21);
        }
        throw undefined_register_access {};
    }
    auto DIT(const std::uint64_t& data) {
        if (Query< SystemSettings, Features, Features::FEAT_DIT >::result()){
            [[unlikely]] if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
            else {
                bitStatus_[21] = Status::ACTIVE;
                value |= data << 21;
            }
        }
        else
            throw undefined_register_access {};
    }
    constexpr auto DIT_Reset() { bitStatus_[21] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto IL() const {
        if (bitStatus_[20] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 20);
    }
    constexpr auto IL(const std::uint64_t& data) {
        bitStatus_[20] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 20);
    }
    constexpr auto IL_Reset() { bitStatus_[20] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto GE() const {
        if (bitStatus_[16] == Status::UNKNOWN || bitStatus_[17] == Status::UNKNOWN || bitStatus_[18] == Status::UNKNOWN || bitStatus_[19] == Status::UNKNOWN ||
            status_ == Status::UNKNOWN)
            throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(15) & (value >> 16);
    }
    constexpr auto GE(const std::uint64_t& data) {
        bitStatus_[16] = Status::ACTIVE;
        bitStatus_[17] = Status::ACTIVE;
        bitStatus_[18] = Status::ACTIVE;
        bitStatus_[19] = Status::ACTIVE;
        if (15 < data) throw std::invalid_argument("This field can only hold values up to 15");
        value |= (data << 16);
    }
    constexpr auto GE_Reset() {
        bitStatus_[16] = Status::UNKNOWN;
        bitStatus_[17] = Status::UNKNOWN;
        bitStatus_[18] = Status::UNKNOWN;
        bitStatus_[19] = Status::UNKNOWN;
    }

    [[nodiscard]] constexpr auto E() const {
        if (bitStatus_[9] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 9);
    }
    constexpr auto E(const std::uint64_t& data) {
        bitStatus_[9] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 9);
    }
    constexpr auto E() { bitStatus_[9] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto A() const {
        if (bitStatus_[8] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 8);
    }
    constexpr auto A(const std::uint64_t& data) {
        bitStatus_[8] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 8);
    }
    constexpr auto A() { bitStatus_[8] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto I() const {
        if (bitStatus_[7] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 7);
    }
    constexpr auto I(const std::uint64_t& data) {
        bitStatus_[7] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 7);
    }
    constexpr auto I() { bitStatus_[7] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto F() const {
        if (bitStatus_[6] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 6);
    }
    constexpr auto F(const std::uint64_t& data) {
        bitStatus_[6] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 6);
    }
    constexpr auto F() { bitStatus_[6] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto T() const {
        if (bitStatus_[5] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 5);
    }
    constexpr auto T(const std::uint64_t& data) {
        bitStatus_[5] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 5);
    }
    constexpr auto T() { bitStatus_[5] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto M() const {
        if (bitStatus_[0] == Status::UNKNOWN || bitStatus_[1] == Status::UNKNOWN || bitStatus_[2] == Status::UNKNOWN || bitStatus_[3] == Status::UNKNOWN ||
            bitStatus_[4] == Status::UNKNOWN || status_ == Status::UNKNOWN)
            throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< AbortMode >(static_cast< std::uint64_t >(0X1F) & (value >> 0));
    }
    constexpr auto M(const std::uint64_t& data) {
        bitStatus_[0] = Status::ACTIVE;
        bitStatus_[1] = Status::ACTIVE;
        bitStatus_[2] = Status::ACTIVE;
        bitStatus_[3] = Status::ACTIVE;
        bitStatus_[4] = Status::ACTIVE;
        if (31 < data) throw std::invalid_argument("This field can only hold values up to 31");
        value |= (data << 0);
    }
    constexpr auto M(AbortMode data) {
        bitStatus_[0] = Status::ACTIVE;
        bitStatus_[1] = Status::ACTIVE;
        bitStatus_[2] = Status::ACTIVE;
        bitStatus_[3] = Status::ACTIVE;
        bitStatus_[4] = Status::ACTIVE;
        auto dat_val  = static_cast< std::uint64_t >(data);
        if (31 < dat_val) throw std::invalid_argument("This field can only hold values up to 31");
        value |= (dat_val << 0);
    }
    constexpr auto M_Reset() {
        bitStatus_[0] = Status::UNKNOWN;
        bitStatus_[1] = Status::UNKNOWN;
        bitStatus_[2] = Status::UNKNOWN;
        bitStatus_[3] = Status::UNKNOWN;
        bitStatus_[4] = Status::UNKNOWN;
    }

    constexpr SPSRfiqRegister() : SpecialRegister() {}
    constexpr explicit SPSRfiqRegister(const std::uint64_t& val) : SpecialRegister(val) {}
    constexpr explicit SPSRfiqRegister(const std::uint64_t& val, Status status) : SpecialRegister(val, status) {}
    ~SPSRfiqRegister() = default;

    NULL_COPY_MOVE(SPSRfiqRegister)
};
struct SPSRirqRegister final : public SpecialRegister {
    // C5.2.21 SPSR_irq, Saved Program Status Register (IRQ mode)
    [[nodiscard]] constexpr auto Value() const {
        if (status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return value;
    }
    constexpr auto Reset() { status_ = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto N() const {
        if (bitStatus_[31] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 31);
    }
    constexpr auto N(const std::uint64_t& data) {
        bitStatus_[31] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 31);
    }
    constexpr auto N_Reset() { bitStatus_[31] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto Z() const {
        if (bitStatus_[30] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 30);
    }
    constexpr auto Z(const std::uint64_t& data) {
        bitStatus_[30] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 30);
    }
    constexpr auto Z_Reset() { bitStatus_[30] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto C() const {
        if (bitStatus_[29] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 29);
    }
    constexpr auto C(const std::uint64_t& data) {
        bitStatus_[29] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 29);
    }
    constexpr auto C_Reset() { bitStatus_[29] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto V() const {
        if (bitStatus_[28] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 28);
    }
    constexpr auto V(const std::uint64_t& data) {
        bitStatus_[28] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 28);
    }
    constexpr auto V_Reset() { bitStatus_[28] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto Q() const {
        if (bitStatus_[27] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 27);
    }
    constexpr auto Q(const std::uint64_t& data) {
        bitStatus_[27] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 27);
    }
    constexpr auto Q_Reset() { bitStatus_[27] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto IT() const {
        if (bitStatus_[10] == Status::UNKNOWN || bitStatus_[11] == Status::UNKNOWN || bitStatus_[12] == Status::UNKNOWN || bitStatus_[13] == Status::UNKNOWN ||
            bitStatus_[14] == Status::UNKNOWN || bitStatus_[15] == Status::UNKNOWN || bitStatus_[25] == Status::UNKNOWN || bitStatus_[26] == Status::UNKNOWN ||
            status_ == Status::UNKNOWN)
            throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        std::uint8_t out_value = 0x00;
        out_value |= static_cast< std::uint8_t >(3) & static_cast< std::uint8_t >(value >> 25);
        out_value |= static_cast< std::uint8_t >(0xFC) & static_cast< std::uint8_t >(value >> 8);
        return static_cast< std::uint64_t >(out_value);
    }
    constexpr auto IT(const std::uint64_t& data) {
        bitStatus_[10] = Status::ACTIVE;
        bitStatus_[11] = Status::ACTIVE;
        bitStatus_[12] = Status::ACTIVE;
        bitStatus_[13] = Status::ACTIVE;
        bitStatus_[14] = Status::ACTIVE;
        bitStatus_[15] = Status::ACTIVE;
        bitStatus_[25] = Status::ACTIVE;
        bitStatus_[26] = Status::ACTIVE;
        if (255 < data) throw std::invalid_argument("This field can only hold values up to 255");
        value |= ((data & 0x00000000'00000003) << 25);
        value |= ((data & 0x00000000'000000FC) << (10 - 2));
    }
    constexpr auto IT_Reset() {
        bitStatus_[10] = Status::UNKNOWN;
        bitStatus_[11] = Status::UNKNOWN;
        bitStatus_[12] = Status::UNKNOWN;
        bitStatus_[13] = Status::UNKNOWN;
        bitStatus_[14] = Status::UNKNOWN;
        bitStatus_[15] = Status::UNKNOWN;
        bitStatus_[25] = Status::UNKNOWN;
        bitStatus_[26] = Status::UNKNOWN;
    }

    // J // NOT USED IN AARCH64

    [[nodiscard]] auto SSBS() const {
        if (Query< SystemSettings, Features, Features::FEAT_SSBS >::result()){
            if (bitStatus_[23] == Status::UNKNOWN || status_ == Status::UNKNOWN)
                throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
            else
                return static_cast< std::uint64_t >(1) & (value >> 23);
        }
        throw undefined_register_access {};
    }
    auto SSBS(const std::uint64_t& data) {
        if (Query< SystemSettings, Features, Features::FEAT_SSBS >::result()){
            [[unlikely]] if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
            else {
                bitStatus_[23] = Status::ACTIVE;
                value |= data << 23;
            }
        }
        else
            throw undefined_register_access {};
    }
    constexpr auto SSBS_Reset() { bitStatus_[23] = Status::UNKNOWN; }

    [[nodiscard]] auto PAN() const {
        if (Query< SystemSettings, Features, Features::FEAT_PAN >::result()){
            if (bitStatus_[22] == Status::UNKNOWN || status_ == Status::UNKNOWN)
                throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
            else
                return static_cast< std::uint64_t >(1) & (value >> 22);
        }
        throw undefined_register_access {};
    }
    auto PAN(const std::uint64_t& data) {
        if (Query< SystemSettings, Features, Features::FEAT_PAN >::result()){
            [[unlikely]] if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
            else {
                bitStatus_[22] = Status::ACTIVE;
                value |= data << 22;
            }
        }
        else
            throw undefined_register_access {};
    }
    constexpr auto PAN_Reset() { bitStatus_[22] = Status::UNKNOWN; }

    [[nodiscard]] auto DIT() const {
        if (Query< SystemSettings, Features, Features::FEAT_DIT >::result()){
            if (bitStatus_[21] == Status::UNKNOWN || status_ == Status::UNKNOWN)
                throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
            else
                return static_cast< std::uint64_t >(1) & (value >> 21);
        }
        throw undefined_register_access {};
    }
    auto DIT(const std::uint64_t& data) {
        if (Query< SystemSettings, Features, Features::FEAT_DIT >::result()){
            [[unlikely]] if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
            else {
                bitStatus_[21] = Status::ACTIVE;
                value |= data << 21;
            }
        }
        else
            throw undefined_register_access {};
    }
    constexpr auto DIT_Reset() { bitStatus_[21] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto IL() const {
        if (bitStatus_[20] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 20);
    }
    constexpr auto IL(const std::uint64_t& data) {
        bitStatus_[20] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 20);
    }
    constexpr auto IL_Reset() { bitStatus_[20] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto GE() const {
        if (bitStatus_[16] == Status::UNKNOWN || bitStatus_[17] == Status::UNKNOWN || bitStatus_[18] == Status::UNKNOWN || bitStatus_[19] == Status::UNKNOWN ||
            status_ == Status::UNKNOWN)
            throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(15) & (value >> 16);
    }
    constexpr auto GE(const std::uint64_t& data) {
        bitStatus_[16] = Status::ACTIVE;
        bitStatus_[17] = Status::ACTIVE;
        bitStatus_[18] = Status::ACTIVE;
        bitStatus_[19] = Status::ACTIVE;
        if (15 < data) throw std::invalid_argument("This field can only hold values up to 15");
        value |= (data << 16);
    }
    constexpr auto GE_Reset() {
        bitStatus_[16] = Status::UNKNOWN;
        bitStatus_[17] = Status::UNKNOWN;
        bitStatus_[18] = Status::UNKNOWN;
        bitStatus_[19] = Status::UNKNOWN;
    }

    [[nodiscard]] constexpr auto E() const {
        if (bitStatus_[9] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 9);
    }
    constexpr auto E(const std::uint64_t& data) {
        bitStatus_[9] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 9);
    }
    constexpr auto E() { bitStatus_[9] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto A() const {
        if (bitStatus_[8] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 8);
    }
    constexpr auto A(const std::uint64_t& data) {
        bitStatus_[8] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 8);
    }
    constexpr auto A() { bitStatus_[8] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto I() const {
        if (bitStatus_[7] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 7);
    }
    constexpr auto I(const std::uint64_t& data) {
        bitStatus_[7] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 7);
    }
    constexpr auto I() { bitStatus_[7] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto F() const {
        if (bitStatus_[6] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 6);
    }
    constexpr auto F(const std::uint64_t& data) {
        bitStatus_[6] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 6);
    }
    constexpr auto F() { bitStatus_[6] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto T() const {
        if (bitStatus_[5] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 5);
    }
    constexpr auto T(const std::uint64_t& data) {
        bitStatus_[5] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 5);
    }
    constexpr auto T() { bitStatus_[5] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto M() const {
        if (bitStatus_[0] == Status::UNKNOWN || bitStatus_[1] == Status::UNKNOWN || bitStatus_[2] == Status::UNKNOWN || bitStatus_[3] == Status::UNKNOWN ||
            bitStatus_[4] == Status::UNKNOWN || status_ == Status::UNKNOWN)
            throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< AbortMode >(static_cast< std::uint64_t >(0X1F) & (value >> 0));
    }
    constexpr auto M(const std::uint64_t& data) {
        bitStatus_[0] = Status::ACTIVE;
        bitStatus_[1] = Status::ACTIVE;
        bitStatus_[2] = Status::ACTIVE;
        bitStatus_[3] = Status::ACTIVE;
        bitStatus_[4] = Status::ACTIVE;
        if (31 < data) throw std::invalid_argument("This field can only hold values up to 31");
        value |= (data << 0);
    }
    constexpr auto M(AbortMode data) {
        bitStatus_[0] = Status::ACTIVE;
        bitStatus_[1] = Status::ACTIVE;
        bitStatus_[2] = Status::ACTIVE;
        bitStatus_[3] = Status::ACTIVE;
        bitStatus_[4] = Status::ACTIVE;
        auto dat_val  = static_cast< std::uint64_t >(data);
        if (31 < dat_val) throw std::invalid_argument("This field can only hold values up to 31");
        value |= (dat_val << 0);
    }
    constexpr auto M_Reset() {
        bitStatus_[0] = Status::UNKNOWN;
        bitStatus_[1] = Status::UNKNOWN;
        bitStatus_[2] = Status::UNKNOWN;
        bitStatus_[3] = Status::UNKNOWN;
        bitStatus_[4] = Status::UNKNOWN;
    }

    constexpr SPSRirqRegister() : SpecialRegister() {}
    constexpr explicit SPSRirqRegister(const std::uint64_t& val) : SpecialRegister(val) {}
    constexpr explicit SPSRirqRegister(const std::uint64_t& val, Status status) : SpecialRegister(val, status) {}
    ~SPSRirqRegister() = default;

    NULL_COPY_MOVE(SPSRirqRegister)
};
struct SPSRundRegister final : public SpecialRegister {
    // C5.2.22 SPSR_und, Saved Program Status Register (Undefined mode)
    [[nodiscard]] constexpr auto Value() const {
        if (status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return value;
    }
    constexpr auto Reset() { status_ = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto N() const {
        if (bitStatus_[31] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 31);
    }
    constexpr auto N(const std::uint64_t& data) {
        bitStatus_[31] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 31);
    }
    constexpr auto N_Reset() { bitStatus_[31] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto Z() const {
        if (bitStatus_[30] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 30);
    }
    constexpr auto Z(const std::uint64_t& data) {
        bitStatus_[30] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 30);
    }
    constexpr auto Z_Reset() { bitStatus_[30] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto C() const {
        if (bitStatus_[29] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 29);
    }
    constexpr auto C(const std::uint64_t& data) {
        bitStatus_[29] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 29);
    }
    constexpr auto C_Reset() { bitStatus_[29] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto V() const {
        if (bitStatus_[28] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 28);
    }
    constexpr auto V(const std::uint64_t& data) {
        bitStatus_[28] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 28);
    }
    constexpr auto V_Reset() { bitStatus_[28] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto Q() const {
        if (bitStatus_[27] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 27);
    }
    constexpr auto Q(const std::uint64_t& data) {
        bitStatus_[27] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 27);
    }
    constexpr auto Q_Reset() { bitStatus_[27] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto IT() const {
        if (bitStatus_[10] == Status::UNKNOWN || bitStatus_[11] == Status::UNKNOWN || bitStatus_[12] == Status::UNKNOWN || bitStatus_[13] == Status::UNKNOWN ||
            bitStatus_[14] == Status::UNKNOWN || bitStatus_[15] == Status::UNKNOWN || bitStatus_[25] == Status::UNKNOWN || bitStatus_[26] == Status::UNKNOWN ||
            status_ == Status::UNKNOWN)
            throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        std::uint8_t out_value = 0x00;
        out_value |= static_cast< std::uint8_t >(3) & static_cast< std::uint8_t >(value >> 25);
        out_value |= static_cast< std::uint8_t >(0xFC) & static_cast< std::uint8_t >(value >> 8);
        return static_cast< std::uint64_t >(out_value);
    }
    constexpr auto IT(const std::uint64_t& data) {
        bitStatus_[10] = Status::ACTIVE;
        bitStatus_[11] = Status::ACTIVE;
        bitStatus_[12] = Status::ACTIVE;
        bitStatus_[13] = Status::ACTIVE;
        bitStatus_[14] = Status::ACTIVE;
        bitStatus_[15] = Status::ACTIVE;
        bitStatus_[25] = Status::ACTIVE;
        bitStatus_[26] = Status::ACTIVE;
        if (255 < data) throw std::invalid_argument("This field can only hold values up to 255");
        value |= ((data & 0x00000000'00000003) << 25);
        value |= ((data & 0x00000000'000000FC) << (10 - 2));
    }
    constexpr auto IT_Reset() {
        bitStatus_[10] = Status::UNKNOWN;
        bitStatus_[11] = Status::UNKNOWN;
        bitStatus_[12] = Status::UNKNOWN;
        bitStatus_[13] = Status::UNKNOWN;
        bitStatus_[14] = Status::UNKNOWN;
        bitStatus_[15] = Status::UNKNOWN;
        bitStatus_[25] = Status::UNKNOWN;
        bitStatus_[26] = Status::UNKNOWN;
    }

    // J // NOT USED IN AARCH64

    [[nodiscard]] auto SSBS() const {
        if (Query< SystemSettings, Features, Features::FEAT_SSBS >::result()){
            if (bitStatus_[23] == Status::UNKNOWN || status_ == Status::UNKNOWN)
                throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
            else
                return static_cast< std::uint64_t >(1) & (value >> 23);
        }
        throw undefined_register_access {};
    }
    auto SSBS(const std::uint64_t& data) {
        if (Query< SystemSettings, Features, Features::FEAT_SSBS >::result()){
            [[unlikely]] if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
            else {
                bitStatus_[23] = Status::ACTIVE;
                value |= data << 23;
            }
        }
        else
            throw undefined_register_access {};
    }
    constexpr auto SSBS_Reset() { bitStatus_[23] = Status::UNKNOWN; }

    [[nodiscard]] auto PAN() const {
        if (Query< SystemSettings, Features, Features::FEAT_PAN >::result()){
            if (bitStatus_[22] == Status::UNKNOWN || status_ == Status::UNKNOWN)
                throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
            else
                return static_cast< std::uint64_t >(1) & (value >> 22);
        }
        throw undefined_register_access {};
    }
    auto PAN(const std::uint64_t& data) {
        if (Query< SystemSettings, Features, Features::FEAT_PAN >::result()){
            [[unlikely]] if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
            else {
                bitStatus_[22] = Status::ACTIVE;
                value |= data << 22;
            }
        }
        else
            throw undefined_register_access {};
    }
    constexpr auto PAN_Reset() { bitStatus_[22] = Status::UNKNOWN; }

    [[nodiscard]] auto DIT() const {
        if (Query< SystemSettings, Features, Features::FEAT_DIT >::result()){
            if (bitStatus_[21] == Status::UNKNOWN || status_ == Status::UNKNOWN)
                throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
            else
                return static_cast< std::uint64_t >(1) & (value >> 21);
        }
        throw undefined_register_access {};
    }
    auto DIT(const std::uint64_t& data) {
        if (Query< SystemSettings, Features, Features::FEAT_DIT >::result()){
            [[unlikely]] if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
            else {
                bitStatus_[21] = Status::ACTIVE;
                value |= data << 21;
            }
        }
        else 
            throw undefined_register_access {};
    }
    constexpr auto DIT_Reset() { bitStatus_[21] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto IL() const {
        if (bitStatus_[20] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 20);
    }
    constexpr auto IL(const std::uint64_t& data) {
        bitStatus_[20] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 20);
    }
    constexpr auto IL_Reset() { bitStatus_[20] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto GE() const {
        if (bitStatus_[16] == Status::UNKNOWN || bitStatus_[17] == Status::UNKNOWN || bitStatus_[18] == Status::UNKNOWN || bitStatus_[19] == Status::UNKNOWN ||
            status_ == Status::UNKNOWN)
            throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(15) & (value >> 16);
    }
    constexpr auto GE(const std::uint64_t& data) {
        bitStatus_[16] = Status::ACTIVE;
        bitStatus_[17] = Status::ACTIVE;
        bitStatus_[18] = Status::ACTIVE;
        bitStatus_[19] = Status::ACTIVE;
        if (15 < data) throw std::invalid_argument("This field can only hold values up to 15");
        value |= (data << 16);
    }
    constexpr auto GE_Reset() {
        bitStatus_[16] = Status::UNKNOWN;
        bitStatus_[17] = Status::UNKNOWN;
        bitStatus_[18] = Status::UNKNOWN;
        bitStatus_[19] = Status::UNKNOWN;
    }

    [[nodiscard]] constexpr auto E() const {
        if (bitStatus_[9] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 9);
    }
    constexpr auto E(const std::uint64_t& data) {
        bitStatus_[9] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 9);
    }
    constexpr auto E() { bitStatus_[9] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto A() const {
        if (bitStatus_[8] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 8);
    }
    constexpr auto A(const std::uint64_t& data) {
        bitStatus_[8] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 8);
    }
    constexpr auto A() { bitStatus_[8] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto I() const {
        if (bitStatus_[7] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 7);
    }
    constexpr auto I(const std::uint64_t& data) {
        bitStatus_[7] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 7);
    }
    constexpr auto I() { bitStatus_[7] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto F() const {
        if (bitStatus_[6] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 6);
    }
    constexpr auto F(const std::uint64_t& data) {
        bitStatus_[6] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 6);
    }
    constexpr auto F() { bitStatus_[6] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto T() const {
        if (bitStatus_[5] == Status::UNKNOWN || status_ == Status::UNKNOWN) throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< std::uint64_t >(1) & (value >> 5);
    }
    constexpr auto T(const std::uint64_t& data) {
        bitStatus_[5] = Status::ACTIVE;
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 5);
    }
    constexpr auto T() { bitStatus_[5] = Status::UNKNOWN; }

    [[nodiscard]] constexpr auto M() const {
        if (bitStatus_[0] == Status::UNKNOWN || bitStatus_[1] == Status::UNKNOWN || bitStatus_[2] == Status::UNKNOWN || bitStatus_[3] == Status::UNKNOWN ||
            bitStatus_[4] == Status::UNKNOWN || status_ == Status::UNKNOWN)
            throw std::out_of_range("Attempt to access architecturally UNKNOWN value");
        return static_cast< AbortMode >(static_cast< std::uint64_t >(0X1F) & (value >> 0));
    }
    constexpr auto M(const std::uint64_t& data) {
        bitStatus_[0] = Status::ACTIVE;
        bitStatus_[1] = Status::ACTIVE;
        bitStatus_[2] = Status::ACTIVE;
        bitStatus_[3] = Status::ACTIVE;
        bitStatus_[4] = Status::ACTIVE;
        if (31 < data) throw std::invalid_argument("This field can only hold values up to 31");
        value |= (data << 0);
    }
    constexpr auto M(AbortMode data) {
        bitStatus_[0] = Status::ACTIVE;
        bitStatus_[1] = Status::ACTIVE;
        bitStatus_[2] = Status::ACTIVE;
        bitStatus_[3] = Status::ACTIVE;
        bitStatus_[4] = Status::ACTIVE;
        auto dat_val  = static_cast< std::uint64_t >(data);
        if (31 < dat_val) throw std::invalid_argument("This field can only hold values up to 31");
        value |= (dat_val << 0);
    }
    constexpr auto M_Reset() {
        bitStatus_[0] = Status::UNKNOWN;
        bitStatus_[1] = Status::UNKNOWN;
        bitStatus_[2] = Status::UNKNOWN;
        bitStatus_[3] = Status::UNKNOWN;
        bitStatus_[4] = Status::UNKNOWN;
    }

    constexpr SPSRundRegister() : SpecialRegister() {}
    constexpr explicit SPSRundRegister(const std::uint64_t& val) : SpecialRegister(val) {}
    constexpr explicit SPSRundRegister(const std::uint64_t& val, Status status) : SpecialRegister(val, status) {}
    ~SPSRundRegister() = default;

    NULL_COPY_MOVE(SPSRundRegister)
};
struct SSBSRegister final : public SpecialRegister {
    // C5.2.23 SSBS, Speculative Store Bypass Safe
    [[nodiscard]] auto Value() const {
        if (Query< SystemSettings, Features, Features::FEAT_SSBS >::result()) return value;
        throw undefined_register_access {};
    }
    constexpr auto Reset() { value = 0x00000000'00000000; }

    [[nodiscard]] auto SSBS() const {
        if (Query< SystemSettings, Features, Features::FEAT_SSBS >::result()) return static_cast< std::uint64_t >(1) & (value >> 12);
        throw undefined_register_access {};
    }
    constexpr auto SSBS(const std::uint64_t& data) {
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 12);
    }
    constexpr auto SSBS_Reset() { Reset(); }

    constexpr SSBSRegister() : SpecialRegister() {}
    constexpr explicit SSBSRegister(const std::uint64_t& val) : SpecialRegister(val) {}
    constexpr explicit SSBSRegister(const std::uint64_t& val, Status status) : SpecialRegister(val, status) {}
    ~SSBSRegister() = default;

    NULL_COPY_MOVE(SSBSRegister)
};
struct TCORegister final : public SpecialRegister {
    // C5.2.24 TCO, Tag Check Override
    [[nodiscard]] auto Value() const {
        if (Query< SystemSettings, Features, Features::FEAT_MTE >::result()) return value;
        throw undefined_register_access {};
    }
    constexpr auto Reset() { value = 0x00000000'00000000; }

    [[nodiscard]] auto TCO() const {
        if (Query< SystemSettings, Features, Features::FEAT_MTE >::result()) return static_cast< std::uint64_t >(1) & (value >> 25);
        throw undefined_register_access {};
    }
    constexpr auto TCO(const std::uint64_t& data) {
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 25);
    }
    constexpr auto TCO_Reset() { Reset(); }

    constexpr TCORegister() : SpecialRegister() {}
    constexpr explicit TCORegister(const std::uint64_t& val) : SpecialRegister(val) {}
    constexpr explicit TCORegister(const std::uint64_t& val, Status status) : SpecialRegister(val, status) {}
    ~TCORegister() = default;

    NULL_COPY_MOVE(TCORegister)
};
struct UAORegister final : public SpecialRegister {
    // C5.2.25 UAO, User Access Override
    [[nodiscard]] auto Value() const {
        if (Query< SystemSettings, Features, Features::FEAT_UAO >::result()) return value;
        throw undefined_register_access {};
    }
    constexpr auto Reset() { value = 0x00000000'00000000; }

    [[nodiscard]] auto UAO() const {
        if (Query< SystemSettings, Features, Features::FEAT_UAO >::result()) return static_cast< std::uint64_t >(1) & (value >> 23);
        throw undefined_register_access {};
    }
    constexpr auto UAO(const std::uint64_t& data) {
        if (1 < data) throw std::invalid_argument("A bit can only hold 0 or 1");
        value |= (data << 23);
    }
    constexpr auto UAO_Reset() { Reset(); }

    constexpr UAORegister() : SpecialRegister() {}
    constexpr explicit UAORegister(const std::uint64_t& val) : SpecialRegister(val) {}
    constexpr explicit UAORegister(const std::uint64_t& val, Status status) : SpecialRegister(val, status) {}
    ~UAORegister() = default;

    NULL_COPY_MOVE(UAORegister)
};

// Special registers
struct SpecialRegisters {

    constexpr SpecialRegisters() :
        CurrentEL({}),
        DAIF({}),
        DIT({}),
        ELR_EL1({}),
        ELR_EL2({}),
        ELR_EL3({}),
        FPCR({}),
        FPSR({}),
        NZCV({}),
        PAN({}),
        SP_EL0({}),
        SP_EL1({}),
        SP_EL2({}),
        SP_EL3({}),
        SPSel({}),
        SPSR_abt({}),
        SPSR_EL1({}),
        SPSR_EL2({}),
        SPSR_EL3({}),
        SPSR_fiq({}),
        SPSR_irq({}),
        SPSR_und({}),
        SSBS({}),
        TCO({}),
        UAO({}) {}
    ~SpecialRegisters() = default;

    CurrentELRegister CurrentEL; // Current Exception Level
    DAIFRegister      DAIF;      // Interrupt Mask Bits
    DITRegister       DIT;       // Data Independent Timing
    ELREL1Register    ELR_EL1;   // Exception Link Register(EL1)
    ELREL2Register    ELR_EL2;   // Exception Link Register(EL2)
    ELREL3Register    ELR_EL3;   // Exception Link Register(EL3)
    FPCRRegister      FPCR;      // Floating - point Control Register
    FPSRRegister      FPSR;      // Floating-point Status Register
    NZCVRegister      NZCV;      // Condition Flags
    PANRegister       PAN;       // Privileged Access Never
    SPEL0Register     SP_EL0;    // Stack Pointer (EL0)
    SPEL1Register     SP_EL1;    // Stack Pointer (EL1)
    SPEL2Register     SP_EL2;    // Stack Pointer (EL2)
    SPEL3Register     SP_EL3;    // Stack Pointer (EL3)
    SPSelRegister     SPSel;     // Stack Pointer Select
    SPSRabtRegister   SPSR_abt;  // Saved Program Status Register(Abort mode)
    SPSREL1Register   SPSR_EL1;  // Saved Program Status Register (EL1)
    SPSREL2Register   SPSR_EL2;  // Saved Program Status Register (EL2)
    SPSREL3Register   SPSR_EL3;  // Saved Program Status Register (EL3)
    SPSRfiqRegister   SPSR_fiq;  // Saved Program Status Register (FIQ mode)
    SPSRirqRegister   SPSR_irq;  // Saved Program Status Register (IRQ mode)
    SPSRundRegister   SPSR_und;  // Saved Program Status Register (Undefined mode)
    SSBSRegister      SSBS;      // Speculative Store Bypass Safe
    TCORegister       TCO;       // Tag Check Override
    UAORegister       UAO;       // User Access Override
};

} // namespace arm_emu