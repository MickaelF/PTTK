#include "logviewermodel.h"

#include <QStandardItemModel>
#include <string_view>

#include "loglineinfo.h"

LogViewerModel::LogViewerModel(QObject* parent) : QAbstractTableModel(parent) {}

int LogViewerModel::rowCount(const QModelIndex&) const
{
    return m_data.size();
}

int LogViewerModel::columnCount(const QModelIndex&) const
{
    return 4;
}

Qt::ItemFlags LogViewerModel::flags(const QModelIndex& index) const
{
    return Qt::ItemIsEnabled;
}

bool LogViewerModel::insertColumns(int column, int count,
                                   const QModelIndex& parent)
{
    beginInsertColumns(parent, column, column + count);
    endInsertColumns();
    return false;
}

bool LogViewerModel::removeColumns(int column, int count,
                                   const QModelIndex& parent)
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
    beginRemoveRows(parent, row, row + count - 1);
    endRemoveRows();
    return true;
}

QVariant LogViewerModel::headerData(int section, Qt::Orientation orientation,
                                    int role) const
{
    const QStringList columnNames {tr("Priority"), tr("Date"), tr("File"),
                                   tr("Text")};
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal) return columnNames[section];
    }
    return QVariant();
}

void LogViewerModel::setLogData(std::vector<std::string> data)
{
    removeRows(0, rowCount());

    m_data = data;
    insertRows(0, m_data.size() - 1);
}

time_t LogViewerModel::firstDate() const
{
    if (m_data.empty()) return 0;
    return LogLineInfo(m_data.front()).date();
}

time_t LogViewerModel::lastDate() const
{
    if (m_data.empty()) return 0;
    return LogLineInfo(m_data.back()).date();
}

QStringList LogViewerModel::fileNames() const
{
    QStringList fileNames;
    for (const auto& data : m_data)
    {
        LogLineInfo info {data};
        QString fileName {std::string(info.fileName()).c_str()};
        if (!fileNames.contains(fileName)) fileNames.push_back(fileName);
    }
    return fileNames;
}

QVariant LogViewerModel::data(const QModelIndex& index, int role) const
{
    LogLineInfo logInfo {m_data[index.row()]};
    if (role == Qt::DisplayRole)
    {
        switch (static_cast<ColumnType>(index.column()))
        {
            case ColumnType::Data: return std::string(logInfo.text()).c_str();
            case ColumnType::Date:
                return ("<b>" + std::string(logInfo.dateStr()) + "</b><br/>" +
                        std::string(logInfo.timeStr()))
                    .c_str();
            case ColumnType::Priority:
                return std::string(logInfo.priority()).c_str();
            case ColumnType::File:
                return std::string(logInfo.fileName()).c_str();
            default: return QVariant();
        }
    }
    else if (role == Qt::TextAlignmentRole &&
             index.column() != static_cast<int>(ColumnType::Data))
        return Qt::AlignCenter;
    return QVariant(QVariant::Invalid);
}
