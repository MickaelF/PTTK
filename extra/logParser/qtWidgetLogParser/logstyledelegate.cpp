#include "logstyledelegate.h"

#include <QApplication>
#include <QMouseEvent>
#include <QPainter>
#include <QPushButton>

#include "log.h"
#include "prioritylabelfactory.h"
#include "style.h"

LogStyleDelegate::LogStyleDelegate(int priorityCellWidth, int cellHeight, QObject *parent)
    : QStyledItemDelegate(parent),
      m_priorityLabelWidth(priorityCellWidth * 0.60f),
      m_priorityCellHorizontalMargin(priorityCellWidth * 0.20f),
      m_priorityLabelHeight(cellHeight * 0.50f),
      m_cellHeight(cellHeight),
      m_pen(QBrush(QColor(AppStyle::softGrey)), 1)
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
        opt.rect.setWidth(opt.rect.width() - 60);
        auto text = index.data(Qt::DisplayRole).toString();
        auto textHeight =
            option.fontMetrics.boundingRect(option.rect, Qt::TextWordWrap, text).height();
        if (textHeight >= m_cellHeight)
        {
            QRect rect {opt.rect.right() + 10, opt.rect.top() + static_cast<int>((opt.rect.height() - 30)* 0.5f), 30, 30};
            QString text {textHeight < option.rect.height() ? "-" : "+"};
            painter->setBrush(QBrush(QColor(AppStyle::softGrey)));
            painter->drawEllipse(rect);
            painter->setPen(QColor(AppStyle::blue));
            QFont font; 
            font.setPixelSize(20);
            font.setBold(true);
            auto oldFont {painter->font()};
            painter->setFont(font);
            painter->drawText(rect, Qt::AlignCenter, text);
            painter->setFont(oldFont);
        }
        QStyledItemDelegate::paint(painter, opt, index);
    }
    else
        QStyledItemDelegate::paint(painter, option, index);
}

bool LogStyleDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                                   const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (index.column() != 3)
        QApplication::restoreOverrideCursor();
    else
    {
        auto text = index.data(Qt::DisplayRole).toString();
        auto textHeight =
            option.fontMetrics.boundingRect(option.rect, Qt::TextWordWrap, text).height();
        if (event->type() == QEvent::MouseMove)
        {
            if ((textHeight > option.rect.height() || option.rect.height() > m_cellHeight) && !QApplication::overrideCursor())
                QApplication::setOverrideCursor(QCursor(Qt::PointingHandCursor));
            else if (textHeight <= option.rect.height() && option.rect.height() == m_cellHeight && QApplication::overrideCursor())
                QApplication::restoreOverrideCursor();
        }
        else if (event->type() == QEvent::MouseButtonRelease)
        {
            if (textHeight > option.rect.height())
                emit resizeRow(index.row(), textHeight + 30);
            else
                emit resizeRow(index.row(), m_cellHeight);
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

void LogStyleDelegate::setTextColumnWidth(int width)
{
    if (m_textColumnWidth != width) m_textColumnWidth = width;
}
