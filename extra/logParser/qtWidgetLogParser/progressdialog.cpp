#include "progressdialog.h"

#include "iprogress.h"
#include "log.h"

ProgressDialog::ProgressDialog(QWidget* parent) : QDialog(parent)
{
    setupUi(this);
}

ProgressDialog::~ProgressDialog()
{
    stopExecution();
}

void ProgressDialog::setProgressObject(IProgress* progress)
{
    m_executionThread = new UpdateProgress(progress, this);
}

void ProgressDialog::run(int start, int max)
{
    show();
    g_progress->setMinimum(start);
    g_progress->setMaximum(max);
    m_executionThread->run();
}

void ProgressDialog::stopExecution()
{
    if (m_executionThread->isRunning()) m_executionThread->terminate();
    close();
}

void ProgressDialog::UpdateProgress::run() 
{
    m_dialog->g_progress->setValue(m_progress->progress());
}
