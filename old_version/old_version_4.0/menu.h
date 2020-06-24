#ifndef MENU_H
#define MENU_H

#include <QWidget>
#include <QPalette>
#include "factory.h"

class Menu: public QWidget
{
    Q_OBJECT
public:
    explicit Menu(QWidget *parent = 0);
    QLabel *pauseEvent;
    QPushButton *ReturnMainMenu;
    QPushButton *Restart;
    QPushButton *Continue;
    bool MenuWinShow;
private:
    QWidget* MenuWidget;
    Factory* fac;
private:
    void showEvent(QShowEvent *);
    void hideEvent(QHideEvent*);
};

#endif // MENU_H
