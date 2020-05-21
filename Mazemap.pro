#-------------------------------------------------
#
# Project created by QtCreator 2019-07-07T14:36:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Mazemap
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    maze.cpp \
    interface.cpp \
    Dungeon.cpp \
    storewidget.cpp \
    factory.cpp \
    sound.cpp

HEADERS  += mainwindow.h \
    maze.h \
    interface.h \
    Dungeon.h \
    storewidget.h \
    factory.h \
    sound.h

FORMS    +=

RESOURCES += \
    sources.qrc
QT += multimedia
CONFIG += resources_big
RC_ICONS = Maze&Dungeon.ico
