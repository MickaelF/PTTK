#pragma once

#include <QDialog>
#include "ui_LogGeneratorDialog.h"

class LogGeneratorDialog : public QDialog, public Ui_LogGeneratorDialog
{
	Q_OBJECT
public: 
	LogGeneratorDialog(QWidget* parent = nullptr); 

private slots: 
	void onGenerateBtnPressed();

private: 
	int m_nbLines {};

};