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

void myBeginResetModel(){
    beginResetModel();
}

void myEndResetModel(){
    endResetModel();
}


public slots:
    void onTreeClicked(const QModelIndex &index)
    {
        if (index.isValid()) {
            QString cellText = index.data().toString();
        }
    }
    void onInsertKey(const QModelIndex &index)
    {
        if (!index.isValid()) {
            return;
        }
        QModelIndex parentIndex=index.parent();
        Task *currentTask = static_cast<Task*>(index.internalPointer());
        Task *parentTask  = currentTask->parentTask; //static_cast<Task*>(parentIndex.internalPointer());
        int idx=parentTask->childTasks.indexOf(currentTask);
        Q_ASSERT(idx>=0);
        beginInsertRows(parentIndex, currentTask->row()+1, currentTask->row()+1);
        TaskData childData ("New",111,222);
        parentTask->insertChildTask(childData,index.row()+1);
        endInsertRows();
    }

    void onInsertAltKey(const QModelIndex &index)
    {
        if (!index.isValid()) {
            return;
        }
        QModelIndex parentIndex=index.parent();
        Task *currentTask = static_cast<Task*>(index.internalPointer());
        beginInsertRows(index, currentTask->columnCount(), currentTask->columnCount());
        TaskData childData ("SubNew",111,222);
        currentTask->appendChildTask(childData);
        endInsertRows();
    }

/*
    Task *parentTask = static_cast<Task*>(currentIndex.internalPointer());
    TaskData childData ("New",111,222);
    //rowsAboutToBeInserted(parentIndex,0,1);
    parentTask->appendChildTask(childData);
    setExpanded(currentIndex,false);
    setExpanded(currentIndex,true);
*/

    void onDeleteKey(const QModelIndex &index)
    {
        if (!index.isValid()) {
            return;
        }
        QModelIndex parentIndex=index.parent();
        Task *currentTask = static_cast<Task*>(index.internalPointer());
        Task *parentTask  = currentTask->parentTask; //static_cast<Task*>(parentIndex.internalPointer());
        int idx=parentTask->childTasks.indexOf(currentTask);
        Q_ASSERT(idx>=0);
        //beginResetModel();

        beginRemoveRows(parentIndex, currentTask->row(), currentTask->row());
        delete currentTask;
        endRemoveRows();
        //parentTask->childTasks.removeAt(idx);
        //pModel->resetModel();
        //endResetModel();
        //pModel->reset();

        //pModel->dataChanged(QModelIndex(),QModelIndex());



        //setExpanded(parentIndex,false);
        //setExpanded(parentIndex,true);
        //setCurrentIndex(parentIndex);
    }



private:
    Task* rootTask;
};

#endif // TASKMODEL_H
