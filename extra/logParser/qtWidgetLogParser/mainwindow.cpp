#include "mainwindow.h"

#include <QFileDialog>

#include "inifile.h"
#include "logParser.h"
#include "loggeneratordialog.h"

namespace
{
constexpr std::string_view iniFileName {"conf.ini"};
}

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
            if (m_ini.lastFolder().has_value()) open(m_ini.lastFolder().value().c_str());
        }
    }

    g_sortOptions->setHidden(true);
    connect(actionOpen, &QAction::triggered, this, &MainWindow::onOpenActionPressed);
    connect(actionLogGenerator, &QAction::triggered, this,
            &MainWindow::onLogGeneratorActionPressed);
}

void MainWindow::onLogGeneratorActionPressed()
{
    LogGeneratorDialog dialog;
    dialog.exec();
    if (dialog.openInEditor()) open(dialog.path());
}

void MainWindow::onOpenActionPressed()
{
    auto path = QFileDialog::getExistingDirectory(this, tr("Open log folder"));
    if (path.isEmpty()) return;
    open(path);
}

void MainWindow::open(const QString& path)
{
    LogParser parser(path.toStdString());
    g_parsedLogWidget->setData(parser.exec(
        parser.numberOfLines() > 15000 ? ParsingType::FileByFile : ParsingType::CompleteLogs));
    m_ini.setLastOpenedFolder(path.toStdString());
    IniFile().save<QtParserIniFile>(m_programDataPath.string(), m_ini);
}
