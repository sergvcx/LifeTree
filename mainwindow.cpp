#include "mainwindow.h"
#include "qMenuBar.h"
#include "filesave.xpm"
/*
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}




MainWindow::~MainWindow()
{
    delete ui;
}
*/

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("LifeTree"));

    createFormInterior();

    QMenuBar *menuBar = new QMenuBar;
    QMenu *menuWindow = menuBar->addMenu(tr("&File"));
    QAction *saveAct = new QAction(menuWindow);
    saveAct->setText(tr("Save"));
    menuWindow->addAction(saveAct);
    connect(saveAct, &QAction::triggered, this, &MainWindow::onSave);

    menuWindow->addAction("&About LifeTree",
                    this,
                    SLOT(aboutQt()),
                    Qt::CTRL + Qt::Key_Q
                   );

    QAction* pCheckableAction = menuWindow->addAction(QPixmap(filesave),"&CheckableItem");
    pCheckableAction->setCheckable(true);
    pCheckableAction->setChecked(true);

    //pmnu->addAction(QPixmap(img4_xpm), "&IconItem");

    QMenu* pmnuSubMenu = new QMenu("&SubMenu", menuWindow);
    menuWindow->addMenu(pmnuSubMenu);
    pmnuSubMenu->addAction("&Test");

    QAction* pDisabledAction = menuWindow->addAction("&DisabledItem");
    pDisabledAction->setEnabled(false);

    menuWindow->addSeparator();
    menuWindow->addAction("&Exit", this, SLOT(quit()));



    setMenuBar(menuBar);

    //onAddNew();
    pRootTask = new Task("root");
    pLifeTask = new Task("life");
    pRootTask->appendChildTask(*(pLifeTask->pTaskData));
    pModel = new TaskModel(pRootTask);
    treeView.setModel(pModel);
    TaskModel &model=*pModel;
    model.loadXML();

    setCentralWidget(&treeView);
    resize(1000,600);

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


}


bool MainWindow::onSave()
{
    pModel->saveXML();
    return true;
}

MainWindow::~MainWindow()
{
    onSave();
}

void MainWindow::createFormInterior()
{
    QWidget * wgt = new QWidget(this);
    setCentralWidget(wgt);

    // Далее здесь следует создать менеджер компоновки
    // для центрального виджета wgt, внутри которого
    // реализовать требуемый интерьер формы.
}
