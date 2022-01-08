#if !defined(LOG_H_INCLUDED_BE236E88_135B_4B56_B3B2_C412EAECC2B0)
    #define LOG_H_INCLUDED_BE236E88_135B_4B56_B3B2_C412EAECC2B0

    #include <API/Api.h>
    #include <DebugUtils/LogType.h>
    #include <ostream>

namespace arm_emu {

    class ARMEMU_API Log {
      public:
        Log()           = delete;
        Log(const Log&) = delete;
        Log(Log&&)      = delete;
        Log& operator=(const Log&) = delete;
        Log& operator=(Log&&) = delete;
        ~Log()                = delete;

        static void SetLogType(LogType type) noexcept;

        static void SetLogTarget(std::ostream* os) noexcept;

        [[nodiscard]] static LogType GetLogType() noexcept;
    };

} // namespace arm_emu

#endif // !defined(LOG_H_INCLUDED_BE236E88_135B_4B56_B3B2_C412EAECC2B0)
