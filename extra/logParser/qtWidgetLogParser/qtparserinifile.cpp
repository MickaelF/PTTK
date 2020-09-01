#include "qtparserinifile.h"

#include <sstream>

#include "stringtools.h"

namespace
{
constexpr std::string_view PreviousOpenedFolder {"PreviousOpenedFolder"};
constexpr std::string_view LastOpenedFolder {"LastOpenedFolder"};
constexpr std::string_view SelectedLanguage {"SelectedLanguage"};
} // namespace

QtParserIniFile::QtParserIniFile(const QtParserIniFile& other) noexcept
    : m_previousFolders(other.m_previousFolders),
      m_lastOpenedFolder(other.m_lastOpenedFolder),
      m_selectedLanguage(other.m_selectedLanguage)
{
}

QtParserIniFile::QtParserIniFile(QtParserIniFile&& other) noexcept
    : m_previousFolders(other.m_previousFolders),
      m_lastOpenedFolder(other.m_lastOpenedFolder),
      m_selectedLanguage(other.m_selectedLanguage)
{
    other.m_lastOpenedFolder.reset();
    for (auto& val : other.m_previousFolders) val.reset();
    other.m_selectedLanguage.reset();
}

QtParserIniFile& QtParserIniFile::operator=(const QtParserIniFile& other) noexcept
{
    m_lastOpenedFolder = other.m_lastOpenedFolder;
    m_previousFolders = other.m_previousFolders;
    m_selectedLanguage = other.m_selectedLanguage;
    return *this;
}

QtParserIniFile& QtParserIniFile::operator=(QtParserIniFile&& other) noexcept
{
    m_lastOpenedFolder = other.m_lastOpenedFolder;
    m_previousFolders = other.m_previousFolders;
    m_selectedLanguage = other.m_selectedLanguage;
    other.m_lastOpenedFolder.reset();
    for (auto& val : other.m_previousFolders) val.reset();
    other.m_selectedLanguage.reset();
    return *this;
}

bool QtParserIniFile::contains(std::string_view name) const
{
    return name == PreviousOpenedFolder || name == LastOpenedFolder || name == SelectedLanguage;
}

void QtParserIniFile::initValue(std::string_view name, std::string_view value)
{
    if (name == PreviousOpenedFolder)
        for (auto& folder : strTls::split(std::string(value), ';')) m_previousFolders.push(folder);
    else if (name == LastOpenedFolder)
        m_lastOpenedFolder = value;
    else if (name == SelectedLanguage)
        m_selectedLanguage = value;
}

std::vector<std::pair<std::string, std::string>> QtParserIniFile::values() const
{
    std::vector<std::pair<std::string, std::string>> values;
    values.reserve(3);
    std::stringstream stream;
    for (const auto& val : m_previousFolders)
        if (val.has_value()) stream << *val << ";";
    values.emplace_back(PreviousOpenedFolder, stream.str());
    if (m_lastOpenedFolder.has_value()) values.emplace_back(LastOpenedFolder, *m_lastOpenedFolder);
    if (m_selectedLanguage.has_value()) values.emplace_back(SelectedLanguage, *m_selectedLanguage);
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

void QtParserIniFile::setSelectedLanguage(const std::string& language)
{
    if (m_selectedLanguage.has_value() && m_selectedLanguage == language) return;

    m_selectedLanguage = language;
}
