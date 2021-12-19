#if !defined(GENERALREGISTERS_H_INCLUDED_4C7B1BF1_279F_403F_9F4D_86B6AB33982C)
    #define GENERALREGISTERS_H_INCLUDED_4C7B1BF1_279F_403F_9F4D_86B6AB33982C

    #include <API/Api.h>
    #include <ProcessingUnit/IProcessingUnit.h>
    #include <Utility/Bitset.h>
    #include <Utility/Utilities.h>
    #include <array>
    #include <bitset>
    #include <variant>

BEGIN_NAMESPACE

enum class RegisterStatus : std::uint32_t
{
    UNKNOWN,
    UNDEFINED,
    ACTIVE,
};

struct [[nodiscard]] GPRegisters {
    
    using Arch64Registers = std::array< std::bitset< 64 >, 32 >;

    [[nodiscard]] auto& PC() noexcept {
        return m_PC;
    }

    [[nodiscard]] auto& PC() const noexcept {
        return m_PC;
    }

    [[nodiscard]] auto& SP() noexcept { // Width 64
        switch (m_PE.EL.to_ulong()) {
            case 0:
                return m_SPEL0.m_value;
                break;
            case 1:
                return m_SPEL1.m_value;
                break;
            case 2:
                return m_SPEL2.m_value;
                break;
            case 3:
                return m_SPEL3.m_value;
                break;
            default:
                assert(false && "Unreachable code path!");
                std::terminate();
        }
    }

    [[nodiscard]] const auto& SP() const noexcept {
        switch (m_PE.EL.to_ulong()) {
            case 0:
                return m_SPEL0.m_value;
                break;
            case 1:
                return m_SPEL1.m_value;
                break;
            case 2:
                return m_SPEL2.m_value;
                break;
            case 3:
                return m_SPEL3.m_value;
                break;
            default:
                assert(false && "Unreachable code path!");
                std::terminate();
        }
    }

    // 64-bit aliases
    [[nodiscard]] auto X(const Bitset& loc) const {
        auto mLoc = loc.ToULong();
        PreConditions(mLoc);

        return m_registers[mLoc];
    }

    // 32-bit aliases
    [[nodiscard]] auto WSP() noexcept { // Width 32
        constexpr std::uint64_t Mask = 0x00000000FFFFFFFF;
        switch (m_PE.EL.to_ulong()) {
            case 0:
                return m_SPEL0.m_value.to_ullong() & Mask;
                break;
            case 1:
                return m_SPEL1.m_value.to_ullong() & Mask;
                break;
            case 2:
                return m_SPEL2.m_value.to_ullong() & Mask;
                break;
            case 3:
                return m_SPEL3.m_value.to_ullong() & Mask;
                break;
            default:
                assert(false && "Unreachable code path!");
                std::terminate();
        }
    }

    [[nodiscard]] void WSP(std::uint32_t data) noexcept { // Width 32
        switch (m_PE.EL.to_ulong()) {
            case 0:
                m_SPEL0.m_value = data;
                break;
            case 1:
                m_SPEL1.m_value = data;
                break;
            case 2:
                m_SPEL2.m_value = data;
                break;
            case 3:
                m_SPEL3.m_value = data;
                break;
            default:
                assert(false && "Unreachable code path!");
                std::terminate();
        }
    }

    [[nodiscard]] void WSP(const std::bitset< 32 >& data) noexcept { // Width 32
        switch (m_PE.EL.to_ulong()) {
            case 0:
                m_SPEL0.m_value = data.to_ullong();
                break;
            case 1:
                m_SPEL1.m_value = data.to_ullong();
                break;
            case 2:
                m_SPEL2.m_value = data.to_ullong();
                break;
            case 3:
                m_SPEL3.m_value = data.to_ullong();
                break;
            default:
                assert(false && "Unreachable code path!");
                std::terminate();
        }
    }

    [[nodiscard]] auto W(const Bitset& loc) noexcept {
        auto mLoc = loc.ToULong();
        PreConditions(mLoc);

        auto returnValue = m_registers[mLoc].to_ullong() & 0x00000000FFFFFFFF;
        return std::bitset< 32 >(static_cast< std::uint32_t >(returnValue));
    }

