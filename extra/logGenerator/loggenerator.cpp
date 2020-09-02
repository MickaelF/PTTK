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
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed vulputate et orci ac mollis. "
        "Praesent.",
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Aenean nibh urna, laoreet non "
        "efficitur ac, maximus vitae velit.\nAliquam commodo sapien sit amet nunc consequat, nec "
        "pellentesque massa ultrices. Mauris.",
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras congue pellentesque lorem, "
        "in faucibus lectus varius at. Donec iaculis rhoncus diam, dignissim volutpat diam "
        "hendrerit blandit. Donec sit amet placerat est. Pellentesque pretium quam eget volutpat "
        "mattis.\nQuisque efficitur sit amet lorem quis faucibus. Sed vitae pretium mauris. Donec "
        "sed condimentum metus, ut convallis lorem. Ut malesuada lectus in libero placerat, sit "
        "amet maximus quam ultrices. Class aptent taciti sociosqu ad litora torquent per conubia "
        "nostra, per inceptos himenaeos. Maecenas vel augue quam.",
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer mattis elit nibh, quis "
        "ornare metus consectetur in. Vivamus sem est, bibendum id lectus a, ultricies vestibulum "
        "mi. Ut ut interdum enim.\nDonec ipsum eros, placerat vel diam quis, lacinia dignissim "
        "tellus. Aliquam nisl felis, venenatis id justo accumsan, finibus elementum risus. "
        "Phasellus ac viverra orci.\nPraesent non blandit nulla, eget dictum est. Maecenas ut ante "
        "nec libero mollis venenatis eget quis augue.\nAliquam vitae diam a dui gravida fermentum. "
        "Sed pellentesque nulla orci, eu volutpat nisl blandit id. Aliquam sapien ex, venenatis in "
        "eros a, fermentum tempor lectus. Etiam accumsan, turpis sed cursus ultrices, diam neque "
        "consequat turpis, at condimentum justo magna at dui. Duis et nunc tristique, semper "
        "sapien vel, ultricies nibh.\nNullam leo mi, feugiat quis porta non, vulputate ac orci."};

    std::time_t now = (m_time.has_value())
                          ? *m_time
                          : std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    Log<false, true>::setLogger(m_logger);
    for (; m_progress < m_nbLines; ++m_progress)
    {
        Priorities priority =
            static_cast<Priorities>(NumberGenerator::generateBetween(0, enumMaxNumber() - 1));
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
