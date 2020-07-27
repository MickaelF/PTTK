#include "abstractreader.h"

AbstractReader::~AbstractReader()
{
    close();
}

bool AbstractReader::openToRead(std::string_view str)
{
    close();
    m_fileStream.open(str.data(), std::ios_base::in);
    return m_fileStream.is_open();
}

bool AbstractReader::openToWrite(std::string_view str)
{
    close();
    m_fileStream.open(str.data(), std::ios_base::out);
    return m_fileStream.is_open();
}

void AbstractReader::close()
{
    if (m_fileStream.is_open()) m_fileStream.close();
}
