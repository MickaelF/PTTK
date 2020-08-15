#include "mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

#include "inifile.h"
#include "loggeneratordialog.h"
#include "prioritylabelfactory.h"

namespace
{
constexpr std::string_view iniFileName {"conf.ini"};
constexpr std::string_view windowName {"Qt Log Parser - %1"};
} // namespace

MainWindow::MainWindow(const std::filesystem::path& programDataPath)
    : QMainWindow(nullptr),
      m_programDataPath(programDataPath)
{
    setupUi(this);
    QApplication::setFont(this->font());
    m_startDialog.setModal(true);
    m_programDataPath.append(iniFileName);
    if (std::filesystem::exists(m_programDataPath))
    {
        auto ini = IniFile().readIni<QtParserIniFile>(m_programDataPath.string());
        if (ini.has_value())
        {
            m_ini = *ini;
            updateOpenRecently();
        }
    }

    if (m_ini.lastFolder().has_value() && QDir(m_ini.lastFolder().value().c_str()).exists())
        open(m_ini.lastFolder().value().c_str());
    else
        displayStartUpDialog();

    setupDisplayPrioritiesBox();
    
    g_displayedFiles->setMenu(&m_fileNamesMenu);

    connect(actionOpen, &QAction::triggered, this, &MainWindow::onOpenActionPressed);
    connect(actionLogGenerator, &QAction::triggered, this,
            &MainWindow::onLogGeneratorActionPressed);
    connect(actionQuit, &QAction::triggered, [&]() { close(); });
    connect(&m_startDialog, &QDialog::accepted, this, &MainWindow::onStartUpDialogAccepted);
    connect(&m_startDialog, &QDialog::rejected, [&]() { close(); });

    connect(g_startDate, &QDateTimeEdit::dateTimeChanged, this, &MainWindow::onStartDateChanged);
    connect(g_endDate, &QDateTimeEdit::dateTimeChanged, this, &MainWindow::onEndDateChanged);
    connect(g_applyBtn, &QPushButton::pressed, this, &MainWindow::onApplyPressed);
    connect(g_defaultBtn, &QPushButton::pressed, this, &MainWindow::onDefaultPressed);
}

void MainWindow::onLogGeneratorActionPressed()
{
    LogGeneratorDialog dialog;
    if (dialog.exec() == QDialog::Accepted && dialog.openInEditor()) open(dialog.path());
}

void MainWindow::mousePressEvent(QMouseEvent* event) 
{
    g_startDate->clearFocus();
    g_endDate->clearFocus();
    QMainWindow::mousePressEvent(event);
}

void MainWindow::onOpenActionPressed()
{
    auto path = QFileDialog::getExistingDirectory(this, tr("Open log folder"));
    if (path.isEmpty()) return;
    open(path);
}

void MainWindow::onOpenRecentlyPressed()
{
    QAction* action {qobject_cast<QAction*>(sender())};
    if (!QDir(action->text()).exists())
    {
        QMessageBox::warning(this, tr("Folder not found"),
                             tr("The selected folder could not be found."));
        m_ini.removeFolder(action->text().toStdString());
        IniFile().save<QtParserIniFile>(m_programDataPath.string(), m_ini);
        displayStartUpDialog();
        return;
    }
    open(action->text());
}

void MainWindow::onStartUpDialogAccepted()
{
    g_sortOptions->setHidden(true);
    m_startDialog.hide();
    open(m_startDialog.pathOpened());
}

void MainWindow::onApplyPressed()
{
    g_parsedLogWidget->setFilterStartDate(g_startDate->dateTime());
    g_parsedLogWidget->setFilterEndDate(g_endDate->dateTime());

    QStringList filteredPriorities;
    for (auto& priority : m_prioritySelection)
        if (!priority->isSelected()) filteredPriorities.push_back(priority->text());
    g_parsedLogWidget->setFilteredPriorities(filteredPriorities);

    g_parsedLogWidget->setFilteredFileNames(m_fileNamesMenu.uncheckedFileNames());

    g_parsedLogWidget->updateFilter();
}

void MainWindow::onDefaultPressed()
{
    updateDate();
    m_fileNamesMenu.checkEverything();
    for (auto& priority : m_prioritySelection) priority->setSelected(true);
    repaint();
}

void MainWindow::onStartDateChanged(const QDateTime& dateTime)
{
    if (dateTime < g_endDate->dateTime()) return;

    blockSignals(true);
    g_endDate->setDateTime(dateTime.addSecs(1));
    blockSignals(false);
}

void MainWindow::onEndDateChanged(const QDateTime& dateTime)
{
    if (dateTime > g_startDate->dateTime()) return;

    blockSignals(true);
    g_startDate->setDateTime(dateTime.addSecs(-1));
    blockSignals(false);
}

void MainWindow::displayStartUpDialog()
{
    QStringList recents;
    for (auto folder : m_ini.previousFolders())
        if (folder.has_value()) recents << folder->c_str();
    setWindowTitle("Qt Log Parser");
    m_startDialog.updateRecentComboBox(recents);
    m_startDialog.show();
}

void MainWindow::setupDisplayPrioritiesBox()
{
    using namespace LogPriority;

    for (int i = 0; i < enumMaxNumber(); ++i)
    {
        const QString name {enumToStr(static_cast<Priorities>(i)).data()};
        auto label {PriorityLabelFactory::makePriorityLabel(name)};
        m_prioritySelection.insert(static_cast<Priorities>(i), label);
        g_displayedPriorities->addWidget(label);
    }
}

void MainWindow::updateOpenRecently()
{
    menuOpenRecently->clear();
    for (auto folder : m_ini.previousFolders())
    {
        if (!folder.has_value()) continue;
        if (!QDir(folder.value().c_str()).exists())
        {
            m_ini.removeFolder(*folder);
            continue;
        }
        QAction* action = new QAction(folder.value().c_str());
        menuOpenRecently->addAction(action);
        connect(action, &QAction::triggered, this, &MainWindow::onOpenRecentlyPressed);
    }

    IniFile().save<QtParserIniFile>(m_programDataPath.string(), m_ini);
}

void MainWindow::open(const QString& path)
{
    try
    {
        g_parsedLogWidget->open(path);
    }
    catch (std::exception& e)
    {
        QMessageBox::critical(this, tr("Parser error"),
                              tr("Could not open log file.\nError : %1").arg(e.what()));
        m_ini.removeFolder(path.toStdString());
        IniFile().save<QtParserIniFile>(m_programDataPath.string(), m_ini);
        displayStartUpDialog();
    }
    if (m_ini.setLastOpenedFolder(path.toStdString())) updateOpenRecently();
    IniFile().save<QtParserIniFile>(m_programDataPath.string(), m_ini);
    setWindowTitle(QString(windowName.data()).arg(path));

    updateDate();
    updateFileNames();
    onApplyPressed();

    g_sortOptions->setVisible(true);
}

void MainWindow::updateDate()
{
    auto startDate = g_parsedLogWidget->firstDate();
    auto endDate = g_parsedLogWidget->lastDate();
    g_startDate->setMinimumDateTime(startDate);
    g_startDate->setDateTime(startDate);
    g_startDate->setMaximumDateTime(endDate);

    g_endDate->setMinimumDateTime(startDate);
    g_endDate->setDateTime(endDate);
    g_endDate->setMaximumDateTime(endDate);
}

void MainWindow::updateFileNames()
{
    m_fileNamesMenu.setFileNames(g_parsedLogWidget->fileNames());
}