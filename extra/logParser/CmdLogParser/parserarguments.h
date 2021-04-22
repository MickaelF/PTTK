#pragma once
#include <optional>
#include <vector>
#include <string>
#include <ctime>
#include "logParser.h"

class ParserArguments
{
public:
    ParserArguments(int argc, char* argv[]);
    ParserArguments() = delete;
    ParserArguments(const ParserArguments&) = delete;
    ParserArguments(ParserArguments&&) = delete;

    const std::string& input() const { return m_inputFile; }
    const std::string& output() const { return m_outputFile; }
    const std::optional<std::time_t>& startDate() const { return m_startDate; }
    const std::optional<std::time_t>& endDate() const { return m_endDate; }

    const std::optional<std::vector<std::string>>& fileNames() const { return m_fileNames; }
    const std::optional<std::vector<std::string>>& priorities() const { return m_parsedPriorities; }
    const std::optional<LogSort>& sort() const { return m_sort; }

private:
    void pushPriorities(std::string_view p);
    void displayHelp();
    void setFileNames(const std::string& fileNames);

    std::string m_inputFile;
    std::string m_outputFile;

    std::optional<std::time_t> m_startDate;
    std::optional<std::time_t> m_endDate;
    std::optional<std::vector<std::string>> m_fileNames;
    std::optional<std::vector<std::string>> m_parsedPriorities;

    std::optional<LogSort> m_sort;
};