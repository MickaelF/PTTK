#pragma once

#include <QComboBox>


class PriorityComboBox : public QComboBox
{
	Q_OBJECT
public: 
	PriorityComboBox(QWidget* parent = nullptr);

private slots:
    void updateComboBoxText(); 
};