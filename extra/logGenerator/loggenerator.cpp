#include "loggenerator.h"

#include <vector>

#include "log.h"
#include "logger.h"
#include "numbergenerator.h"
#include "stringtools.h"

LogGenerator::LogGenerator(const std::filesystem::path& outputPath, int nbLines,
                           std::optional<std::time_t> time)
    :
      m_outPath(outputPath),m_nbLines(nbLines), m_time(time)
{
    if (!std::filesystem::exists(m_outPath))
        std::filesystem::create_directories(m_outPath);
    std::filesystem::path logFile = m_outPath;
    logFile.append(logFileName);
    m_outStream.open(logFile, std::ios_base::out);
    if (!m_outStream.is_open()) throw std::runtime_error("Cannot create output file.");

    Logger::swapStream(m_outStream, m_outPath);
}

void LogGenerator::exec()
{
    const std::vector<std::string> randomFileName {"file1", "file2", "file3",
                                                   "file4", "file5", "file6"};
    const std::vector<std::string> logText {
        "This is a log on one line.", "This is \na log on two lines", "This\nis\ntoo\nmany\nlines"};

    std::time_t now = (m_time.has_value())
                          ? *m_time
                          : std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    for (; m_progress < m_nbLines; ++m_progress)
    {
        LogPriority priority = static_cast<LogPriority>(NumberGenerator::generateBetween(
            static_cast<int>(LogPriority::Debug), static_cast<int>(LogPriority::Execution)));
        int fileId =
            NumberGenerator::generateBetween(0, static_cast<int>(randomFileName.size() - 1));
        int logTextId = NumberGenerator::generateBetween(0, static_cast<int>(logText.size() - 1));
        int lineNumber = NumberGenerator::generateBetween(0, 300);
        now += NumberGenerator::generateBetween(2, 3600);

        Logger::setSpecificLogDate(strTls::timeTToString(now, "[%D-%T]"));

        Log<true, false>(priority, randomFileName[fileId], lineNumber) << logText[logTextId];
    }
    Logger::waitForEmpty();
    lDebug << "Generation Ended successfully.";
    Logger::swapStream(m_outStream, m_outPath);
    m_outStream.close();
}
