#include "logparser.h"
#include <filesystem>
#include <fstream>
#include <sstream>

LogParser::LogParser(std::string inputPath) : m_input(std::move(inputPath))
{
    std::filesystem::path userPath {inputPath};
    if (!std::filesystem::exists(userPath)) throw std::runtime_error("Cannot parse a file that does not exists : " + inputPath);
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
        throw std::runtime_error("Could not create directory " + path);
    
    m_output = std::move(path);
}

void LogParser::setStartDate(std::string date)
{
    struct std::tm tm{};
    std::istringstream ss(date.c_str());
    ss >> std::get_time(&tm, "%D"); 
    m_startDate = std::move(std::mktime(&tm));
}

void LogParser::setEndDate(std::string date) 
{
    struct std::tm tm{};
    std::istringstream ss(date.c_str());
    ss >> std::get_time(&tm, "%D");
    m_endDate = std::move(std::mktime(&tm));
}

void LogParser::setSortTechnique(Sort sort) 
{
    m_sort = sort;
}

void LogParser::setFileNames(std::string fileNames)
{
    
}

void LogParser::exec()
{
    std::ifstream stream; 
    stream.open(m_input);
    if (!stream.is_open())
        throw std::runtime_error("Failed to open the log file !");
    if (m_startDate.has_value())
    {
        bool found = false; 
        std::string line; 
        while (!found || std::getline(stream, line))
        {
            //if ()
        }
    }
}