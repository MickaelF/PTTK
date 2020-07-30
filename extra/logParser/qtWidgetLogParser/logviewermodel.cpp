#include "logviewermodel.h"

#include <QStandardItemModel>
#include <string_view>

#include "loglineinfo.h"

namespace
{
QColor priorityColor(std::string_view priority)
{
    if (priority == "Info")
        return QColor {36, 191, 78};
    else if (priority == "Debug")
        return QColor {36, 46, 191};
    else if (priority == "Warning")
        return QColor {229, 235, 63};
    else if (priority == "Error")
        return QColor {235, 183, 63};
    else if (priority == "Fatal")
        return QColor {214, 0, 0};
    else if (priority == "Remember")
        return QColor {240, 29, 240};
    else if (priority == "Execution")
        return QColor {0, 0, 0};
}
} // namespace

LogViewerModel::LogViewerModel(QObject* parent) : QAbstractTableModel(parent) {
}

int LogViewerModel::rowCount(const QModelIndex&) const
{
    return m_data.size();
}

int LogViewerModel::columnCount(const QModelIndex&) const
{
    return 5;
}

Qt::ItemFlags LogViewerModel::flags(const QModelIndex& index) const
{
    return Qt::ItemIsEnabled;
}

bool LogViewerModel::insertColumns(int column, int count, const QModelIndex& parent)
{
    beginInsertColumns(parent, column, column + count);

    endInsertColumns();
    return false;
}

bool LogViewerModel::removeColumns(int column, int count, const QModelIndex& parent)
{
    beginRemoveColumns(parent, column, column + count);
    endRemoveColumns();
    return false;
}

bool LogViewerModel::insertRows(int row, int count, const QModelIndex& parent)
{
    beginInsertRows(parent, row, row + count);
    endInsertRows();
    return false;
}

bool LogViewerModel::removeRows(int row, int count, const QModelIndex& parent)
{
    if (count <= 0) return true;
    if (count > rowCount()) return false;
    beginRemoveRows(parent, row, row + count);
    endRemoveRows();
    return true;
}

QVariant LogViewerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    const QStringList columnNames {"", tr("Priority"), tr("Date"),tr("File"), tr("Text")};
    if (role == Qt::DisplayRole) 
    {
        if (orientation == Qt::Horizontal)
            return columnNames[section];
        else
            return section;
    }
    return QVariant();
}

void LogViewerModel::setLogData(const std::vector<std::string>& data)
{
    removeRows(0, rowCount());

    m_data = data;
    insertRows(0, m_data.size());
}

QVariant LogViewerModel::data(const QModelIndex& index, int role) const
{
    LogLineInfo logInfo {m_data[index.row()]};
    if (role == Qt::BackgroundColorRole &&
        index.column() == static_cast<int>(ColumnType::PriorityColor))
        return priorityColor(logInfo.priority());
    else if (role == Qt::DisplayRole)
    {
        switch (static_cast<ColumnType>(index.column()))
        {
            case ColumnType::Data: return std::string(logInfo.text()).c_str();
            case ColumnType::Date: return std::string(logInfo.dateTimeStr()).c_str();
            case ColumnType::Priority: return std::string(logInfo.priority()).c_str();
            case ColumnType::File: return std::string(logInfo.fileName()).c_str();
            default: return QVariant();
        }
    }
    else if (role == Qt::TextAlignmentRole && index.column() != static_cast<int>(ColumnType::Data))
        return Qt::AlignCenter;
    return QVariant(QVariant::Invalid);
}
