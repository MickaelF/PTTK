#include "cmdlogparser.h"

#include <filesystem>
#include <iostream>
#include <string_view>

#include "../logparser.h"
#include "stringtools.h"

namespace
{
constexpr std::string_view helpArg {"-h"};
constexpr std::string_view inputArg {"-i"};
constexpr std::string_view outputArg {"-o"};
constexpr std::string_view rememberArg {"-R"};
constexpr std::string_view errorArg {"-E"};
constexpr std::string_view warningArg {"-W"};
constexpr std::string_view fatalArg {"-F"};
constexpr std::string_view infoArg {"-I"};
constexpr std::string_view executionArg {"-E"};
constexpr std::string_view debugArg {"-D"};
constexpr std::string_view dateStartArg {"--dateStart"};
constexpr std::string_view dateEndArg {"--dateEnd"};
constexpr std::string_view sortArg {"--sort"};
constexpr std::string_view sortDateArg {"date"};
constexpr std::string_view sortTypeArg {"type"};
constexpr std::string_view sortFilesArg {"files"};
constexpr std::string_view fileNameArg {"-f"};

std::string outputDefaultFileName(std::string_view input)
{
    auto dateTime = std::move(strTls::dateTimeToString("%m%d%Y_%H%M%S"));
    auto fileName {"/log-parsed-" + dateTime + ".txt"};

    auto path {std::filesystem::path(input)};

    return path.parent_path().string() + fileName;
}
} // namespace

CmdLogParser::CmdLogParser(int argc, char* argv[])
{
    if (argc == 1)
    {
        displayHelp();
        return;
    }

    for (int i = 1; i < argc; ++i)
    {
        if (argv[i] == helpArg)
            displayHelp();
        else if (argv[i] == inputArg)
            m_inputFile = argv[++i];
        else if (argv[i] == outputArg)
            m_outputFile = argv[++i];
        else if (argv[i] == rememberArg)
            m_parsedPriorities->push_back("Remember");
        else if (argv[i] == errorArg)
            m_parsedPriorities->push_back("Error");
        else if (argv[i] == warningArg)
            m_parsedPriorities->push_back("Warning");
        else if (argv[i] == fatalArg)
            m_parsedPriorities->push_back("Fatal");
        else if (argv[i] == infoArg)
            m_parsedPriorities->push_back("Info");
        else if (argv[i] == debugArg)
            m_parsedPriorities->push_back("Debug");
        else if (argv[i] == executionArg)
            m_parsedPriorities->push_back("Execution");
        else if (argv[i] == dateStartArg)
            m_startDate = strTls::toTimeT(argv[++i], "%D");
        else if (argv[i] == dateEndArg)
            m_endDate = strTls::toTimeT(argv[++i], "%D");
        else if (argv[i] == fileNameArg)
            setFileNames(argv[++i]);
        else if (argv[i] == sortArg)
        {
            i++;
            if (argv[i] == sortDateArg)
                m_sort = Sort::Date;
            else if (argv[i] == sortTypeArg)
                m_sort = Sort::Type;
            else if (argv[i] == sortFilesArg)
                m_sort = Sort::Files;
        }
    }
}

void CmdLogParser::displayHelp()
{
    std::cout
        << "CmdLogParser - Help\n"
        << "\n"
        << "Parse and create a new file with only awaited logs.\n"
        << "Call: CmdLogParser -i {log_file_path} [options]\n"
        << "Options: \n"
        << "\t-h\n"
        << "\t\tDisplay help\n"
        << "\t-i {path_to_log_file}\n"
        << "\t\tWhere to find the log file to parse."
        << "\t-o {path_to_output_file}\n"
        << "\t\tWhere to write result file. By default, it output the parsed file beside "
           "the input file.\n"
        << "\t-R\n"
        << "\t\tDisplay logs with priority \"Remember\"\n"
        << "\t-E\n"
        << "\t\tDisplay logs with priority \"Error\"\n"
        << "\t-F\n"
        << "\t\tDisplay logs with priority \"Fatal\"\n"
        << "\t-W\n"
        << "\t\tDisplay logs with priority \"Warning\"\n"
        << "\t-I\n"
        << "\t\tDisplay logs with priority \"Info\"\n"
        << "\t-D\n"
        << "\t\tDisplay logs with priority \"Debug\"\n"
        << "\t-E\n"
        << "\t\tDisplay logs with priority \"Execution\"\n"
        << "\t-f {filenames;seperated;by;semicolon}\n"
        << "\t\tSelect only the logs from files specified in this option\n"
        << "\t--dateStart MM/DD/YY\n"
        << "\t\tEvery log before this date won't be displayed\n"
        << "\t--dateEnd MM/DD/YY\n"
        << "\t\tEvery log after this date won't be displayed\n"
        << "\t--sort {priority/date/files}\n"
        << "\t\tSort logs by priority, by date or by files. By default, they are sorted by date\n"
        << std::endl;
}

void CmdLogParser::setFileNames(const std::string& fileNames)
{
    m_fileNames = std::move(strTls::split(fileNames, ';'));
}

void CmdLogParser::exec()
{
    if (m_inputFile.empty() || !std::filesystem::exists(m_inputFile))
        throw std::runtime_error("Cannot parser file that does not exists.");

    LogParser parser;
    parser.createComparaisonFunctions(m_parsedPriorities, m_endDate, m_fileNames);
    parser.setInputFile(m_inputFile);
    if (m_startDate.has_value()) parser.startAtDate(*m_startDate);
    std::function<std::string_view(const LogLineInfo& info)> compareFunction; 
    switch (m_sort)
    {
        case Sort::Date:
            compareFunction = [](const LogLineInfo& info)->std::string_view
            {
                return info.dateStr();
            };
            break;
        case Sort::Type:
            compareFunction =
                [](const LogLineInfo& info) -> std::string_view { return info.priority(); };
            break;
        case Sort::Files:
            compareFunction =
                [](const LogLineInfo& info) -> std::string_view { return info.fileName(); };
            break;
        default: throw std::runtime_error("Cannot parse logs with unknown sort selected.");
    }

    auto logs = std::move(parser.exec(compareFunction));
    if (m_outputFile.empty()) m_outputFile = std::move(outputDefaultFileName(m_inputFile));
    createOutput(logs);   
}

void CmdLogParser::createOutput(const std::map<std::string, std::vector<std::string>> logs) const 
{
    std::ofstream outStream; 
    outStream.open(m_outputFile);
    if (!outStream.is_open()) throw std::runtime_error("Could not write in the output file.");

    for (auto pair : logs)
    {
        outStream << pair.first << "\n";
        for (auto line : pair.second) outStream << line << "\n";
        outStream << "\n";
    }
}