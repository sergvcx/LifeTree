#include "mainwindow.h"
#include <QApplication>
#include <QTreeView>
#include <qtXml/QDomDocument>
#include <qtXml/QDomNode>
#include <QFile>
#include <QtDebug>
#include <QDir>
#include <QMap>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow mainWindow;
    mainWindow.show();
    app.exec();
    return 0;
}

