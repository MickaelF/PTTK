#include "logger.h"
#include "loggenerator.h"
#include "generatorarguments.h"

#include <filesystem>
#if defined (WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <ShlObj.h>
#endif
//This needs to be included after
#include "log.h"


constexpr std::string_view companyName{ "PotatoThunder" };
std::filesystem::path getLogPath(const std::string& executableName)
{
    std::filesystem::path programDataPath;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    PWSTR path{ NULL };
    HRESULT hr = SHGetKnownFolderPath(FOLDERID_ProgramData, KF_FLAG_DEFAULT, NULL, &path);
    if (SUCCEEDED(hr)) programDataPath = path;
#endif
    programDataPath.append(std::string(companyName) + "/" + executableName);
    if (!std::filesystem::exists(programDataPath))
        std::filesystem::create_directories(programDataPath);
    return programDataPath;
}

int main(int argc, char* argv[])
{
    Logger logger(getLogPath(std::filesystem::path(argv[0]).stem().string()));
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
        generator = std::make_unique<LogGenerator>(arguments->path(), arguments->nbLines());
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
