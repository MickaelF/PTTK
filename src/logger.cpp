#include "logger.h"
#ifdef LOG_TO_FILE || LOG_EXECUTION_TIMERS
#include <filesystem>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    #include <ShlObj.h>
#endif
#include <sstream>

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

// Borrowed from https://kjellkod.wordpress.com/2013/01/22/exploring-c11-part-2-localtime-and-time-again/
std::tm threadSafeLocalTime(const std::time_t& time)
{
    std::tm tm_snapshot;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    localtime_s(&tm_snapshot, &time);
#else
    localtime_r(&time, &tm_snapshot); // POSIX
#endif
    return tm_snapshot;
}

constexpr std::string_view logFileName {"log.txt"};
constexpr std::string_view executionFileName {"execution.txt"};
constexpr std::string_view companyName {"PotatoThunder"};
} // namespace

void Logger::setFolderPath(std::string_view path) 
{
    m_defaultPath = std::move(std::string(companyName) + '/' + std::string(path));
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
#ifdef LOG_EXECUTION_TIMERS
    auto executionLogPath = basePath;
    executionLogPath.append(executionFileName);
    m_executionFileStream.open(executionLogPath, std::ios_base::out | std::ios_base::app);
    isOpened = isOpened && m_executionFileStream.is_open();
#endif // LOG_EXECUTION_TIMERS
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

#ifdef LOG_EXECUTION_TIMERS
void Logger::appendExecutionLog(std::string_view str)
{
    get().append(str, get().m_executionLogQueue);
}
#endif

void Logger::append(std::string_view str, std::queue<std::string>& queue)
{
    std::time_t end_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream ss;
    ss << std::put_time(&threadSafeLocalTime(end_time), "[%D-%T]");
    queue.push(std::move(ss.str() + std::string(str)));
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

#ifdef LOG_EXECUTION_TIMERS
        if (!m_executionLogQueue.empty())
        {
            while (!m_executionLogQueue.empty())
            {
                m_executionFileStream << m_executionLogQueue.front();
                m_executionLogQueue.pop();
            }
            m_executionFileStream << std::flush;
        }
#endif //LOG_EXECUTION_TIMERS
        std::this_thread::sleep_for(std::chrono::milliseconds {500});
    }
    m_fileStream.close();
}
#endif