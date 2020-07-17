#include <QApplication>
#include "mainwindow.h"
#include "logger.h"

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
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
