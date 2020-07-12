#include "logviewerwidget.h"

LogViewerWidget::LogViewerWidget(QWidget* parent) : QTreeWidget(parent) {}

void LogViewerWidget::setModel(const std::map<std::string, std::vector<std::string>>& model)
{
    for (const auto& pairInfo : model)
    {
        QTreeWidgetItem* rootItem = new QTreeWidgetItem();
        rootItem->setText(0, pairInfo.first.c_str());
        for (const auto& logTxt : pairInfo.second) 
        {
            QTreeWidgetItem* log = new QTreeWidgetItem(rootItem);
            log->setText(0, logTxt.c_str());
        }
    }
}
