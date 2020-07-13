#pragma once
#include <QProgressDialog>
#include <QTimer>
#include <QObject>

class IProgress;

class ProgressDialog : public QObject
{
    Q_OBJECT
public:
    ProgressDialog(const QString& title, int min, int max, const IProgress& progress);
    ~ProgressDialog() = default;
    void start();

private slots:
    void onTimerKicked();

private:
    QProgressDialog m_progressDialog;
    QTimer m_timer;
    const IProgress& m_progress;
};
