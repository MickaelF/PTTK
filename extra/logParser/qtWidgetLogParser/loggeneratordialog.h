#pragma once

#include <QDialog>
#include "ui_LogGeneratorDialog.h"

class LogGeneratorDialog : public QDialog, public Ui_LogGeneratorDialog
{
	Q_OBJECT
public: 
	LogGeneratorDialog(QWidget* parent = nullptr); 
	bool openInEditor() const { return m_openInEditor; }
    const QString& path() const { return m_path; }

private slots: 
	void onGenerateBtnPressed();

private: 
	int m_nbLines {};
    bool m_openInEditor {false};
	QString m_path; 
};