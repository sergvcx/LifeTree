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
    Task* parentTask;
    TaskData* taskData;
    QList<Task*> childTasks;
    QList<Task*> copyTasks;


    Task(){
        taskData=0;
        parentTask=0;
    }

    Task(QString Name, int Time=0, int Cost=0 ){
        taskData=new TaskData;
        taskData->cost=Cost;
        taskData->time=Time;
        taskData->name=Name;
        parentTask=0;
    }


    Task(QString Name, Task& parent, int Time=0, int Cost=0){
        taskData=new TaskData;
        taskData->cost=Cost;
        taskData->time=Time;
        taskData->name=Name;
        parentTask=&parent;
        parentTask->childTasks.append(this);
    }

    ~Task(){
        for(int i=0;i<copyTasks.count();i++){
            delete copyTasks.value(i);
        }
        delete taskData;
    }

    void appendChild(Task& childTask){
        if (childTask.parentTask==0){
            childTasks.append(&childTask);
            childTask.parentTask=this;
        }
        else{
            Task* copyTask= new Task;
            copyTask->taskData=childTask.taskData;
            childTasks.append(copyTask);
            copyTask->parentTask=this;
            copyTasks.append(copyTask);
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
                return QVariant(taskData->name);
            case 1:
                return QVariant(QString::number(taskData->time)+"("+QString::number(getTotalTime())+")");
            case 2:
                return QVariant(QString::number(taskData->cost)+"("+QString::number(getTotalCost())+")");
        }

        //return dataTask.value(column);
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
        if (taskData->visited)
            return;
        taskData->visited=true;
        for(int i=0; i<childTasks.count(); i++){
            childTasks.value(i)->getTotalTime_(totalTime);
        }
        totalTime+=taskData->time;
    }
    int getTotalCost()
    {
        int totalCost=0;
        resetVisits();
        getTotalCost_(totalCost);
        return totalCost;
    }

    void getTotalCost_(int& totalCost){
        if (taskData->visited)
            return;
        taskData->visited=true;
        for(int i=0; i<childTasks.count(); i++){
            childTasks.value(i)->getTotalCost_(totalCost);
        }
        totalCost+=taskData->cost;
    }

    void resetVisits(){
        for(int i=0; i<childTasks.count(); i++){
            childTasks.value(i)->resetVisits();
        }
        taskData->visited=false;
    }


};


#endif // TASK_H
