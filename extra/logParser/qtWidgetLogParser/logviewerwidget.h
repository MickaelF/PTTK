#pragma once

#include <QTableView>

#include "logParser.h"
#include "logstyledelegate.h"
#include "logviewermodel.h"
class LogFilterProxyModel;

class LogViewerWidget : public QTableView
{
	Q_OBJECT
public:
	LogViewerWidget(QWidget* parent = nullptr);

	void open(const QString& openPath);
    QDateTime firstDate() const; 
	QDateTime lastDate() const; 
	QStringList fileNames() const; 

	void setFilterStartDate(const QDateTime& date) const; 
	void setFilterEndDate(const QDateTime& date) const;
    void setFilteredPriorities(const QStringList& priorities) const; 
	void setFilteredFileNames(const QStringList& priorities) const; 
    void updateFilter() const; 

protected:
    void resizeEvent(QResizeEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override; 

private slots: 
	void onResizeRow(int index, int height);

private:
	void launchParsing();

	LogViewerModel m_model;
    LogFilterProxyModel* m_sortFilter {nullptr};
	LogParser m_parser;
    LogStyleDelegate m_styleDelegate; 
	std::filesystem::directory_entry m_fileIt;
};