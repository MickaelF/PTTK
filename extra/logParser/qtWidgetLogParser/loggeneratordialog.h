#pragma once

#include <QDialog>
#include "ui_LogGeneratorDialog.h"

class LogGeneratorDialog : public QDialog, public Ui_LogGeneratorDialog
{
	Q_OBJECT
public: 
	LogGeneratorDialog(QWidget* parent = nullptr); 
	bool openInEditor() const { return g_open->isChecked(); }
    QString path() const { return g_outputPath->text(); }

private slots: 
	void onGenerateBtnPressed();
    void onOutputPathChanged(const QString& path);

private: 
	int m_nbLines {};
};