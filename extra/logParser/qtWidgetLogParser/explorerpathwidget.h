#pragma once
#include <QLineEdit>

class ExplorerPathWidget : public QLineEdit
{
    Q_OBJECT
public: 
    enum class ExplorerType
    {
        ExistingFile, 
        ExistingFolder
    };
    explicit ExplorerPathWidget(QWidget* parent = nullptr); 
    void setFileType(const QString& type);
    void setExplorerType(ExplorerType type);
    void setCaption(const QString& caption); 

protected:
    void mousePressEvent(QMouseEvent* event) override; 

private:
    ExplorerType m_explorerType {ExplorerType::ExistingFolder};
    QString m_fileType;
    QString m_caption; 
};