#include "logstyledelegate.h"

#include <QPainter>

#include "prioritylabelfactory.h"

LogStyleDelegate::LogStyleDelegate(int priorityCellWidth, int cellHeight, QObject* parent)
    : QStyledItemDelegate(parent),
      m_priorityLabelWidth(priorityCellWidth * 0.80f),
      m_priorityCellHorizontalMargin(priorityCellWidth * 0.10f),
      m_priorityLabelHeight(cellHeight * 0.50f),
      m_cellHeight(cellHeight),
      m_cellVerticalMargin(cellHeight * 0.25f)
{
}

void LogStyleDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option,
                             const QModelIndex& index) const
{
    if (index.column() == 0)
    {
        auto label = PriorityLabelFactory::makePriorityLabel(index.data().toString());
        label->setFixedSize(m_priorityLabelWidth, m_priorityLabelHeight);
        auto pos = option.rect;
        painter->drawPixmap(pos.x() + m_priorityCellHorizontalMargin,
                            pos.y() + m_cellVerticalMargin, m_priorityLabelWidth,
                            m_priorityLabelHeight, label->grab());
        label->deleteLater();
    }
    else
        QStyledItemDelegate::paint(painter, option, index);
}
