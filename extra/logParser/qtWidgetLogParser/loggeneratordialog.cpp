#include "loggeneratordialog.h"

#include <QDateTime>
#include <QFileDialog>
#include <QStandardItemModel>

#include "alertdialog.h"
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

        ProgressDialog progress; 
        progress.setProgressObject(&generator);
        progress.run(0, g_lineNumberSP->value());

        generator.exec(g_dateStart->dateTime().toTime_t());
        progress.stopExecution();
    }
    catch (std::exception& e)
    {
        AlertDialog alert(AlertDialog::Level::Error, AlertDialog::Option::Ok, tr(e.what()));
        close();
        return;
    }

    AlertDialog generationEnded(AlertDialog::Level::Info, AlertDialog::Option::Ok,
                                tr("Log file generated"));
    close();
}