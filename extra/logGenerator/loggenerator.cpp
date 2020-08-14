#include "loggenerator.h"

#include <vector>

#include "log.h"
#include "numbergenerator.h"
#include "stringtools.h"

LogGenerator::LogGenerator(const std::filesystem::path& outputPath, int nbLines,
                           std::optional<std::time_t> time)
    : m_outPath(outputPath),
      m_nbLines(nbLines),
      m_time(time),
      m_logger(outputPath)
{
}

void LogGenerator::exec()
{
    using namespace LogPriority;
    const std::vector<std::string> randomFileName {"file1", "file2", "file3",
                                                   "file4", "file5", "file6"};
    const std::vector<std::string> logText {
        "This is a log on one line.",
        "This is a log on one line too, but this one is pretty long. Since every other log are "
        "reaaaally short, this seems like a good idea to have at least one long one, to see what it "
        "looks like.",
        "This is \na log on two lines", "This\nis\ntoo\nmany\nlines"};

    std::time_t now = (m_time.has_value())
                          ? *m_time
                          : std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    Log<false, true>::setLogger(m_logger);
    for (; m_progress < m_nbLines; ++m_progress)
    {
        Priorities priority =
            static_cast<Priorities>(NumberGenerator::generateBetween(0, enumMaxNumber()));
        int fileId =
            NumberGenerator::generateBetween(0, static_cast<int>(randomFileName.size() - 1));
        int logTextId = NumberGenerator::generateBetween(0, static_cast<int>(logText.size() - 1));
        int lineNumber = NumberGenerator::generateBetween(0, 300);
        now += NumberGenerator::generateBetween(2, 3600);
        std::string date = strTls::timeTToString(now, "[%Y-%m-%d %T]");
        m_logger.setSpecificLogDate(date);
        Log<false, true>(priority, randomFileName[fileId], lineNumber) << logText[logTextId];
    }
    lDebug << "Generation Ended successfully.";
    m_outStream.close();
}
