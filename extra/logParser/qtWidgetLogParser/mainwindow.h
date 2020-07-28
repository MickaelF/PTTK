#pragma once

#include <QMainWindow>

#include "ui_MainWindow.h"
#include "qtparserinifile.h"
#include "startupdialog.h"
#include <filesystem>

class MainWindow : public QMainWindow, public Ui_MainWindow
{
    Q_OBJECT
public:
    MainWindow(const std::filesystem::path& programDataPath);

private slots:
    void onLogGeneratorActionPressed();
    void onOpenActionPressed();
    void onOpenRecentlyPressed();
    void onStartUpDialogAccepted();

private:
    void displayStartUpDialog(); 
    void updateOpenRecently(); 
    void open(const QString& path);
    QtParserIniFile m_ini;
    StartUpDialog m_startDialog; 
    std::filesystem::path m_programDataPath;
};
