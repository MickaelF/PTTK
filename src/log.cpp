#include "log.h"
#include "macro.h"
#include "logger.h"

#ifdef DISPLAY_LOG
    #include <iostream>
#endif

namespace
{
const char* enumToStr(Log::Priority p)
{
    switch (p)
    {
        case Log::Priority::Debug: return "Debug";
        case Log::Priority::Info: return "Info";
        case Log::Priority::Warning: return "Warning";
        case Log::Priority::Error: return "Error";
        case Log::Priority::Fatal: return "Fatal";
        case Log::Priority::Remember: return "Remember";
        case Log::Priority::Execution: return "Execution";
        default: return "Unknown";
    }
}
} // namespace

Log::Log(Priority priority, std::string_view fileName, int lineNumber) : m_priority(priority)
{
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    constexpr char delimiter {'\\'};
#else
    constexpr char delimiter {'/'};
#endif
    m_stream << "[" << enumToStr(priority) << "-"
             << fileName.substr(fileName.find_last_of(delimiter) + 1) << ":" << lineNumber
             << "] - ";
}

Log::~Log()
{
#ifdef DISPLAY_LOG
    static std::mutex m_mutex;
    std::lock_guard<std::mutex> lock {m_mutex};
    std::cout << m_stream.str() << std::endl;
#endif
#if defined(LOG_TO_FILE) || defined(LOG_EXECUTION_TIMERS)
    Logger::appendLog(m_stream.str()+"\n");
#endif
}
