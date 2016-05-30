#ifndef TASK_H
#define TASK_H
#include <QString>
#include <QList>
#include <QVariant>

class Task;
class TaskData
{
public:
    TaskData(){
        time=0;
        cost=0;
        visited=false;
    }
    TaskData& operator= (TaskData& td){
        name=td.name;
        time=td.time;
        cost=td.cost;
        description=td.description;
        id=td.id;
        visited=td.visited;
        //listTask=td.listTask;
        return *this;
    }
    TaskData(QString Name, int Time=0, int Cost=0 )
    {
        name=Name;
        time=Time;
        cost=Cost;
        //id  =Id;
    }

    double time;
    int cost;
    bool visited;
    QString name;
    QString description;
    QString id;
    QList<Task*> listTask;
    int duplucates;
};

class Task {
public:
    int level;
    bool expanded;
    Task* parentTask;
    TaskData* pTaskData;
    QList<Task*> childTasks;

    Qt::CheckState      checkSt;
    Qt::CheckState      checkState() { return checkSt; }

    Task(){
        expanded=false;
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
        pTaskData->listTask.append(this);
    }

/*
    Task(QString Name, Task& parent, int Time=0, int Cost=0):Task(){
        pTaskData=new TaskData;
        pTaskData->cost=Cost;
        pTaskData->time=Time;
        pTaskData->name=Name;
        parentTask=&parent;
        parentTask->childTasks.append(this);
    }
*/
    ~Task(){
        while(!childTasks.isEmpty()){
            Task* childTask=childTasks.last();
            delete childTask;
            //childTasks.removeLast();
        }
        // удаляем всех владельцов этой TaskData и удаляем из списка детей их родителей
        while (pTaskData->listTask.count()){
            Task* ownerTask=pTaskData->listTask.last();
            if (ownerTask->parentTask){
                int idx=ownerTask->parentTask->childTasks.indexOf(ownerTask);
                Q_ASSERT(idx>=0);
                Q_ASSERT(ownerTask->childTasks.count()==0);
                ownerTask->parentTask->childTasks.removeAt(idx);
            }
            pTaskData->listTask.removeLast();

        }
        // удалем саму pDataData
        delete pTaskData;
    }




    TaskData* appendChildTask(TaskData& taskData){
        // добавляем задачу во все текущие клоны родителя
        TaskData* pCloneChildData=new TaskData;
        *pCloneChildData=taskData;
        Task* pCloneChildTask;
        foreach(Task* pCloneCurrentTask, this->pTaskData->listTask) {
            pCloneChildTask = new Task;
            pCloneChildTask->pTaskData=pCloneChildData;     // связываем с общей датой
            pCloneChildTask->parentTask=pCloneCurrentTask;
            pCloneChildTask->level=level+1;
            pCloneChildData->listTask.append(pCloneChildTask);
            pCloneCurrentTask->childTasks.append(pCloneChildTask);
        }
        return pCloneChildData;
    }

    TaskData* insertChildTask(TaskData& taskData, int before){
        // добавляем задачу во все текущие клоны родителя
        TaskData* pCloneChildData=new TaskData;
        *pCloneChildData=taskData;
        Task* pCloneChildTask;
        foreach(Task* pCloneCurrentTask, this->pTaskData->listTask) {
            pCloneChildTask = new Task;
            pCloneChildTask->pTaskData=pCloneChildData;     // связываем с общей датой
            pCloneChildTask->parentTask=pCloneCurrentTask;
            pCloneChildTask->level=level+1;
            pCloneChildData->listTask.append(pCloneChildTask);
            pCloneCurrentTask->childTasks.insert(before,pCloneChildTask);
        }
        return pCloneChildData;
    }

    /*
    TaskData* appendChildTask(TaskData* taskData){
        // добавляем задачу во все текущие клоны родителя
        TaskData* pCloneChildData=new TaskData;
        *pCloneChildData=taskData;
        Task* pCloneChildTask;
        foreach(Task* pCloneCurrentTask, this->pTaskData->listTask) {
            pCloneChildTask = new Task;
            pCloneChildTask->pTaskData=pCloneChildData;     // связываем с общей датой
            pCloneChildTask->parentTask=pCloneCurrentTask;
            pCloneChildTask->level=level+1;
            pCloneChildData->listTask.append(pCloneChildTask);
            pCloneCurrentTask->childTasks.append(pCloneChildTask);
        }
        return pCloneChildData;
    }*/
    bool containsChild(TaskData* pTaskData){
        foreach (Task* child, childTasks){
            if (child->pTaskData==pTaskData)
                return true;
        }
        return false;
    }

    void appendLinkedChildTask(TaskData* pLinkChildTaskData){
        // добавляем линк во все текущие клоны родителя
        Task* pCloneChildTask;
        foreach(Task* pCloneCurrentTask, this->pTaskData->listTask) {
            if (pCloneCurrentTask->containsChild(pLinkChildTaskData)) continue;
            pCloneChildTask = new Task;
            pCloneChildTask->pTaskData=pLinkChildTaskData;                 // связываем с общей датой
            pCloneChildTask->parentTask=pCloneCurrentTask;
            pCloneChildTask->level=level+1;
            //pCloneChildTask->linked=true;
            pLinkChildTaskData->listTask.append(pCloneChildTask);       // связываем с нов
            pCloneCurrentTask->childTasks.append(pCloneChildTask);

            Task* pLinkChildTask=pLinkChildTaskData->listTask.first();
            foreach(Task* pLinkChildChildTask, pLinkChildTask->childTasks){
                //if (pCloneChildTask==pLinkChildChildTask->parentTask) continue;
                pCloneChildTask->appendLinkedChildTask(pLinkChildChildTask->pTaskData);
            }
        }
    }
    // pCloneCurrentTask
    //    pCloneChildTask
    //    pCloneChildTask
    //    pCloneChildTask->pTaskData=pLinkChildTaskData
    //                                LinkChildTask->childTasks


    // pCloneCurrentTask
    //    pCloneChildTask
    //    pCloneChildTask
    //    pCloneChildTask->pTaskData=pLinkChildTaskData


/*
    int appendClone(Task& cloneTask){

    }
        // добавляем в свою задачу
            childTasks.append(childTask);
        }
        else{
            Task* linkedTask= new Task; // create copy
            linkedTask->pTaskData=childTask->pTaskData;
            linkedTask->linked=true;
            linkedTask->parentTask=this;
            linkedTask->pTaskData->listTask.append(this); // добавляем владельца
            childTasks.append(linkedTask);
            for(int i=0; i<childTask->childCount(); i++){
                linkedTask->appendChild(childTask->child(i));
            }

        }
    }
*/
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
        return 4;
    }

    QVariant data(int column)
    {

        switch (column){
            case 0:
                return QVariant(pTaskData->name);
            case 1:
                return QVariant(QString::number(pTaskData->time,'g',2)+"("+QString::number(getTotalTime())+")");
            case 2:
                return QVariant(QString::number(pTaskData->cost)+"("+QString::number(getTotalCost())+")");
            case 3:
                return QVariant(QString::number(pTaskData->listTask.count()));

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
                pTaskData->time=value.toDouble();
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

    double getTotalTime(){
        double totalTime=0;
        resetVisits();
        getTotalTime_(totalTime);
        return totalTime;
    }

    void getTotalTime_(double& totalTime){
        if (pTaskData->visited)
            return;
        pTaskData->visited=true;
        for(int i=0; i<childTasks.count(); i++){
            if (childTasks.value(i)->checkSt)
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
            if (childTasks.value(i)->checkSt)
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
