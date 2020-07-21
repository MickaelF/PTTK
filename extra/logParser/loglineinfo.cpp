#include "loglineinfo.h"
#include "stringtools.h"

namespace 
{
constexpr int timeOpenBracket {0};
constexpr int dateStringSize {19};
constexpr int timeCloseBracket {18};

constexpr int infoOpenBracket {19}; 
}

LogLineInfo::LogLineInfo(std::string_view line) 
{
    if (line.empty() || line[timeOpenBracket] != '[' || line[timeCloseBracket] != ']' ||
        line[infoOpenBracket] != '[')
        return; 

    m_date = line.substr(timeOpenBracket, dateStringSize);

    auto priorityEnd = line.find('-', infoOpenBracket);
    m_priority = line.substr(infoOpenBracket + 1, priorityEnd - infoOpenBracket - 1);
    auto fileNameEnd = line.find(':', priorityEnd);
    m_fileName = line.substr(priorityEnd + 1, fileNameEnd - priorityEnd - 1);
    auto fileLineNumberEnd = line.find(']', fileNameEnd);
    m_fileLineNumber = line.substr(fileNameEnd + 1, fileLineNumberEnd - fileNameEnd - 1);
    m_text = line.substr(line.find_first_of("-", fileLineNumberEnd));
    m_hasInfo = true; 
}

std::time_t LogLineInfo::date() const
{
    return strTls::toTimeT(m_date, "[%D-%T]");
}

std::string_view LogLineInfo::dateStr() const
{
    return m_date.substr(1, 8);
}

std::string_view LogLineInfo::text() const
{
    return m_text;
}

