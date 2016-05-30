#include "mytreeview.h"
#include <QApplication>

void MyTreeView::onTreeDoubleClicked(const QModelIndex & index){
    if (!index.isValid()) {
        return;
    }
    //parentOfDeletedIndex=&(index.parent());

    //emit deleteTaskEvent(index);
}

void MyTreeView::onCollapsed(const QModelIndex & index){

    //expand(index);
}

void MyTreeView::getListExpanded(QModelIndex& index, QList<QModelIndex*>& listExpanded)
{

    if (!index.isValid()) {
        return;
    }

    if (!isExpanded(index))
        return;

    listExpanded<< &index;

    int childCount = index.model()->rowCount(index);
    for (int i = 0; i < childCount; i++) {
        const QModelIndex &child = index.child(i, 0);
        // Recursively call the function for each child node.
        //getListExpanded(child,listExpanded);
    }
}
//void MyTreeView::expand(){
    //foreach (QModelIndex* index)

//}
/*
if (!view->expanded(index)) {
        view->expand(index);
    }



    QStringList List;
    this->expandeIndexes;
    // prepare list
    // PS: getPersistentIndexList() function is a simple `return this->persistentIndexList()` from TreeModel model class
    foreach (QModelIndex index, model->>getPersistentIndexList())
    {
        if (view->isExpanded(index))
        {
            List << index.data(Qt::DisplayRole).toString();
        }
    }

    // save list
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.beginGroup("MainWindow");
    settings.setValue("ExpandedItems", QVariant::fromValue(List));
    settings.endGroup();

    //model->persistentIndexList();
} */
/*
void MyTreeView::restoreState(void)
{
    QStringList List;

    // get list
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.beginGroup("MainWindow");
    List = settings.value("ExpandedItems").toStringList();
    settings.endGroup();

    foreach (QString item, List)
    {
        // search `item` text in model
        QModelIndexList Items = model->match(model->index(0, 0), Qt::DisplayRole, QVariant::fromValue(item));
        if (!Items.isEmpty())
        {
            // Information: with this code, expands ONLY first level in QTreeView
            view->setExpanded(Items.first(), true);
        }
    }
}
*/
void MyTreeView::keyPressEvent(QKeyEvent* event)
{
   QModelIndex currentIndex=this->currentIndex();
   if (!currentIndex.isValid()) return;

   QModelIndex parentIndex=currentIndex.parent();

   if (event->key()==Qt::Key_Delete && parentIndex.isValid()){
        emit deleteKeyEvent(currentIndex);
   }

   if (event->key()==Qt::Key_Insert){
       if(event->modifiers() & Qt::AltModifier){
          Task *currentTask = static_cast<Task*>(currentIndex.internalPointer());
           emit insertAltKeyEvent(currentIndex);
           setCurrentIndex(currentIndex.child(currentTask->childCount()-1,0));
       }
       else {
           Task *parentTask = static_cast<Task*>(parentIndex.internalPointer());
           emit insertKeyEvent(currentIndex);
           setCurrentIndex(parentIndex.child(currentIndex.row()+1,0));
       }
       //resizeColumnToContents(0);
       //return;
   }
   resizeColumnToContents(0);
   QTreeView::keyPressEvent(event);

}

