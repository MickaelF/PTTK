#pragma once
#include <string_view>
#include <fstream>
#include <pttk/macroutils.h>

class AbstractReader
{
public: 
	AbstractReader() = default; 
	virtual ~AbstractReader(); 
    DELETE_COPY_CONSTR_ASSIGN(AbstractReader)

    void close(); 

protected:
    bool openToRead(std::string_view str);
    bool openToWrite(std::string_view str);
	std::fstream m_fileStream; 
};