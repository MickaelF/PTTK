#include "qtparserinifile.h"

#include <sstream>

#include "stringtools.h"

namespace
{
constexpr std::string_view PreviousOpenedFolder {"PreviousOpenedFolder"};
constexpr std::string_view LastOpenedFolder {"LastOpenedFolder"};
} // namespace

QtParserIniFile::QtParserIniFile(const QtParserIniFile& other) noexcept
    : m_previousFolders(other.m_previousFolders),
      m_lastOpenedFolder(other.m_lastOpenedFolder)
{
}

QtParserIniFile::QtParserIniFile(QtParserIniFile&& other) noexcept
    : m_previousFolders(other.m_previousFolders),
      m_lastOpenedFolder(other.m_lastOpenedFolder)
{
    other.m_lastOpenedFolder.reset();
    for (auto& val : other.m_previousFolders) val.reset();
}

QtParserIniFile& QtParserIniFile::operator=(const QtParserIniFile& other) noexcept
{
    m_lastOpenedFolder = other.m_lastOpenedFolder;
    m_previousFolders = other.m_previousFolders;
    return *this;
}

QtParserIniFile& QtParserIniFile::operator=(QtParserIniFile&& other) noexcept
{
    m_lastOpenedFolder = other.m_lastOpenedFolder;
    m_previousFolders = other.m_previousFolders;
    other.m_lastOpenedFolder.reset();
    for (auto& val : other.m_previousFolders) val.reset();
    return *this;
}

bool QtParserIniFile::contains(std::string_view name) const
{
    return name == PreviousOpenedFolder || name == LastOpenedFolder;
}

void QtParserIniFile::initValue(std::string_view name, std::string_view value)
{
    if (name == PreviousOpenedFolder)
    {
        int i = 0;
        for (auto& folder : strTls::split(std::string(value), ';')) m_previousFolders.push(folder);
    }
    else if (name == LastOpenedFolder)
        m_lastOpenedFolder = value;
}

std::vector<std::pair<std::string, std::string>> QtParserIniFile::values() const
{
    std::vector<std::pair<std::string, std::string>> values;
    values.reserve(2);
    std::stringstream stream;
    for (const auto& val : m_previousFolders)
        if (val.has_value()) stream << *val << ";";
    values.emplace_back(PreviousOpenedFolder, stream.str());
    if (m_lastOpenedFolder.has_value()) values.emplace_back(LastOpenedFolder, *m_lastOpenedFolder);
    return values;
}

const ContigousArray<std::string, QtParserIniFile::MaxSizePreviousFolders>&
QtParserIniFile::previousFolders() const
{
    return m_previousFolders;
}

void QtParserIniFile::removeFolder(const std::string& folder)
{
    m_previousFolders.remove(folder);
}

bool QtParserIniFile::setLastOpenedFolder(const std::string& folder)
{
    if (folder == *m_lastOpenedFolder) return false;

    m_lastOpenedFolder = folder;
    m_previousFolders.push(folder);
    return true;
}
