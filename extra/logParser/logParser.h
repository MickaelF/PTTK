#pragma once
#include <string>
#include <vector>
#include <ctime>
#include <optional>

class LogParser
{
public:
    enum class Sort
    { 
		Date, Type, Files
    };

	LogParser(std::string inputPath); 
	void setParsedLogPriority(std::vector<std::string> priority);

	void setOutputFilePath(std::string path); 

	void setStartDate(std::string date); 
	void setEndDate(std::string date);

	void setSortTechnique(Sort sort);
	void setFileNames(std::string fileNames);

	void exec();

private: 
	std::optional<std::vector<std::string>> m_parsedPriorities; 
	std::string m_input; 
	std::optional<std::string> m_output; 

	std::optional<std::time_t> m_startDate; 
	std::optional<std::time_t> m_endDate; 
	std::optional<std::vector<std::string>> m_fileNames; 

	Sort m_sort {Sort::Date};
};