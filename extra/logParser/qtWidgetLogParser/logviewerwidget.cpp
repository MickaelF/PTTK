#include "logviewerwidget.h"

#include <QApplication>
#include <QDateTime>
#include <QMouseEvent>
#include <QSortFilterProxyModel>

#include "log.h"
#include "logParser.h"
#include "logfilterproxymodel.h"
#include "loglineinfo.h"
#include "logviewerhorizontalheader.h"
#include "progressdialog.h"

namespace
{
constexpr int cPriorityColumnWidth {160};
constexpr int cRowHeight {60};
} // namespace

LogViewerWidget::LogViewerWidget(QWidget* parent)
    : QTableView(parent),
      m_sortFilter(new LogFilterProxyModel(this)),
      m_styleDelegate(cPriorityColumnWidth, cRowHeight)
{
    auto hHeader = new LogViewerHorizontalHeader();
    setHorizontalHeader(hHeader);
    m_sortFilter->setDynamicSortFilter(false);
    m_sortFilter->setSourceModel(&m_model);
    setModel(m_sortFilter);

    setColumnWidth(0, cPriorityColumnWidth);
    setColumnWidth(1, 120);
    setColumnWidth(2, 100);
    setShowGrid(false);

    hHeader->setSectionResizeMode(0, QHeaderView::Fixed);
    hHeader->setSectionResizeMode(1, QHeaderView::Fixed);
    hHeader->setSectionResizeMode(2, QHeaderView::Interactive);
    hHeader->setSectionResizeMode(3, QHeaderView::Stretch);

    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader()->setDefaultSectionSize(cRowHeight);

    setItemDelegate(&m_styleDelegate);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setFocusPolicy(Qt::NoFocus);
    setSelectionMode(QAbstractItemView::NoSelection);
    setFrameStyle(QFrame::NoFrame);
    m_styleDelegate.setTextColumnWidth(hHeader->sectionSize(3));
    setMouseTracking(true);

    connect(hHeader, &LogViewerHorizontalHeader::sortByColumn,
            [&](int section, Qt::SortOrder order) { sortByColumn(section, order); });
    connect(&m_styleDelegate, &LogStyleDelegate::resizeRow, this, &LogViewerWidget::onResizeRow);
}

void LogViewerWidget::open(const QString& openPath)
{
    m_parser.setInputPath(openPath.toStdString());
    launchParsing();
    m_sortFilter->resetParameters();
}

QDateTime LogViewerWidget::firstDate() const
{
    return QDateTime::fromSecsSinceEpoch(m_model.firstDate());
}

QDateTime LogViewerWidget::lastDate() const
{
    return QDateTime::fromSecsSinceEpoch(m_model.lastDate());
}

QStringList LogViewerWidget::fileNames() const
{
    return m_model.fileNames();
}

void LogViewerWidget::setFilterStartDate(const QDateTime& date) const
{
    m_sortFilter->setStartDate(date);
}

void LogViewerWidget::setFilterEndDate(const QDateTime& date) const
{
    m_sortFilter->setEndDate(date);
}

void LogViewerWidget::setFilteredPriorities(const QStringList& priorities) const
{
    m_sortFilter->setFilteredPriorities(priorities);
}

void LogViewerWidget::setFilteredFileNames(const QStringList& priorities) const
{
    m_sortFilter->setFilteredFileNames(priorities);
}

void LogViewerWidget::updateFilter() const
{
    m_sortFilter->invalidate();
}

void LogViewerWidget::resizeEvent(QResizeEvent* event)
{
    QTableView::resizeEvent(event);
    m_styleDelegate.setTextColumnWidth(horizontalHeader()->sectionSize(3));
}

void LogViewerWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (!rect().contains(event->pos()) && QApplication::overrideCursor())
        QApplication::restoreOverrideCursor();
    QTableView::mouseMoveEvent(event);
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
}

void LogViewerWidget::onResizeRow(int index, int height)
{
    setRowHeight(index, height);
}