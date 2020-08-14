#include "logfilterproxymodel.h"

LogFilterProxyModel::LogFilterProxyModel(QObject* parent) : QSortFilterProxyModel(parent) {}

bool LogFilterProxyModel::filterAcceptsRow(int row, const QModelIndex& parent) const
{
    return true;
}

void LogFilterProxyModel::setStartDate(const QDateTime& date) {}
void LogFilterProxyModel::setEndDate(const QDateTime& date) {}