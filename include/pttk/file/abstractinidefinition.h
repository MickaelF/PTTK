#pragma once
#include <string_view>
#include <vector>
#include <pttk/macroutils.h>

class AbstractIniDescription
{
public:
    AbstractIniDescription() = default;
    ~AbstractIniDescription() = default;
    DELETE_COPY_CONSTR_ASSIGN(AbstractIniDescription)
    virtual bool contains(std::string_view name) const = 0;
    virtual void initValue(std::string_view name, std::string_view value) = 0;

    virtual std::vector<std::pair<std::string, std::string>> values() const = 0;
};