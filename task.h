#ifndef TASK_H
#define TASK_H
#include <QString>
#include <QList>
#include <QVariant>

class TaskData
{
public:
    TaskData(){
        time=0;
        cost=0;
        duplucates=1;
        visited=false;
    }
    TaskData& operator= (TaskData& td){
        time=td.time;
        cost=td.cost;
        duplucates=td.duplucates;
        visited=td.visited;
        return *this;
    }

    int time;
    int cost;
    bool visited;
    QString name;
    QString description;
    int duplucates;
};

class Task {
public:
    int level;
    bool linked;
    Task* parentTask;
    TaskData* pTaskData;
    QList<Task*> childTasks;

    Qt::CheckState      checkSt;
    Qt::CheckState      checkState() { return checkSt; }

    Task(){
        linked=false;
        level=0;
        checkSt=Qt::Checked;
        pTaskData=0;
        parentTask=0;
    }

    Task(QString Name, int Time=0, int Cost=0 ):Task(){
        pTaskData=new TaskData;
        pTaskData->cost=Cost;
        pTaskData->time=Time;
        pTaskData->name=Name;
        parentTask=0;
    }


    Task(QString Name, Task& parent, int Time=0, int Cost=0):Task(){
        pTaskData=new TaskData;
        pTaskData->cost=Cost;
        pTaskData->time=Time;
        pTaskData->name=Name;
        parentTask=&parent;
        parentTask->childTasks.append(this);
    }

    ~Task(){
        for(int i=0;i<childTasks.count();i++){
            if (childTasks.value(i)->linked==false)
                delete childTasks.value(i);
        }
        delete pTaskData;
    }

    void appendChild(Task* childTask){
        if (childTask->parentTask==0){
            childTasks.append(childTask);
            childTask->parentTask=this;
            childTask->level=level+1;
        }
        else{
            Task* linkedTask= new Task; // create copy
            linkedTask->pTaskData=childTask->pTaskData;
            linkedTask->linked=true;
            linkedTask->parentTask=this;
            childTasks.append(linkedTask);
            for(int i=0; i<childTask->childCount(); i++){
                linkedTask->appendChild(childTask->child(i));
            }

        }
    }

    Task* child(int row)
    {
        return childTasks.value(row);
    }

    int childCount() const
    {
        return childTasks.count();
    }
    // The row() function reports the item's location within its parent's list of items:
    int row()
    {
        if (parentTask)
            return parentTask->childTasks.indexOf(const_cast<Task*>(this));
        return 0;
    }

    int columnCount()
    {
        //return dataTask.count();
        return 3;
    }

    QVariant data(int column)
    {

        switch (column){
            case 0:
                return QVariant(pTaskData->name);
            case 1:
                return QVariant(QString::number(pTaskData->time)+"("+QString::number(getTotalTime())+")");
            case 2:
                return QVariant(QString::number(pTaskData->cost)+"("+QString::number(getTotalCost())+")");
        }

        //return dataTask.value(column);
    }
    void setData(int column, QVariant value)
    {

        switch (column){
            case 0:
                pTaskData->name=value.toString();
                break;
            case 1:
                pTaskData->time=value.toInt();
                break;
            case 2:
                pTaskData->cost=value.toInt();
                break;
        }

        //return dataTask.value(column);
    }
    void setCheckState(Qt::CheckState& st){
        checkSt=st;
    }

    Task* parent()
    {
        return parentTask;
    }

    int getTotalTime(){
        int totalTime=0;
        resetVisits();
        getTotalTime_(totalTime);
        return totalTime;
    }

    void getTotalTime_(int& totalTime){
        if (pTaskData->visited)
            return;
        pTaskData->visited=true;
        for(int i=0; i<childTasks.count(); i++){
            childTasks.value(i)->getTotalTime_(totalTime);
        }
        totalTime+=pTaskData->time;
    }
    int getTotalCost()
    {
        int totalCost=0;
        resetVisits();
        getTotalCost_(totalCost);
        return totalCost;
    }

    void getTotalCost_(int& totalCost){
        if (pTaskData->visited)
            return;
        pTaskData->visited=true;
        for(int i=0; i<childTasks.count(); i++){
            childTasks.value(i)->getTotalCost_(totalCost);
        }
        totalCost+=pTaskData->cost;
    }

    void resetVisits(){
        for(int i=0; i<childTasks.count(); i++){
            childTasks.value(i)->resetVisits();
        }
        pTaskData->visited=false;
    }


};


#endif // TASK_H
