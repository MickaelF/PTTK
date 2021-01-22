#pragma once
#include <sstream>
#include <string_view>
//#include <source_location> As soon as it is available, use this instead of defined macros
#include <iostream>
#include <mutex>

#include "logger.h"
#include "macro.h"
#include "logpriority.h"
#include "pttkpath.h"

template <bool Console, bool Generator>
class Log
{
public:
    Log(LogPriority::Priorities p, std::string_view fileName, int lineNumber)
    {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
        constexpr char delimiter {'\\'};
#else
        constexpr char delimiter {'/'};
#endif
        m_stream << "[" << LogPriority::enumToStr(p) << "-"
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

    Log& operator<<(size_t message)
    {
        m_stream << message;
        return *this;
    }
    static void setupAsDefault(std::string_view execName, std::string_view companyName) 
    {
        const auto dataPath {
        pttkPath::getDataPath(execName, companyName)};

        static Logger logger(dataPath);
        setLogger(logger);
    }
    static void setLogger(Logger& logger) { m_logger = &logger; }

private:
    std::ostringstream m_stream;
    static Logger* m_logger;
};

template <bool Console, bool Generator>
Logger* Log<Console, Generator>::m_logger {nullptr};

#define BasicLog Log<true, false>
#define lFatal BasicLog(LogPriority::Priorities::Fatal, __FILE__, __LINE__)
#define lError BasicLog(LogPriority::Priorities::Error, __FILE__, __LINE__)
#define lInfo BasicLog(LogPriority::Priorities::Info, __FILE__, __LINE__)
#define lWarning BasicLog(LogPriority::Priorities::Warning, __FILE__, __LINE__)
#define lDebug BasicLog(LogPriority::Priorities::Debug, __FILE__, __LINE__)
#define lRemember BasicLog(LogPriority::Priorities::Remember, __FILE__, __LINE__)
#define lExecution BasicLog(LogPriority::Priorities::Execution, __FILE__, __LINE__)
