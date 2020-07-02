#include "loglineinfo.h"

namespace 
{
constexpr int timeOpenBracket {0};
constexpr int dateStringSize {8};
constexpr int timeCloseBracket {18};

constexpr int infoOpenBracket {19}; 
}

LogLineInfo::LogLineInfo(std::string_view line)
{
    if (line.empty() || line[timeOpenBracket] != '[' || line[timeCloseBracket] != ']' ||
        line[infoOpenBracket] != '[')
        return; 

    m_date = line.substr(timeOpenBracket + 1, dateStringSize);

    auto priorityEnd = line.find('-', infoOpenBracket);
    m_priority = line.substr(infoOpenBracket + 1, priorityEnd - infoOpenBracket - 1);
    auto fileNameEnd = line.find(':', priorityEnd);
    m_fileName = line.substr(priorityEnd + 1, fileNameEnd - priorityEnd - 1);
    m_hasInfo = true; 
}

std::time_t LogLineInfo::date() const
{
return std::time_t();
}

