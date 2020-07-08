#include "logparser.h"

#include <algorithm>

#include "loglineinfo.h"

void LogParser::setInputFile(std::string_view file)
{
    m_stream.open(file.data());
    if (!m_stream.is_open()) throw std::runtime_error("Failed to open the log file !");
}

void LogParser::startAtDate(std::time_t start)
{
    bool notFound = true;
    std::string line;
    while (notFound && std::getline(m_stream, line))
    {
        LogLineInfo info {line};
        if (info.hasInfo() && info.date() > start) notFound = false;
    }
}

std::map<std::string, std::vector<std::string>> LogParser::exec(
    std::function<std::string_view(const LogLineInfo&)> compFunc)
{
    std::map<std::string, std::vector<std::string>> logs;
    std::string line;
    std::string current;
    int j = 0;
    while (std::getline(m_stream, line))
    {
        LogLineInfo info {line};
        if (info.hasInfo())
        {
            if (!comparaisonFunc(info)) continue;
            current = compFunc(info);
        }
        else if (current.empty())
            continue;

        logs[current].push_back(line);
        j++;
    }
    m_stream.close();
    return std::move(logs);
}

bool LogParser::comparaisonFunc(LogLineInfo& info) const
{
    return std::none_of(std::cbegin(m_logTests), std::cend(m_logTests),
                        [&](auto func) { return !func(info); });
}

void LogParser::createComparaisonFunctions(
    const std::optional<std::vector<std::string>>& priorities,
    const std::optional<std::time_t>& endDate,
    const std::optional<std::vector<std::string>>& fileName)
{
    if (priorities.has_value())
    {
        m_parsedPriorities = *priorities;
        auto hasPriority = [&](LogLineInfo& info) -> bool {
            return std::find(std::cbegin(m_parsedPriorities), std::cend(m_parsedPriorities),
                             info.priority()) != std::cend(m_parsedPriorities);
        };
        m_logTests.push_back(hasPriority);
    }

    if (endDate.has_value())
    {
        m_endDate = *endDate;
        auto beforeEndDate = [&](LogLineInfo& info) -> bool { return m_endDate >= info.date(); };
        m_logTests.push_back(beforeEndDate);
    }

    if (fileName.has_value())
    {
        m_fileNames = *fileName;
        auto hasFileName = [&](LogLineInfo& info) -> bool {
            return std::find(std::cbegin(m_fileNames), std::cend(m_fileNames), info.fileName()) !=
                   std::cend(m_fileNames);
        };
        m_logTests.push_back(hasFileName);
    }
}
