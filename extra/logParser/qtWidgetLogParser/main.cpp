#include <QApplication>
#include "mainwindow.h"
#include "logger.h"
#include "log.h"

int main(int argc, char* argv[])
{
    Logger::setFolderPath(argv[0]);
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
