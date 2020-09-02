#pragma once

#include <QDialog>
#include "ui_LogGeneratorDialog.h"

class LogGeneratorDialog : public QDialog, public Ui_LogGeneratorDialog
{
	Q_OBJECT
public: 
	explicit LogGeneratorDialog(QWidget* parent = nullptr); 
	bool openInEditor() const { return g_open->isChecked(); }
    const QString& path() const { return m_path; }

private slots: 
	void onGenerateBtnPressed();

private: 
	int m_nbLines {0};
    QString m_path;
};