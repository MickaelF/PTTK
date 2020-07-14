#include "mainwindow.h"
#include "loggeneratordialog.h"
#include <QFIleDialog>

MainWindow::MainWindow() : QMainWindow(nullptr)
{
    setupUi(this);

    //g_sortOptions->setHidden(true);
    connect(actionOpen, &QAction::triggered, this, &MainWindow::onOpenActionPressed);
    connect(actionLogGenerator, &QAction::triggered, this, &MainWindow::onLogGeneratorActionPressed);
}

void MainWindow::onLogGeneratorActionPressed() 
{
    LogGeneratorDialog dialog;
    dialog.exec();
}

void MainWindow::onOpenActionPressed()
{
    auto path = QFileDialog::getOpenFileName(this, tr("Open log file"), QString(), "Logs (*.txt)");
    if (path.isEmpty()) return;

    
}
