#include "loggenerator.h"

#include <random>
#include <vector>
#include "log.h"
#include "logger.h"
#include "stringtools.h"

LogGenerator::LogGenerator(std::filesystem::path outputPath, int nbLines) : m_nbLines(nbLines)
{
	// TODO Move everything related to random to seperate class in pttk lib
	//if (m_nbLines == 0)
	//{
	//
	//	std::random_device rd; // Will be used to obtain a seed for the random number engine
	//	std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
	//	std::uniform_int_distribution distrib(1, 1000000);
	//	m_nbLines = distrib(gen);
	//}
	m_outStream.open(outputPath, std::ios_base::out | std::ios_base::app);
	if (!m_outStream.is_open()) throw std::runtime_error("Cannot create output file.");

	Logger::swapStream(m_outStream);
}

void LogGenerator::exec()
{
	const std::vector<std::string> randomFileName{ "file1", "file2", "file3", "file4", "file5", "file6" };
	const std::vector<std::string> logText{
		"This is a log on one line.",
		"This is \na log on two lines",
		"This\nis\ntoo\nmany\nlines"
	};

	std::random_device rd; // Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()

	std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	for (int i = 0; i < m_nbLines; ++i)
	{
		LogGeneratorPriority priority = static_cast<LogGeneratorPriority>(std::uniform_int_distribution(static_cast<int>(LogGeneratorPriority::Debug),
			static_cast<int>(LogGeneratorPriority::Execution))(gen));
		int fileId = std::uniform_int_distribution(0, static_cast<int>(randomFileName.size() - 1))(gen);
		int logTextId = std::uniform_int_distribution(0, static_cast<int>(logText.size() - 1))(gen);
		int lineNumber = std::uniform_int_distribution(0, 300)(gen);
		now += std::uniform_int_distribution(2, 3600)(gen);

		Logger::setSpecificLogDate(strTls::timeTToString(now, "[%D-%T]"));

		Log<true>(priority, randomFileName[fileId], lineNumber)<< logText[logTextId];
	}
	Logger::swapStream(m_outStream);
}