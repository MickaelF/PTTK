#pragma once

#include <QTableView>

#include "logviewermodel.h"

class LogViewerWidget : public QTableView
{
    Q_OBJECT
public:
    LogViewerWidget(QWidget* parent = nullptr);

    void open(const QString& openPath);

private:
    LogViewerModel m_model;
};