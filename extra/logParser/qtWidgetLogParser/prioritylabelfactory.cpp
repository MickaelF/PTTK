#include "prioritylabelfactory.h"

#include <pttk/logpriority.h>

#include <QPainter>
namespace
{
QColor priorityColor(const QString& priority)
{
    using namespace LogPriority;
    if (priority == QString(enumToStr(Priorities::Info).data()))
        return QColor {131, 204, 237};
    else if (priority == QString(enumToStr(Priorities::Debug).data()))
        return QColor {78, 72, 115};
    else if (priority == QString(enumToStr(Priorities::Warning).data()))
        return QColor {242, 161, 167};
    else if (priority == QString(enumToStr(Priorities::Error).data()))
        return QColor {242, 129, 87};
    else if (priority == QString(enumToStr(Priorities::Fatal).data()))
        return QColor {222, 71, 71};
    else if (priority == QString(enumToStr(Priorities::Remember).data()))
        return QColor {155, 143, 235};
    else if (priority == QString(enumToStr(Priorities::Execution).data()))
        return QColor {156, 39, 84};
    return QColor();
}

} // namespace

PriorityLabel* PriorityLabelFactory::makePriorityLabel(const QString& name,
                                                       const QSize& size,
                                                       QWidget* parent)
{
    auto* label = new PriorityLabel(priorityColor(name), name, parent);
    label->setFixedSize(size);
    label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    return label;
}

PriorityLabel::PriorityLabel(const QColor& backgroundColor, const QString& text,
                             QWidget* parent)
    : QLabel(text, parent),
      m_backgroundColor(backgroundColor)
{
    QWidget::setAttribute(Qt::WA_TranslucentBackground);
    auto font = this->font();
    font.setBold(true);
    setFont(font);
    auto palette = this->palette();
    palette.setColor(QPalette::ColorRole::WindowText, QColor(255, 255, 255));
    palette.setColor(QPalette::ColorRole::Window, QColor(0, 0, 0, 0));
    setPalette(palette);
    setMouseTracking(true);
}

void PriorityLabel::setSelected(bool state)
{
    if (m_isSelected != state) m_isSelected = state;
}

void PriorityLabel::paintEvent(QPaintEvent* event)
{
    QPainter painter {this};
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(
        QBrush(m_isSelected ? m_backgroundColor : m_unselectedColor));
    painter.drawRoundedRect(rect(), 20, 20);
    QLabel::paintEvent(event);
}

void PriorityLabel::mousePressEvent(QMouseEvent* event)
{
    m_isSelected = !m_isSelected;
    repaint();
}

void PriorityLabel::mouseMoveEvent(QMouseEvent* event)
{
    this->setCursor(Qt::PointingHandCursor);
}
