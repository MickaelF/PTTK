#include "loglineinfo.h"

#include <pttk/stringtools.h>

namespace
{
constexpr int timeOpenBracket {0};
constexpr int dateStringSize {21};
constexpr int timeCloseBracket {20};

constexpr int infoOpenBracket {21};
} // namespace

LogLineInfo::LogLineInfo(std::string_view line)
{
    if (line.empty() || line[timeOpenBracket] != '[' ||
        line[timeCloseBracket] != ']' || line[infoOpenBracket] != '[')
        return;

    m_date = line.substr(timeOpenBracket, dateStringSize);

    auto priorityEnd = line.find('-', infoOpenBracket);
    m_priority =
        line.substr(infoOpenBracket + 1, priorityEnd - infoOpenBracket - 1);
    auto fileNameEnd = line.find(':', priorityEnd);
    m_fileName = line.substr(priorityEnd + 1, fileNameEnd - priorityEnd - 1);
    auto fileLineNumberEnd = line.find(']', fileNameEnd);
    m_fileLineNumber =
        line.substr(priorityEnd + 1, fileLineNumberEnd - priorityEnd - 1);
    m_text = line.substr(line.find_first_of("-", fileLineNumberEnd) + 2);
    m_hasInfo = true;
}

std::time_t LogLineInfo::date() const
{
    return strTls::toTimeT(m_date, "[%Y-%m-%d %T]");
}

std::string_view LogLineInfo::dateStr() const
{
    return m_date.substr(1, 10);
}

std::string_view LogLineInfo::dateTimeStr() const
{
    return m_date.substr(1, timeCloseBracket - 1);
}

std::string_view LogLineInfo::timeStr() const
{
    return m_date.substr(12, 8);
}

std::string_view LogLineInfo::text() const
{
    return m_text;
}
