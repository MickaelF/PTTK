#pragma once

#include <QTableView>

#include "logParser.h"
#include "logviewermodel.h"
class QSortFilterProxyModel;

class LogViewerWidget : public QTableView
{
	Q_OBJECT
public:
	LogViewerWidget(QWidget* parent = nullptr);

	void open(const QString& openPath);

private:
	void launchParsing();

	LogViewerModel m_model;
	QSortFilterProxyModel* m_sortFilter{ nullptr };
	LogParser m_parser;
	std::filesystem::directory_entry m_fileIt;
};