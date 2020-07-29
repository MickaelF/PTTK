#pragma once
#include <sstream>
#include <string_view>
//#include <source_location> As soon as it is available, use this instead of defined macros
#include <mutex>

#include "logger.h"
#include "macro.h"

#include <iostream>


enum class LogPriority
{
    Debug,
    Info,
    Warning,
    Error,
    Fatal,
    Remember,
    Execution
};


template <bool Console, bool Generator>
class Log
{
public:
    Log(LogPriority p, std::string_view fileName, int lineNumber)
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
        if constexpr (Console)
        {
            static std::mutex m_mutex;
            std::lock_guard<std::mutex> lock {m_mutex};
            std::cout << m_stream.str() << std::endl;
        }
        m_logger->appendLog<Generator>(m_stream.str() + "\n");
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

    static constexpr std::string_view enumToStr(LogPriority p)
    {
        switch (p)
        {
            case LogPriority::Debug: return "Debug";
            case LogPriority::Info: return "Info";
            case LogPriority::Warning: return "Warning";
            case LogPriority::Error: return "Error";
            case LogPriority::Fatal: return "Fatal";
            case LogPriority::Remember: return "Remember";
            case LogPriority::Execution: return "Execution";
            default: return "Unknown";
        }
    }

    static void setLogger(Logger& logger)
    {
        m_logger = &logger;
    }

private:
    std::ostringstream m_stream;
    static Logger* m_logger;
};

template<bool Console, bool Generator>
Logger* Log<Console, Generator>::m_logger{nullptr};

#define BasicLog Log<true, false>
#define lFatal BasicLog(LogPriority::Fatal, __FILE__, __LINE__)
#define lError BasicLog(LogPriority::Error, __FILE__, __LINE__)
#define lInfo BasicLog(LogPriority::Info, __FILE__, __LINE__)
#define lWarning BasicLog(LogPriority::Warning, __FILE__, __LINE__)
#define lDebug BasicLog(LogPriority::Debug, __FILE__, __LINE__)
#define lRemember BasicLog(LogPriority::Remember, __FILE__, __LINE__)
#define lExecution BasicLog(LogPriority::Execution, __FILE__, __LINE__)
