#ifndef INTERFACE_H
#define INTERFACE_H

#include <QPainter>
#include <QMovie>
#include <QResizeEvent>
#include <QMessageBox>
#include <QApplication>
#include "factory.h"
#include "sign.h"
class interface : public QWidget
{
    Q_OBJECT
public:
    explicit interface(QWidget *parent = 0);
    void resizeEvent(QResizeEvent*);
public:
    Sign* sign;
    QTimer* timer;
    QPushButton* button_NetPlay;
    QPushButton* button_LocalDungeon;
    QPushButton *button_Quit;
    QPushButton *button_About;
    bool isok;
    bool Dungeonisok;
    bool surfaceShow;
    void showMianMenu();
private:
    Factory *fac;
    QLabel* msgLabel;
    QLabel* GifLabel;
    QProgressBar* probar;//进度条
    QPainter* painter;
public slots:
    void NetPlayStart();
    void LocalDungeonStart();
    void loading();
    void Quit();
    void AboutShow();
};

#endif // INTERFACE_H
