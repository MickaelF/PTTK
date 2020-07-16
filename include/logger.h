#pragma once
#include <fstream>
#include <filesystem>
#include <queue>
#include <string>
#include <string_view>
#include <thread>
#include "macro.h"
#include "stringtools.h"

constexpr std::string_view logFileName{ "log.ptLog" };
class Logger
{
public:
    class LogDataFile
    {
    public:
        LogDataFile(const std::filesystem::path& path);

        void write() const;

    private:
        const std::string m_filePath;
        std::string m_logFileName;
        std::string m_lastModification;
        int m_nbLines;
    }
    ~Logger();
    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    template<bool Specific>
    static void appendLog(std::string_view str)
    {
        get().m_mainThreadLogQueue.push(currentDate<Specific>() + std::string(str));
    }

    static void waitForEmpty();
    
    static void close();
    static void setFolderPath(const char* executableName);

    static void swapStream(std::ofstream& other, std::filesystem::path& outputPath);
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
    void updateData(int nbNewLines);
    std::ofstream m_fileStream;
    std::queue<std::string> m_mainThreadLogQueue; 
    std::queue<std::string> m_logQueue;
    std::thread m_loggingThread; 
    bool m_isRunning {true};

    std::string m_specificDate; 
    std::filesystem::path m_logPath;
};
