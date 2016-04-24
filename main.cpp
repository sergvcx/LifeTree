#include "mainwindow.h"
#include <QApplication>
#include <QTreeView>
#include "taskmodel.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;


    Task root("root");
    Task life("life",root);
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

    TaskModel model(&root);
    QTreeView treeView;
    treeView.setModel(&model);

    treeView.show();
    return a.exec();
}
