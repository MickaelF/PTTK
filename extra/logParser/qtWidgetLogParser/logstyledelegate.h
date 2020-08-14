#pragma once

#include <QPen>
#include <QStyledItemDelegate>

class LogStyleDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    LogStyleDelegate(int priorityCellWidth, int cellHeight, QObject *parent = nullptr);
    ~LogStyleDelegate() = default;
    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,
                     const QModelIndex &index) override;
    void setTextColumnWidth(int width);

signals:
    void resizeRow(int index, int height);

private:
    const int m_priorityLabelWidth, m_priorityCellHorizontalMargin, m_priorityLabelHeight;
    const int m_cellHeight;
    int m_textColumnWidth {0};
    QPen m_pen;
};