#include "logviewerwidget.h"

#include <QHeaderView>
#include <QScrollBar>

#include "log.h"

LogViewerWidget::LogViewerWidget(QWidget* parent) : QTableView(parent)
{
    setModel(&m_model);
    auto header = horizontalHeader();
    header->setSectionResizeMode(0, QHeaderView::Fixed);
    header->setSectionResizeMode(1, QHeaderView::Fixed);
    header->setSectionResizeMode(2, QHeaderView::Fixed);
    header->setSectionResizeMode(3, QHeaderView::Interactive);
    header->setSectionResizeMode(4, QHeaderView::Stretch);
    setColumnWidth(0, 10);
    setColumnWidth(1, 70);
    setColumnWidth(2, 120);
    setColumnWidth(3, 100);
    setColumnWidth(4, 600);

    connect(verticalScrollBar(), &QScrollBar::sliderMoved, this, &LogViewerWidget::onSliderMoved);
}

void LogViewerWidget::open(const QString& openPath)
{
    std::vector<std::string> bla;
    for (int i = 0; i< 3000; i++)
    bla.push_back(
        "[2077-09-01 12:19:26][Warning-file1:62] - This is a log on one lineThis is a log on ");
    m_model.setLogData(bla);
}

void LogViewerWidget::onSliderMoved(int value)
{
    // Value is the row number displayed at the top of the screen.
    // TODO load next or previous files when comming to the top or the botttom.
}