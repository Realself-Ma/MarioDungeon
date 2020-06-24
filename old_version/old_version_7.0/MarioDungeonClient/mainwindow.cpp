#include "mainwindow.h"
#include <QMessageBox>
#include <QStyleFactory>
#include <QPalette>
#include <QPixmap>
#include <QTime>
QMainWindow* ptr_MianWindow;//全局变量定义必须放在CPP文件中，外部文件引用extern声明可以放在头文件
interface* surface;//开始界面
QGridLayout*gLayout_Map;//地图网格布局管理器
QWidget *MazeWidget[12][16];//地图窗口数组
QWidget* People;//角色
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉边框
    ptr_MianWindow=this;
    initialWindow_Layout();
    //初始化时间种子
    QTime time=QTime::currentTime();
    qsrand(time.msec()+time.second()*1000);
}

MainWindow::~MainWindow()
{
}
void MainWindow::initialWindow_Layout()
{
    gLayout_Map=new QGridLayout;
    hLayout=new QHBoxLayout;
    gLayout_Map->setContentsMargins(0,0,0,0);//让布局紧贴主窗口，不留空白
    gLayout_Map->setSpacing(0);//让两个控件之间的间隔为0
    hLayout->setContentsMargins(0,0,0,0);
    hLayout->setSpacing(0);
    //初始化迷宫砖块
    for(int i=0; i <12 ;i++)
    {
        for(int j=0; j<16; j++)
        {
            MazeWidget[i][j]=new QWidget(this);
            gLayout_Map->addWidget(MazeWidget[i][j],i,j);
            MazeWidget[i][j]->hide();
        }
    }
    //初始化角色
    People=new QWidget(this);
    People->hide();

    QWidget *Mapwidget=new QWidget(this);

    surface=new interface(this);
    surface->setMinimumSize(832,672);
    connect(surface->timer,SIGNAL(timeout()),this,SLOT(ShowWidget()));//显示控制界面()));//传递过来的参数有问题，会一直调用槽函数
    gLayout_Map->addWidget(surface,0,0);
    Mapwidget->setMinimumSize(832,672);
    Mapwidget->setLayout(gLayout_Map);

    MainWidget=new QWidget(this);
    d.initialDugeon();//希望显示在MainWidget之上的窗口，要在初始化MainWidget后再初始化

    hLayout->addWidget(Mapwidget);
    hLayout->addWidget(d.infoWidget);

    MainWidget->setLayout(hLayout);
    MainWidget->setFocusPolicy(Qt::StrongFocus);

    //设置widget为Mainwindow的中心窗口
    this->setCentralWidget(MainWidget);
}

//角色跳跃方式移动(结合布局管理器)
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(surface->Dungeonisok)//地牢模式
    {
        d.keyPressEvent(event);
    }
    else
    {
        return;
    }
}

//重写鼠标点击和移动事件，使没有边框的窗口可以通过鼠标移动
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    this->windowPos = this->pos();                // 获得部件当前位置
    this->mousePos = event->globalPos();     // 获得鼠标位置
    this->dPos = mousePos - windowPos;       // 移动后部件所在的位置
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    this->move(event->globalPos() - this->dPos);
}
void MainWindow::ShowWidget()
{
    if(surface->isok)
    {
        d.NameEdit->show();
        d.SetDone->show();
    }
}
