#include "parserexecution.h"

#include "logParser.h"
#include "loglineinfo.h"
#include "parserarguments.h"

ParserExecution::ParserExecution(const ParserArguments& arguments)
    : m_args(arguments)
{
    m_outFile.open(m_args.output());
    if (!m_outFile.is_open())
        throw std::runtime_error("Could not write in the output file.");
}

ParserExecution::~ParserExecution()
{
    m_outFile.flush();
    m_outFile.close();
}

void ParserExecution::exec()
{
    LogParser parser {};
    parser.createComparaisonFunctions(m_args.priorities(), m_args.startDate(),
                                      m_args.endDate(), m_args.fileNames());
    if (m_args.sort().has_value()) parser.setSortType(*m_args.sort());
    auto logs = parser.exec();
    for (auto& pair : logs)
    {
        m_outFile << pair.first << "\n";
        for (auto line : pair.second) m_outFile << line << "\n";
        m_outFile << "\n";
    }
}
