#pragma once
#include <ctime>
#include <functional>
#include <optional>
#include <string>
#include <vector>

class LogLineInfo;

class LogParser
{
public:
    enum class Sort
    {
        Date,
        Type,
        Files
    };

    LogParser(std::string inputPath);
    void setParsedLogPriority(std::vector<std::string> priority);

    void setOutputFilePath(std::string path);

    void setStartDate(std::string date);
    void setEndDate(std::string date);

    void setSortTechnique(Sort sort);
    void setFileNames(std::string fileNames);

    void exec();

private:
    bool comparaisonFunc(LogLineInfo& info) const;
    void createComparaisonFunctions();
    void execSortDate(std::ifstream& stream, std::ofstream& outStream) const;
    void execSpecialSort(std::ifstream& stream, std::ofstream& outStream,
                                 std::function<std::string_view(const LogLineInfo&)> func) const;

    std::optional<std::vector<std::string>> m_parsedPriorities;
    std::string m_inputFile;
    std::optional<std::string> m_outputFile;

    std::optional<std::time_t> m_startDate;
    std::optional<std::time_t> m_endDate;
    std::optional<std::vector<std::string>> m_fileNames;
    std::vector<std::function<bool(LogLineInfo&)>> m_logTests;

    Sort m_sort {Sort::Date};
};