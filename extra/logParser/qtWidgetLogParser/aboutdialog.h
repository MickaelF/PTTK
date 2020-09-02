#pragma once

#include <QDialog>
#include "ui_AboutDialog.h"

class AboutDialog : public QDialog, public Ui_AboutDialog
{
    Q_OBJECT
public:
    AboutDialog(QWidget* parent = nullptr);

private slots: 
    void openGitHub() const; 
    void openWebsite() const;
};