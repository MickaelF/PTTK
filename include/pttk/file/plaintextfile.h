#pragma once
#include "abstractreader.h"
#include <pttk/macroutils.h>
#include <optional>

class PlainTextFile : public AbstractReader
{
public: 
	PlainTextFile() = default;
    ~PlainTextFile() = default;
    DELETE_COPY_CONSTR_ASSIGN(PlainTextFile)
	std::optional<std::string> wholeFile(std::string_view str); 
    bool writeFile(std::string_view path, std::string_view content);    

};