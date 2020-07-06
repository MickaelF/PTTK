#include <iostream>
#include "logger.h"
#include "log.h"
#include <filesystem>
#include <string>
#include <random>
#include "loggenerator.h"


namespace
{
constexpr std::string_view helpArg {"-h"};
constexpr std::string_view outputArg {"-o"};
constexpr std::string_view nbLineArg {"-n"};
}


void displayHelp() {
    std::cout << "LogGenerator - Help\n"
              << "\n"
              << "Create a new log file with awaited features.\n"
              << "Call: LogGenerator -o {output_path} [options]\n"
              << "Options: \n"
              << "\t-h\n"
              << "\t\tDisplay help\n"
              << "\t-n {integer}"
              << "\t\tNumber of lines of the output\n"
              << std::endl;
}


int main(int argc, char* argv[])
{
    Logger::setFolderPath(argv[0]);
    if (argc == 1)
    {
        displayHelp();
        return 0;
    }

    std::string output; 
    std::string nbLinesStr;

    for (int i = 1; i < argc; ++i) 
    {
        if (argv[i] == helpArg)
            displayHelp();
        else if (argv[i] == outputArg)
            output = argv[++i];
        else if (argv[i] == nbLineArg)
            nbLinesStr = argv[++i];
        else
        {
            std::cerr << "Unknown argument " << argv[i] << " ." << std::endl;
        }
    }

    
    if (output.empty())
    {
        lFatal << "Output path must be specified to be able to run the generator.";
        return -1;
    }

    std::filesystem::path path {output}; 
    try
    {
        std::filesystem::create_directories(path.parent_path());
    }
    catch(std::exception& e)
    {
        lFatal << e.what();
        return -2;
    }
    int nbLines = 0; 
    if (nbLinesStr.empty()) 
    {
        std::random_device rd; // Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
        std::uniform_int_distribution distrib(100, 30000);
        nbLines = distrib(gen);
    }
    else
    {
        try
        {
            nbLines = std::stoi(nbLinesStr);
        }
        catch (std::exception& e)
        {
            lFatal << e.what();
            return -3;
        }
    }

    try
    {
        LogGenerator generator(path, nbLines);
        generator.exec();
    }
    catch(std::exception& e)
    {
        lFatal << e.what();
        return -4;
    }
    lInfo << "Generation succeeded"; 
    return 0;
}