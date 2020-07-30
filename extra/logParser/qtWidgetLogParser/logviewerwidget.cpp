#include "logviewerwidget.h"


LogViewerWidget::LogViewerWidget(QWidget* parent)
    : QTableView(parent)
{
    setModel(&m_model);
}

void LogViewerWidget::open(const QString& openPath) {
    
}
