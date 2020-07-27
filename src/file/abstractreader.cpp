#include "abstractreader.h"

AbstractReader::~AbstractReader()
{
    close();
}

bool AbstractReader::read(std::string_view str)
{
    close();
    m_fileStream.open(str.data());
    return m_fileStream.is_open();
}

void AbstractReader::close()
{
    if (m_fileStream.is_open()) m_fileStream.close();
}
