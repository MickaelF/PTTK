#pragma once
#include <string_view>
#include <vector>

class AbstractIniDescription
{
public:
    AbstractIniDescription() = default;
    ~AbstractIniDescription() = default;
    AbstractIniDescription(const AbstractIniDescription&) = delete;
    AbstractIniDescription(AbstractIniDescription&&) = delete;
    AbstractIniDescription& operator=(const AbstractIniDescription&) = delete;
    AbstractIniDescription& operator=(AbstractIniDescription&&) = delete;
    virtual bool contains(std::string_view name) const = 0;
    virtual void initValue(std::string_view name, std::string_view value) = 0;

    virtual std::vector<std::pair<std::string, std::string>> values() const = 0;
};