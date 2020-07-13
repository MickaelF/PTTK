#include "progressdialog.h"

#include "iprogress.h"
#include "log.h"

ProgressDialog::ProgressDialog(const QString& title, int min, int max, const IProgress& progress):
    m_progress(progress)
{
    using namespace std::chrono_literals;
    m_progressDialog.setAutoClose(true);
    m_progressDialog.setLabelText(title);
    m_progressDialog.setMinimum(min);
    m_progressDialog.setMaximum(max);

    m_timer.setSingleShot(false);
    m_timer.setInterval(50ms);
    m_timer.start();
    connect(&m_timer, &QTimer::timeout, this, &ProgressDialog::onTimerKicked);
}

void ProgressDialog::start()
{
    m_progressDialog.show();
    m_timer.start();
}

void ProgressDialog::onTimerKicked()
{
    m_progressDialog.setValue(m_progress.progress());
}

