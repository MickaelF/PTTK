#include "logstyledelegate.h"

#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>

#include "log.h"
#include "prioritylabelfactory.h"

LogStyleDelegate::LogStyleDelegate(int priorityCellWidth, int cellHeight, QObject *parent)
    : QStyledItemDelegate(parent),
      m_priorityLabelWidth(priorityCellWidth * 0.80f),
      m_priorityCellHorizontalMargin(priorityCellWidth * 0.10f),
      m_priorityLabelHeight(cellHeight * 0.50f),
      m_cellHeight(cellHeight),
      m_pen(QBrush(QColor("#E0E0E0")), 1)
{
}

void LogStyleDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{
    const QPen &oldPen = painter->pen();
    painter->setPen(m_pen);
    QPoint p1 = QPoint(option.rect.bottomLeft().x() - 1, option.rect.bottomLeft().y());
    QPoint p2 = QPoint(option.rect.bottomRight().x() + 1, option.rect.bottomRight().y());
    painter->drawLine(p1, p2);
    painter->setPen(oldPen);

    // TODO Remove use of magic number, replace them by their meaning
    if (index.column() == 0)
    {
        auto label = PriorityLabelFactory::makePriorityLabel(
            index.data().toString(), QSize(m_priorityLabelWidth, m_priorityLabelHeight));
        painter->drawPixmap(option.rect.x() + m_priorityCellHorizontalMargin,
                            option.rect.y() + (option.rect.height() - m_priorityLabelHeight) * 0.5f,
                            m_priorityLabelWidth, m_priorityLabelHeight, label->grab());
        label->deleteLater();
    }
    else if (index.column() == 1)
    {
        auto label = new QLabel(index.data().toString());
        label->setAttribute(Qt::WA_TranslucentBackground);
        label->setGeometry(option.rect);
        label->setAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        painter->drawPixmap(option.rect.x(), option.rect.y(), label->grab());
        label->deleteLater();
    }
    else if (index.column() == 3)
    {
        QStyleOptionViewItem opt {option};
        opt.rect.setLeft(opt.rect.left() + 10);
        opt.rect.setWidth(opt.rect.width() - 40);
        auto text = index.data(Qt::DisplayRole).toString();
        auto textHeight =
            option.fontMetrics.boundingRect(option.rect, Qt::TextWordWrap, text).height();
        if (textHeight >= m_cellHeight)
        {
            QPushButton button;
            button.setFixedSize(20, 20);
            if (textHeight > option.rect.height())
                button.setText("+");
            else 
                button.setText("-");
            painter->drawPixmap(opt.rect.right() + 5, opt.rect.top(), button.grab());
        }
        QStyledItemDelegate::paint(painter, opt, index);
    }
    else
        QStyledItemDelegate::paint(painter, option, index);
}

bool LogStyleDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                   const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (index.column() != 3 || event->type() != QEvent::MouseButtonRelease)
        return QStyledItemDelegate::editorEvent(event, model, option, index);

    auto text = index.data(Qt::DisplayRole).toString();
    auto textHeight = option.fontMetrics.boundingRect(option.rect, Qt::TextWordWrap, text).height();
    if (textHeight > option.rect.height())
        emit resizeRow(index.row(), textHeight + 30);
    else
        emit resizeRow(index.row(), m_cellHeight);
    return true;
}

void LogStyleDelegate::setTextColumnWidth(int width)
{
    if (m_textColumnWidth != width) m_textColumnWidth = width;
}
