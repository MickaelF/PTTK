#include "logger.h"
#ifdef LOG_TO_FILE 
#include <filesystem>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    #include <ShlObj.h>
#endif
#include "stringtools.h"

namespace
{
std::filesystem::path getLogPath(std::string_view folderName)
{
    std::filesystem::path programDataPath;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    PWSTR path {NULL};
    HRESULT hr = SHGetKnownFolderPath(FOLDERID_ProgramData, KF_FLAG_DEFAULT, NULL, &path);
    if (SUCCEEDED(hr)) programDataPath = path;
#endif
    programDataPath.append(folderName);
    if (!std::filesystem::exists(programDataPath))
        std::filesystem::create_directories(programDataPath);
    return programDataPath;
}

constexpr std::string_view logFileName {"log.txt"};
constexpr std::string_view executionFileName {"execution.txt"};
constexpr std::string_view companyName {"PotatoThunder"};
} // namespace

void Logger::setFolderPath(std::filesystem::path executableName) 
{
    m_defaultPath = std::move(std::string(companyName) + '/' + executableName.stem().string());
}

std::string Logger::m_defaultPath; 

Logger::Logger()
{
    auto basePath {getLogPath(m_defaultPath)};

    bool isOpened {true};
#ifdef LOG_TO_FILE
    auto logPath = basePath; 
    logPath.append(logFileName);
    m_fileStream.open(logPath, std::ios_base::out | std::ios_base::app);
    isOpened = isOpened && m_fileStream.is_open();
#endif
    if (isOpened)
    {
        m_loggingThread = std::thread{&Logger::flush, this};
    }
}

Logger::~Logger()
{
    close();
}

#ifdef LOG_TO_FILE
void Logger::appendLog(std::string_view str)
{
    get().append(str, get().m_logQueue);
}
#endif

void Logger::append(std::string_view str, std::queue<std::string>& queue)
{
    queue.push(std::move(strTls::dateTimeToString("[%D-%T]") + std::string(str)));
}

void Logger::close()
{
    Logger& logger {get()};
    logger.m_isRunning = false;
    logger.m_loggingThread.join();
}

Logger& Logger::get()
{
    static Logger log;
    return log;
}

void Logger::flush()
{
    while (m_isRunning)
    {
#ifdef LOG_TO_FILE
        if (!m_logQueue.empty())
        {
            while (!m_logQueue.empty())
            {
                m_fileStream << m_logQueue.front();
                m_logQueue.pop();
            }
            m_fileStream << std::flush;
        }
#endif // LOG_TO_FILE
        std::this_thread::sleep_for(std::chrono::milliseconds {500});
    }
    m_fileStream.close();
}
#endif