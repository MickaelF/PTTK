#pragma once

#include <QStyledItemDelegate>
#include <QPen>

class LogStyleDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    LogStyleDelegate(int priorityCellWidth, int cellHeight, QObject* parent = nullptr);
    ~LogStyleDelegate() = default;
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    QSize sizeHint(const QStyleOptionViewItem &option,
                                        const QModelIndex &index) const override;
    void setTextColumnWidth(int width);

private: 
    const int m_priorityLabelWidth, m_priorityCellHorizontalMargin, m_priorityLabelHeight; 
    const int m_cellHeight;
    int m_textColumnWidth {0}; 
    QPen m_pen; 
};