#include "mainwindow.h"
#include "loggeneratordialog.h"

MainWindow::MainWindow() : QMainWindow(nullptr)
{
    setupUi(this);

    connect(actionLogGenerator, &QAction::triggered, this, &MainWindow::onLogGeneratorActionPressed);
}

void MainWindow::onLogGeneratorActionPressed() 
{
    LogGeneratorDialog dialog;
    dialog.exec();
}