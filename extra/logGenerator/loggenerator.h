#pragma once
#include <string_view>
#include <filesystem>
#include <fstream>

class LogGenerator
{
public: 
	LogGenerator(const std::filesystem::path& outputPath, int nbLines);
	void exec(); 

private: 
	std::ofstream m_outStream; 
	const int m_nbLines;
};