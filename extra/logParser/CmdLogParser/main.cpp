#include "parserarguments.h"
#include "parserexecution.h"
#include "logger.h"
#include "log.h"
#include "path.h"

constexpr std::string_view companyName {"PotatoThunder"};

int main(int argc, char* argv[])
{
    Logger logger(pttkPath::getDataPath(std::filesystem::path(argv[0]).stem().string(), companyName));
    BasicLog::setLogger(logger);
    std::unique_ptr<ParserArguments> args {nullptr};
    try
    {
        args = std::make_unique<ParserArguments>(argc, argv);
    }
    catch (std::exception& e)
    {
        lFatal << e.what();
        return -1;
    }

    std::unique_ptr<ParserExecution> parser {nullptr};
    try
    {
        parser = std::make_unique<ParserExecution>(*args);
        parser->exec();
    }
    catch (std::exception& e)
    {
        lFatal << e.what();
        return -2;
    }

    return 0;
}
