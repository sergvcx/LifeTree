#ifndef MYTREEVIEW_H
#define MYTREEVIEW_H
#include <QTreeView>
#include <QObject>
#include "taskmodel.h"
class MyTreeView : public QTreeView
{
    Q_OBJECT
    TaskModel* model;
    QModelIndex* parentOfDeletedIndex;

public:

    void setMyModel(TaskModel &paramModel){
        model=&paramModel;
        QTreeView::setModel(&paramModel);
    }
    void saveState(void);
    void restoreState(void);

    //MyTreeView();
    void getListExpanded(QModelIndex& index, QList<QModelIndex*>& listExpanded);

signals:
    void deleteKeyEvent(const QModelIndex & index);
    void insertKeyEvent(const QModelIndex & index);
    void insertAltKeyEvent(const QModelIndex & index);

public slots:
    void onCollapsed(const QModelIndex & index);
    void onExpanded(const QModelIndex & index);
    void onTreeDoubleClicked(const QModelIndex & index);
    void onTreeClicked(const QModelIndex & index);
    void keyPressEvent(QKeyEvent* event);

};

#endif // MYTREEVIEW_H
