#include "logfilterproxymodel.h"

#include "loglineinfo.h"
#include "logviewermodel.h"

LogFilterProxyModel::LogFilterProxyModel(QObject* parent) : QSortFilterProxyModel(parent) {}

bool LogFilterProxyModel::filterAcceptsRow(int row, const QModelIndex& parent) const
{
    LogLineInfo info {static_cast<LogViewerModel*>(sourceModel())->logDataRef()[row]};

    if (m_filteredPriorities.contains(std::string(info.priority()).c_str())) return false;

    if (m_filteredFileNames.contains(std::string(info.fileName()).c_str())) return false; 

    auto rowDate =
        QDateTime::fromString(std::string(info.dateTimeStr()).c_str(), "yyyy-MM-dd hh:mm:ss");
    if (rowDate < m_startDate || rowDate > m_endDate) return false;

    return true;
}

void LogFilterProxyModel::setStartDate(const QDateTime& date)
{
    if (m_startDate != date) m_startDate = date;
}
void LogFilterProxyModel::setEndDate(const QDateTime& date)
{
    if (m_endDate != date) m_endDate = date;
}

void LogFilterProxyModel::setFilteredPriorities(const QStringList& priorities)
{
    m_filteredPriorities = priorities;
}

void LogFilterProxyModel::setFilteredFileNames(const QStringList& priorities)
{
    m_filteredFileNames = priorities;
}
