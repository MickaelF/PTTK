#include "pttk/stringtools.h"

#include <chrono>
#include <iomanip>
#include <sstream>

namespace
{
// Borrowed from
// https://kjellkod.wordpress.com/2013/01/22/exploring-c11-part-2-localtime-and-time-again/
std::tm threadSafeLocalTime(const std::time_t& time)
{
    std::tm tm_snapshot {};
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    localtime_s(&tm_snapshot, &time);
#else
    localtime_r(&time, &tm_snapshot); // POSIX
#endif
    return tm_snapshot;
}
} // namespace

namespace strTls
{
std::vector<std::string> split(std::string str, char delimiter)
{
    std::vector<std::string> result;
    auto id = str.find_first_of(delimiter);
    while (id != std::string::npos)
    {
        auto substr = str.substr(0, id);
        if (!substr.empty()) result.push_back(substr);
        str.erase(0, id + 1);
        id = str.find_first_of(delimiter);
    }
    if (!str.empty()) result.push_back(str);
    return result;
}

std::time_t toTimeT(std::string_view str, std::string_view dateFormat)
{
    struct std::tm tm
    {
    };
    std::istringstream ss(str.data());
    ss >> std::get_time(&tm, dateFormat.data());
    return std::mktime(&tm);
}

std::string currentDateTimeToString(std::string_view dateFormat)
{
    std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream ss;
    auto lt = threadSafeLocalTime(now);
    ss << std::put_time(&lt, dateFormat.data());
    return std::move(ss.str());
}

std::string timeTToString(std::time_t time, std::string_view dateFormat)
{
    std::stringstream ss;
    auto lt = threadSafeLocalTime(time);
    ss << std::put_time(&lt, dateFormat.data());
    return std::move(ss.str());
}
} // namespace strTls
