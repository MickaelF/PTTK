#pragma once

#include <QStyledItemDelegate>
#include <QLabel>

class LogStyleDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    LogStyleDelegate(int priorityCellWidth, int cellHeight, QObject* parent = nullptr);
    ~LogStyleDelegate() = default;
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

private: 
    const int m_priorityLabelWidth, m_priorityCellHorizontalMargin, m_priorityLabelHeight; 
    const int m_cellHeight, m_cellVerticalMargin;
};