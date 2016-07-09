#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QSpinBox>

#include "taskmodel.h"
#include "mytreeview.h"

//namespace Ui {
//class MainWindow;
//}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Task* pRootTask;
    Task* pLifeTask;

    //TaskModel model(&root);
    TaskModel* pModel;//=&model;
    MyTreeView treeView;



private:
    //Ui::MainWindow *ui;
    void createFormInterior();
    QAction *saveAct;

private slots:
    //void newFile();
    //void open();
    bool onSave();
    //void about();
};



#endif // MAINWINDOW_H
