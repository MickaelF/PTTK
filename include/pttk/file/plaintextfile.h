#pragma once
#include "abstractreader.h"
#include <optional>

class PlainTextFile : public AbstractReader
{
public: 
	PlainTextFile() = default;
    ~PlainTextFile() = default;
    PlainTextFile(const PlainTextFile&) = delete;
    PlainTextFile(PlainTextFile&&) = delete;
    PlainTextFile& operator=(const PlainTextFile&) = delete;
    PlainTextFile& operator=(PlainTextFile&&) = delete;
	std::optional<std::string> wholeFile(std::string_view str); 
};