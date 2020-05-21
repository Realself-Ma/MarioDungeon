#include "interface.h"
#include <QPalette>
#include <QPixmap>
#include <QFont>
#include <QDebug>
int msgNum=0;
QString msg[] = {"加载中.","加载中..","加载中...","加载中...."};
interface::interface(QWidget *parent) :
    QWidget(parent)
{
    fac=new Factory();
    msgLabel=fac->CreateQLabel(this,380,440,100,75,"","color:white",QFont("Microsoft YaHei",13));
    msgLabel->hide();

    probar=fac->CreateQProgressBar(this,180,500,480,15,true);
    probar->hide();

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(loading()));

    QFont font("Microsoft YaHei" ,16, 55);
    QString button_style="QPushButton{background-color:transparent;color:white;border-radius:5px;}"
                         "QPushButton:hover{background-color:palegreen; color: orangered;}"
                         "QPushButton:pressed{background-color:aquamarine;border-style:inset;}";

    button_classical= fac->CreateQPushButton(this,360,285,120,30,"迷宫模式",button_style,font);//开始界面上的play按钮
    connect(button_classical,SIGNAL(clicked()),this,SLOT(classicalStart()));


    button_Dungeon= fac->CreateQPushButton(this,360,325,120,30,"地牢模式",button_style,font);
    connect(button_Dungeon,SIGNAL(clicked()),this,SLOT(DungeonStart()));

    button_Quit= fac->CreateQPushButton(this,360,365,120,30,"离开游戏",button_style,font);
    connect(button_Quit,SIGNAL(clicked()),this,SLOT(Quit()));

    button_About= fac->CreateQPushButton(this,360,405,120,30,"关于",button_style,font);
    connect(button_About,SIGNAL(clicked()),this,SLOT(AboutShow()));

    isok=false;
    classicalisok=false;
    Dungeonisok=false;
    surfaceShow=true;
}
void interface::drawPage(QPainter *painter, QString url)
{
    painter->drawImage(geometry(),QImage(url));
}
void interface::drawStartPage()
{
    painter = new QPainter(this);
    if(!isok)
        drawPage(painter,":/interface/image/interface/StartPage.png");//游戏开始页
    else if(classicalisok)
        drawPage(painter,":/interface/image/interface/MazeDemo.png");//迷宫开始页
    else
          drawPage(painter,":/interface/image/interface/SetNamePage.PNG");//地牢开始页
    painter->end();
}

void interface::paintEvent(QPaintEvent*)
{
    drawStartPage();
}
void interface::showMianMenu()
{
    button_classical->show();
    button_Dungeon->show();
    button_Quit->show();
    button_About->show();
    surfaceShow=true;
}

void interface::classicalStart()
{
    button_classical->hide();//play按钮隐藏
    button_Dungeon->hide();
    button_Quit->hide();
    button_About->hide();
    msgLabel->show();//提示消息显示

    timer->start(100);//启动计数器
    probar->show();//进度前显示
    classicalisok=true;
}
void interface::DungeonStart()
{
    button_classical->hide();//play按钮隐藏
    button_Dungeon->hide();
    button_Quit->hide();
    button_About->hide();
    msgLabel->show();//提示消息显示
    timer->start(100);//启动计数器
    probar->show();//进度前显示
    Dungeonisok=true;
}

void interface::loading()
{
    msgLabel->setText(msg[(msgNum++)%4]);//提示信息的变化
    probar->setValue(probar->value()+20);
    if(probar->value() >= 99)
    {
        timer->stop();
        msgLabel->hide();
        probar->hide();
        isok=true;
        probar->setValue(0);
        this->update();//更新窗口，触发绘图事件
    }

}
void interface::Quit()
{
    QMessageBox message(QMessageBox::Information,"退出游戏","是否退出？",QMessageBox::Yes|QMessageBox::No,this);
    message.setIconPixmap(QPixmap(":/info/image/information/退出.png"));
    message.setWindowIcon(QIcon(":/info/image/information/关于.ico"));
    message.setButtonText(QMessageBox::Yes, QString("确 定"));
    message.setButtonText(QMessageBox::No, QString("返 回"));
    if(message.exec()==QMessageBox::Yes)
    {
        qApp->quit();//退出当前应用程序，需要 QApplication的头文件
    }
    else
    {
        return;
    }
}
void interface::AboutShow()
{
    QMessageBox message(QMessageBox::Information,"关于",
                        "Written By Realself,in SWJTU,2019.08",QMessageBox::Yes,this);
    message.setIconPixmap(QPixmap(":/info/image/information/关于.ico"));
    message.button(QMessageBox::Yes)->hide();//去除按钮
    message.exec();
}
