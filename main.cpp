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

//QMap<QString,>
void transverseNode(const QDomNode& Node, Task& parent){
    QDomNode domNode= Node.firstChild();
    while(!domNode.isNull()){
        Task* childTask;
        if (domNode.isElement()){
            QDomElement domElement=domNode.toElement();
            if (!domElement.isNull()){
                if (domElement.tagName()=="task") {
                    QString name=domElement.attribute("name","");
                    int time=0;
                    int cost=0;
                    if (domElement.hasAttribute("time")){
                        time=domElement.attribute("time","").toInt();
                    }
                    if (domElement.hasAttribute("cost")){
                        cost=domElement.attribute("cost","").toInt();
                    }

                    qDebug() << domElement.attribute("name","");
                    childTask=new Task(name,parent,time,cost);
                }
                else {
                    qDebug() << domElement.tagName() << "\tText" << domElement.text();
                }
            }
        }
        transverseNode(domNode, *childTask);
        domNode= domNode.nextSibling();
    }

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

