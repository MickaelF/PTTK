#pragma once

#include <QTreeView>

class LogViewerWidget : public QTreeView
{
	Q_OBJECT
public: 
	LogViewerWidget(QWidget* parent = nullptr);

    void setData(const std::map<std::string, std::vector<std::string>>& data);

private: 

};
