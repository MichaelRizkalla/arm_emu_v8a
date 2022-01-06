#if !defined(PROGRAMEVENT_H_INCLUDED_4D2997E2_221E_4EEA_B4AF_D25CC14D4561)
    #define PROGRAMEVENT_H_INCLUDED_4D2997E2_221E_4EEA_B4AF_D25CC14D4561

    #include <Compiler/Compiler.h>
    #include <Event/IEvent.h>
    #include <Program/IResult.h>
    #include <Program/Program.h>

namespace arm_emu {

    struct LoadProgramEvent : public IEvent {
        LoadProgramEvent(CompilationResult::ObjectData objectData, Program::EntryPoint entryPoint);
        EventType                               GetEventType() const noexcept override;
        std::underlying_type_t< EventCategory > GetEventCategories() const noexcept override;

        const CompilationResult::ObjectData& GetObjectData() const noexcept;
        Program::EntryPoint                  GetEntryPoint() const noexcept;

      private:
        CompilationResult::ObjectData m_objectData;
        Program::EntryPoint           m_entryPoint;
    };

    struct UnloadProgramEvent : public IEvent {
        EventType                               GetEventType() const noexcept override;
        std::underlying_type_t< EventCategory > GetEventCategories() const noexcept override;
    };

    struct LoadProgramSuccessEvent : public IEvent {
        LoadProgramSuccessEvent(IResult* result);
        EventType                               GetEventType() const noexcept override;
        std::underlying_type_t< EventCategory > GetEventCategories() const noexcept override;

        IResult* GetResult() const noexcept;

      private:
        IResult* m_result;
    };

    struct LoadProgramFailureEvent : public IEvent {
        LoadProgramFailureEvent(std::string failureMessage);
        EventType                               GetEventType() const noexcept override;
        std::underlying_type_t< EventCategory > GetEventCategories() const noexcept override;

        const std::string& GetFailureMessage() const noexcept;

      private:
        std::string m_failureMessage;
    };

    struct StepInProgramAvailableEvent : public IEvent {
        EventType                               GetEventType() const noexcept override;
        std::underlying_type_t< EventCategory > GetEventCategories() const noexcept override;
    };

    struct StepInProgramEvent : public IEvent {
        EventType                               GetEventType() const noexcept override;
        std::underlying_type_t< EventCategory > GetEventCategories() const noexcept override;
    };

    struct StepInProgramFinishedEvent : public IEvent {
        StepInProgramFinishedEvent(bool result);
        EventType                               GetEventType() const noexcept override;
        std::underlying_type_t< EventCategory > GetEventCategories() const noexcept override;

        bool GetResult() const noexcept;

      private:
        bool m_result;
    };

} // namespace arm_emu

#endif // !defined(PROGRAMEVENT_H_INCLUDED_4D2997E2_221E_4EEA_B4AF_D25CC14D4561)
