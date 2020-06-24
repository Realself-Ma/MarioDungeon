#ifndef INTERFACE_H
#define INTERFACE_H

#include <QPainter>
#include <QMessageBox>
#include <QApplication>
#include "factory.h"
class interface : public QWidget
{
    Q_OBJECT
public:
    explicit interface(QWidget *parent = 0);
public:
    QTimer* timer;
    bool isok;
    bool classicalisok;
    bool Dungeonisok;
    bool surfaceShow;
    void showMianMenu();
private:
    Factory *fac;
    QLabel* msgLabel;
    QPushButton* button_classical;
    QPushButton* button_Dungeon;
    QPushButton *button_Quit;
    QPushButton *button_About;
    QProgressBar* probar;//进度条
    QPainter* painter;
private:
    void paintEvent(QPaintEvent*);
    void drawPage(QPainter*painter,QString url);
    void drawStartPage();
public slots:
    void classicalStart();
    void DungeonStart();
    void loading();
    void Quit();
    void AboutShow();
};

#endif // INTERFACE_H
