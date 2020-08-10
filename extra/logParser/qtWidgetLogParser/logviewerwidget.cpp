#include "logviewerwidget.h"

#include <QSortFilterProxyModel>

#include "log.h"
#include "logParser.h"
#include "loglineinfo.h"
#include "logstyledelegate.h"
#include "logviewerhorizontalheader.h"
#include "progressdialog.h"

namespace
{
constexpr int cPriorityColumnWidth {100};
constexpr int cRowHeight {60};
} // namespace

LogViewerWidget::LogViewerWidget(QWidget* parent)
    : QTableView(parent),
      m_sortFilter(new QSortFilterProxyModel(this))
{
    auto hHeader = new LogViewerHorizontalHeader();
    setHorizontalHeader(hHeader);
    m_sortFilter->setDynamicSortFilter(false);
    m_sortFilter->setSourceModel(&m_model);
    setModel(m_sortFilter);

    setColumnWidth(0, cPriorityColumnWidth);
    setColumnWidth(1, 120);
    setColumnWidth(2, 100);
    setColumnWidth(3, 600);
    setShowGrid(false);

    hHeader->setSectionResizeMode(0, QHeaderView::Fixed);
    hHeader->setSectionResizeMode(1, QHeaderView::Fixed);
    hHeader->setSectionResizeMode(2, QHeaderView::Interactive);
    hHeader->setSectionResizeMode(3, QHeaderView::Stretch);

    auto vHeader = verticalHeader();
    vHeader->setSectionResizeMode(QHeaderView::Fixed);
    vHeader->setDefaultSectionSize(cRowHeight);

    setItemDelegate(new LogStyleDelegate(cPriorityColumnWidth, cRowHeight, this));
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setFocusPolicy(Qt::NoFocus);
    setSelectionMode(QAbstractItemView::NoSelection);
    setFrameStyle(QFrame::NoFrame);

    connect(hHeader, &LogViewerHorizontalHeader::sortByColumn,
            [&](int section, Qt::SortOrder order) { sortByColumn(section, order); });
}

void LogViewerWidget::open(const QString& openPath)
{
    m_parser.setInputPath(openPath.toStdString());
    launchParsing();
}

void LogViewerWidget::launchParsing()
{
    ProgressDialog progress(tr("Parsing log files..."), 0, m_parser.numberOfLines(), m_parser);
    std::promise<std::vector<std::string>> p;
    auto f = p.get_future();
    std::thread thread = std::thread {&LogParser::execToVector, &m_parser, std::move(p)};
    progress.start();
    thread.join();
    m_model.setLogData(f.get());
    resizeRowsToContents();
}