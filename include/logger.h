#pragma once
#include <fstream>
#include <queue>
#include <string>
#include <string_view>
#include <thread>
#include "macro.h"

#if defined(LOG_TO_FILE)
class Logger
{
public:
    ~Logger();
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;

    static void appendLog(std::string_view str);
    void append(std::string_view str, std::queue<std::string>& queue);
    static void close();
    static void setFolderPath(std::filesystem::path executableName);
    static void swapStream(std::ofstream& other);

private:
    static std::string m_defaultPath; 
    static Logger& get();
    Logger();
    void flush();
    std::ofstream m_fileStream;
    std::queue<std::string> m_logQueue;
    std::thread m_loggingThread; 
    bool m_isRunning {true};
};
#endif
