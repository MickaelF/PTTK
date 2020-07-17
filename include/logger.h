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

struct LogInfo
{
    std::filesystem::path filePath;
    int numberLines;
};

class LogDataFile
{
public:

    LogDataFile(const std::filesystem::path& folder);

    void write() const;
    std::ofstream& stream();
    void incrementLineNumber(int nbNewLines);

private:
    void parseDataLogInfo(std::string_view line);
    LogInfo parseLogInfo(std::string_view line) const;
    void createNewFile();

    int m_totalLineNumber{0};
    int m_numberOfFiles{0};
    std::vector<LogInfo> m_filesInfo;
    std::filesystem::path m_baseFolder;
    const std::filesystem::path m_dataFilePath;
    std::ofstream m_currentLog;
};

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
    static std::string m_projectName;
    static Logger& get();
    Logger();
    void flush();
    std::queue<std::string> m_mainThreadLogQueue; 
    std::queue<std::string> m_logQueue;
    std::thread m_loggingThread; 
    bool m_isRunning {true};

    std::string m_specificDate;
    LogDataFile m_data;
};
