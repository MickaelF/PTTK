#include "logviewerwidget.h"
#include "loglineinfo.h"
#include <string_view>
#include <QStandardItemModel>

namespace
{
    QColor priorityColor(std::string_view priority) {
        if (priority == "Info") return QColor {255, 0, 0};
        else if (priority == "Debug") return QColor {255, 255, 0};
        else if (priority == "Warning") return QColor {255, 0, 255};
        else if (priority == "Error") return QColor {0, 255, 0};
        else if (priority == "Fatal") return QColor {0, 255, 255};
        else if (priority == "Remember") return QColor {0, 0, 255};
        else if (priority == "Execution") return QColor {0, 0, 0};
    
    }
}

LogViewerWidget::LogViewerWidget(QWidget* parent) : QTreeView(parent) {}

void LogViewerWidget::setData(const std::map<std::string, std::vector<std::string>>& data)
{
    const QStringList columnNames {tr("Color"), tr("Date"), tr("Priority"), tr("File"), tr("Text")};

    QStandardItemModel* model = new QStandardItemModel(data.size(), columnNames.size(),this);
    model->setHorizontalHeaderLabels(columnNames);
    int i = 0;
    for (const auto& pairInfo : data)
    {
        QStandardItem* rootItem = new QStandardItem(pairInfo.second.size(), columnNames.size());
        rootItem->setText(pairInfo.first.c_str());
        int j = 0;
        for (const auto& logTxt : pairInfo.second) 
        {
            LogLineInfo logInfo{logTxt};
            QStandardItem* coloredItem = new QStandardItem();
            coloredItem->setData(priorityColor(logInfo.priority()), Qt::BackgroundRole);
            rootItem->setChild(j, 0, coloredItem);
            rootItem->setChild(j, 1, new QStandardItem(std::string(logInfo.dateTimeStr()).c_str()));
            rootItem->setChild(j, 2, new QStandardItem(std::string(logInfo.priority()).c_str()));
            rootItem->setChild(j, 3, new QStandardItem(std::string(logInfo.fileLineNumber()).c_str()));
            rootItem->setChild(j, 4, new QStandardItem(std::string(logInfo.text()).c_str()));
            j++;
        }
        model->setItem(i, 0, rootItem);
        i++;
    }
    setModel(model);
}
