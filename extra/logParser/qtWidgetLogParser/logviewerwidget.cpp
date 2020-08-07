#include "logviewerwidget.h"

#include <QHeaderView>
#include <QScrollBar>

#include "log.h"
#include "logstyledelegate.h"

namespace
{
constexpr int cPriorityColumnWidth {100};
constexpr int cRowHeight {60};
} // namespace

LogViewerWidget::LogViewerWidget(QWidget* parent) : QTableView(parent)
{
    setModel(&m_model);
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

    setShowGrid(false);
    setFrameStyle(QFrame::NoFrame);
    connect(verticalScrollBar(), &QScrollBar::sliderMoved, this, &LogViewerWidget::onSliderMoved);
}

void LogViewerWidget::open(const QString& openPath)
{
    std::vector<std::string> bla;
    bla.push_back(
        "[2077-09-01 12:19:26][Warning-file1:62] - This is a log on one lineThis is a log on ");
    bla.push_back(
        "[2077-09-01 12:19:26][Error-file1:62] - This is a log on one lineThis is a log on ");
    bla.push_back(
        "[2077-09-01 12:19:26][Info-file1:62] - This is a log on one lineThis is a log on ");
    bla.push_back(
        "[2077-09-01 12:19:26][Execution-file1:62] - This is a log on one lineThis is a log on ");
    bla.push_back(
        "[2077-09-01 12:19:26][Remember-file1:62] - This is a log on one lineThis is a log on ");
    bla.push_back(
        "[2077-09-01 12:19:26][Debug-file1:62] - This is a log on one lineThis is a log on ");
    bla.push_back(
        "[2077-09-01 12:19:26][Fatal-file1:62] - This is a log on one lineThis is a log on ");
    m_model.setLogData(bla);
}

void LogViewerWidget::onSliderMoved(int value)
{
    // Value is the row number displayed at the top of the screen.
    // TODO load next or previous files when comming to the top or the botttom.
}