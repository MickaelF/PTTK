#pragma once

#include <QTableView>

#include "logParser.h"
#include "logviewermodel.h"
class QTemporaryDir;
class QSortFilterProxyModel;

class LogViewerWidget : public QTableView
{
    Q_OBJECT
public:
    LogViewerWidget(QWidget* parent = nullptr);

    void open(const QString& openPath);

private slots:
    void onSectionClicked(int section);

private:
    void launchParsing();

    LogViewerModel m_model;
    QTemporaryDir* m_tempDir {nullptr};
    QSortFilterProxyModel* m_sortFilter {nullptr};
    LogParser m_parser;
    std::filesystem::directory_entry m_fileIt;

    int m_currentlySortedSection {-1};
    Qt::SortOrder m_currentSortOrder {Qt::AscendingOrder};
};