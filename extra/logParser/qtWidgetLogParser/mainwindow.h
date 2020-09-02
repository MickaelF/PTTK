#pragma once

#include <QMainWindow>
#include <QTemporaryDir>
#include <QTranslator>
#include <filesystem>

#include "filenamesmenu.h"
#include "logpriority.h"
#include "qtparserinifile.h"
#include "startupdialog.h"
#include "ui_MainWindow.h"
class PriorityLabel;

class MainWindow : public QMainWindow, public Ui_MainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(const std::filesystem::path& programDataPath);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void changeEvent(QEvent* event) override; 

private slots:
    void onLogGeneratorActionPressed();
    void onOpenActionPressed();
    void onOpenRecentlyPressed();
    void onStartUpDialogAccepted();
    void onApplyPressed();
    void onDefaultPressed();

    void onStartDateChanged(const QDateTime& dateTime);
    void onEndDateChanged(const QDateTime& dateTime);

    void onAboutPressed();

private:
    void initLanguageMenu();
    void displayStartUpDialog();
    void setupDisplayPrioritiesBox();
    void updateOpenRecently();
    void open(const QString& path);
    void updateDate();
    void updateFileNames();
    void loadLanguage(const QString& language, bool needRestart = false);
    void defaultLanguage();

    QtParserIniFile m_ini;
    StartUpDialog m_startDialog;
    std::filesystem::path m_programDataPath;
    QMap<LogPriority::Priorities, PriorityLabel*> m_prioritySelection;
    FileNamesMenu m_fileNamesMenu;
    QTranslator m_translator; 
};
