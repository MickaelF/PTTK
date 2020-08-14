#pragma once

#include <QTableView>

#include "logParser.h"
#include "logstyledelegate.h"
#include "logviewermodel.h"
class QSortFilterProxyModel;

class LogViewerWidget : public QTableView
{
	Q_OBJECT
public:
	LogViewerWidget(QWidget* parent = nullptr);

	void open(const QString& openPath);

protected:
    void resizeEvent(QResizeEvent* event) override; 

private slots: 
	void onResizeRow(int index, int height);

private:
	void launchParsing();

	LogViewerModel m_model;
	QSortFilterProxyModel* m_sortFilter{ nullptr };
	LogParser m_parser;
    LogStyleDelegate m_styleDelegate; 
	std::filesystem::directory_entry m_fileIt;
};