#pragma once
#include <pttk/iprogress.h>
#include <pttk/logger.h>

#include <filesystem>
#include <fstream>
#include <optional>
#include <string_view>

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
