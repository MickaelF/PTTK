#pragma once

#include <QMainWindow>

#include "ui_MainWindow.h"

class MainWindow : public QMainWindow, public Ui_MainWindow
{
    Q_OBJECT
public:
    MainWindow();

private slots:
    void onLogGeneratorActionPressed();
    void onOpenActionPressed();

private:
    void open(const QString& path);
};
