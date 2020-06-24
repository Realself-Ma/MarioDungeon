#include "help.h"

Help::Help(QWidget *parent) : QWidget(parent)
{
    fac=new Factory();
    QString button_style="QPushButton{background-color:transparent;color:papayawhip;border-radius:5px;}"
                         "QPushButton:hover{background-color:palegreen; color: orangered;}"
                         "QPushButton:pressed{background-color:aquamarine;border-style:inset;}";
    QString HelpWidget_style="color:orangered;background-color:black";
    QFont fontLabel("Microsoft YaHei" ,12, 75);
    QWidget* HelpWidget=new QWidget(this);
    HelpWidget->setStyleSheet(HelpWidget_style);
    QLabel* label_W=fac->CreateQLabel(HelpWidget,120,20,80,30,"W ↑",HelpWidget_style,fontLabel);
    QLabel* label_W_info=fac->CreateQLabel(HelpWidget,260,20,140,30,"上",HelpWidget_style,fontLabel);
    QLabel* label_S=fac->CreateQLabel(HelpWidget,120,60,80,30,"S ↓",HelpWidget_style,fontLabel);
    QLabel* label_S_info=fac->CreateQLabel(HelpWidget,260,60,140,30,"下",HelpWidget_style,fontLabel);
    QLabel* label_A=fac->CreateQLabel(HelpWidget,120,100,80,30,"A ←",HelpWidget_style,fontLabel);
    QLabel* label_A_info=fac->CreateQLabel(HelpWidget,260,100,140,30,"左",HelpWidget_style,fontLabel);
    QLabel* label_D=fac->CreateQLabel(HelpWidget,120,140,80,30,"D →",HelpWidget_style,fontLabel);
    QLabel* label_D_info=fac->CreateQLabel(HelpWidget,260,140,140,30,"右",HelpWidget_style,fontLabel);
    QLabel* label_Pickaxe=fac->CreateQLabel(HelpWidget,120,180,80,30,"P",HelpWidget_style,fontLabel);
    QLabel* label_Pickaxe_info=fac->CreateQLabel(HelpWidget,220,180,220,30,"开启/关闭破墙功能",HelpWidget_style,fontLabel);
    QLabel* label_Downstairs=fac->CreateQLabel(HelpWidget,120,220,80,30,"N",HelpWidget_style,fontLabel);
    QLabel* label_Downstairs_info=fac->CreateQLabel(HelpWidget,220,220,220,30,"下楼器",HelpWidget_style,fontLabel);
    Return=fac->CreateQPushButton(HelpWidget,180,280,120,30,"返回",button_style,fontLabel);

}
