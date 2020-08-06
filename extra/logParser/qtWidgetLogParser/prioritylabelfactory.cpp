#include "prioritylabelfactory.h"
#include <QPainter>
namespace
{
QColor priorityColor(const QString& priority)
{
    if (priority == "Info")
        return QColor {36, 191, 78};
    else if (priority == "Debug")
        return QColor {36, 46, 191};
    else if (priority == "Warning")
        return QColor {237, 213, 33};
    else if (priority == "Error")
        return QColor {237, 118, 33};
    else if (priority == "Fatal")
        return QColor {237, 33, 33};
    else if (priority == "Remember")
        return QColor {240, 29, 240};
    else if (priority == "Execution")
        return QColor {0, 0, 0};
}

constexpr char* Style {"color: #FFFFFF;"};
} // namespace

PriorityLabel* PriorityLabelFactory::makePriorityLabel(const QString& name, QWidget* parent)
{
    auto* label = new PriorityLabel(priorityColor(name), name, parent);
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    return label;
}

PriorityLabel::PriorityLabel(const QColor& backgroundColor, const QString& text, QWidget* parent)
    : QLabel(text, parent),
      m_backgroundColor(backgroundColor)
{
    QWidget::setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet(Style);
    setMouseTracking(true);
}

void PriorityLabel::paintEvent(QPaintEvent* event) 
{
    QPainter painter {this};
    painter.setBrush(QBrush(m_isSelected ? m_backgroundColor : m_unselectedColor));
    painter.drawRoundedRect(rect(), 25, 25);
    QLabel::paintEvent(event);
}

void PriorityLabel::mousePressEvent(QMouseEvent* event) 
{
    m_isSelected = !m_isSelected;
}
