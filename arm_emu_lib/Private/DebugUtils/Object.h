#if !defined(DEBUGOBJECT_H_INCLUDED_CCE60986_4AC5_4F94_8751_44CD47159EBE)
    #define DEBUGOBJECT_H_INCLUDED_CCE60986_4AC5_4F94_8751_44CD47159EBE

    #include <API/Api.h>
    #include <DebugUtils/Logger.h>
    #include <string>

namespace arm_emu {

    class LoggedObject {
      public:
        LoggedObject()                        = default;
        LoggedObject(const LoggedObject&)     = default;
        LoggedObject(LoggedObject&&) noexcept = default;
        LoggedObject& operator=(const LoggedObject&) = default;
        LoggedObject& operator=(LoggedObject&&) = default;
        LoggedObject(std::string name) : m_logger(std::move(name)) {
        }
        virtual ~LoggedObject() = default;

        template < class... Args >
        void Log(LogType type, const char* logMsg, Args&&... args) const {
            m_logger.Log(type, logMsg, std::forward< Args >(args)...);
        }

      protected:
        Logger m_logger { Logger::UnknownObject };
    };

    class DebugObject_ : public LoggedObject {
      public:
        DebugObject_()                        = default;
        DebugObject_(const DebugObject_&)     = default;
        DebugObject_(DebugObject_&&) noexcept = default;
        DebugObject_& operator=(const DebugObject_&) = default;
        DebugObject_& operator=(DebugObject_&&) = default;
        DebugObject_(std::string name) : LoggedObject(std::move(name)) {
        }
        virtual ~DebugObject_() = default;

        template < class... Args >
        void LogTrace(LogType type, const char* logMsg, Args&&... args) const {
            m_logger.LogTrace(type, logMsg, std::forward< Args >(args)...);
        }
    };

    class ReleaseObject_ : public LoggedObject {
      public:
        ReleaseObject_()                          = default;
        ReleaseObject_(const ReleaseObject_&)     = default;
        ReleaseObject_(ReleaseObject_&&) noexcept = default;
        ReleaseObject_& operator=(const ReleaseObject_&) = default;
        ReleaseObject_& operator=(ReleaseObject_&&) = default;
        ReleaseObject_(std::string name) : LoggedObject(std::move(name)) {
        }
        virtual ~ReleaseObject_() = default;

        template < class... >
        void LogTrace(LogType type, const char*, ...) const {
        }
    };

    #ifdef NDEBUG
    using Object = ReleaseObject_;
    #else  // NDEBUG
    using Object = DebugObject_;
    #endif // NDEBUG

} // namespace arm_emu

#endif // !defined(DEBUGOBJECT_H_INCLUDED_CCE60986_4AC5_4F94_8751_44CD47159EBE)
