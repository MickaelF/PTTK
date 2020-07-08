#include "generatorarguments.h"
#include <string_view>
#include <iostream>
#include <algorithm>
#include <string>
#include "numbergenerator.h"
#include "log.h"

namespace
{
	constexpr std::string_view helpArg{ "-h" };
	constexpr std::string_view outputArg{ "-o" };
	constexpr std::string_view nbLineArg{ "-n" };
    constexpr int minValue{100};
    constexpr int maxValue{100000};
}

void GeneratorArguments::displayHelp() const
{
	std::cout << "LogGenerator - Help\n"
		<< "\n"
		<< "Create a new log file with awaited features.\n"
		<< "Call: LogGenerator -o {output_path} [options]\n"
		<< "Options: \n"
		<< "\t-h\n"
		<< "\t\tDisplay help\n"
		<< "\t-n {integer}"
		<< "\t\tNumber of lines of the output(max 1 000 000 lines)\n"
		<< std::endl;
}

GeneratorArguments::GeneratorArguments(int argc, char* argv[])
{
    std::string_view nbLinesStr;
    for (int i = 1; i < argc; ++i)
	{
		if (argv[i] == helpArg)
        {
			displayHelp();
            return;
        }
		else if (argv[i] == outputArg)
			m_outputPath = argv[++i];
		else if (argv[i] == nbLineArg)
			nbLinesStr = argv[++i];
		else
			std::cerr << "Unknown argument " << argv[i] << " ." << std::endl;
	}

    if (!nbLinesStr.empty())
        m_nbLines = std::clamp(std::stoi(nbLinesStr.data()), minValue, maxValue);
    else
        m_nbLines = NumberGenerator::generateBetween(minValue, maxValue);

    std::filesystem::create_directories(m_outputPath.parent_path());
    
}

