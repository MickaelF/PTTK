#pragma once
#include <QDateTime>

#include <QSortFilterProxyModel>

class LogFilterProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT
public:
    LogFilterProxyModel(QObject* parent = nullptr);
	
	void setStartDate(const QDateTime& date);
    void setEndDate(const QDateTime& date); 

protected: 
	bool filterAcceptsRow(int row, const QModelIndex& parent) const override; 

private: 
	QDateTime m_startDate; 
	QDateTime m_endDate; 

};