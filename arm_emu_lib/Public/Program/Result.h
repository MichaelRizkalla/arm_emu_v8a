#if !defined(RESULT_H_INCLUDED_8464974F_949F_4271_A68B_E6E862CDF441)
    #define RESULT_H_INCLUDED_8464974F_949F_4271_A68B_E6E862CDF441

    #include <API/Api.h>
    #include <Program/IResult.h>
    #include <Utility/UniqueRef.h>
    #include <cstdint>
    #include <memory>

namespace arm_emu {

    class ARMEMU_API Result final : public IResult {
      public:
        Result(SharedRef< ResultElement > resultElement);
        Result(Result&&) noexcept;
        Result& operator=(Result&&) noexcept;
        ~Result() final;

        Result(const Result&) = delete;
        Result& operator=(const Result&) = delete;

        bool          IsReady() const noexcept final;
        State         GetState() const noexcept final;
        std::uint64_t GetGPRegisterValue(std::uint8_t registerLocation) const final;
        std::uint64_t GetPC() const noexcept final;
        void          WaitReady() final;
        void          WaitForState(IResult::State state) final;
        bool          CanStepIn() const noexcept final;

      private:
        class Impl;
        UniqueRef< Impl > m_result;

        static UniqueRef< Impl > ConstructResultImpl(SharedRef< ResultElement >&& resultElement);
    };

} // namespace arm_emu

#endif // !defined(RESULT_H_INCLUDED_8464974F_949F_4271_A68B_E6E862CDF441)
