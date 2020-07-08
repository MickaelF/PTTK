#pragma once
#include <fstream>

class ParserArguments;

class ParserExecution
{
public: 
    explicit ParserExecution(const ParserArguments& arguments);
    ~ParserExecution();
	ParserExecution() = delete;
    ParserExecution(const ParserExecution&) = delete;
    ParserExecution(ParserExecution&&) = delete;
    ParserExecution& operator=(const ParserExecution&) = delete;
    ParserExecution& operator=(ParserExecution&&) = delete;

    void exec(); 

private: 
    const ParserArguments& m_args;
    std::ofstream m_outFile; 
};