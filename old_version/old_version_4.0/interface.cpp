#include "interface.h"
#include <QPalette>
#include <QPixmap>
#include <QMovie>
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

    GifLabel=new QLabel(this);
    GifLabel->setScaledContents(true);//设置标签填满窗口

    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(loading()));

    QFont font("Microsoft YaHei" ,20, 85);
    QString button_style="QPushButton{background-color:transparent;color:papayawhip;border-radius:5px;}"
                         "QPushButton:hover{background-color:palegreen; color: orangered;}"
                         "QPushButton:pressed{background-color:aquamarine;border-style:inset;}";

    button_NetPlay= fac->CreateQPushButton(this,340,190,160,60,"联网对战",button_style,font);
    connect(button_NetPlay,SIGNAL(clicked()),this,SLOT(NetPlayStart()));


    button_LocalDungeon= fac->CreateQPushButton(this,340,270,160,60,"单机模式",button_style,font);
    connect(button_LocalDungeon,SIGNAL(clicked()),this,SLOT(LocalDungeonStart()));

    button_Quit= fac->CreateQPushButton(this,340,350,160,60,"离开游戏",button_style,font);
    connect(button_Quit,SIGNAL(clicked()),this,SLOT(Quit()));

    button_About= fac->CreateQPushButton(this,340,430,160,60,"关于",button_style,font);
    connect(button_About,SIGNAL(clicked()),this,SLOT(AboutShow()));

    isok=false;
    Dungeonisok=false;
    surfaceShow=true;
}
void interface::resizeEvent(QResizeEvent*)
{
    GifLabel->clear();
    if(!isok)
    {
        QMovie *movie = new QMovie(":/interface/image/interface/StartPage.gif");
        GifLabel->setMovie(movie);
        movie->start();
    }
    else
    {
        //QMovie *movie = new QMovie(":/interface/image/interface/SetNamePage.PNG");
        //GifLabel->setMovie(movie);
        //movie->start();
        GifLabel->setStyleSheet("border-image: url(:/interface/image/interface/SetNamePage.PNG);");
    }
    GifLabel->resize(this->size());
}

void interface::showMianMenu()
{
    button_NetPlay->show();
    button_LocalDungeon->show();
    button_Quit->show();
    button_About->show();
    surfaceShow=true;
}

void interface::NetPlayStart()
{

}

void interface::LocalDungeonStart()
{
    button_NetPlay->hide();//play按钮隐藏
    button_LocalDungeon->hide();
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
        resizeEvent(nullptr);
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
