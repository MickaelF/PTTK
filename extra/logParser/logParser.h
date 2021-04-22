#pragma once
#include <pttk/iprogress.h>
#include <pttk/logger.h>

#include <ctime>
#include <fstream>
#include <functional>
#include <future>
#include <map>
#include <optional>
#include <string>
#include <vector>
class LogLineInfo;

enum class LogSort
{
    Date,
    Type,
    Files
};

class LogParser : public IProgress
{
public:
    LogParser() = default;

    void setInputPath(const std::filesystem::path& folder);

    void setSortType(LogSort sort);
    std::function<std::string_view(const LogLineInfo& info)>
    createRetrieveFunc() const;

    std::map<std::string, std::vector<std::string>> exec();
    void execToFilesNoParam(const std::string& outPath);
    void execToVector(std::promise<std::vector<std::string>>&& p);

    void createComparaisonFunctions(
        const std::optional<std::vector<std::string>>& priorities,
        const std::optional<std::time_t>& startDate,
        const std::optional<std::time_t>& endDate,
        const std::optional<std::vector<std::string>>& fileName);
    int progress() const override { return m_progress; }
    int numberOfLines() const { return m_logData.totalNumberOfLines(); }

private:
    bool comparaisonFunc(LogLineInfo& info) const;

    std::vector<std::string> m_parsedPriorities;
    std::time_t m_startDate {0};
    std::time_t m_endDate {0};
    std::vector<std::string> m_fileNames;
    std::vector<std::function<bool(LogLineInfo&)>> m_logTests;
    LogSort m_sort {LogSort::Date};
    LogDataFile m_logData;
    int m_progress {0};

    int m_currentFile {0};
};
