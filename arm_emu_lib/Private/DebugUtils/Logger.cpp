#include <DebugUtils/Log.h>
#include <DebugUtils/Logger.h>
#include <iostream>
#include <set>

BEGIN_NAMESPACE

namespace {
    static std::ostream* logStream = &std::cout;
    static LogType       logType   = LogType::Other;
#ifndef NDEBUG
    static std::set< std::string > logTraceEntities { Logger::AllEntities };
    static std::set< std::string > logNormalOnly {};
    static std::set< std::string > neverLogEntities {};
#endif // NDEBUG
} // namespace

#ifndef NDEBUG
void Logger::SetLogLevelForTarget(LogLevel state, std::string entity) noexcept {
    if (entity == UnknownObject) {
        return;
    }

    switch (state) {
        case LogLevel::None:
            if (entity == AllEntities) {
                logTraceEntities.clear();
            }
            logTraceEntities.erase(entity);
            neverLogEntities.emplace(std::move(entity));
            break;
        case LogLevel::Normal:
            if (entity == AllEntities) {
                logTraceEntities.clear();
                neverLogEntities.clear();
            } else {
                logTraceEntities.erase(entity);
                neverLogEntities.erase(entity);
                logNormalOnly.emplace(std::move(entity));
            }
            break;
        case LogLevel::Trace:
            if (entity == AllEntities) {
                neverLogEntities.clear();
            }
            neverLogEntities.erase(entity);
            logTraceEntities.emplace(std::move(entity));
            break;
    }
}
#endif // NDEBUG

void Log::SetLogType(LogType type) noexcept {
    logType = type;
}

void Log::SetLogTarget(std::ostream* os) noexcept {
    logStream = os;
}

LogType Log::GetLogType() noexcept {
    return logType;
}

std::ostream* Logger::GetLogTarget() noexcept {
    return logStream;
}

void Logger::LogInternal(LogType type, std::string&& logMsg) const {
    if (!static_cast< bool >(static_cast< std::underlying_type_t< LogType > >(type) &
                            static_cast< std::underlying_type_t< LogType > >(logType))) {
        return;
    }

#ifdef NDEBUG
    (*logStream) << m_name << ": " << logMsg << '\n';
#else  // NDEBUG | !NDEBUG
    if ((neverLogEntities.find(m_name) == neverLogEntities.end()) &&
        !((neverLogEntities.size() == 1) && (*neverLogEntities.begin() == AllEntities))) {
        (*logStream) << m_name << ": " << logMsg << '\n';
    }
#endif // !NDEBUG
}

#ifndef NDEBUG
void Logger::LogTraceInternal(LogType type, std::string&& logMsg) const {
    if (!static_cast< bool >(static_cast< std::underlying_type_t< LogType > >(type) &
                            static_cast< std::underlying_type_t< LogType > >(logType))) {
        return;
    }

    if (logTraceEntities.find(m_name) != logTraceEntities.end()) {
        (*logStream) << m_name << ": " << logMsg << '\n';
    } else if (logTraceEntities.find(AllEntities) != logTraceEntities.end() &&
               logNormalOnly.find(m_name) == logNormalOnly.end()) {
        if (neverLogEntities.find(m_name) == neverLogEntities.end()) {
            (*logStream) << m_name << ": " << logMsg << '\n';
        }
    }
}
#endif // NDEBUG

END_NAMESPACE
