#pragma once

#include <QTreeWidget>

class LogViewerWidget : public QTreeWidget
{
	Q_OBJECT
public: 
	LogViewerWidget(QWidget* parent = nullptr);

	void setModel(const std::map<std::string, std::vector<std::string>>& model);

private: 

};
