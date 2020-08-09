#pragma once

#include <QHeaderView>

class LogViewerHorizontalHeader : public QHeaderView
{
	Q_OBJECT
public:
	LogViewerHorizontalHeader(QWidget* parent = nullptr);

protected:
	void mouseMoveEvent(QMouseEvent* event) override;

signals:
	void sortByColumn(int, Qt::SortOrder);

private slots:
	void onSectionClicked(int section);

private:
    int m_currentlySortedSection {-1};
    Qt::SortOrder m_currentSortOrder {Qt::AscendingOrder};
};