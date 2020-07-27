#pragma once
#include "abstractreader.h"
#include "abstractinidefinition.h"
#include <type_traits>
#include <optional>

class IniFile : public AbstractReader
{
public:
    IniFile() = default; 

    template <typename T>
    std::optional<T> readIni(std::string_view path)
    {
        static_assert(std::is_base_of<AbstractIniDescription, T>::value,
                      "Templated type needs to be derived from AbstractIniDescription");
        if (!read(path)) return std::nullopt;

        T definition; 
        std::string line;
        while (std::getline(m_fileStream, line))
        {
            std::string_view lineView(line);
            auto separator = lineView.find('=');
            auto name = lineView.substr(0, separator); 
            if (!definition.contains(name)) continue;
            auto value = lineView.substr(separator + 1);
            definition.initValue(name, value);
        }
        close();
        return definition;
    }

    template <typename T>
    bool save(const T& file) const 
    {
        static_assert(std::is_base_of<AbstractIniDescription, T>::value,
                      "Templated type needs to be derived from AbstractIniDescription");
        
    }
};