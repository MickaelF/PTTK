#include "explorerpathwidget.h"

#include <QFileDialog>

ExplorerPathWidget::ExplorerPathWidget(QWidget* parent) : QLineEdit(parent)
{
    setMouseTracking(true);
}

void ExplorerPathWidget::mousePressEvent(QMouseEvent* event)
{
    switch (m_explorerType)
    {
        case ExplorerType::ExistingFile:
        {
            auto path = QFileDialog::getOpenFileName(this, m_caption, QString(), m_fileType);
            if (!path.isEmpty()) setText(path);
            break;
        }
        case ExplorerType::ExistingFolder:
        {
            auto path = QFileDialog::getExistingDirectory(this, m_caption);
            if (!path.isEmpty()) setText(path);
            break;
        }
    }
    clearFocus();
}

void ExplorerPathWidget::setFileType(const QString& type)
{
    if (m_fileType != type) m_fileType = type;
}
void ExplorerPathWidget::setExplorerType(ExplorerType type)
{
    if (m_explorerType != type) m_explorerType = type;
}

void ExplorerPathWidget::setCaption(const QString& caption)
{
    if (m_caption != caption) m_caption = caption;
}
