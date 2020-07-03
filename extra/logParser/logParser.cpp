#include "logparser.h"

#include <filesystem>
#include <fstream>
#include <map>

#include "loglineinfo.h"
#include "stringtools.h"

namespace
{
std::string outputDefaultFileName(std::string_view input)
{
    auto dateTime = std::move(strTls::dateTimeToString("%m%d%Y_%H%M%S"));
    auto fileName {"/log-parsed-" + dateTime + ".txt"};

    auto path {std::filesystem::path(input)};

    return path.parent_path().string() + fileName;
}
} // namespace

LogParser::LogParser(std::string inputPath) : m_inputFile(std::move(inputPath))
{
    std::filesystem::path userPath {m_inputFile};
    if (!std::filesystem::exists(userPath))
        throw std::runtime_error("Cannot parse a file that does not exists.");
}

void LogParser::setParsedLogPriority(std::vector<std::string> priority)
{
    m_parsedPriorities = std::move(priority);
}

void LogParser::setOutputFilePath(std::string path)
{
    std::filesystem::path userPath {path};
    if (!userPath.has_filename() && !std::filesystem::exists(userPath) &&
        !std::filesystem::create_directories(userPath))
        throw std::runtime_error("Could not create directory for output path.");

    m_outputFile = std::move(path);
}

void LogParser::setStartDate(std::string date)
{
    m_startDate = strTls::toTimeT(date, "%D");
}

void LogParser::setEndDate(std::string date)
{
    m_endDate = strTls::toTimeT(date, "%D");
}

void LogParser::setSortTechnique(Sort sort)
{
    m_sort = sort;
}

void LogParser::setFileNames(std::string fileNames)
{
    m_fileNames = std::move(strTls::split(std::move(fileNames), ';'));
}

void LogParser::exec()
{
    createComparaisonFunctions();
    std::ifstream stream;
    stream.open(m_inputFile);
    if (!stream.is_open()) throw std::runtime_error("Failed to open the log file !");

    std::ofstream outStream;
    if (!m_outputFile.has_value()) m_outputFile = outputDefaultFileName(m_inputFile);
    outStream.open(*m_outputFile, std::ios_base::out | std::ios_base::app);
    if (!outStream.is_open()) throw std::runtime_error("Cannot open output file!");

    if (m_startDate.has_value())
    {
        bool notFound = true;
        std::string line;
        while (notFound && std::getline(stream, line))
        {
            LogLineInfo info {line};
            if (info.hasInfo() && info.date() > m_startDate) notFound = false;
        }
    }

    switch (m_sort)
    {
        case Sort::Date: execSortDate(stream, outStream); break;
        case Sort::Type:
            execSpecialSort(stream, outStream, [](const LogLineInfo& info) -> std::string_view {
                return info.priority();
            });
            break;
        case Sort::Files:
            execSpecialSort(stream, outStream, [](const LogLineInfo& info) -> std::string_view {
                return info.fileName();
            });
            break;
        default: break;
    }
    stream.close();
    outStream.close();
}

bool LogParser::comparaisonFunc(LogLineInfo& info) const
{
    return std::none_of(std::cbegin(m_logTests), std::cend(m_logTests),
                        [&](auto func) { return !func(info); });
}

void LogParser::createComparaisonFunctions()
{
    auto hasPriority = [&](LogLineInfo& info) -> bool {
        return std::find(std::cbegin(*m_parsedPriorities), std::cend(*m_parsedPriorities),
                         info.priority()) != std::cend(*m_parsedPriorities);
    };

    auto hasFileName = [&](LogLineInfo& info) -> bool {
        return std::find(std::cbegin(*m_fileNames), std::cend(*m_fileNames), info.fileName()) !=
               std::cend(*m_fileNames);
    };

    auto beforeEndDate = [&](LogLineInfo& info) -> bool { return *m_endDate >= info.date(); };

    if (m_parsedPriorities.has_value()) m_logTests.push_back(hasPriority);
    if (m_endDate.has_value()) m_logTests.push_back(beforeEndDate);
    if (m_fileNames.has_value()) m_logTests.push_back(hasFileName);
}

void LogParser::execSortDate(std::ifstream& stream, std::ofstream& outStream) const
{
    std::string line;
    while (std::getline(stream, line))
    {
        LogLineInfo info {line};
        if (info.hasInfo())
            if (!comparaisonFunc(info)) continue;
        outStream << line << "\n";
    }
}

void LogParser::execSpecialSort(std::ifstream& stream, std::ofstream& outStream,
                                std::function<std::string_view(const LogLineInfo&)> func) const
{
    std::map<std::string, std::vector<std::string>> logs;
    std::string line;
    std::string current;
    while (std::getline(stream, line))
    {
        LogLineInfo info {line};
        if (info.hasInfo())
        {
            if (!comparaisonFunc(info)) continue;
            current = func(info);
        }
        logs[current].push_back(line);
    }

    for (auto pair : logs)
    {
        outStream << pair.first << "\n";
        for (auto line : pair.second) outStream << line << "\n";
        outStream << "\n";
    }
}
