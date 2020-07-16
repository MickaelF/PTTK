#include "loggeneratordialog.h"

#include <QCheckBox>
#include <QDateTime>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>

#include "executiontimer.h"
#include "loggenerator.h"
#include "progressdialog.h"

LogGeneratorDialog::LogGeneratorDialog(QWidget* parent) : QDialog(parent)
{
    setupUi(this);
    g_dateStart->setDateTime(QDateTime::currentDateTime());
    connect(g_cancelBtn, &QPushButton::clicked, this, [&]() { close(); });
    connect(g_generateBtn, &QPushButton::clicked, this, &LogGeneratorDialog::onGenerateBtnPressed);
    connect(g_open, &QCheckBox::stateChanged, this,
            [&](int state) { m_openInEditor = (state == Qt::Checked); });
}

void LogGeneratorDialog::onGenerateBtnPressed()
{
        auto path = QFileDialog::getExistingDirectory(this, tr("Generated file path"), QString());
        if (path.isEmpty()) return;
        m_path = path;
        try
        {
            LogGenerator generator {path.toStdString(), g_lineNumberSP->value(),
                                    g_dateStart->dateTime().toTime_t()};
            ProgressDialog progress(tr("Generating log file..."), 0, g_lineNumberSP->value(),
                                    generator);
            std::thread thread = std::thread {&LogGenerator::exec, &generator};
            progress.start();
            thread.join();
        }
        catch (std::exception& e)
        {
            QMessageBox::critical(this, tr("Log generation error"), e.what());
            close();
            return;
        }
        QMessageBox::information(this, tr("Log generation ended"), tr("Log file generated!"));
        close();
}
