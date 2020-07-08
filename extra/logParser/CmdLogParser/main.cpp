#include "parserarguments.h"
#include "parserexecution.h"
#include "logger.h"
#include "log.h"
#include <filesystem>

int main(int argc, char *argv[])
{
    Logger::setFolderPath(argv[0]);
    std::unique_ptr<ParserArguments> args {nullptr};
    try
    {
        args = std::make_unique<ParserArguments>(argc, argv);
    }
    catch (std::exception e)
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
    catch (std::exception e)
    {
        lFatal << e.what();
        return -2;
    }

    return 0;
}