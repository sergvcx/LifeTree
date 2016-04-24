#include "taskmodel.h"

TaskModel::TaskModel(Task *HeadTask, QObject *parent):QAbstractItemModel(parent)
{
    rootTask= HeadTask;
    //rootTask->appendChild(*HeadTask);
}

TaskModel::~TaskModel()
{

}

QVariant TaskModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()){
        return QVariant();
    }
    if (role != Qt::DisplayRole)
        return QVariant();

    Task *task = static_cast<Task*>(index.internalPointer());

    return task->data(index.column());

}
bool TaskModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

}

int TaskModel::rowCount(const QModelIndex &parent) const
{
    Task *parentTask;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentTask = rootTask;
    else
        parentTask = static_cast<Task*>(parent.internalPointer());

    return parentTask->childCount();
}

QModelIndex TaskModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    Task *parentTask;

    if (!parent.isValid())
        parentTask = rootTask;
    else
        parentTask = static_cast<Task*>(parent.internalPointer());

    Task *childTask = parentTask->child(row);
    if (childTask)
        return createIndex(row, column, childTask);
    else
        return QModelIndex();
}

QModelIndex TaskModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    Task *childTask = static_cast<Task*>(index.internalPointer());
    Task *parentTask = childTask->parent();

    if (parentTask == rootTask)
        return QModelIndex();

    return createIndex(parentTask->row(), 0, parentTask);
}

int TaskModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<Task*>(parent.internalPointer())->columnCount();
    else
        return rootTask->columnCount();
}

Qt::ItemFlags TaskModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return QAbstractItemModel::flags(index);
}

QVariant TaskModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole){
        switch(section){
            case 0:
                return QVariant("Task");
            case 1:
                return QVariant("Time");
            case 2:
                return QVariant("Cost");
        }

        //return rootTask->data(section);
    }
    return QVariant();
}

