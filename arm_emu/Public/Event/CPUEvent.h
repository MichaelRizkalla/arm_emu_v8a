#if !defined(CPUEVENT_H_INCLUDED_6ED404CA_A015_4CC5_BFF5_20EDD76DB4EC)
    #define CPUEVENT_H_INCLUDED_6ED404CA_A015_4CC5_BFF5_20EDD76DB4EC

    #include <CPU/SystemSettings.h>
    #include <Event/IEvent.h>

namespace arm_emu {

    struct CreateCPUEvent : public IEvent {
        CreateCPUEvent(SystemSettings settings);
        EventType                               GetEventType() const noexcept override;
        std::underlying_type_t< EventCategory > GetEventCategories() const noexcept override;

        SystemSettings GetSystemSettings() const noexcept;

      private:
        SystemSettings m_settings;
    };

    struct CPUCreationSuccessEvent : public IEvent {
        CPUCreationSuccessEvent(SystemSettings settings);
        EventType                               GetEventType() const noexcept override;
        std::underlying_type_t< EventCategory > GetEventCategories() const noexcept override;

        SystemSettings GetSystemSettings() const noexcept;

      private:
        SystemSettings m_settings;
    };

    struct CPUCreationFailureEvent : public IEvent {
        CPUCreationFailureEvent(std::string failureMessage);
        EventType                               GetEventType() const noexcept override;
        std::underlying_type_t< EventCategory > GetEventCategories() const noexcept override;

        std::string GetMessage() const noexcept;

      private:
        std::string m_failureMessage;
    };

} // namespace arm_emu

#endif // !defined(CPUEVENT_H_INCLUDED_6ED404CA_A015_4CC5_BFF5_20EDD76DB4EC)
