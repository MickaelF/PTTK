#include "stringtools.h"

namespace strTls
{
std::vector<std::string> split(std::string str, char delimiter)
{
	std::vector<std::string> result; 
	auto id = str.find_first_of(delimiter);
	while (id != std::string::npos)
	{
		auto substr = str.substr(0, id);
		if (!substr.empty())
			result.push_back(substr);
		str.erase(0, id +1);
		id = str.find_first_of(delimiter);
	}
	if (!str.empty())
		result.push_back(str);
	return result;
}
}

