#pragma once
#include <string_view>

#include "abstractinidefinition.h"
#include "contiguousarray.h"

class QtParserIniFile : public AbstractIniDescription
{
public:
    static constexpr unsigned int MaxSizePreviousFolders {5};
    QtParserIniFile() = default;
    ~QtParserIniFile() = default;
    QtParserIniFile(const QtParserIniFile& other) noexcept;
    QtParserIniFile(QtParserIniFile&& other) noexcept;

    QtParserIniFile& operator=(const QtParserIniFile& other) noexcept;
    QtParserIniFile& operator=(QtParserIniFile&& other) noexcept;
    bool contains(std::string_view name) const override;
    void initValue(std::string_view name, std::string_view value) override;

    std::vector<std::pair<std::string, std::string>> values() const override;

    const std::optional<std::string>& lastFolder() const { return m_lastOpenedFolder; }
    const ContigousArray<std::string, MaxSizePreviousFolders>& previousFolders() const;
    void removeFolder(const std::string& folder);

    bool setLastOpenedFolder(const std::string& folder);

private:
    ContigousArray<std::string, MaxSizePreviousFolders> m_previousFolders;
    std::optional<std::string> m_lastOpenedFolder;
};