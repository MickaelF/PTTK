#pragma once

#include <QMainWindow>
#include <QTemporaryDir>
#include <filesystem>

#include "logpriority.h"
#include "qtparserinifile.h"
#include "startupdialog.h"
#include "ui_MainWindow.h"
class PriorityLabel;

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
    void setupDisplayPrioritiesBox();
    void updateOpenRecently();
    void open(const QString& path);
    QtParserIniFile m_ini;
    StartUpDialog m_startDialog;
    std::filesystem::path m_programDataPath;
    QMap<LogPriority::Priorities, PriorityLabel*> m_prioritySelection;
};
