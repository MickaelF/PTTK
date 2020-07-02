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
    void append(std::string_view str, std::queue<std::string>& queue);
    static void close();
    static void setFolderPath(std::filesystem::path executableName);

private:
    static std::string m_defaultPath; 
    static Logger& get();
    Logger();
    void flush();
#ifdef LOG_TO_FILE
    std::ofstream m_fileStream;
    std::queue<std::string> m_logQueue;
#endif
    std::thread m_loggingThread; 
    bool m_isRunning {true};
};
#endif
