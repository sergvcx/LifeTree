#include "taskmodel.h"

#include <qtXml/QDomDocument>
#include <qtXml/QDomNode>
#include <QFile>
#include <QtDebug>

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
    if (role != Qt::DisplayRole && role !=  Qt::CheckStateRole )
        return QVariant();

    if (index.column() > 0 && role ==  Qt::CheckStateRole  )
        return QVariant();

    Task *task = static_cast<Task*>(index.internalPointer());
    if (role == Qt::CheckStateRole ){
         return task->checkState();
    }
    return task->data(index.column());


}
bool TaskModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    //emit layoutAboutToBeChanged();
    if (role==Qt::EditRole) {
        Task *task = static_cast<Task*>(index.internalPointer());
        task->setData(index.column(),value);
        emit dataChanged(QModelIndex(),QModelIndex());
    }
    if (role==Qt::CheckStateRole) {
        Task *task = static_cast<Task*>(index.internalPointer());
        Qt::CheckState checkState = static_cast<Qt::CheckState>(value.toInt());
        task->setCheckState(checkState);
        emit dataChanged(QModelIndex(),QModelIndex());
        return true;
    }

    return false;
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
    //if (!index.isValid())
    //   return 0;

    if (!index.isValid()) return Qt::ItemIsEnabled;

      Qt::ItemFlags flags;
      if (index.column() == 0) flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable | Qt::ItemIsEditable;
      if (index.column() == 1) flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
      if (index.column() == 2) flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
      return flags;

    //return QAbstractItemModel::flags(index);
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
            case 3:
                return QVariant("Duplicates");
        }

        //return rootTask->data(section);
    }
    return QVariant();
}

void TaskModel::onTreeClicked(const QModelIndex &index)
{
    if (index.isValid()) {
        QString cellText = index.data().toString();
    }
}
void TaskModel::onInsertKey(const QModelIndex &index)
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
    TaskData childData ("New",0,0);
    parentTask->insertChildTask(childData,index.row()+1);
    endInsertRows();
}

void TaskModel::onInsertAltKey(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    QModelIndex parentIndex=index.parent();
    Task *currentTask = static_cast<Task*>(index.internalPointer());
    beginInsertRows(index, currentTask->columnCount(), currentTask->columnCount());
    TaskData childData ("SubNew",0,0);
    currentTask->appendChildTask(childData);
    endInsertRows();
}



void TaskModel::onDeleteKey(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    QModelIndex parentIndex=index.parent();
    Task *currentTask = static_cast<Task*>(index.internalPointer());
    Task *parentTask  = currentTask->parentTask; //static_cast<Task*>(parentIndex.internalPointer());
    int idx=parentTask->childTasks.indexOf(currentTask);
    Q_ASSERT(idx>=0);

    beginRemoveRows(parentIndex, currentTask->row(), currentTask->row());
    delete currentTask;
    endRemoveRows();
}

void TaskModel::onCtrlDeleteKey(const QModelIndex &index)
{
    if (!index.isValid()) {
        return;
    }
    QModelIndex parentIndex=index.parent();
    Task *currentTask = static_cast<Task*>(index.internalPointer());
    Task *parentTask  = currentTask->parentTask; //static_cast<Task*>(parentIndex.internalPointer());
    int idx=parentTask->childTasks.indexOf(currentTask);
    Q_ASSERT(idx>=0);

    beginRemoveRows(parentIndex, currentTask->row(), currentTask->row());

    if (currentTask->pTaskData->listTask.count()==1)
        delete currentTask;
    else{
        parentTask->childTasks.removeAt(idx); // удаляем из родителся
        idx=currentTask->pTaskData->listTask.indexOf(currentTask);
        currentTask->pTaskData->listTask.removeAt(idx); // удаляем из спсика владельцев taskData
        currentTask->pTaskData=0;
        currentTask->childTasks.clear();
        delete currentTask;
    }

    endRemoveRows();
}

void TaskModel::onCopyKey(const QModelIndex &index){
    if (!index.isValid()) {
        return;
    }
    exchIndex=index;
}

void TaskModel::onLinkKey(const QModelIndex &index){
    if (!index.isValid()) {
        return;
    }
    QModelIndex parentIndex=index.parent();
    Task *currentTask = static_cast<Task*>(index.internalPointer());
    Task *parentTask  = currentTask->parentTask; //static_cast<Task*>(parentIndex.internalPointer());
    Task *originTask  = static_cast<Task*>(exchIndex.internalPointer());

    //int idx=parentTask->childTasks.indexOf(currentTask);
    //Q_ASSERT(idx>=0);
    beginInsertRows(parentIndex, 0, parentTask->columnCount()+1);
    parentTask->appendLinkedChildTask(originTask->pTaskData);
    //parentTask->insertChildTask(*(exchTask->pTaskData),index.row()+1);
    endInsertRows();

}


