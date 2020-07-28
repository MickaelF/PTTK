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

    g_outputPath->setCaption("Select output folder");
    g_outputPath->setExplorerType(ExplorerPathWidget::ExplorerType::ExistingFolder);
    g_dateStart->setDateTime(QDateTime::currentDateTime());
    connect(g_cancelBtn, &QPushButton::clicked, this, [&]() { close(); });
    connect(g_generateBtn, &QPushButton::clicked, this, &LogGeneratorDialog::onGenerateBtnPressed);
    connect(g_outputPath, &ExplorerPathWidget::textChanged, this,
            &LogGeneratorDialog::onOutputPathChanged);
}

void LogGeneratorDialog::onGenerateBtnPressed()
{
        try
        {
            LogGenerator generator {g_outputPath->text().toStdString(), g_lineNumberSP->value(),
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
            reject();
            return;
        }
        QMessageBox::information(this, tr("Log generation ended"), tr("Log file generated!"));
        accept();
}

void LogGeneratorDialog::onOutputPathChanged(const QString& path) 
{
    bool pathExists = QDir(path).exists();
    g_generateBtn->setEnabled(pathExists);

    if (!pathExists)
    {
        QMessageBox::warning(
            this, tr("Output Path Error"),
            tr("The selected folder does not seems to exists. Please correct output path."));
        g_outputPath->blockSignals(true);
        g_outputPath->setText(QString());    
        g_outputPath->blockSignals(false);
    }
}
