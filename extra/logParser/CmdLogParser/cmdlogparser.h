#pragma once
#include <functional>
#include <optional>
#include <vector>
#include <map>

#include "../loglineinfo.h"

class CmdLogParser
{
public:
    enum class Sort
    {
        Date,
        Type,
        Files
    };

    CmdLogParser(int argc, char* argv[]);
    CmdLogParser() = delete;
    CmdLogParser(const CmdLogParser&) = delete;
    CmdLogParser(CmdLogParser&&) = delete;

    void exec(); 

private:
    void displayHelp();
    void setFileNames(const std::string& fileNames);
    void createOutput(const std::map<std::string, std::vector<std::string>> logs) const;

    std::optional<std::vector<std::string>> m_parsedPriorities;
    std::string m_inputFile;
    std::string m_outputFile;

    std::optional<std::time_t> m_startDate;
    std::optional<std::time_t> m_endDate;
    std::optional<std::vector<std::string>> m_fileNames;

    Sort m_sort {Sort::Date};
};