#pragma once
#include <ctime>
#include <fstream>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <vector>

class LogLineInfo;

class LogParser
{
public:
    LogParser() = default;

    void setInputFile(std::string_view file);
    void startAtDate(std::time_t start);

    std::map<std::string, std::vector<std::string>> exec(
        std::function<std::string_view(const LogLineInfo&)> compFunc);
    void createComparaisonFunctions(std::optional<std::vector<std::string>>& priorities,
                                    std::optional<std::time_t>& endDate,
                                    std::optional<std::vector<std::string>>& fileName);

private:
    bool comparaisonFunc(LogLineInfo& info) const;

    std::vector<std::string> m_parsedPriorities;
    std::time_t m_endDate {0};
    std::vector<std::string> m_fileNames;
    std::vector<std::function<bool(LogLineInfo&)>> m_logTests;
    std::ifstream m_stream;
};