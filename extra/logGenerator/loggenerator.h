#pragma once
#include <string_view>
#include <filesystem>
#include <optional>
#include <fstream>
#include "iprogress.h"
#include "logger.h"

class LogGenerator : public IProgress
{
public: 
    LogGenerator(const std::filesystem::path& outputPath, int nbLines,
                 std::optional<std::time_t> time = std::nullopt);
    void exec(); 
	int progress() const override { return m_progress; }

private: 
	std::ofstream m_outStream; 
    std::filesystem::path m_outPath;
	const int m_nbLines;
    int m_progress {0};
    std::optional<std::time_t> m_time;
    Logger m_logger;
};
