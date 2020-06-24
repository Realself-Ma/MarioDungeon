#include "menu.h"

Menu::Menu(QWidget *parent) :
    QWidget(parent)
{
    fac=new Factory();
    MenuWinShow=false;
    QString button_style="QPushButton{background-color:transparent;color:papayawhip;border-radius:5px;}"
                         "QPushButton:hover{background-color:palegreen; color: orangered;}"
                         "QPushButton:pressed{background-color:aquamarine;border-style:inset;}";
    QString MenuWidget_style="color:orangered;background-color:transparent";
    QFont fontLabel("Microsoft YaHei" ,12, 75);
    QFont fontPauseEvent("Microsoft YaHei",14,100);
    QWidget* MenuWidget=new QWidget(this);
    MenuWidget->setStyleSheet(MenuWidget_style);

    pauseEvent=fac->CreateQLabel(MenuWidget,60,50,400,100,"PAUSE",MenuWidget_style,fontPauseEvent);
    ReturnMainMenu=fac->CreateQPushButton(MenuWidget,200,160,120,30,"返回主菜单",button_style,fontLabel);
    Restart=fac->CreateQPushButton(MenuWidget,200,200,120,30,"重新开始",button_style,fontLabel);
    Continue=fac->CreateQPushButton(MenuWidget,200,240,120,30,"继续游戏",button_style,fontLabel);
    ReturnRooms=fac->CreateQPushButton(MenuWidget,200,200,120,30,"返回房间",button_style,fontLabel);
    ReturnRooms->hide();
    Help=fac->CreateQPushButton(MenuWidget,200,280,120,30,"帮助",button_style,fontLabel);
}
void Menu::showEvent(QShowEvent *)
{
    MenuWinShow=true;
}
void Menu::hideEvent(QHideEvent*)
{
    MenuWinShow=false;
}

