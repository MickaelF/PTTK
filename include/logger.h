#pragma once
#include <fstream>
#include <queue>
#include <string>
#include <string_view>
#include <thread>
#include "macro.h"
#include "stringtools.h"

class Logger
{
public:
    ~Logger();
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    template<bool Specific>
    static void appendLog(std::string_view str)
    {
        get().m_mainThreadLogQueue.push(currentDate<Specific>() + std::string(str));
    }
    
    static void close();
    static void setFolderPath(const char* executableName);

    static void swapStream(std::ofstream& other);
    static void stopUsingSpecificLogDate(); 
    static void setSpecificLogDate(std::string_view date); 

private:
    template<bool Specific>
    static std::string currentDate()
    {
    if constexpr (Specific)
        return get().m_specificDate; 
    else
        return strTls::currentDateTimeToString("[%D-%T]");
    }
    static std::string m_defaultPath; 
    static Logger& get();
    Logger();
    void flush();
    std::ofstream m_fileStream;
    std::queue<std::string> m_mainThreadLogQueue; 
    std::queue<std::string> m_logQueue;
    std::thread m_loggingThread; 
    bool m_isRunning {true};

    std::string m_specificDate; 
};
