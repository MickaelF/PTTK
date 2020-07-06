#pragma once
#include <string_view>
#include <ctime>

class LogLineInfo
{
public: 
	LogLineInfo(std::string_view line); 

	std::string_view priority() const { return m_priority; }
    std::string_view fileName() const { return m_fileName; }
	std::time_t date() const;
    std::string_view dateStr() const; 
	const bool hasInfo() const { return m_hasInfo; }

private: 
	bool m_hasInfo {false};
	std::string_view m_priority; 
	std::string_view m_date; 
	std::string_view m_fileName; 
};