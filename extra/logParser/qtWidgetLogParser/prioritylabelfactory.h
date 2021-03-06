#pragma once
#include <QLabel>

class PriorityLabel : public QLabel
{
    Q_OBJECT
public:
    PriorityLabel(const QColor& backgroundColor, const QString& text, QWidget* parent);
    bool isSelected() const { return m_isSelected; }
    void setSelected(bool state);
    QString priority() const { return text(); }

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override; 

private:
    const QColor m_backgroundColor;
    const QColor m_unselectedColor {100, 100, 100, 100};

    bool m_isSelected {true};
};

class PriorityLabelFactory
{
public:
    static PriorityLabel* makePriorityLabel(const QString& name, const QSize& size = QSize(80, 30), 
                                            QWidget* parent = nullptr);
};
