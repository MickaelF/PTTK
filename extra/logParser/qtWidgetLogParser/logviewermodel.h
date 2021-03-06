#pragma once

#include <QAbstractTableModel>

class LogViewerModel: public QAbstractTableModel
{
    Q_OBJECT
public:
    enum class ColumnType
    {
        Priority,
        Date,
        File,
        Data
    };
    explicit LogViewerModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    Qt::ItemFlags flags(const QModelIndex& index) const override; 

    bool insertColumns(int column, int count, const QModelIndex& parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex& parent = QModelIndex()) override;
    bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void setLogData(std::vector<std::string> data);

    std::vector<std::string>& logDataRef() { return m_data; }

    time_t firstDate() const; 
    time_t lastDate() const; 
    QStringList fileNames() const; 

private:
    std::vector<std::string> m_data;
};
