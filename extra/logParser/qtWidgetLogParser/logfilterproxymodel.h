#pragma once
#include <QDateTime>

#include <QSortFilterProxyModel>

#include <optional>

class LogFilterProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT
public:
    LogFilterProxyModel(QObject* parent = nullptr);
	
	void setStartDate(const std::optional<QDateTime>& date);
    void setEndDate(const std::optional<QDateTime>& date); 
	void setFilteredPriorities(const QStringList& priorities);
    void setFilteredFileNames(const QStringList& priorities);
    void resetParameters(); 
    

protected: 
	bool filterAcceptsRow(int row, const QModelIndex& parent) const override; 

private: 
	std::optional<QDateTime> m_startDate {std::nullopt}; 
	std::optional<QDateTime> m_endDate {std::nullopt}; 
	QStringList m_filteredPriorities;
    QStringList m_filteredFileNames;
};