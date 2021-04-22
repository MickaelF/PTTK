#include "loggeneratordialog.h"

#include <pttk/executiontimer.h>

#include <QCheckBox>
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>

#include "loggenerator.h"
#include "progressdialog.h"

LogGeneratorDialog::LogGeneratorDialog(QWidget* parent) : QDialog(parent)
{
    setupUi(this);
    setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowTitleHint |
                   Qt::WindowCloseButtonHint);

    g_dateStart->setDateTime(QDateTime::currentDateTime());
    connect(g_cancelBtn, &QPushButton::clicked, this, [&]() { close(); });
    connect(g_generateBtn, &QPushButton::clicked, this,
            &LogGeneratorDialog::onGenerateBtnPressed);
}

void LogGeneratorDialog::onGenerateBtnPressed()
{
    m_path = QFileDialog::getExistingDirectory(this, "Select output folder");
    if (m_path.isEmpty()) return;
    try
    {
        LogGenerator generator {m_path.toStdString(), g_lineNumberSP->value(),
                                g_dateStart->dateTime().toTime_t()};
        ProgressDialog progress(tr("Generating log file..."), 0,
                                g_lineNumberSP->value(), generator);
        std::thread thread = std::thread {&LogGenerator::exec, &generator};
        progress.start();
        thread.join();
    }
    catch (std::exception& e)
    {
        QMessageBox::critical(this, tr("Log generation error"), e.what());
        reject();
        return;
    }
    QMessageBox::information(this, tr("Log generation ended"),
                             tr("Log file generated!"));
    accept();
}
