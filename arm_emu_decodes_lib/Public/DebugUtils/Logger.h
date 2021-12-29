#if !defined(LOGGER_H_INCLUDED_A16F693C_0D0F_4F0C_B361_73FA25DF0D41)
    #define LOGGER_H_INCLUDED_A16F693C_0D0F_4F0C_B361_73FA25DF0D41

    #include <API/Api.h>
    #include <DebugUtils/Clock.h>
    #include <DebugUtils/LogLevel.h>
    #include <DebugUtils/LogType.h>
    #include <fmt/format.h>
    #include <ostream>
    #include <string>

namespace arm_emu {

    class Logger {
      public:
        // Unknown Objects are ignored!
        static constexpr const char* UnknownObject = "UnknownObject";

    #ifndef NDEBUG
        static constexpr const char* AllEntities = "All";

        static void SetLogLevelForTarget(LogLevel state, std::string entity = UnknownObject) noexcept;
    #endif // NDEBUG

        [[nodiscard]] Logger(std::string name) : m_name(std::move(name)) {
        }

        template < class... Args >
        void Log(LogType type, const char* logMsg, Args&&... args) const {
            auto stamp = Clock::Stamp();
            return LogInternal(type, /*Logger::Formatter("{}{}: {}", stamp.m_time,*/
                               Logger::Formatter(logMsg, std::forward< Args >(args)...)) /*)*/;
        }

    #ifndef NDEBUG
        template < class... Args >
        void LogTrace(LogType type, const char* logMsg, Args&&... args) const {
            return LogTraceInternal(type, Formatter(logMsg, std::forward< Args >(args)...));
        }
    #endif // NDEBUG

      private:
        ARMEMU_API void LogInternal(LogType type, std::string&& logMsg) const;

    #ifndef NDEBUG
        ARMEMU_API void LogTraceInternal(LogType type, std::string&& logMsg) const;
    #endif // NDEBUG

        template < class... Args >
        [[nodiscard]] static auto Formatter(const char* logMsg, Args&&... args) {
            // TODO: Add better formatting
            return fmt::vformat(logMsg, fmt::make_format_args(std::forward< Args >(args)...));
        }

        [[nodiscard]] static std::ostream* GetLogTarget() noexcept;

        std::string m_name;
    };

} // namespace arm_emu

#endif // !defined(LOGGER_H_INCLUDED_A16F693C_0D0F_4F0C_B361_73FA25DF0D41)
