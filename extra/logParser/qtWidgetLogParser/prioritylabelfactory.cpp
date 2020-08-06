#include "prioritylabelfactory.h"

#include "logpriority.h"
#include <QPainter>
namespace
{
QColor priorityColor(const QString& priority)
{
    using namespace LogPriority;
    if (priority == QString(enumToStr(Priorities::Info).data()))
        return QColor {36, 191, 78};
    else if (priority == QString(enumToStr(Priorities::Debug).data()))
        return QColor {36, 46, 191};
    else if (priority == QString(enumToStr(Priorities::Warning).data()))
        return QColor {237, 213, 33};
    else if (priority == QString(enumToStr(Priorities::Error).data()))
        return QColor {237, 118, 33};
    else if (priority == QString(enumToStr(Priorities::Fatal).data()))
        return QColor {237, 33, 33};
    else if (priority == QString(enumToStr(Priorities::Remember).data()))
        return QColor {240, 29, 240};
    else if (priority == QString(enumToStr(Priorities::Execution).data()))
        return QColor {0, 0, 0};
}

constexpr char* Style {"color: #FFFFFF;"};
} // namespace

PriorityLabel* PriorityLabelFactory::makePriorityLabel(const QString& name, const QSize& size,
                                                       QWidget* parent)
{
    auto* label = new PriorityLabel(priorityColor(name), name, parent);
    label->setFixedSize(size);
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
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QBrush(m_isSelected ? m_backgroundColor : m_unselectedColor));
    painter.drawRoundedRect(rect(), 25, 25);
    QLabel::paintEvent(event);
}

void PriorityLabel::mousePressEvent(QMouseEvent* event)
{
    m_isSelected = !m_isSelected;
    repaint();
}
