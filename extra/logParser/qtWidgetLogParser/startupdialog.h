#pragma once

#include <QDialog>

#include "ui_StartUpDialog.h"

class StartUpDialog : public QDialog, public Ui_StartUpDialog
{
    Q_OBJECT
public:
    StartUpDialog(QWidget* parent = nullptr);
    const QString& pathOpened() const { return m_pathOpened; }
    void updateRecentComboBox(const QStringList& recents);

protected: 
    void changeEvent(QEvent* event);

private slots:
    void onOpenPressed();
    void onGeneratePressed();
    void onOpenRecentPressed();

private:
    QString m_pathOpened;
};