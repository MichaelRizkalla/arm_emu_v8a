#if !defined(IRESULT_H_INCLUDED_872F0592_578C_4899_8572_323DA74740EC)
    #define IRESULT_H_INCLUDED_872F0592_578C_4899_8572_323DA74740EC

    #include <API/Api.h>
    #include <cstdint>

namespace arm_emu {

    class ResultElement;

    class IResult {
      public:
        enum class State
        {
            Waiting,
            Running,
            Ready,
            Interrupted,

            StepInMode,
        };

        IResult()                   = default;
        IResult(IResult&&) noexcept = default;
        IResult& operator=(IResult&&) noexcept = default;
        virtual ~IResult()                     = default;

        IResult(const IResult&) = delete;
        IResult& operator=(const IResult&) = delete;

        /* Minimal functionalities provided by a Result */
        virtual bool          IsReady() const noexcept                                = 0;
        virtual State         GetState() const noexcept                               = 0;
        virtual std::uint64_t GetGPRegisterValue(std::uint8_t registerLocation) const = 0;
        virtual void          WaitReady()                                             = 0;
        virtual void          WaitForState(IResult::State state)                      = 0;
        virtual bool          CanStepIn() const noexcept                              = 0;
    };

} // namespace arm_emu

#endif // !defined(IRESULT_H_INCLUDED_872F0592_578C_4899_8572_323DA74740EC)