int appendTaskNode(QDomDocument& doc,  QDomElement& parentElement, Task* pTask){
    QDomElement childElement;

    if (pTask->pTaskData->listTask.first()!=pTask){
        childElement = doc.createElement("link");

        QDomAttr attrID = doc.createAttribute("with");
        attrID.setValue(pTask->pTaskData->id);
        childElement.setAttributeNode(attrID);

        QDomAttr attrEnabled = doc.createAttribute("enabled");
        if (pTask->checkSt==Qt::Checked)
            attrEnabled.setValue("1");
        else
            attrEnabled.setValue("0");
        childElement.setAttributeNode(attrEnabled);

        parentElement.appendChild(childElement);

        //for(int i=0; i<pTask->childCount();i++){
        //    Task* pChildTask=pTask->child(i);
        //    if (!pChildTask->linked)
        //        appendTaskNode(doc,childElement,pChildTask);
        //}
    }
    else {
        childElement = doc.createElement("task");

        QDomAttr attrName = doc.createAttribute("name");
        attrName.setValue(pTask->pTaskData->name);
        childElement.setAttributeNode(attrName);

        QDomAttr attrTime = doc.createAttribute("time");
        attrTime.setValue(QString::number(pTask->pTaskData->time,'g',2));
        childElement.setAttributeNode(attrTime);

        QDomAttr attrCost = doc.createAttribute("cost");
        attrCost.setValue(QString::number(pTask->pTaskData->cost));
        childElement.setAttributeNode(attrCost);

        QDomAttr attrID = doc.createAttribute("id");
        attrID.setValue(pTask->pTaskData->id);
        childElement.setAttributeNode(attrID);

        QDomAttr attrEnabled = doc.createAttribute("enabled");
        if (pTask->checkSt==Qt::Checked)
            attrEnabled.setValue("1");
        else
            attrEnabled.setValue("0");
        childElement.setAttributeNode(attrEnabled);

        parentElement.appendChild(childElement);

        for(int i=0; i<pTask->childCount();i++){
            Task* pChildTask=pTask->child(i);
            appendTaskNode(doc,childElement,pChildTask);
        }
    }

}



QMap<QString,TaskData* > mapTaskData;
// чтени XML - рекурсивный разбор XML узла
void transverseNode(const QDomNode& Node, Task* parent){
    QDomNode domNode= Node.firstChild();
    while(!domNode.isNull()){
        Task* pChildTask=0;
        if (domNode.isElement()){
            QDomElement domElement=domNode.toElement();
            if (!domElement.isNull()){
                if (domElement.tagName()=="task") {
                    QString name=domElement.attribute("name","");
                    double time=0;
                    int cost=0;
                    bool enabled=false;
                    QString id="0";
                    if (domElement.hasAttribute("time")){
                        time=domElement.attribute("time","").toDouble();
                    }
                    if (domElement.hasAttribute("id")){
                        id=domElement.attribute("id","");
                    }
                    if (domElement.hasAttribute("cost")){
                        cost=domElement.attribute("cost","").toInt();
                    }
                    if (domElement.hasAttribute("enabled")){
                        enabled=domElement.attribute("enabled","")=="1";
                    }
                    while (mapTaskData.contains(id)){
                        bool ok;
                        int i=id.toInt(&ok);
                        if (ok){
                            i++;
                            id=QString::number(i);
                        }
                        else {
                            id="0";
                        }
                    }


                    qDebug() << domElement.attribute("name","");    // открывающийся тэг
                    //pChildTask=new Task(name,parent,time,cost);
                    TaskData childData;
                    childData.name=name;
                    childData.time=time;
                    childData.cost=cost;
                    childData.id  =id;
                    childData.enabled=enabled;
                    TaskData* pChildTaskData=parent->appendChildTask(childData);

                    pChildTask = pChildTaskData->listTask.last();
                    mapTaskData[id]=pChildTaskData;
                }
                else {
                    qDebug() << domElement.tagName() << "\tText" << domElement.text(); // закрывающийся тэг
                }
                if (domElement.tagName()=="link") {
                    QString with =domElement.attribute("with","");
                    //mapTaskData.contains(id)){
                    TaskData* pChildTaskData= mapTaskData[with];
                    parent->appendLinkedChildTask(pChildTaskData);


                    pChildTask = pChildTaskData->listTask.last();
                    if (domElement.hasAttribute("enabled")){
                        if (domElement.attribute("enabled","")=="1")
                            pChildTask->checkSt=Qt::Checked;
                        else
                            pChildTask->checkSt=Qt::Unchecked;
                    }

                    qDebug() << domElement.attribute("with","");

                }
                else {
                    qDebug() << domElement.tagName() << "\tText" << domElement.text();
                }
            }
        }
        //pChildTask=0;
        //parent->childTasks.last());
        transverseNode(domNode, pChildTask);
        domNode= domNode.nextSibling();
    }
}


int TaskModel::saveXML(){
    //QFile file(fileName);
    Task* pRootTask=rootTask->child(0);
    QFile file("../LifeTree/lifeout.xml");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) return -1;

    QDomDocument doc("MyLife");
    QDomElement root  = doc.createElement("task");
    QDomAttr attrName = doc.createAttribute("name");
    attrName.setValue(pRootTask->pTaskData->name);
    root.setAttributeNode(attrName);
    doc.appendChild(root);

    for(int i=0; i<pRootTask->childCount();i++){
        Task* pChildTask=pRootTask->child(i);
        appendTaskNode(doc,root,pChildTask);
    }

    QString xml = doc.toString();
    QTextStream out(&file);
    out.setCodec("UTF-8");
    out <<  xml;
    file.close();
    return 0;
}

int TaskModel::loadXML(){
    QDomDocument domDoc;
    QFile file("../LifeTree/lifeout.xml");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        if (domDoc.setContent(&file)){
            QDomElement rootElement = domDoc.documentElement();
            QString name=rootElement.attribute("name","");
            int time=0; //docElement.attribute("time","").toInt();
            int cost=0; //docElement.attribute("cost","").toInt();
            qDebug() << rootElement.attribute("name","");


            //transverseNode(rootElement,root.childTasks.first());
            transverseNode(rootElement,rootTask->childTasks.first());
        }
        file.close();
    }
}

