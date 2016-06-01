#ifndef TASKMODEL_H
#define TASKMODEL_H

#include <QAbstractItemModel>
#include "task.h"
#include "QKeyEvent"

class TaskModel: public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit TaskModel(Task *RootTask, QObject *parent =0);
    QVariant data(const QModelIndex &index, int role) const;
    ~TaskModel();
    int rowCount(const QModelIndex &parent) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;


    Qt::ItemFlags flags(const QModelIndex &index) const ;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const ;
    QModelIndex parent(const QModelIndex &index) const ;
    int columnCount(const QModelIndex &parent = QModelIndex()) const ;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

public slots:
    void onTreeClicked(const QModelIndex &index);
    void onInsertKey(const QModelIndex &index);
    void onInsertAltKey(const QModelIndex &index);
    void onDeleteKey(const QModelIndex &index);
    void onCopyKey(const QModelIndex &index);
    void onLinkKey(const QModelIndex &index);

private:
    Task* rootTask;
    QModelIndex exchIndex;
};

#endif // TASKMODEL_H
