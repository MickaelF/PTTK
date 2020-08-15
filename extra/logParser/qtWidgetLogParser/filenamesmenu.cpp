#include "filenamesmenu.h"

FileNamesMenu::FileNamesMenu(QWidget* parent) : QMenu(parent) {}

void FileNamesMenu::setFileNames(QStringList fileNames)
{
    clear();
    qSort(fileNames);
    auto headerActions {new FileNamesActionWidget(this)};
    connect(headerActions, &FileNamesActionWidget::everythingPressed, this,
            &FileNamesMenu::onEverythingPressed);
    connect(headerActions, &FileNamesActionWidget::nothingPressed, this,
            &FileNamesMenu::onNothingPressed);
    addAction(headerActions);
    addSeparator();
    for (const auto& fileName : fileNames)
    {
        QAction* action {new QAction(fileName, this)};
        action->setCheckable(true);
        action->setChecked(true);
        addAction(action);
    }
}

QStringList FileNamesMenu::uncheckedFileNames() const
{
    QStringList uncheckedActions;
    for (auto& action : actions())
        if (action->isCheckable() && !action->isChecked())
            uncheckedActions.push_back(action->text());
    return uncheckedActions;
}

void FileNamesMenu::checkEverything() const
{
    for (auto& action : actions())
        if (action->isCheckable() && !action->isChecked()) action->setChecked(true);
}

void FileNamesMenu::onEverythingPressed() const
{
    checkEverything();
}

void FileNamesMenu::onNothingPressed() const 
{
    for (auto& action : actions())
        if (action->isCheckable() && action->isChecked()) action->setChecked(false);
}

void FileNamesMenu::mouseReleaseEvent(QMouseEvent* e)
{
    QAction* action = activeAction();
    if (!action || !action->isEnabled())
    {
        QMenu::mouseReleaseEvent(e);
        return;
    }

    QSignalBlocker blocker(action);
    action->setEnabled(false);
    action->setChecked(!action->isChecked());
    QMenu::mouseReleaseEvent(e);
    action->setEnabled(true);
}
