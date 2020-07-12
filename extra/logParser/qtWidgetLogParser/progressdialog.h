#pragma once

#include <QDialog>
#include <QThread>
#include <functional>

#include "ui_ProgressDialog.h"

class IProgress;

class ProgressDialog : public QDialog, public Ui_ProgressDialog
{
public:
    ProgressDialog(QWidget* parent = nullptr);
    ~ProgressDialog();

    void setProgressObject(IProgress* progress);
    void run(int start, int max);

    void stopExecution();

private:
    class UpdateProgress : public QThread
    {
        Q_OBJECT
    public:
        UpdateProgress(IProgress* p, QObject* parent = nullptr)
            : QThread(parent),
              m_progress(p),
              m_dialog(qobject_cast<ProgressDialog*>(parent))
        {
        }
        void run();

    private:
        IProgress* m_progress {nullptr};
        ProgressDialog* m_dialog {nullptr};
    };

    UpdateProgress* m_executionThread {nullptr};
};