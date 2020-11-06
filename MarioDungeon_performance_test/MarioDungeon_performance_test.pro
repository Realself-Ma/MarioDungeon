#-------------------------------------------------
#
# Project created by QtCreator 2020-11-03T19:56:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MarioDungeon_performance_test
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    test.cpp

HEADERS  += widget.h \
    test.h \
    request.h

FORMS    +=
QT += network
