#include "mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

#include "inifile.h"
#include "logParser.h"
#include "loggeneratordialog.h"

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
    //else
        // TODO Display folder selection screen.

    g_sortOptions->setHidden(true);
    connect(actionOpen, &QAction::triggered, this, &MainWindow::onOpenActionPressed);
    connect(actionLogGenerator, &QAction::triggered, this,
            &MainWindow::onLogGeneratorActionPressed);
}

void MainWindow::onLogGeneratorActionPressed()
{
    LogGeneratorDialog dialog;
    if (dialog.exec() == QDialog::Accepted && dialog.openInEditor()) open(dialog.path());
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
        // TODO Display folder selection screen.
        return; 
    }
    open(action->text());
}

void MainWindow::updateOpenRecently()
{
    menuOpenRecently->clear();
    for (auto folder : m_ini.previousFolders())
    {
        if (!folder.has_value() || !QDir(folder.value().c_str()).exists()) return;
        QAction* action = new QAction(folder.value().c_str());
        menuOpenRecently->addAction(action);
        connect(action, &QAction::triggered, this, &MainWindow::onOpenRecentlyPressed);
    }
}

void MainWindow::open(const QString& path)
{
    try
    {
        LogParser parser(path.toStdString());
        g_parsedLogWidget->setData(parser.exec(
            parser.numberOfLines() > 15000 ? ParsingType::FileByFile : ParsingType::CompleteLogs));
        if (m_ini.setLastOpenedFolder(path.toStdString())) updateOpenRecently();
        IniFile().save<QtParserIniFile>(m_programDataPath.string(), m_ini);
        setWindowTitle(QString(windowName.data()).arg(path));
    }
    catch(std::exception& e)
    {
        QMessageBox::critical(this, tr("Parser error"),
                              tr("Could not open log file.\nError : %1").arg(e.what()));
        // TODO Display folder selection screen.
    }
}
