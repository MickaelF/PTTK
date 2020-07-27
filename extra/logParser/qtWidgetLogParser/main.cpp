#include <QApplication>
#include "mainwindow.h"
#include "logger.h"
#include "log.h"
#include "path.h"

constexpr std::string_view companyName{ "PotatoThunder" };

int main(int argc, char* argv[])
{
    Logger logger(path::getDataPath(std::filesystem::path(argv[0]).stem().string(), companyName));
    BasicLog::setLogger(logger);
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
