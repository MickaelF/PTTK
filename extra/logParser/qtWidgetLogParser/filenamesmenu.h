#pragma once

#include <QMenu>
#include <QHBoxLayout>
#include <QPushButton>
#include <QWidgetAction>

class FileNamesActionWidget : public QWidgetAction
{
    Q_OBJECT
public:
    explicit FileNamesActionWidget(QObject* parent = nullptr) : QWidgetAction(parent) {}

protected:
    QWidget* createWidget(QWidget* parent) override
    {
        QWidget* widget = new QWidget(parent);
        QHBoxLayout* layout = new QHBoxLayout(widget);
        auto everything {new QPushButton(tr("Everything"), widget)};
        auto nothing {new QPushButton(tr("Nothing"), widget)};

        connect(everything, &QPushButton::pressed, this, &FileNamesActionWidget::everythingPressed);
        connect(nothing, &QPushButton::pressed, this, &FileNamesActionWidget::nothingPressed);

        layout->addWidget(everything);
        layout->addWidget(nothing);

        return widget;
    }

signals:
    void everythingPressed();
    void nothingPressed();
};

class FileNamesMenu : public QMenu
{
    Q_OBJECT
public:
    explicit FileNamesMenu(QWidget* parent = nullptr);

    void setFileNames(QStringList filenames);
    QStringList uncheckedFileNames() const; 
    void checkEverything() const;

protected:
    void mouseReleaseEvent(QMouseEvent* e) override;
    void changeEvent(QEvent* event) override; 

private slots:
    void onEverythingPressed() const;
    void onNothingPressed() const;

private:
    void constructWidget();
    QStringList m_fileNames; 

};