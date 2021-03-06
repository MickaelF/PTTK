#include "startupdialog.h"

#include <QAction>
#include <QFileDialog>
#include <QMenu>

#include "loggeneratordialog.h"

StartUpDialog::StartUpDialog(QWidget* parent) : QDialog(parent)
{
    setupUi(this);
    setWindowFlags(Qt::WindowSystemMenuHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    g_recent->setMenu(new QMenu());
    connect(g_open, &QPushButton::pressed, this, &StartUpDialog::onOpenPressed);
    connect(g_generate, &QPushButton::pressed, this, &StartUpDialog::onGeneratePressed);
}

void StartUpDialog::onOpenPressed()
{
    m_pathOpened = QFileDialog::getExistingDirectory(this, "Open log directory");
    if (!m_pathOpened.isEmpty()) accept();
}

void StartUpDialog::onGeneratePressed()
{
    LogGeneratorDialog dialog;
    if (dialog.exec() == QDialog::Accepted && dialog.openInEditor())
    {
        m_pathOpened = dialog.path();
        accept();
    }
}

void StartUpDialog::onOpenRecentPressed()
{
    QAction* action {qobject_cast<QAction*>(sender())};
    m_pathOpened = action->text();
    accept();
}

void StartUpDialog::updateRecentComboBox(const QStringList& recents)
{
    auto menu = g_recent->menu();
    menu->clear();
    if (recents.isEmpty())
    {
        g_recent->setDisabled(true);
        return;
    }
    if (!g_recent->isEnabled()) g_recent->setEnabled(true);
    for (auto& recent : recents)
        menu->addAction(recent, this, &StartUpDialog::onOpenRecentPressed);
}

void StartUpDialog::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange)
    {
        g_generate->setText(tr("Generate"));
        g_recent->setText(tr("Recent"));
        g_open->setText(tr("Open"));
        g_generateDesc->setText(tr("Generate: Generate a dummy log file to experiment with the reader."));
        g_openDesc->setText(tr("Open: Select a folder containing logs to be read."));
        g_recentDesc->setText(tr("Recent: Open an previously opened folder."));
    }
    else
        QWidget::changeEvent(event);
}
