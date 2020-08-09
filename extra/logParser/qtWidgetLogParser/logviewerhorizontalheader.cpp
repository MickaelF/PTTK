#include "logviewerhorizontalheader.h"

#include <QProxyStyle>
#include <QPainter>


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
}

LogViewerHorizontalHeader::LogViewerHorizontalHeader(QWidget* parent) : QHeaderView(Qt::Horizontal, parent)
{
	setStyleSheet("QHeaderView::section {border: none;} "
		"QHeaderView::up-arrow { image: url(:/resources/empty);}"
		"QHeaderView::down-arrow { image: url(:/resources/empty);}");
	setStyle(new HeaderStyle());
	setSectionsClickable(true);

	setMouseTracking(true);

	connect(this, &QHeaderView::sectionClicked, this,
		&LogViewerHorizontalHeader::onSectionClicked);
}

void LogViewerHorizontalHeader::mouseMoveEvent(QMouseEvent* event)
{
	setCursor(Qt::PointingHandCursor);
}

void LogViewerHorizontalHeader::onSectionClicked(int section)
{
	if (section == 3)
	{
		if (isSortIndicatorShown())
			setSortIndicatorShown(false);
		return;
	}

	m_currentlySortedSection = section;
	if (section != m_currentlySortedSection)
		m_currentSortOrder = Qt::DescendingOrder;
	else
		m_currentSortOrder =
		m_currentSortOrder == Qt::AscendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder;

	if (!isSortIndicatorShown())
		setSortIndicatorShown(true);
	setSortIndicator(m_currentlySortedSection, m_currentSortOrder);
	emit sortByColumn(m_currentlySortedSection, m_currentSortOrder);

}
