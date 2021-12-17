#if !defined(RESULTELEMENT_H_INCLUDED_807CE4C4_75FF_43A3_B9D7_7850F8E2B1DC)
    #define RESULTELEMENT_H_INCLUDED_807CE4C4_75FF_43A3_B9D7_7850F8E2B1DC

    #include <API/API.h>
    #include <API/HiddenAPI.h>
    #include <Interrupt/Interrupt.h>
    #include <Program/IResult.h>
    #include <array>
    #include <bitset>
    #include <cassert>
    #include <condition_variable>
    #include <mutex>
    #include <optional>

BEGIN_NAMESPACE

struct ARMEMU_API ResultElement : public std::enable_shared_from_this< ResultElement > {

    ~ResultElement();

    void Signal(IResult::State state) noexcept;
    void StoreGPRegisters(std::array< std::bitset< 64 >, 31 > registerData) noexcept;

    IResult::State GetState() const noexcept;
    std::uint64_t  GetGPRegisterValue(std::uint8_t registerLocation) const;

    void WaitReady();
    void WaitForState(IResult::State state);

    void      StepInSetup(std::condition_variable_any& condVar) noexcept;
    void      StepInFinalize() noexcept;
    Interrupt GetStepInInterrupt() noexcept;

    void StepIn();

  private:
    /* TODO: rework step in function */
    std::array< std::bitset< 64 >, 31 > m_data {};
    IResult::State                      m_state { IResult::State::Waiting };
    std::mutex                          m_mutex {};
    std::condition_variable             m_condVar {};
    Interrupt                           m_stepInSource { CreateInterrupt() };
    std::condition_variable_any*        m_stepInCondVar { nullptr };
};

END_NAMESPACE

#endif // !defined(RESULTELEMENT_H_INCLUDED_807CE4C4_75FF_43A3_B9D7_7850F8E2B1DC)
