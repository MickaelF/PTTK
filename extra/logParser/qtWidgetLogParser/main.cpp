#include <QApplication>
#include <QMessageBox>
#include <QStyleFactory>
#include <QTranslator>

#include "log.h"
#include "logger.h"
#include "mainwindow.h"
#include "pttkpath.h"
#include "style.h"

constexpr std::string_view companyName {"PotatoThunder"};

int main(int argc, char* argv[])
{
    const auto dataPath {
        pttkPath::getDataPath(std::filesystem::path(argv[0]).stem().string(), companyName)};
    Logger logger(dataPath);
    BasicLog::setLogger(logger);
    lInfo << "Starting application";
    QApplication app(argc, argv);
    app.setStyle(QStyleFactory::create("Fusion"));
    app.setStyleSheet(AppStyle::stylesheet);

    MainWindow window {dataPath};
    window.show();
    int execRet = 0;
    try
    {
        execRet = app.exec();
    }
    catch (std::exception& e)
    {
        lFatal << e.what();
        QMessageBox::critical(
            nullptr, "Fatal error",
            QString("An unhandled error made the application crash : %1").arg(e.what()));
        execRet = -1;
    }
    lInfo << "End execution";
    return execRet;
}
