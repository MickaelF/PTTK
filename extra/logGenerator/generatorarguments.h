#pragma once 
#include <filesystem>

class GeneratorArguments
{
public: 
    explicit GeneratorArguments(int argc, char* argv[]); 
    GeneratorArguments() = delete; 
    GeneratorArguments(const GeneratorArguments&) = delete; 
    GeneratorArguments(GeneratorArguments&&) = delete; 
    GeneratorArguments& operator=(const GeneratorArguments&) = delete; 
    GeneratorArguments& operator=(GeneratorArguments&&) = delete; 

    const std::filesystem::path& path() const { return m_outputPath; }
    int nbLines() const { return m_nbLines; }

private: 
    void displayHelp() const; 

    std::filesystem::path m_outputPath; 
    int m_nbLines{0};
};