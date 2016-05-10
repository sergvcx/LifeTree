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

QMap<QString,Task* > mapTask;
void transverseNode(const QDomNode& Node, Task& parent){
    QDomNode domNode= Node.firstChild();
    while(!domNode.isNull()){
        Task* pChildTask;
        if (domNode.isElement()){
            QDomElement domElement=domNode.toElement();
            if (!domElement.isNull()){
                if (domElement.tagName()=="task") {
                    QString name=domElement.attribute("name","");
                    int time=0;
                    int cost=0;
                    QString id="id-000";
                    if (domElement.hasAttribute("time")){
                        time=domElement.attribute("time","").toInt();
                    }
                    if (domElement.hasAttribute("id")){
                        id=domElement.attribute("id","");
                    }
                    if (domElement.hasAttribute("cost")){
                        cost=domElement.attribute("cost","").toInt();
                    }
                    while (mapTask.contains(id)){
                        id=id+"0";
                    }


                    qDebug() << domElement.attribute("name","");
                    pChildTask=new Task(name,parent,time,cost);
                    pChildTask->pTaskData->id=id;

                    mapTask[id]=pChildTask;
                }
                else {
                    qDebug() << domElement.tagName() << "\tText" << domElement.text();
                }
            }
        }
        transverseNode(domNode, *pChildTask);
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
    QDomElement childElement = doc.createElement("task");

    QDomAttr attrName = doc.createAttribute("name");
    attrName.setValue(pTask->pTaskData->name);
    childElement.setAttributeNode(attrName);

    QDomAttr attrTime = doc.createAttribute("time");
    attrTime.setValue(QString::number(pTask->pTaskData->time));
    childElement.setAttributeNode(attrTime);

    QDomAttr attrCost = doc.createAttribute("cost");
    attrCost.setValue(QString::number(pTask->pTaskData->cost));
    childElement.setAttributeNode(attrCost);

    QDomAttr attrID = doc.createAttribute("id");
    attrID.setValue(pTask->pTaskData->id);
    childElement.setAttributeNode(attrID);

    //QDomAttr attrParentID = doc.createAttribute("parent");
    //attrParentID.setValue(pTask->parentTask->pTaskData->id);
    //childElement.setAttributeNode(attrParentID);

    parentElement.appendChild(childElement);

    for(int i=0; i<pTask->childCount();i++){
        Task* pChildTask=pTask->child(i);
        appendTaskNode(doc,childElement,pChildTask);
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
    out <<  xml;
    file.close();
}





int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    //a.resize(10,10);

    Task root("root");
    //Task life("life",root);

    QDomDocument domDoc;
    //QFile file("d:/life.xml");
    //qDebug() << QDir::currentPath() ;
    //QCoreApplication::applicationDirPath();
    //qDebug() << QDir::currentPath() ;
    QFile file("../LifeTree/life.xml");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        if (domDoc.setContent(&file)){
            QDomElement docElement = domDoc.documentElement();
            QString name=docElement.attribute("name","");
            int time=0; //docElement.attribute("time","").toInt();
            int cost=0; //docElement.attribute("cost","").toInt();
            qDebug() << docElement.attribute("name","");
            Task* lifeTask=new Task(name,root,time,cost);

            transverseNode(docElement,*lifeTask);
        }
        file.close();
    }


    life2xml(root.child(0),"../LifeTree/lifeout.xml");

    QDomDocument domDoc2;


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
    Task* dance=root.child(0)->child(0);
    Task* sport=root.child(0)->child(1);
    Task* meet =root.child(0)->child(2);
    dance->appendChild(sport);
    meet->appendChild(dance);
    QTreeView treeView;
    //treeView.setColumnWidth(0,500);
    treeView.setModel(&model);

    treeView.show();
    return a.exec();
}

