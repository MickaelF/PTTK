#include <iostream>
#include <string_view>
#include "logparser.h"

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
}

void displayHelp() 
{
    std::cout << "LogParser - Help\n"
              << "\n"
              << "Parse and create a new file with only awaited logs.\n"
              << "Call: LogParser -i {log_file_path} [options]\n"
              << "Options: \n"
              << "\t-h\n"
              << "\t\tDisplay help\n"
              << "\t-i path_to_log_file\n"
              << "\t\tWhere to find the log file to parse. By default, it look for the default "
                 "path where log are written.\n"
              << "\t-o path_to_output_file\n"
              << "\t\tWhere to write result file. By default, it's created in the same folder of "
                 "the parsed file.\n"
              << "\t-R\n"
              << "\t\tSelect Remember logs\n"
              << "\t-E\n"
              << "\t\tSelect error logs\n"
              << "\t-F\n"
              << "\t\tSelect fatal logs\n"
              << "\t-W\n"
              << "\t\tSelect warning logs\n"
              << "\t-I\n"
              << "\t\tSelect info logs\n"
              << "\t-D\n"
              << "\t\tSelect debug logs\n"
              << "\t-E\n"
              << "\t\tSelect execution logs\n"
              << "\t-f {filenames;seperated;by;semicolon}\n"
              << "\t\tSelect only the logs from files specified in this option\n"
              << "\t--dateStart MM/DD/YY\n"
              << "\t\tEvery log before this date won't be displayed\n"
              << "\t--dateEnd MM/DD/YY\n"
              << "\t\tEvery log after this date won't be displayed\n"
              << "\t--sort {type/date/files}\n"
              << "\t\tSort logs by type, by date or by files. By default, they are sorted by date\n"
              << std::endl;
}

int main(int argc, char *argv[])
{
    
    if (argc == 1)
    {
        displayHelp();
        return 0;
    }
    std::vector<std::string> priorities; 
    std::string dateStart; 
    std::string dateEnd;
    std::string inputFile; 
    std::string outputFile; 
    std::string fileNames; 
    LogParser::Sort sort{LogParser::Sort::Date};
    for (int i = 1; i < argc; ++i)
    {
        if (argv[i] == helpArg)
            displayHelp();
        else if (argv[i] == inputArg)
            inputFile = argv[++i];
        else if (argv[i] == outputArg)
            outputFile = argv[++i];
        else if (argv[i] == rememberArg)
            priorities.push_back("Remember");
        else if (argv[i] == errorArg)
            priorities.push_back("Error");
        else if (argv[i] == warningArg)
            priorities.push_back("Warning");
        else if (argv[i] == fatalArg)
            priorities.push_back("Fatal");
        else if (argv[i] == infoArg)
            priorities.push_back("Info");
        else if (argv[i] == debugArg)
            priorities.push_back("Debug");
        else if (argv[i] == executionArg)
            priorities.push_back("Execution");
        else if (argv[i] == dateStartArg)
            dateStart = argv[++i];
        else if (argv[i] == dateEndArg)
            dateEnd = argv[++i];
        else if (argv[i] == fileNameArg)
            fileNames = argv[++i];
        else if (argv[i] == sortArg)
        {
            i++;
            if (argv[i] == sortDateArg) sort = LogParser::Sort::Date;
            else if (argv[i] == sortTypeArg) sort = LogParser::Sort::Type;
            else if (argv[i] == sortFilesArg) sort = LogParser::Sort::Files; 
        }
        if (inputFile.empty())
        {
            std::cout << "Input file path is needed to start the execution !\n";
            return -1;
        }
        try
        {
            LogParser parser(std::move(inputFile));
            if (!outputFile.empty())
                parser.setOutputFilePath(std::move(outputFile));
            if (!priorities.empty())
                parser.setParsedLogPriority(std::move(priorities));
            if (!dateStart.empty())
                parser.setStartDate(std::move(dateStart));
            if (!dateEnd.empty())
                parser.setEndDate(std::move(dateEnd));
            if (!fileNames.empty())
                parser.setFileNames(std::move(fileNames));
            parser.setSortTechnique(sort);

            parser.exec();
        }
        catch (std::exception e)
        {
            std::cout << e.what() << std::endl;
            return -1;
        }
    }

    return 0; 
}