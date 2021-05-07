#include "pttk/file/plaintextfile.h"

#include <string>

std::optional<std::string> PlainTextFile::wholeFile(std::string_view str)
{
    if (!openToRead(str))
        return std::nullopt;
    std::string file;
    std::string line;
    while (std::getline(m_fileStream, line))
        file += line + '\n';
    close();
    return file;
}
