#pragma once

#include <QTableView>

#include "logviewermodel.h"
#include "logParser.h"
class QTemporaryDir;

class LogViewerWidget : public QTableView
{
    Q_OBJECT
public:
    LogViewerWidget(QWidget* parent = nullptr);

    void open(const QString& openPath);

private slots:
    void onSliderMoved(int value);
    void onSectionClicked(int section);

private:
    void launchParsing(); 

    LogViewerModel m_model;
    QTemporaryDir* m_tempDir {nullptr};
    LogParser m_parser; 
    std::filesystem::directory_entry m_fileIt; 
};