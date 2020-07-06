#include "cmdlogparser.h"
#include "logger.h"
#include "log.h"
#include <filesystem>

int main(int argc, char *argv[])
{
    Logger::setFolderPath(argv[0]);
    try
    {
        CmdLogParser parser(argc, argv);
        parser.exec();
    }
    catch (std::exception e)
    {
        lFatal << e.what();
        return -1;
    }

    return 0;
}