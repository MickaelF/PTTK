#pragma once
#include <fstream>
#include <queue>
#include <string>
#include <string_view>
#include <thread>
#include "macro.h"
#include "stringtools.h"
#include <filesystem>

struct LogInfo
{
    std::filesystem::path filePath;
    int numberLines;
};

class LogDataFile
{
public:
    LogDataFile(const std::filesystem::path& folder);
    LogDataFile& operator=(const LogDataFile& data);
    LogDataFile& operator=(LogDataFile&& data); 

    void write() const;
    std::ofstream& stream();
    void incrementLineNumber(int nbNewLines);
    const std::vector<LogInfo> files() const { return m_filesInfo; }
    int totalNumberOfLines() const { return m_totalLineNumber;}

private:
    void parseDataLogInfo(std::string_view line);
    LogInfo parseLogInfo(std::string_view line) const;
    void createNewFile();

    int m_totalLineNumber{0};
    int m_numberOfFiles{0};
    std::vector<LogInfo> m_filesInfo;
    std::filesystem::path m_baseFolder;
    std::filesystem::path m_dataFilePath;
    std::ofstream m_currentLog;
};

class Logger
{
public:    
    Logger(const std::filesystem::path& logFilePath) : m_data(logFilePath)
    {
        m_loggingThread = std::thread{ &Logger::flush, this };
    }

    ~Logger()
    {
        close();
    }

    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;

    template <bool SpecificDate>
    void appendLog(std::string_view str)
    {
        if constexpr(SpecificDate)
            m_mainThreadLogQueue.push(currentDate() + std::string(str));
        else
            m_mainThreadLogQueue.push(m_specificDate + std::string(str));
    }

    void waitForEmpty()
    {
        using namespace std::chrono_literals;
        while (!m_mainThreadLogQueue.empty() || !m_logQueue.empty()) {std::this_thread::sleep_for(10ms);}
    }
    
    void close()
    {
        waitForEmpty();
        m_isRunning = false;
        m_loggingThread.join();
    }

    std::string currentDate()
    {
        return strTls::currentDateTimeToString("[%D-%T]");
    }

    void setSpecificLogDate(const std::string& date)
    {
        m_specificDate = date;
    }

private:
    void flush(){
        using namespace std::chrono_literals;
        constexpr auto waitDuration {500ms};
        while (m_isRunning)
        {
            if (!m_mainThreadLogQueue.empty())
            {
                m_logQueue.swap(m_mainThreadLogQueue);
                int nbNewLines = m_logQueue.size();
                while (!m_logQueue.empty())
                {
                    m_data.stream() << m_logQueue.front();
                    m_logQueue.pop();
                }
                m_data.stream() << std::flush;
                m_data.incrementLineNumber(nbNewLines);
            }
            std::this_thread::sleep_for(waitDuration);
        }
    }
    std::queue<std::string> m_mainThreadLogQueue; 
    std::queue<std::string> m_logQueue;
    std::thread m_loggingThread; 
    bool m_isRunning {true};
    LogDataFile m_data;
    std::string m_specificDate;
};
