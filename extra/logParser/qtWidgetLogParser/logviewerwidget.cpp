#include "logviewerwidget.h"

#include <QHeaderView>
#include <QPainter>
#include <QProxyStyle>
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
class HeaderStyle : public QProxyStyle
{
public:
    HeaderStyle() = default;
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter,
                     const QWidget* widget = nullptr) const override
    {
        if (element == CE_HeaderLabel)
        {
            auto ho = *qstyleoption_cast<const QStyleOptionHeader*>(option);
            QFont font;
            font.setBold(true);
            font.setCapitalization(QFont::Capitalization::AllUppercase);
            font.setPixelSize(12);

            QFontMetrics metrics(font);
            QRect oRect = ho.rect;
            int textWidth = metrics.width(ho.text);
            int textHeight = metrics.height();

            int leftMargin = ho.section != 3 ? (oRect.width() - textWidth) * 0.5 : 10;
            QRect oTextRect =
                QRect(oRect.left() + leftMargin, oRect.top() + (oRect.height() - textHeight) / 2,
                      textWidth * 1.2, textHeight);
            painter->setPen(QPen(QColor(32, 119, 188)));
            painter->setFont(font);
            painter->drawText(oTextRect, ho.text);
            QPixmap sortPixmap;
            switch (ho.sortIndicator)
            {
                case QStyleOptionHeader::SortUp:
                    sortPixmap = QIcon(":/resources/arrowUp").pixmap(10, 10);
                    break;
                case QStyleOptionHeader::SortDown:
                    sortPixmap = QIcon(":/resources/arrowDown").pixmap(10, 10);
                    break;
                default: return;
            }

            if (!sortPixmap.isNull())
            {
                QRect sortRect = QRect(oTextRect.left() + oTextRect.width(),
                                       oRect.top() + (oRect.height() - sortPixmap.height()) / 2,
                                       sortPixmap.width(), sortPixmap.height());
                painter->drawPixmap(sortRect, sortPixmap);
            }
        }
        else
            QProxyStyle::drawControl(element, option, painter, widget);
    }
};

constexpr int cPriorityColumnWidth {100};
constexpr int cRowHeight {60};
} // namespace

LogViewerWidget::LogViewerWidget(QWidget* parent)
    : QTableView(parent),
      m_sortFilter(new QSortFilterProxyModel(this))
{
    m_sortFilter->setDynamicSortFilter(false);
    m_sortFilter->setSourceModel(&m_model);
    setModel(m_sortFilter);

    auto header = horizontalHeader();
    header->setSectionResizeMode(0, QHeaderView::Fixed);
    header->setSectionResizeMode(1, QHeaderView::Fixed);
    header->setSectionResizeMode(2, QHeaderView::Interactive);
    header->setSectionResizeMode(3, QHeaderView::Stretch);
    setColumnWidth(0, cPriorityColumnWidth);
    setColumnWidth(1, 120);
    setColumnWidth(2, 100);
    setColumnWidth(3, 600);
    setShowGrid(false);
    header->setStyleSheet("QHeaderView::section {border: none;} "
                          "QHeaderView::up-arrow { image: url(:/resources/empty);}"
                          "QHeaderView::down-arrow { image: url(:/resources/empty);}");
    header->setStyle(new HeaderStyle());

    auto vHeader = verticalHeader();
    vHeader->setSectionResizeMode(QHeaderView::Fixed);
    vHeader->setDefaultSectionSize(cRowHeight);

    setItemDelegate(new LogStyleDelegate(cPriorityColumnWidth, cRowHeight, this));
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setFocusPolicy(Qt::NoFocus);
    setSelectionMode(QAbstractItemView::NoSelection);
    setFrameStyle(QFrame::NoFrame);

    connect(horizontalHeader(), &QHeaderView::sectionClicked, this,
            &LogViewerWidget::onSectionClicked);
}

void LogViewerWidget::open(const QString& openPath)
{
    m_parser.setInputPath(openPath.toStdString());
    launchParsing();
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
    m_model.setLogData(f.get());
}

void LogViewerWidget::onSectionClicked(int section)
{
    if (section == 3)
    {
        if (horizontalHeader()->isSortIndicatorShown())
            horizontalHeader()->setSortIndicatorShown(false);
        return;
    }

    m_currentlySortedSection = section;
    if (section != m_currentlySortedSection)
        m_currentSortOrder = Qt::DescendingOrder;
    else
        m_currentSortOrder =
            m_currentSortOrder == Qt::AscendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder;
    sortByColumn(m_currentlySortedSection, m_currentSortOrder);
    if (!horizontalHeader()->isSortIndicatorShown())
        horizontalHeader()->setSortIndicatorShown(true);
    horizontalHeader()->setSortIndicator(m_currentlySortedSection, m_currentSortOrder);
}