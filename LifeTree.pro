#-------------------------------------------------
#
# Project created by QtCreator 2016-04-23T15:12:52
#
#-------------------------------------------------

QT       += core gui
QT      +=xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LifeTree
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    taskmodel.cpp \
    task.cpp \
    mytreeview.cpp \
    menus.cpp

HEADERS  += mainwindow.h \
    taskmodel.h \
    task.h \
    mytreeview.h \
    menus.h

FORMS    += mainwindow.ui
