#pragma once
#include <ctime>
#include <fstream>
#include <functional>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "iprogress.h"
class LogLineInfo;

enum class LogSort
{
	Date,
	Type,
	Files
};

class LogParser : public IProgress
{
public:
	LogParser() = default;

	void setInputFile(std::string_view file);
	void startAtDate(std::time_t start);
	void setSortType(LogSort sort);
	std::function<std::string_view(const LogLineInfo& info)> createRetrieveFunc() const;

	std::map<std::string, std::vector<std::string>> exec();

	void createComparaisonFunctions(const std::optional<std::vector<std::string>>& priorities,
		const std::optional<std::time_t>& endDate,
		const std::optional<std::vector<std::string>>& fileName);
	int progress() const override { return m_progress; }
	int numberOfLines() const { return m_numberOfLines; }

private:
	bool comparaisonFunc(LogLineInfo& info) const;

	std::vector<std::string> m_parsedPriorities;
	std::time_t m_endDate{ 0 };
	std::vector<std::string> m_fileNames;
	std::vector<std::function<bool(LogLineInfo&)>> m_logTests;
	std::ifstream m_stream;
	LogSort m_sort{LogSort::Date};
	int m_progress{ 0 };
	int m_numberOfLines{ 0 };
};
