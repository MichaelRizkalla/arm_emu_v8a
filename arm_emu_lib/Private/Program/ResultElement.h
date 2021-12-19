#if !defined(RESULTELEMENT_H_INCLUDED_807CE4C4_75FF_43A3_B9D7_7850F8E2B1DC)
    #define RESULTELEMENT_H_INCLUDED_807CE4C4_75FF_43A3_B9D7_7850F8E2B1DC

    #include <API/API.h>
    #include <API/HiddenAPI.h>
    #include <Interrupt/Interrupt.h>
    #include <ProcessingUnit/A64Registers/GeneralRegisters.h>
    #include <Program/IResult.h>
    #include <array>
    #include <bitset>
    #include <cassert>
    #include <condition_variable>
    #include <mutex>
    #include <optional>

BEGIN_NAMESPACE

struct ARMEMU_API ResultElement : public std::enable_shared_from_this< ResultElement > {

    ResultElement(bool isStepInAllowed);
    ~ResultElement();

    void Signal(IResult::State state) noexcept;
    void StoreGPRegisters(GPRegisters::Arch64Registers registerData) noexcept;
    void StorePC(std::uint64_t programCounter) noexcept;

    IResult::State GetState() const noexcept;
    std::uint64_t  GetGPRegisterValue(std::uint8_t registerLocation) const;
    std::uint64_t  GetPC() const noexcept;

    void WaitReady();
    void WaitForState(IResult::State state);

    bool                         CanStepIn() const noexcept;
    std::condition_variable_any& StepInSetup(std::condition_variable_any& condVarToTrigger,
                                             Interrupt                    stepInDone) noexcept;
    void                         StepInFinalize() noexcept;
    Interrupt                    GetStepInInterrupt() noexcept;

    void StepIn();
    void SignalStepInValidity(bool isStepInAllowed) noexcept;

  private:
    // TODO: add all read data into frame struct
    GPRegisters::Arch64Registers m_data {};
    std::uint64_t                m_programCounter { 0 };
    IResult::State               m_state { IResult::State::Waiting };
    std::mutex                   m_mutex {};
    std::condition_variable      m_condVar {};

    Interrupt                    m_stepInSource { CreateInterrupt() };
    std::condition_variable_any* m_stepInCondVar { nullptr };

    Interrupt                   m_steppedInInterrupt { nullptr };
    std::condition_variable_any m_steppedIn {};

    bool m_isStepInAllowed;
};

END_NAMESPACE

#endif // !defined(RESULTELEMENT_H_INCLUDED_807CE4C4_75FF_43A3_B9D7_7850F8E2B1DC)
