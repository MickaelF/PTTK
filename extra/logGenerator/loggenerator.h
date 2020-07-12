#pragma once
#include <string_view>
#include <filesystem>
#include <optional>
#include <fstream>
#include "iprogress.h"

class LogGenerator : public IProgress
{
public: 
	LogGenerator(const std::filesystem::path& outputPath, int nbLines);
    void exec(std::optional<std::time_t> time); 
	int progress() const override { return m_progress; }

private: 
	std::ofstream m_outStream; 
	const int m_nbLines;
    int m_progress {0};
};