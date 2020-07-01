#pragma once
#include <fstream>
#include <queue>
#include <string>
#include <string_view>
#include <thread>
#include "macro.h"

#if defined(LOG_TO_FILE) || defined(LOG_EXECUTION_TIMERS)
class Logger
{
public:
    ~Logger();
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;

#ifdef LOG_TO_FILE
    static void appendLog(std::string_view str);
#endif
#ifdef LOG_EXECUTION_TIMERS
    static void appendExecutionLog(std::string_view str);
#endif // LOG_EXECUTION_TIMERS

    void append(std::string_view str, std::queue<std::string>& queue);
    static void close();

    static void setFolderPath(std::string_view path);

private:
    static std::string m_defaultPath; 
    static Logger& get();
    Logger();
    void flush();
#ifdef LOG_TO_FILE
    std::ofstream m_fileStream;
    std::queue<std::string> m_logQueue;
#endif
#ifdef LOG_EXECUTION_TIMERS
    std::ofstream m_executionFileStream;
    std::queue<std::string> m_executionLogQueue;
#endif // LOG_EXECUTION_TIMERS

    std::thread m_loggingThread; 
    bool m_isRunning {true};
};
#endif
