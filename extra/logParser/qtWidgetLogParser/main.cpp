#include <QApplication>
#include <QStyleFactory>
#include "mainwindow.h"
#include "logger.h"
#include "log.h"
#include "path.h"
#include "style.h"

constexpr std::string_view companyName{ "PotatoThunder" };

int main(int argc, char* argv[])
{
    const auto dataPath {
        path::getDataPath(std::filesystem::path(argv[0]).stem().string(), companyName)};
    Logger logger(dataPath);
    BasicLog::setLogger(logger);
    lInfo << "Starting application"; 
    QApplication app(argc, argv);
    app.setStyle(QStyleFactory::create("Fusion"));
    app.setStyleSheet(AppStyle::stylesheet);

    MainWindow window {dataPath};
    window.show();
    auto execRet = app.exec();
    lInfo << "End execution";
    return execRet;
}
