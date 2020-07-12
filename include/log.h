#pragma once
#include <sstream>
#include <string_view>
//#include <source_location> As soon as it is available, use this instead of defined macros
#include <mutex>

#include "logger.h"
#include "macro.h"

#ifdef DISPLAY_LOG
    #include <iostream>
#endif

template <bool Specific = false>
class Log
{
public:
    enum class Priority
    {
        Debug,
        Info,
        Warning,
        Error,
        Fatal,
        Remember,
        Execution
    };

    Log(Priority p, std::string_view fileName, int lineNumber)
    {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
        constexpr char delimiter {'\\'};
#else
        constexpr char delimiter {'/'};
#endif
        m_stream << "[" << enumToStr(p) << "-"
                 << fileName.substr(fileName.find_last_of(delimiter) + 1) << ":" << lineNumber
                 << "] - ";
    }

    ~Log()
    {
        static std::mutex m_mutex;
        std::lock_guard<std::mutex> lock {m_mutex};
        std::cout << m_stream.str() << std::endl;
        Logger::appendLog<Specific>(m_stream.str() + "\n");
    }

    Log(Log& o) = delete;
    Log(Log&& o) = delete;
    Log& operator=(Log& o) = delete;
    Log& operator=(Log&& o) = delete;

    Log& operator<<(bool b)
    {
        m_stream << std::boolalpha << b;
        return *this;
    }

    Log& operator<<(const std::string& message)
    {
        m_stream << message;
        return *this;
    }

    Log& operator<<(const char* message)
    {
        m_stream << std::string(message);
        return *this;
    }

    Log& operator<<(int message)
    {
        m_stream << message;
        return *this;
    }

    Log& operator<<(unsigned int message)
    {
        m_stream << message;
        return *this;
    }

    Log& operator<<(float message)
    {
        m_stream << message;
        return *this;
    }

    Log& operator<<(double message)
    {
        m_stream << message;
        return *this;
    }

    Log& operator<<(char message)
    {
        m_stream << message;
        return *this;
    }

    Log& operator<<(int64_t message)
    {
        m_stream << message;
        return *this;
    }

    static constexpr char* enumToStr(Log::Priority p)
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

private:
    std::ostringstream m_stream;
};

#define LogPriority Log<false>::Priority
#define LogGeneratorPriority Log<true>::Priority

#define BasicLog Log<false>
#define lFatal BasicLog(LogPriority::Fatal, __FILE__, __LINE__)
#define lError BasicLog(LogPriority::Error, __FILE__, __LINE__)
#define lInfo BasicLog(LogPriority::Info, __FILE__, __LINE__)
#define lWarning BasicLog(LogPriority::Warning, __FILE__, __LINE__)
#define lDebug BasicLog(LogPriority::Debug, __FILE__, __LINE__)
#define lRemember BasicLog(LogPriority::Remember, __FILE__, __LINE__)
#define lExecution BasicLog(LogPriority::Execution, __FILE__, __LINE__)
