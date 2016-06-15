#include "mainwindow.h"
#include <QApplication>
#include <QTreeView>
#include <qtXml/QDomDocument>
#include <qtXml/QDomNode>
#include <QFile>
#include <QtDebug>
#include <QDir>
#include <QMap>
#include "taskmodel.h"
#include "mytreeview.h"
#include "qmenubar.h"

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
/*
int buildXML(QDomDocument& doc, QDomElement& parent, Task& task){

    QDomElement root = doc.createElement("task");

    for(int i=0; i<task->childCount();i++){
        QDomElement element = doc.createElement(pRootTask->"MyML");
    }
doc.appendChild(root);

QDomElement tag = doc.createElement("Greeting");
root.appendChild(tag);

QDomText t = doc.createTextNode("Hello World");
tag.appendChild(t);


}*/


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
int life2xml(Task* pRootTask,char* fileName){
    //QFile file(fileName);
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
}






int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    //MainWindow w;
    //a.resize(10,10);
/*
 *
 *
 *         QMenuBar mnuBar;
        QMenu*   pmnu   = new QMenu("&Menu");

        pmnu->addAction("&About Qt",
                        &app,
                        SLOT(aboutQt()),
                        Qt::CTRL + Qt::Key_Q
                       );

        pmnu->addSeparator();

        QAction* pCheckableAction = pmnu->addAction("&CheckableItem");
        pCheckableAction->setCheckable(true);
        pCheckableAction->setChecked(true);

        pmnu->addAction(QPixmap(img4_xpm), "&IconItem");

        QMenu* pmnuSubMenu = new QMenu("&SubMenu", pmnu);
        pmnu->addMenu(pmnuSubMenu);
        pmnuSubMenu->addAction("&Test");

        QAction* pDisabledAction = pmnu->addAction("&DisabledItem");
        pDisabledAction->setEnabled(false);

        pmnu->addSeparator();

        pmnu->addAction("&Exit", &app, SLOT(quit()));

        mnuBar.addMenu(pmnu);
        mnuBar.show();
*/
    Task  root("root");
    Task  life("life");
    root.appendChildTask(*(life.pTaskData));
    //life->parentTask=&root;

    QDomDocument domDoc;
    //QFile file("d:/life.xml");
    //qDebug() << QDir::currentPath() ;
    //QCoreApplication::applicationDirPath();
    //qDebug() << QDir::currentPath() ;
    //QFile file("../LifeTree/lifeout.xml");
    QFile file("../LifeTree/lifeout.xml");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        if (domDoc.setContent(&file)){
            QDomElement rootElement = domDoc.documentElement();
            QString name=rootElement.attribute("name","");
            int time=0; //docElement.attribute("time","").toInt();
            int cost=0; //docElement.attribute("cost","").toInt();
            qDebug() << rootElement.attribute("name","");


            transverseNode(rootElement,root.childTasks.first());
        }
        file.close();
    }







    /*
        Task sport("sport",life,10,0);
            Task swim("swim",sport,10,0);
            Task pres("pres",sport,10,0);
                Task GoldsGym("GoldsGym",swim,0,10);
                swim.appendChild(GoldsGym);
                pres.appendChild(GoldsGym);
        Task dancing("dancing",life);
            Task hustle("hustle",dancing,1,0);
            Task zouk("zouk",dancing,2,0);
                Task shoes("shoes",0,100);
                pres.appendChild(shoes);
                zouk.appendChild(shoes);
                hustle.appendChild(shoes);
        pres.appendChild(dancing);
    */

    TaskModel model(&root);
    TaskModel* pModel=&model;
    //Task* dance=root.child(0)->child(0);
    //Task* sport=root.child(0)->child(1);
    //Task* meet =root.child(0)->child(2);
    //dance->appendChild(sport);
    //meet->appendChild(dance);



    MyTreeView treeView;
    //treeView.setColumnWidth(0,500);
    treeView.setModel(&model);
    //QModelIndexList d=model.persistentIndexList();

    QObject::connect(&treeView, SIGNAL(clicked(const QModelIndex &)), &treeView, SLOT(onTreeClicked(const QModelIndex &)));
    QObject::connect(&treeView, SIGNAL(deleteKeyEvent(const QModelIndex &)), &model, SLOT(onDeleteKey(const QModelIndex &)));
    QObject::connect(&treeView, SIGNAL(insertKeyEvent(const QModelIndex &)), &model, SLOT(onInsertKey(const QModelIndex &)));
    QObject::connect(&treeView, SIGNAL(insertAltKeyEvent(const QModelIndex &)), &model, SLOT(onInsertAltKey(const QModelIndex &)));
    QObject::connect(&treeView, SIGNAL(copyKeyEvent(const QModelIndex &)), &model, SLOT(onCopyKey(const QModelIndex &)));
    QObject::connect(&treeView, SIGNAL(linkKeyEvent(const QModelIndex &)), &model, SLOT(onLinkKey(const QModelIndex &)));
    //QObject::connect(&treeView, SIGNAL(doubleClicked(const QModelIndex &)), &model, SLOT(onTreeDoubleClicked(const QModelIndex &)));
    QObject::connect(&treeView, SIGNAL(doubleClicked(const QModelIndex &)), &treeView, SLOT(onTreeDoubleClicked(const QModelIndex &)));
    QObject::connect(&treeView, SIGNAL(collapsed(const QModelIndex &)), &treeView, SLOT(onCollapsed(const QModelIndex &)));
    QObject::connect(&treeView, SIGNAL(expanded(const QModelIndex &)), &treeView, SLOT(onExpanded(const QModelIndex &)));



    treeView.show();

    app.exec();
    life2xml(root.child(0),"../LifeTree/lifeout.xml");
    return 0;
}

