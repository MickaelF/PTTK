#include "loggeneratordialog.h"

#include <QDateTime>
#include <QFileDialog>
#include <QStandardItemModel>
#include <QMessageBox>

#include "loggenerator.h"
#include "executiontimer.h"
#include "progressdialog.h"

LogGeneratorDialog::LogGeneratorDialog(QWidget* parent) : QDialog(parent)
{
    setupUi(this);
    g_dateStart->setDateTime(QDateTime::currentDateTime());
    connect(g_cancelBtn, &QPushButton::clicked, this, [&]() { close(); });
    connect(g_generateBtn, &QPushButton::clicked, this, &LogGeneratorDialog::onGenerateBtnPressed);
}

void LogGeneratorDialog::onGenerateBtnPressed()
{
    auto path = QFileDialog::getSaveFileName(this, tr("Generated file path"), QString(), "Text file (*.txt)");
    if (path.isEmpty()) return;

    try
    {
        LogGenerator generator {path.toStdString(), g_lineNumberSP->value()};

        ProgressDialog progress(tr("Generating log file..."), 0, g_lineNumberSP->value(), generator);
        progress.start();
        generator.exec(g_dateStart->dateTime().toTime_t());;
    }
    catch (std::exception& e)
    {
        QMessageBox::critical(this, tr("Log generation error"), e.what());
        close();
        return;
    }
    QMessageBox::information(this,tr("Log generation ended"), tr("Log file generated!"));
    close();
}
