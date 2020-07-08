#pragma once
#include <string>
#include <vector>
#include <ctime>

namespace strTls
{
	std::vector<std::string> split(std::string str, char delimiter); 
	std::time_t toTimeT(std::string_view str, std::string_view dateFormat);
    std::string currentDateTimeToString(std::string_view dateFormat); 
	std::string timeTToString(std::time_t time, std::string_view dateFormat);
}