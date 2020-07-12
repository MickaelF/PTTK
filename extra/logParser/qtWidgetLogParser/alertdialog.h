#pragma once

#include <QDialog>
#include <QString>

#include "ui_AlertDialog.h"

class AlertDialog : public QDialog, public Ui_AlertDialog
{
    Q_OBJECT
public:
    enum class Level
    {
        Info,
        Warning,
        Error,
        Fatal
    };
    enum class Option
    {
        Ok,
        CancelOrContinue
    };

    AlertDialog(AlertDialog::Level l, AlertDialog::Option o, const QString& message,
                const QString& title = QString(), QWidget* parent = nullptr);

private:
};