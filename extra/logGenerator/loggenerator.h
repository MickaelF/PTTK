#pragma once
#include <string_view>
#include <filesystem>
#include <optional>
#include <fstream>
#include "iprogress.h"

class LogGenerator : public IProgress
{
public: 
	LogGenerator(const std::filesystem::path& outputPath, int nbLines,
                 std::optional<std::time_t> time);
    void exec(); 
	int progress() const override { return m_progress; }

private: 
	std::ofstream m_outStream; 
	const int m_nbLines;
    int m_progress {0};
    std::optional<std::time_t> m_time;
};