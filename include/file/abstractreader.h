#pragma once
#include <string_view>
#include <fstream>

class AbstractReader
{
public: 
	AbstractReader() = default; 
	virtual ~AbstractReader(); 
	AbstractReader(const AbstractReader&) = delete;
    AbstractReader(AbstractReader&&) = delete; 
	AbstractReader& operator=(const AbstractReader&) = delete;
    AbstractReader& operator=(AbstractReader&&) = delete; 

    void close(); 

protected:
    bool read(std::string_view str);
	std::ifstream m_fileStream; 
};