#include "startupdialog.h"

#include <QAction>
#include <QFileDialog>
#include <QMenu>

#include "loggeneratordialog.h"

StartUpDialog::StartUpDialog(QWidget* parent) : QDialog(parent)
{
    setupUi(this);
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
        QAction* action = menu->addAction(recent, this, &StartUpDialog::onOpenRecentPressed);
}