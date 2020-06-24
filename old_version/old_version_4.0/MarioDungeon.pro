#-------------------------------------------------
#
# Project created by QtCreator 2020-05-22T09:26:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MarioDungeon
TEMPLATE = app


SOURCES += main.cpp \
    Dungeon.cpp \
    storewidget.cpp \
    factory.cpp \
    sound.cpp \
    interface.cpp \
    mainwindow.cpp \
    menu.cpp

HEADERS  += \
    Dungeon.h \
    storewidget.h \
    factory.h \
    sound.h \
    interface.h \
    mainwindow.h \
    menu.h
QT += multimedia
RC_ICONS = MarioDungeon.ico

RESOURCES += \
    sources.qrc
