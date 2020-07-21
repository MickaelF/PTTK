#include "logviewerwidget.h"
#include "loglineinfo.h"
#include <string_view>

LogViewerWidget::LogViewerWidget(QWidget* parent) : QTreeWidget(parent) {}

void LogViewerWidget::setModel(const std::map<std::string, std::vector<std::string>>& model)
{
    for (const auto& pairInfo : model)
    {
        QTreeWidgetItem* rootItem = new QTreeWidgetItem(this);
        rootItem->setText(0, pairInfo.first.c_str());
        for (const auto& logTxt : pairInfo.second) 
        {
            LogLineInfo logInfo{logTxt};
            QTreeWidgetItem* log = new QTreeWidgetItem(rootItem);
            log->setText(0, logInfo.text().data());/*
            log->setText(1, logInfo.dateStr().data());
            log->setText(2, logInfo.fileName().data());
            log->setText(3, logInfo.fileLineNumber().data());
            log->setText(4, logInfo.fileLineNumber().data());*/
        }
    }
}
