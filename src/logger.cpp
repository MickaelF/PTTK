#include "logger.h"
#include <filesystem>
#if defined (WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <ShlObj.h>
#endif
#include "stringtools.h"
#include <mutex>

namespace
{	
	using namespace std::chrono_literals;
	std::filesystem::path getLogPath(std::string_view folderName)
	{
		std::filesystem::path programDataPath;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
		PWSTR path{ NULL };
		HRESULT hr = SHGetKnownFolderPath(FOLDERID_ProgramData, KF_FLAG_DEFAULT, NULL, &path);
		if (SUCCEEDED(hr)) programDataPath = path;
#endif
		programDataPath.append(folderName);
		if (!std::filesystem::exists(programDataPath))
			std::filesystem::create_directories(programDataPath);
		return programDataPath;
	}

	constexpr std::string_view logFileName{ "log.txt" };
	constexpr std::string_view executionFileName{ "execution.txt" };
	constexpr std::string_view companyName{ "PotatoThunder" };
	constexpr auto waitDuration {500ms};

	static std::mutex fileStreamMutex;
} // namespace

void Logger::setFolderPath(const char* executableName)
{
    std::filesystem::path path {executableName};
    m_defaultPath = std::move(std::string(companyName) + '/' + path.stem().string());
}

void Logger::swapStream(std::ofstream& other)
{
	std::lock_guard<std::mutex> lock(fileStreamMutex);
	get().m_fileStream.swap(other);
}

void Logger::stopUsingSpecificLogDate()
{
	get().m_specificDate.clear();
}

void Logger::setSpecificLogDate(std::string_view date)
{
	get().m_specificDate = date;
}

std::string Logger::m_defaultPath;

Logger::Logger()
{
	auto basePath{ getLogPath(m_defaultPath) };

	auto logPath = basePath;
	logPath.append(logFileName);
	m_fileStream.open(logPath, std::ios_base::out | std::ios_base::app);
	if (m_fileStream.is_open())
		m_loggingThread = std::thread{ &Logger::flush, this };
}

Logger::~Logger()
{
	close();
}

void Logger::close()
{
	Logger& logger{ get() };
    waitForEmpty();
	logger.m_isRunning = false;
	logger.m_loggingThread.join();
}

void Logger::waitForEmpty()
{
    Logger& log = get();
    while (!log.m_mainThreadLogQueue.empty() || !log.m_logQueue.empty()) {std::this_thread::sleep_for(10ms);}
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
		if (!m_mainThreadLogQueue.empty())
		{
			m_logQueue.swap(m_mainThreadLogQueue);
			std::lock_guard<std::mutex> lock(fileStreamMutex);
			while (!m_logQueue.empty())
			{
				m_fileStream << m_logQueue.front();
				m_logQueue.pop();
			}
			m_fileStream << std::flush;
		}
		std::this_thread::sleep_for(waitDuration);
	}
	m_fileStream.close();
}
