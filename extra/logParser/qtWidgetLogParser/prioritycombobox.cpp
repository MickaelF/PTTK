#include "prioritycombobox.h"

#include <QLineEdit>
#include <QStandardItemModel>
#include <QListView>

#include "log.h"

namespace
{
constexpr int nbPriorities {static_cast<int>(LogPriority::Execution)};
}

PriorityComboBox::PriorityComboBox(QWidget* parent) : QComboBox(parent)
{
    QStandardItemModel* model = new QStandardItemModel {nbPriorities, 1, this};
    for (int r = 0; r < nbPriorities; ++r)
    {
        QStandardItem* item = new QStandardItem(BasicLog::enumToStr(static_cast<LogPriority>(r)).data());
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData(Qt::Checked, Qt::CheckStateRole);
        model->setItem(r, 0, item);
    }
    setModel(model);

    QLineEdit* line = new QLineEdit(this);
    setLineEdit(line);
    line->setReadOnly(true);
    updateComboBoxText();
    QListView* view = static_cast<QListView*>(this->view());
    view->setSpacing(2);
    connect(this, QOverload<int>::of(&PriorityComboBox::highlighted), this,
            &PriorityComboBox::updateComboBoxText);
}

void PriorityComboBox::updateComboBoxText()
{
    auto line = lineEdit();
    int count = 0;
    QStandardItemModel* model = static_cast<QStandardItemModel*>(this->model());
    for (int i = 0; i < nbPriorities; ++i)
    {
        auto item = model->item(i);
        if (item->data(Qt::CheckStateRole) == Qt::Checked) count++;
    }
    line->setText(QString {"%1 %2 %3"}.arg(QString::number(count),
                                                 (count > 1) ? tr("priorities") : tr("priority"), tr("selected")));
}
