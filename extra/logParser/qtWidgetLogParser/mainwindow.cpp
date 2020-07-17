#include "mainwindow.h"
#include "loggeneratordialog.h"
#include <QFileDialog>
#include "logParser.h"

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
    if (dialog.openInEditor())
        open(dialog.path());
}

void MainWindow::onOpenActionPressed()
{
    auto path = QFileDialog::getOpenFileName(this, tr("Open log folder"), QString());
    if (path.isEmpty()) return;
    open(path);
    
}

void MainWindow::open(const QString& path)
{
    LogParser parser;
    //parser.setInputFile(path.toStdString());
    g_parsedLogWidget->setModel(parser.exec());
}