    [[nodiscard]] auto read(const Bitset& loc) const noexcept {
        auto mLoc = loc.ToULong();
        PreConditions(mLoc);

        return m_registers[mLoc];
    }

    [[nodiscard]] auto read(const std::uint8_t loc) const noexcept {
        PreConditions(loc);

        return m_registers[loc];
    }

    auto write(std::uint8_t loc, const std::uint64_t data) noexcept {
        if (loc == 31) {
            SP() = data;
            return;
        }
        PreConditions(loc);

        m_registers[loc] = data;
    }
    auto write(const Bitset& loc, const std::uint64_t data) noexcept {
        auto mLoc = loc.ToULong();
        if (mLoc == 31) {
            SP() = data;
            return;
        }
        PreConditions(mLoc);

        m_registers[mLoc] = data;
    }
    auto write(const Bitset& loc, std::bitset< 64 > data) noexcept {
        auto mLoc = loc.ToULong();
        if (mLoc == 31) {
            SP() = std::move(data);
            return;
        }
        PreConditions(mLoc);

        m_registers[mLoc] = std::move(data);
    }
    auto write(const Bitset& loc, const std::uint32_t data) noexcept {
        auto mLoc = loc.ToULong();
        if (mLoc == 31) {
            SP() = static_cast< std::uint64_t >(data);
            return;
        }
        PreConditions(mLoc);

        m_registers[mLoc] = static_cast< std::uint64_t >(data);
    }
    auto write(const Bitset& loc, const std::bitset< 32 >& data) noexcept {
        auto mLoc = loc.ToULong();
        if (mLoc == 31) {
            SP() = data.to_ullong();
            return;
        }
        PreConditions(mLoc);

        m_registers[mLoc] = data.to_ullong();
    }
    auto write(const Bitset& loc, const Bitset& data) noexcept {
        auto mLoc = loc.ToULong();
        if (mLoc == 31) {
            assert(data.Size() <= 64);
            SP() = data.ToULLong();
            return;
        }
        PreConditions(mLoc);

        assert(data.Size() <= 64);
        m_registers[mLoc] = data.ToULLong();
    }

    auto ReadBulk() const noexcept {
        return m_registers;
    }

    GPRegisters(const IProcessingUnit::ProcessState& PE) : m_PE(PE) {
    }
    ~GPRegisters() = default;

    GPRegisters(const GPRegisters&) = delete;
    GPRegisters(GPRegisters&&)      = delete;
    GPRegisters& operator=(const GPRegisters&) = delete;
    GPRegisters& operator=(GPRegisters&&) = delete;

    // TODO: constructor
  private:
    struct SPELRegister {
        std::bitset< 64 > m_value { 0 };
        // RegisterStatus    m_status { RegisterStatus::UNKNOWN };
    };

    void PreConditions(std::uint8_t loc) const noexcept {
        assert(loc >= 0 && loc < m_registers.size());
    }


    static constexpr std::size_t Procedural_link_register_index = 30;
    // TODO: register 30's role as a link on procedure calls.
    // R0 - R30 (31 registers)
    // R31 - SP
    // R32 - PC
    Arch64Registers m_registers {};
    // std::bitset< 64 > m_SP { 0 };
    std::uint64_t m_PC { 0 };

    // C5.2.11 SP_EL0, Stack Pointer (EL0)
    SPELRegister m_SPEL0;
    // C5.2.12 SP_EL1, Stack Pointer (EL1)
    SPELRegister m_SPEL1;
    // C5.2.13 SP_EL2, Stack Pointer (EL2)
    SPELRegister m_SPEL2;
    // C5.2.14 SP_EL3, Stack Pointer (EL3)
    SPELRegister m_SPEL3;

    const IProcessingUnit::ProcessState& m_PE;
};

END_NAMESPACE

#endif // !defined(GENERALREGISTERS_H_INCLUDED_4C7B1BF1_279F_403F_9F4D_86B6AB33982C)
