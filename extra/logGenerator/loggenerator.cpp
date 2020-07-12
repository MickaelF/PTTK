#include "loggenerator.h"

#include <vector>

#include "log.h"
#include "logger.h"
#include "numbergenerator.h"
#include "stringtools.h"

LogGenerator::LogGenerator(const std::filesystem::path& outputPath, int nbLines)
    : m_nbLines(nbLines)
{
    m_outStream.open(outputPath, std::ios_base::out);
    if (!m_outStream.is_open()) throw std::runtime_error("Cannot create output file.");

    Logger::swapStream(m_outStream);
}

void LogGenerator::exec(std::optional<std::time_t> time)
{
    const std::vector<std::string> randomFileName {"file1", "file2", "file3",
                                                   "file4", "file5", "file6"};
    const std::vector<std::string> logText {
        "This is a log on one line.", "This is \na log on two lines", "This\nis\ntoo\nmany\nlines"};

    std::time_t now = (time.has_value())
                          ? *time
                          : std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    for (m_progress; m_progress < m_nbLines; ++m_progress)
    {
        LogGeneratorPriority priority = static_cast<LogGeneratorPriority>(
            NumberGenerator::generateBetween(static_cast<int>(LogGeneratorPriority::Debug),
                                             static_cast<int>(LogGeneratorPriority::Execution)));
        int fileId =
            NumberGenerator::generateBetween(0, static_cast<int>(randomFileName.size() - 1));
        int logTextId = NumberGenerator::generateBetween(0, static_cast<int>(logText.size() - 1));
        int lineNumber = NumberGenerator::generateBetween(0, 300);
        now += NumberGenerator::generateBetween(2, 3600);

        Logger::setSpecificLogDate(strTls::timeTToString(now, "[%D-%T]"));

        Log<true>(priority, randomFileName[fileId], lineNumber) << logText[logTextId];
    }
    Logger::swapStream(m_outStream);
}