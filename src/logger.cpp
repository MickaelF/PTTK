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

    constexpr std::string_view logDataFileName{ "data.ptlData" };
	constexpr std::string_view companyName{ "PotatoThunder" };
	constexpr auto waitDuration {500ms};

	static std::mutex fileStreamMutex;
} // namespace

void Logger::setFolderPath(const char* executableName)
{
    std::filesystem::path path {executableName};
    m_defaultPath = std::string(companyName) + '/' + path.stem().string();
}

void Logger::swapStream(std::ofstream& other, std::filesystem::path& outputPath)
{
	std::lock_guard<std::mutex> lock(fileStreamMutex);
	get().m_fileStream.swap(other);
    get().m_logPath.swap(outputPath);
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

Logger::Logger() :m_logPath(getLogPath(m_defaultPath))
{
    auto logPath = m_logPath;
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
            int nbNewLines = m_logQueue.size();
			std::lock_guard<std::mutex> lock(fileStreamMutex);
			while (!m_logQueue.empty())
			{
				m_fileStream << m_logQueue.front();
				m_logQueue.pop();
			}
			m_fileStream << std::flush;
            updateData(nbNewLines);
		}
		std::this_thread::sleep_for(waitDuration);
	}
    m_fileStream.close();
}

void Logger::updateData(int nbLines)
{
    auto logPath = m_logPath;
    logPath.append(logDataFileName);
    if (std::filesystem::exists(logPath))
    {

    }
    auto lastModification = currentDate<false>();
}

Logger::LogDataFile::LogDataFile(const std::filesystem::path &path) : m_filePath(path.c_str())
{
    constexpr std::string_view fileNameLabel {"fileName"};
    constexpr std::string_view lineNumberLabel{"lines"};
    constexpr std::string_view lastModificationLabel{"lastModification"};
    std::ifstream dataFile(m_filePath);
    std::string line;
    std::getline(dataFile, line);
    size_t nextEqualSign = line.find('=');
    while(nextEqualSign != std::string::npos)
    {
        auto startId = line.rfind(' ', nextEqualSign);
        auto endValue = line.find('\"', nextEqualSign + 1);
        auto id = line.substr(startId, nextEqualSign - startId);
        auto value = line.substr(nextEqualSign + 1, endValue - nextEqualSign + 1);
        if (id == fileNameLabel)
            m_logFileName = value;
        else if (id == lineNumberLabel)
            m_nbLines = std::stoi(value);
        else if (id == lastModificationLabel)
            m_lastModification = value;
    }
    auto indexLine = line.find("lines") + 7;
    auto substr = line.substr(indexLine, line.find('\"', indexLine) - indexLine);
    m_nbLines = std::stoi(substr);
}

void Logger::LogDataFile::write() const
{
    std::ofstream out(m_filePath);
    out << "<info file=" << m_filePath << " lines=\"" << m_nbLines << "\" lastModification=\"" << currentDate<false>() << "\"/>";
}
