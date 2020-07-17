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
    m_projectName = std::string(companyName) + '/' + path.stem().string();
}

void Logger::swapStream(std::ofstream& other, std::filesystem::path& outputPath)
{
    // TODO
}

void Logger::stopUsingSpecificLogDate()
{
	get().m_specificDate.clear();
}

void Logger::setSpecificLogDate(std::string_view date)
{
	get().m_specificDate = date;
}

std::string Logger::m_projectName;

Logger::Logger() : m_data(getLogPath(m_projectName))
{
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
                m_data.stream() << m_logQueue.front();
				m_logQueue.pop();
			}
            m_data.stream() << std::flush;
            m_data.incrementLineNumber(nbNewLines);
		}
		std::this_thread::sleep_for(waitDuration);
    }
}

LogDataFile::LogDataFile(const std::filesystem::path &folder) : m_baseFolder(folder), m_dataFilePath(folder.string() + "/" + logDataFileName.data())
{
    constexpr std::string_view endLine {"</logs>"};
    if (std::filesystem::exists(m_dataFilePath))
    {
        std::ifstream dataFile(m_dataFilePath.c_str());
        std::string line;
        std::getline(dataFile, line);
        parseDataLogInfo(line);
        m_filesInfo.reserve(m_numberOfFiles);
        while(std::getline(dataFile, line) && line != endLine)
        {
            m_filesInfo.emplace_back(parseLogInfo(line));
        }
        m_currentLog.open(m_filesInfo.back().filePath.c_str(), std::ios_base::out | std::ios_base::app);
    }
    else
    {
        createNewFile();
        write();
    }
}

void LogDataFile::write() const
{
    std::ofstream outFile(m_dataFilePath.c_str());
    outFile << "<logs totalLines=\"" <<m_totalLineNumber<<"\" files=\""<<m_numberOfFiles<<"\">\n";
    for (const auto& file : m_filesInfo)
        outFile<<"\t<file name=" <<file.filePath.filename() <<" lines=\""<<file.numberLines<<"\"/>\n";
    outFile <<"</logs>";
    outFile.close();
}

std::ofstream &LogDataFile::stream() { return m_currentLog; }

void LogDataFile::incrementLineNumber(int nbNewLines)
{
    constexpr uintmax_t maxFileSize{5000000}; // 5Mo // TODO make this calibrable
    m_totalLineNumber += nbNewLines;
    auto& currentFile = m_filesInfo.back();
    currentFile.numberLines += nbNewLines;
    if (std::filesystem::file_size(currentFile.filePath) >= maxFileSize)
        createNewFile();
    write();
}

LogInfo LogDataFile::parseLogInfo(std::string_view line) const
{
    constexpr std::string_view fileNameLabel {"name"};
    constexpr std::string_view lineNumberLabel{"lines"};
    size_t nextEqualSign = line.find('=');
    LogInfo info{};
    while(nextEqualSign != std::string::npos)
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
    constexpr std::string_view baseFileName{"part"};
    constexpr std::string_view fileStem{".ptLog"};
    std::filesystem::path path = m_baseFolder;
    path.append(baseFileName.data() + std::to_string(++m_numberOfFiles) + fileStem.data());
    m_filesInfo.push_back(LogInfo{path,0});
    if (m_currentLog.is_open())
        m_currentLog.close();
    m_currentLog.open(path.c_str());
}

void LogDataFile::parseDataLogInfo(std::string_view line)
{
    constexpr std::string_view lineNumberLabel{"totalLines"};
    constexpr std::string_view fileNumber{"files"};

    size_t nextEqualSign = line.find('=');
    while(nextEqualSign != std::string::npos)
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
