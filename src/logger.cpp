#include "logger.h"

#include "log.h"

namespace
{
constexpr std::string_view logDataFileName {"data.ptlData"};
}

LogDataFile::LogDataFile(const std::filesystem::path& folder, bool createIfNotExisting)
    : m_baseFolder(folder),
      m_dataFilePath(folder.string() + "/" + logDataFileName.data())
{
    load(folder, createIfNotExisting);
}

void LogDataFile::load(const std::filesystem::path& folder, bool createIfNotExisting)
{
    constexpr std::string_view endLine {"</logs>"};
    if (m_baseFolder != folder)
    {
        m_baseFolder = folder;
        m_dataFilePath = folder.string() + "/" + logDataFileName.data();
    }
    if (std::filesystem::exists(m_dataFilePath) && !std::filesystem::is_empty(m_dataFilePath))
    {
        std::ifstream dataFile(m_dataFilePath.c_str());
        std::string line;
        std::getline(dataFile, line);
        parseDataLogInfo(line);
        m_filesInfo.reserve(m_numberOfFiles);
        while (std::getline(dataFile, line) && line != endLine)
            m_filesInfo.emplace_back(parseLogInfo(line));

        m_currentLog.open(m_filesInfo.back().filePath.c_str(),
                          std::ios_base::out | std::ios_base::app);
    }
    else if (std::filesystem::exists(m_dataFilePath) && std::filesystem::is_empty(m_dataFilePath))
    {
        /*lError << "An error occured while trying to read the log file. Every existing logs are "
                  "moved to a backup folder.";*/
        std::filesystem::rename(m_baseFolder, m_baseFolder.string() + "--backup" +
                                                  strTls::currentDateTimeToString("%F-%T"));
        std::filesystem::create_directories(m_baseFolder);
        createNewFile();
        write();
    }
    else if (createIfNotExisting)
    {
        createNewFile();
        write();
    }
    else
    {
        throw std::runtime_error("Could not found log files in specified folder");
    }
}

LogDataFile& LogDataFile::operator=(const LogDataFile& data)
{
    m_baseFolder = data.m_baseFolder;
    m_totalLineNumber = data.m_totalLineNumber;
    m_numberOfFiles = data.m_numberOfFiles;
    m_filesInfo = data.m_filesInfo;
    m_dataFilePath = data.m_dataFilePath;
    return *this;
}

LogDataFile& LogDataFile::operator=(LogDataFile&& data)
{
    m_baseFolder = std::move(data.m_baseFolder);
    m_totalLineNumber = data.m_totalLineNumber;
    m_numberOfFiles = data.m_numberOfFiles;
    m_filesInfo = std::move(data.m_filesInfo);
    m_dataFilePath = std::move(data.m_dataFilePath);

    data.m_totalLineNumber = 0;
    data.m_numberOfFiles = 0;
    return *this;
}

void LogDataFile::write() const
{
    std::ofstream outFile(m_dataFilePath.c_str());
    outFile << "<logs totalLines=\"" << m_totalLineNumber << "\" files=\"" << m_numberOfFiles
            << "\">\n";
    for (const auto& file : m_filesInfo)
        outFile << "\t<file name=" << file.filePath.filename() << " lines=\"" << file.numberLines
                << "\"/>\n";
    outFile << "</logs>";
    outFile.close();
}

std::ofstream& LogDataFile::stream()
{
    return m_currentLog;
}

void LogDataFile::incrementLineNumber(int nbNewLines)
{
    constexpr uintmax_t maxFileSize {5000000}; // 5Mo
    m_totalLineNumber += nbNewLines;
    auto& currentFile = m_filesInfo.back();
    currentFile.numberLines += nbNewLines;
    if (std::filesystem::file_size(currentFile.filePath) >= maxFileSize) createNewFile();
    write();
}

LogInfo LogDataFile::parseLogInfo(std::string_view line) const
{
    constexpr std::string_view fileNameLabel {"name"};
    constexpr std::string_view lineNumberLabel {"lines"};
    size_t nextEqualSign = line.find('=');
    LogInfo info {};
    while (nextEqualSign != std::string::npos)
    {
        auto startId = line.rfind(' ', nextEqualSign) + 1;
        auto endValue = line.find('\"', nextEqualSign + 2);
        auto id = line.substr(startId, nextEqualSign - startId);
        auto value = line.substr(nextEqualSign + 2, endValue - nextEqualSign - 2);

        if (id == fileNameLabel)
            info.filePath = std::filesystem::path(m_baseFolder.string() + "/" + std::string(value));
        else if (id == lineNumberLabel)
            info.numberLines = std::stoi(value.data());
        nextEqualSign = line.find('=', endValue);
    }
    return info;
}

void LogDataFile::createNewFile()
{
    constexpr std::string_view baseFileName {"part"};
    constexpr std::string_view fileStem {".ptLog"};
    std::filesystem::path path = m_baseFolder;
    path.append(baseFileName.data() + std::to_string(++m_numberOfFiles) + fileStem.data());
    m_filesInfo.push_back(LogInfo {path, 0});
    if (m_currentLog.is_open()) m_currentLog.close();
    m_currentLog.open(path.c_str());
}

void LogDataFile::parseDataLogInfo(std::string_view line)
{
    constexpr std::string_view lineNumberLabel {"totalLines"};
    constexpr std::string_view fileNumber {"files"};

    size_t nextEqualSign = line.find('=');
    while (nextEqualSign != std::string::npos)
    {
        auto startId = line.rfind(' ', nextEqualSign) + 1;
        auto endValue = line.find('\"', nextEqualSign + 2);
        auto id = line.substr(startId, nextEqualSign - startId);
        auto value = line.substr(nextEqualSign + 2, endValue - nextEqualSign - 2);
        if (id == lineNumberLabel)
            m_totalLineNumber = std::stoi(value.data());
        else if (id == fileNumber)
            m_numberOfFiles = std::stoi(value.data());
        nextEqualSign = line.find('=', endValue);
    }
}

Logger::Logger(const std::filesystem::path& logFilePath) : m_data(logFilePath)
{
    m_loggingThread = std::thread {&Logger::flush, this};
}

Logger::~Logger()
{
    close();
}

void Logger::waitForEmpty()
{
    using namespace std::chrono_literals;
    while (!m_mainThreadLogQueue.empty() || !m_logQueue.empty())
    { std::this_thread::sleep_for(10ms); }
}

void Logger::close()
{
    waitForEmpty();
    m_isRunning = false;
    m_loggingThread.join();
}

std::string Logger::currentDate()
{
    return strTls::currentDateTimeToString("[%Y-%m-%d %T]");
}

void Logger::setSpecificLogDate(const std::string& date)
{
    m_specificDate = date;
}

void Logger::flush()
{
    using namespace std::chrono_literals;
    constexpr auto waitDuration {500ms};
    while (m_isRunning)
    {
        if (!m_mainThreadLogQueue.empty())
        {
            m_logQueue.swap(m_mainThreadLogQueue);
            int nbNewLines = m_logQueue.size();
            while (!m_logQueue.empty())
            {
                m_data.stream() << m_logQueue.front();
                m_logQueue.pop();
            }
            m_data.stream() << std::flush;
            m_data.incrementLineNumber(nbNewLines);
        }
        std::this_thread::sleep_for(waitDuration);
    }
}
