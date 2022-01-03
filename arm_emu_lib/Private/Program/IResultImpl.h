#if !defined(IRESULTIMPL_H_INCLUDED_9C2AEC68_EF13_4782_8607_129F5AB1D37E)
    #define IRESULTIMPL_H_INCLUDED_9C2AEC68_EF13_4782_8607_129F5AB1D37E

    #include <API/Api.h>
    #include <Memory/IMemory.h>
    #include <Program/IResult.h>
    #include <vector>

BEGIN_NAMESPACE

class IResult::ResultFrame::Impl {
  public:
    Impl();

    Impl(GPRegisters::Arch64Registers registers, std::uint64_t PC, std::uint64_t SP, bool N, bool C, bool Z, bool V,
         std::pmr::vector< IMemory::DataUnit > processMemory);

    std::uint64_t GetGPRegisterValue(std::uint8_t registerLocation) const;

    std::uint64_t GetPC() const noexcept;

    std::uint64_t GetSP() const noexcept;

    bool GetN() const noexcept;

    bool GetZ() const noexcept;

    bool GetC() const noexcept;

    bool GetV() const noexcept;

    const std::pmr::vector< IMemory::DataUnit >& GetProcessMemory() const noexcept;

  private:
    GPRegisters::Arch64Registers          m_registers;
    std::uint64_t                         m_PC;
    std::uint64_t                         m_SP;
    bool                                  m_N;
    bool                                  m_C;
    bool                                  m_Z;
    bool                                  m_V;
    std::pmr::vector< IMemory::DataUnit > m_processMemory;
};

END_NAMESPACE

#endif // !defined(IRESULTIMPL_H_INCLUDED_9C2AEC68_EF13_4782_8607_129F5AB1D37E)
