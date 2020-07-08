#include "parserexecution.h"
#include "parserarguments.h"
#include "../logParser.h"
#include "../loglineinfo.h"

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

    std::function<std::string_view(const LogLineInfo& info)> compareFunction;
    switch (m_args.sort())
    {
        case ParserArguments::Sort::Date :
            compareFunction = [](const LogLineInfo& info) -> std::string_view {
                return info.dateStr();
            };
            break;
        case ParserArguments::Sort::Type:
            compareFunction = [](const LogLineInfo& info) -> std::string_view {
                return info.priority();
            };
            break;
        case ParserArguments::Sort::Files:
            compareFunction = [](const LogLineInfo& info) -> std::string_view {
                return info.fileName();
            };
            break;
        default: throw std::runtime_error("Cannot parse logs with unknown sort selected.");
    }

    auto logs = std::move(parser.exec(compareFunction));
    for (auto& pair : logs)
    {
        m_outFile << pair.first << "\n";
        for (auto line : pair.second) m_outFile << line << "\n";
        m_outFile << "\n";
    }
}
