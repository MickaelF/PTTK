#include <pttk/log.h>
#include <pttk/logger.h>
#include <pttk/pttkpath.h>

#include "generatorarguments.h"
#include "loggenerator.h"
constexpr std::string_view companyName {"PotatoThunder"};

int main(int argc, char* argv[])
{
    Logger logger(pttkPath::getDataPath(
        std::filesystem::path(argv[0]).stem().string(), companyName));
    BasicLog::setLogger(logger);
    std::unique_ptr<GeneratorArguments> arguments {nullptr};
    try
    {
        arguments = std::make_unique<GeneratorArguments>(argc, argv);
    }
    catch (std::exception& e)
    {
        lFatal << e.what();
        return -1;
    }

    std::unique_ptr<LogGenerator> generator {nullptr};
    try
    {
        generator = std::make_unique<LogGenerator>(arguments->path(),
                                                   arguments->nbLines());
        generator->exec();
    }
    catch (std::exception& e)
    {
        lFatal << e.what();
        return -2;
    }
    lInfo << "Generation succeeded";
    return 0;
}
