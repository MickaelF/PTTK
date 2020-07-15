#include "parserexecution.h"
#include "parserarguments.h"
#include "logParser.h"
#include "loglineinfo.h"

ParserExecution::ParserExecution(const ParserArguments& arguments) : m_args(arguments) 
{
    m_outFile.open(m_args.output());
    if (!m_outFile.is_open()) throw std::runtime_error("Could not write in the output file.");
}

ParserExecution::~ParserExecution() 
{
    m_outFile.flush();
    m_outFile.close();
}

void ParserExecution::exec()
{
    LogParser parser;
    parser.createComparaisonFunctions(m_args.priorities(), m_args.endDate(), m_args.fileNames());
    parser.setInputFile(m_args.input());
    if (m_args.startDate().has_value()) parser.startAtDate(*m_args.startDate());
    if (m_args.sort().has_value()) parser.setSortType(*m_args.sort());
    auto logs = std::move(parser.exec());
    for (auto& pair : logs)
    {
        m_outFile << pair.first << "\n";
        for (auto line : pair.second) m_outFile << line << "\n";
        m_outFile << "\n";
    }
}
