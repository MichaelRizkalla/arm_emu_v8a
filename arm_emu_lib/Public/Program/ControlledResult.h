#if !defined(CONTROLLEDRESULT_H_INCLUDED_409AD162_A520_4607_A4A4_846500EB4904)
    #define CONTROLLEDRESULT_H_INCLUDED_409AD162_A520_4607_A4A4_846500EB4904

    #include <API/Api.h>
    #include <Program/IResult.h>
    #include <Utility/UniqueRef.h>

namespace arm_emu {

    class ARMEMU_API ControlledResult final : public IResult {
      public:
        ControlledResult(SharedRef< ResultElement > resultElement);
        ControlledResult(ControlledResult&&) noexcept;
        ControlledResult& operator=(ControlledResult&&) noexcept;
        ~ControlledResult() final;

        ControlledResult(const ControlledResult&) = delete;
        ControlledResult& operator=(const ControlledResult&) = delete;

        bool          IsReady() const noexcept final;
        State         GetState() const noexcept final;
        std::uint64_t GetGPRegisterValue(std::uint8_t registerLocation) const final;
        std::uint64_t GetPC() const noexcept final;
        void          WaitReady() final;
        void          WaitForState(IResult::State state) final;
        bool          CanStepIn() const noexcept final;

        void StepIn();

      private:
        class Impl;
        UniqueRef< Impl > m_result;

        static UniqueRef< Impl > ConstructResultImpl(SharedRef< ResultElement >&& resultElement);
    };

} // namespace arm_emu

#endif // !defined(CONTROLLEDRESULT_H_INCLUDED_409AD162_A520_4607_A4A4_846500EB4904)
