#include "logviewerwidget.h"

#include <QHeaderView>
#include <QScrollBar>
#include <QSortFilterProxyModel>
#include <QTemporaryDir>

#include "log.h"
#include "logParser.h"
#include "loglineinfo.h"
#include "logstyledelegate.h"
#include "progressdialog.h"

namespace
{
constexpr int cPriorityColumnWidth {100};
constexpr int cRowHeight {60};
constexpr int cMaxLines {1000000};
} // namespace

LogViewerWidget::LogViewerWidget(QWidget* parent) : QTableView(parent)
{

    QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(&m_model);
    setModel(proxyModel);
    setSortingEnabled(true);

    auto header = horizontalHeader();
    header->setSectionResizeMode(0, QHeaderView::Fixed);
    header->setSectionResizeMode(1, QHeaderView::Fixed);
    header->setSectionResizeMode(2, QHeaderView::Interactive);
    header->setSectionResizeMode(3, QHeaderView::Stretch);
    setColumnWidth(0, cPriorityColumnWidth);
    setColumnWidth(1, 120);
    setColumnWidth(2, 100);
    setColumnWidth(3, 600);
    setStyleSheet("LogViewerWidget::item { border: 0px; padding: 50px}");

    auto vHeader = verticalHeader();
    vHeader->setSectionResizeMode(QHeaderView::Fixed);
    vHeader->setDefaultSectionSize(cRowHeight);

    setItemDelegate(new LogStyleDelegate(cPriorityColumnWidth, cRowHeight, this));
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setFocusPolicy(Qt::NoFocus);
    setSelectionMode(QAbstractItemView::NoSelection);

    setShowGrid(false);
    setFrameStyle(QFrame::NoFrame);
    connect(verticalScrollBar(), &QScrollBar::sliderMoved, this, &LogViewerWidget::onSliderMoved);
    connect(horizontalHeader(), &QHeaderView::sectionClicked, this,
            &LogViewerWidget::onSectionClicked);
}

void LogViewerWidget::open(const QString& openPath)
{
    m_parser.setInputPath(openPath.toStdString());
    launchParsing();
}

void LogViewerWidget::onSliderMoved(int value)
{
    // Value is the row number displayed at the top of the screen.
    // TODO load next or previous files when comming to the top or the botttom.
    lInfo << value;
}
void LogViewerWidget::onSectionClicked(int section)
{
    /*verticalScrollBar()->setValue(0);
    if (m_parser.numberOfLines() > cMaxLines)
    {
        m_parser.setSortType(static_cast<LogSort>(section));
        launchParsing();
    }*/
    /*else
    {
        auto& data = m_model.logDataRef();
        std::sort(data.begin(), data.end(), [&section](auto& left, auto& right) {
            switch (static_cast<LogSort>(section))
            {
                case LogSort::Date: return LogLineInfo(left).date() < LogLineInfo(right).date();
                case LogSort::Files:
                    return LogLineInfo(left).fileName() < LogLineInfo(right).fileName();
                case LogSort::Type:
                    return LogLineInfo(left).priority() < LogLineInfo(right).priority();
            }
            return false;
        });
        emit data
    }*/
}

void LogViewerWidget::launchParsing()
{
    if (m_tempDir) delete m_tempDir;
    m_tempDir = new QTemporaryDir();
    ProgressDialog progress(tr("Parsing log files..."), 0, m_parser.numberOfLines(), m_parser);
    std::promise<std::vector<std::string>> p;
    auto f = p.get_future();
    std::thread thread = std::thread {&LogParser::execToVector, &m_parser, std::move(p)};
    progress.start();
    thread.join();
    lInfo << "Vector size " << f.get().size();
    m_model.setLogData(f.get());

   /* if (m_parser.numberOfLines() > cMaxLines) {}
    else
    {
        std::vector<std::string> data;
        for (auto& p : std::filesystem::directory_iterator(m_tempDir->path().toStdString()))
        {
            if (!p.is_regular_file()) continue;
            std::ifstream file(p.path().c_str());
            std::string line;
            while (std::getline(file, line))
            {
                if (LogLineInfo(line).hasInfo())
                    data.push_back(line);
                else
                    data.back().append("\n" + line);
            }
        }
        lInfo << "Vector size " << data.size();
        m_model.setLogData(std::move(data));
    }*/
}
