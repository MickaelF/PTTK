#include "progressdialog.h"

#include <pttk/iprogress.h>
#include <pttk/log.h>

ProgressDialog::ProgressDialog(const QString& title, int min, int max,
                               const IProgress& progress)
    : m_progress(progress)
{
    m_progressDialog.setWindowFlags(Qt::WindowTitleHint);
    using namespace std::chrono_literals;
    m_progressDialog.setAutoClose(true);
    m_progressDialog.setLabelText(title);
    m_progressDialog.setMinimum(min);
    m_progressDialog.setMaximum(max);
    m_progressDialog.setCancelButtonText(QString());

    m_timer.setSingleShot(false);
    m_timer.setInterval(50ms);
    m_timer.start();
    connect(&m_timer, &QTimer::timeout, this, &ProgressDialog::onTimerKicked);
}

void ProgressDialog::start()
{
    m_timer.start();
    m_progressDialog.exec();
}

void ProgressDialog::onTimerKicked()
{
    m_progressDialog.setValue(m_progress.progress());
}
