#pragma once
#include <condition_variable>
#include <filesystem>
#include <fstream>
#include <queue>
#include <string>
#include <string_view>
#include <thread>

#include "macro.h"
#include "stringtools.h"

struct LogInfo
{
    std::filesystem::path filePath;
    int numberLines;
};

class LogDataFile
{
public:
    LogDataFile() = default;
    LogDataFile(const std::filesystem::path& folder, bool createIfNotExisting = true);
    LogDataFile& operator=(const LogDataFile& data);
    LogDataFile& operator=(LogDataFile&& data);

    void load(const std::filesystem::path& folder, bool createIfNotExisting = true);
    void write() const;
    std::ofstream& stream();
    void incrementLineNumber(int nbNewLines);
    const std::vector<LogInfo> files() const { return m_filesInfo; }
    int totalNumberOfLines() const { return m_totalLineNumber; }

private:
    void parseDataLogInfo(std::string_view line);
    LogInfo parseLogInfo(std::string_view line) const;
    void createNewFile();

    int m_totalLineNumber {0};
    int m_numberOfFiles {0};
    std::vector<LogInfo> m_filesInfo;
    std::filesystem::path m_baseFolder;
    std::filesystem::path m_dataFilePath;
    std::ofstream m_currentLog;
};

class Logger
{
public:
    Logger(const std::filesystem::path& logFilePath);

    ~Logger();

    Logger(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(const Logger&) = delete;

    template <bool SpecificDate>
    void appendLog(std::string_view str)
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        if constexpr (SpecificDate)
            m_logQueue.push(m_specificDate + std::string(str));
        else
            m_logQueue.push(currentDate() + std::string(str));
    }
    void close();

    std::string currentDate();

    void setSpecificLogDate(const std::string& date);

private:
    void flush();

    std::queue<std::string> m_logQueue;
    std::thread m_loggingThread;
    bool m_isRunning {true};
    LogDataFile m_data;
    std::string m_specificDate;
    std::mutex m_mutex;
    std::condition_variable m_condition;
};
