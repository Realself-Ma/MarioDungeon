#include "maze.h"
#include <QTime>
#include <QDebug>

point::point():i(0),j(0),state(0){}
point::point(int _i,int _j,int _state):i(_i),j(_j),state(_state){}
bool point::operator==(const point &p)
{
    if(p.i==i&&p.j==j&&p.state==state)
    {
        return true;
    }
    else
    {
        return false;
    }
}

maze::maze(QWidget *parent): QWidget(parent)
{
   recordMatrix=nullptr;
   Matrix=nullptr;
   fac=new Factory();

   BASIC_WIDTH=0;
   BASIC_HEIGHT=0;
   iNum=0;
   lastheight=0;
   lastwidth=0;
   isPlay=false;
   //isShow=false;
   isAIAnimationButton=false;
   isAutoMoveButton=false;
   group=new QSequentialAnimationGroup;
   timer=new QTimer(this);
   connect(timer,SIGNAL(timeout()),this,SLOT(moveCharacter()));
   connect(timer,SIGNAL(timeout()),this,SLOT(ShowPath()));

   //初始化时间种子
   QTime time=QTime::currentTime();
   qsrand(time.msec()+time.second()*1000);

   MazeStack=new QStack<point>;
   move[0].i=-1;move[0].j=0;
   move[1].i=0;move[1].j=1;
   move[2].i=1;move[2].j=0;
   move[3].i=0;move[3].j=-1;
   initialMapStytle();
}
maze::~maze()
{
}

void maze::initialMaze()
{
    MazeStack->clear();
    x=y=1;
    exit_x=height-2;
    exit_y=width-2;
    if(Matrix)
    {
        for(int i=0;i<last_height;i++)
        {
            delete [] Matrix[i];
        }
        delete [] Matrix;
    }
    if(recordMatrix)
    {
        for(int i=0;i<last_height;i++)
        {
            delete [] recordMatrix[i];
        }
        delete [] recordMatrix;
    }
    Matrix=new point *[height];
    recordMatrix=new point *[height];
    for(int i=0;i<height;i++)
    {
        Matrix[i]=new point[width];
        recordMatrix[i]=new point[width];
    }
    for(int i=0;i<height;i++)
        for(int j=0;j<width;j++)
            recordMatrix[i][j].state=1;
}
void maze::initialMapStytle()
{
    MapStytle[0][0]="border-image: url(:/new/image/MagicGril/down1.png);";
    MapStytle[0][1]="border-image: url(:/new/image/MagicGril/down2.png);";
    MapStytle[0][2]="border-image: url(:/new/image/MagicGril/down3.png);";
    MapStytle[0][3]="border-image: url(:/new/image/MagicGril/up1.png);";
    MapStytle[0][4]="border-image: url(:/new/image/MagicGril/up2.png);";
    MapStytle[0][5]="border-image: url(:/new/image/MagicGril/up3.png);";
    MapStytle[0][6]="border-image: url(:/new/image/MagicGril/left1.png);";
    MapStytle[0][7]="border-image: url(:/new/image/MagicGril/left2.png);";
    MapStytle[0][8]="border-image: url(:/new/image/MagicGril/left3.png);";
    MapStytle[0][9]="border-image: url(:/new/image/MagicGril/right1.png);";
    MapStytle[0][10]="border-image: url(:/new/image/MagicGril/right2.png);";
    MapStytle[0][11]="border-image: url(:/new/image/MagicGril/right3.png);";
    MapStytle[0][12]="border-image: url(:/new/image/MagicGril/Wall.png);";
    MapStytle[0][13]="border-image: url(:/new/image/MagicGril/Load.png);";
    MapStytle[0][14]="border-image: url(:/new/image/MagicGril/box.png);";
    MapStytle[0][15]="border-image: url(:/new/image/MagicGril/MagicGrill.png);";

    MapStytle[1][0]="border-image: url(:/Naruto/image/Naruto/down1.png);";
    MapStytle[1][1]="border-image: url(:/Naruto/image/Naruto/down2.png);";
    MapStytle[1][2]="border-image: url(:/Naruto/image/Naruto/down3.png);";
    MapStytle[1][3]="border-image: url(:/Naruto/image/Naruto/up1.png);";
    MapStytle[1][4]="border-image: url(:/Naruto/image/Naruto/up2.png);";
    MapStytle[1][5]="border-image: url(:/Naruto/image/Naruto/up3.png);";
    MapStytle[1][6]="border-image: url(:/Naruto/image/Naruto/left1.png);";
    MapStytle[1][7]="border-image: url(:/Naruto/image/Naruto/left2.png);";
    MapStytle[1][8]="border-image: url(:/Naruto/image/Naruto/left3.png);";
    MapStytle[1][9]="border-image: url(:/Naruto/image/Naruto/right1.png);";
    MapStytle[1][10]="border-image: url(:/Naruto/image/Naruto/right2.png);";
    MapStytle[1][11]="border-image: url(:/Naruto/image/Naruto/right3.png);";
    MapStytle[1][12]="border-image: url(:/Naruto/image/Naruto/Wall.png);";
    MapStytle[1][13]="border-image: url(:/Naruto/image/Naruto/Load.png);";
    MapStytle[1][14]="border-image: url(:/Naruto/image/Naruto/box.png);";
    MapStytle[1][15]="border-image: url(:/Naruto/image/Naruto/Naruto.png);";

    MapStytle[2][0]="border-image: url(:/SuperMarie/image/SuperMarie/down1.png);";
    MapStytle[2][1]="border-image: url(:/SuperMarie/image/SuperMarie/down2.png);";
    MapStytle[2][2]="border-image: url(:/SuperMarie/image/SuperMarie/down3.png);";
    MapStytle[2][3]="border-image: url(:/SuperMarie/image/SuperMarie/up1.png);";
    MapStytle[2][4]="border-image: url(:/SuperMarie/image/SuperMarie/up2.png);";
    MapStytle[2][5]="border-image: url(:/SuperMarie/image/SuperMarie/up3.png);";
    MapStytle[2][6]="border-image: url(:/SuperMarie/image/SuperMarie/left1.png);";
    MapStytle[2][7]="border-image: url(:/SuperMarie/image/SuperMarie/left2.png);";
    MapStytle[2][8]="border-image: url(:/SuperMarie/image/SuperMarie/left3.png);";
    MapStytle[2][9]="border-image: url(:/SuperMarie/image/SuperMarie/right1.png);";
    MapStytle[2][10]="border-image: url(:/SuperMarie/image/SuperMarie/right2.png);";
    MapStytle[2][11]="border-image: url(:/SuperMarie/image/SuperMarie/right3.png);";
    MapStytle[2][12]="border-image: url(:/SuperMarie/image/SuperMarie/Wall.png);";
    MapStytle[2][13]="border-image: url(:/SuperMarie/image/SuperMarie/Load.png);";
    MapStytle[2][14]="border-image: url(:/SuperMarie/image/SuperMarie/box.png);";
    MapStytle[2][15]="border-image: url(:/SuperMarie/image/SuperMarie/SuperMarie.png);";

}
void maze::initialControlWidget()
{
    gLayout_Control=new QGridLayout;
    gLayout_Control->setSpacing(10);
    Controlwidget=new QWidget(ptr_MianWindow);
    Controlwidget->setStyleSheet("background-color:lightGray");
    QString transparent_style="background-color:transparent";
    QString button_style="QPushButton{background-color:white; color: black;border-radius:1px;border:2px groove gray;border-style:outset;}"
                         "QPushButton:hover{background-color:lightGray; color: black;}"
                         "QPushButton:pressed{background-color:gray;border-style:inset;}";

    QLabel *label_w=fac->CreateQLabel(Controlwidget,"迷宫宽度",transparent_style);
    QLabel *label_h=fac->CreateQLabel(Controlwidget,"迷宫高度",transparent_style);

    QLabel *label_blank[10];
    for(int i=0;i<10;i++)
        label_blank[i]=fac->CreateQLabel(Controlwidget,"",transparent_style);

    StytleNum=2;
    label_Stytle=fac->CreateQLabel(Controlwidget,"",MapStytle[StytleNum][15]);
    QLabel *label_select=fac->CreateQLabel(Controlwidget,"选择地图风格",transparent_style);

    sp_w=fac->CreateQSpinBox(Controlwidget,transparent_style);
    sp_h=fac->CreateQSpinBox(Controlwidget,transparent_style);

    SelectMapStytle=fac->CreateQComboBox(Controlwidget,2);
    connect(SelectMapStytle,SIGNAL(currentIndexChanged(int)),this,SLOT(MapStytleSet()));
    //currentIndexChanged(int) 中的int 去掉，就不能被识别为信号

    QPushButton *GenerateButton=fac->CreateQPushButton(Controlwidget,"生成迷宫",button_style);

    quitButton=fac->CreateQPushButton(Controlwidget,"主菜单",button_style);
    AIAnimationButton=fac->CreateQPushButton(Controlwidget,"AI操作",button_style);
    AutoMoveButton=fac->CreateQPushButton(Controlwidget,"自动寻路",button_style);
    AIAnimationButton->setEnabled(false);
    AutoMoveButton->setEnabled(false);
    quitButton->setEnabled(false);

    connect(GenerateButton,SIGNAL(clicked()),this,SLOT(CreateMaze_Layout()));
    connect(AutoMoveButton,SIGNAL(clicked()),this,SLOT(timeStart()));
    connect(AIAnimationButton,SIGNAL(clicked()),this,SLOT(ShowAnimation()));
    connect(quitButton,SIGNAL(clicked()),this,SLOT(quit()));

    gLayout_Control->addWidget(label_w,0,0);
    gLayout_Control->addWidget(label_h,1,0);
    gLayout_Control->addWidget(label_blank[0],0,2);

    gLayout_Control->addWidget(sp_w,0,1);
    gLayout_Control->addWidget(label_blank[1],1,2);

    gLayout_Control->addWidget(sp_h,1,1);

    gLayout_Control->addWidget(label_blank[2],2,0,1,3);

    gLayout_Control->addWidget(label_select,3,0);
    gLayout_Control->addWidget(label_blank[3],3,2);

    gLayout_Control->addWidget(SelectMapStytle,3,1);
    gLayout_Control->addWidget(label_Stytle,4,0,1,2);

    gLayout_Control->addWidget(GenerateButton,5,1);

    gLayout_Control->addWidget(AutoMoveButton,6,1);

    gLayout_Control->addWidget(AIAnimationButton,7,1);
    gLayout_Control->addWidget(label_blank[4],8,0,1,3);
    gLayout_Control->addWidget(quitButton,9,1);
    Controlwidget->setLayout(gLayout_Control);
    Controlwidget->hide();
}
bool maze::isOdd(int num)
{
    return !(num%2==0);
}

void maze::CreateMaze_Layout()
{
    //停止之前可能的工作
    timer->stop();
    group->stop();
    iNum=1;

    if((sp_h->value()<6||sp_h->value()>58)||(sp_w->value()<6||sp_w->value()>58))
    {
        QMessageBox message(QMessageBox::NoIcon, "警告！", "输入的数据需在6-58之间");
        message.setIconPixmap(QPixmap(":/info/image/information/warning.png"));
        message.exec();
        return;
    }
    else
    {
        height=sp_h->value();
        width=sp_w->value();
    }
    //当h和w都为偶数时，迷宫的出口（height-2，width-2）会被墙给封住
    if(!isOdd(height)&&!isOdd(width))
    {
        if(height>width)
        {
            height+=1;
            width=height;
        }
        else
        {
            width+=1;
            height=width;
        }
    }
    BASIC_WIDTH=840/width;//52
    BASIC_HEIGHT=680/height;//56

    if(surface->surfaceShow)
    {
        surface->hide();
        gLayout_Map->removeWidget(surface);
        surface->surfaceShow=false;
    }

    People->setMinimumSize(BASIC_WIDTH,BASIC_HEIGHT);

    initialMaze();
    CreateMaze();
    autoFindPath();
    StytleNum=SelectMapStytle->currentIndex();
    ShowMaze_Layout();
    lastheight=height;
    lastwidth=width;
    last_height=height;
    AIAnimationButton->setEnabled(true);
    AutoMoveButton->setEnabled(true);
    quitButton->setEnabled(true);
    isPlay=true;
}
void maze::ShowMaze_Layout()
{
    if(isShow)
    {
        for(int i=0;i<lastheight;i++)
        {
            for(int j=0;j<lastwidth;j++)
            {
                MazeWidget[i][j]->hide();
            }
        }
    }
    gLayout_Map->addWidget(People,x,y);
    People->setStyleSheet(MapStytle[StytleNum][0]);
    People->show();//用就显示
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width;j++)
        {
            MazeWidget[i][j]->setMinimumSize(BASIC_WIDTH,BASIC_HEIGHT);
            MazeWidget[i][j]->show();//要用的窗口必须要显示出来
            if(Matrix[i][j].state == 0)//状态0 代表 墙 障碍
                MazeWidget[i][j]->setStyleSheet(MapStytle[StytleNum][12]);
            if(Matrix[i][j].state == 1)//状态1 代表 草地 非障碍
                MazeWidget[i][j]->setStyleSheet(MapStytle[StytleNum][13]);
        }
    }
    Matrix[exit_x][exit_y].state=1;
    MazeWidget[exit_x][exit_y]->setStyleSheet(MapStytle[StytleNum][14]);
    isShow=true;
}

void maze::timeStart()
{
    isAutoMoveButton=true;
    isAIAnimationButton=false;
    AIAnimationButton->setEnabled(false);
    timer->start(50);
}
void maze::moveCharacter()//设置移动时的图片
{
    if(!isAutoMoveButton)
    {
        if(PathStack[iNum+1].i < PathStack[iNum].i)//up
        {
            People->setStyleSheet(MapStytle[StytleNum][3+iNum%3]);
        }
        if(PathStack[iNum+1].i > PathStack[iNum].i)//down
        {
            People->setStyleSheet(MapStytle[StytleNum][0+iNum%3]);
        }
        if(PathStack[iNum+1].j > PathStack[iNum].j)//right
        {
            People->setStyleSheet(MapStytle[StytleNum][9+iNum%3]);
        }
        if(PathStack[iNum+1].j < PathStack[iNum].j)//left
        {
            People->setStyleSheet(MapStytle[StytleNum][6+iNum%3]);
        }
        iNum++;
        if(iNum == PathStack.size()-1)
        {
            timer->stop();
            iNum = 0;
            AutoMoveButton->setEnabled(true);
            quitButton->setEnabled(true);
            return;
        }
    }
}
void maze::ShowPath()
{
    if(!isAIAnimationButton)
    {
        point temp=PathStack[iNum];
        MazeWidget[temp.i][temp.j]->setStyleSheet(MapStytle[StytleNum][0]);
        iNum++;
        if(iNum == PathStack.size()-1)
        {
            iNum = 0;
            for(int i=0;i<PathStack.size();i++)
            {
                point temp=PathStack[i];
                MazeWidget[temp.i][temp.j]->setStyleSheet(MapStytle[StytleNum][13]);
                MazeWidget[height-2][width-2]->setStyleSheet(MapStytle[StytleNum][14]);
            }
            timer->stop();
            AIAnimationButton->setEnabled(true);
            isAutoMoveButton=false;
            isAIAnimationButton=true;
            return;
        }

    }
}
void maze::ShowAnimation()
{
    group->clear();//动画组清空
    isAIAnimationButton=true;
    isAutoMoveButton=false;
    AutoMoveButton->setEnabled(false);
    quitButton->setEnabled(false);
    for(int i=0; i<PathStack.size()-1;i++)
    {
        QPropertyAnimation* animation = new QPropertyAnimation(People, "pos");//动作初始化,People是操作对象
        animation->setDuration(200);//设置动作间隔                              //"pos"是操作属性，与QPoint 对应
                                                                              //"geometry" 与QRect/QRectF 对应
        animation->setStartValue(QPoint(PathStack[i].j*BASIC_WIDTH,PathStack[i].i*BASIC_HEIGHT));
        animation->setEndValue(QPoint(PathStack[i+1].j*BASIC_WIDTH,PathStack[i+1].i*BASIC_HEIGHT));
        animation->setEasingCurve(QEasingCurve::Linear);
        group->addAnimation(animation);//向动画组中添加动作
    }
    timer->start(200);//设置计时间隔，必须与动作间隔大小一样，才会同步
    group->start();//动画组启用
}

void maze::quit()
{
    Controlwidget->hide();
    hideMaze();
    resetMaze();
    surface->isok=false;

    surface->show();//开始界面显示后，主窗口大小还是不变
    ptr_MianWindow->resize(840,680);//需要重新设置主窗口大小

    surface->showMianMenu();
}
void maze::hideMaze()
{

    for(int i=0;i<lastheight;i++)
    {
        for(int j=0;j<lastwidth;j++)
        {
            MazeWidget[i][j]->hide();
        }
    }
    People->hide();
    isShow=false;
    surface->classicalisok=false;
}

void maze::resetMaze()
{
    AIAnimationButton->setEnabled(false);
    AutoMoveButton->setEnabled(false);
    quitButton->setEnabled(false);
    sp_h->setValue(0);
    sp_w->setValue(0);
    SelectMapStytle->setCurrentIndex(2);
}
void maze::MapStytleSet()
{
   label_Stytle->setStyleSheet(MapStytle[SelectMapStytle->currentIndex()][15]);
}

void maze::setDirectionFalse(bool &up,bool &down,bool &left,bool &right)
{
    up=down=left=right=false;
}

/*
 *  迷宫的生成，其实就是一个找出所有可能的路径的过程：
 *       从起始点（可以随意设置，但要保证i和j都大于0,因为设定是四周都是墙壁），每次从往上、下、左、右四个方向中选取一个方向向前探测（每次前进的格数要大于1，
 *       为1的时候会走遍除四周外所有的方格），可以前进就向前前进，前进前把当前的位置压入记录栈中，
 *       方便上下左右四个方向都前进不了时回退到上一个记录点，不行再回退，栈为空时就结束循环。这样可以保证把所有可能的路径都尝试一遍。
 */
void maze::CreateMaze()
{
    int i=1,j=1;//初始位置在满足i>0,j>0的任意一点都可以，都为奇数比较好，可以不相等
    Matrix[i][j].state=1;
    point temp;
    temp.state=1;
    bool up=false,down=false,left=false,right=false;
    while(true)
    {
        temp.i=i;
        temp.j=j;

        int randNum=qrand()%4;//randNum是随机的，所以每次生成的迷宫也是随机的
        switch(randNum)
        {
        case 0://up
            if(!up&&i>2&&Matrix[i-2][j].state==0)//0代表障碍
            {
                MazeStack->push(temp);
                Matrix[i-2][j].state=1;
                Matrix[i-1][j].state=1;
                i=i-2;
                setDirectionFalse(up,down,left,right);
            }
            else
            {
                up=true;
            }
            break;
        case 1://down
            if(!down&&i<height-3&&Matrix[i+2][j].state==0)
            {
                MazeStack->push(temp);
                Matrix[i+2][j].state=1;
                Matrix[i+1][j].state=1;
                i=i+2;
                setDirectionFalse(up,down,left,right);
            }
            else
            {
                down=true;
            }
            break;
        case 2://left
            if(!left&&j>2&&Matrix[i][j-2].state==0)
            {
                MazeStack->push(temp);
                Matrix[i][j-2].state=1;
                Matrix[i][j-1].state=1;
                j=j-2;
                setDirectionFalse(up,down,left,right);
            }
            else
            {
                left=true;
            }
            break;
        case 3://right
            if(!right&&j<width-3&&Matrix[i][j+2].state==0)
            {
                MazeStack->push(temp);
                Matrix[i][j+2].state=1;
                Matrix[i][j+1].state=1;
                j=j+2;
                setDirectionFalse(up,down,left,right);
            }
            else
            {
                right=true;
            }
            break;
        }
        if(up&&down&&left&&right)//上下左右四个方向都不能移动时，就回退
        {
            if(!MazeStack->isEmpty())
            {
              i=MazeStack->top().i;
              j=MazeStack->top().j;
              MazeStack->pop();
              setDirectionFalse(up,down,left,right);
            }
            else//如果栈为空的话就返回，此时迷宫矩阵已经创建完毕
            {
                return;
            }
        }
    }
}
void maze::autoFindPath()
{
   PathStack.clear();
   point temp(this->x,this->y,0);
   PathStack.push(temp);
   int i,j,d;
   while(!PathStack.isEmpty())
   {
       point top=PathStack.top();
       PathStack.pop();
       i=top.i,j=top.j,d=top.state;
       while(d<4)
       {
           temp.i=i+move[d].i;
           temp.j=j+move[d].j;
           if(temp.i==exit_x&&temp.j==exit_y)
           {
               //如果忘记压入top ,会导致最后出现跳步
               PathStack.push(top);
               PathStack.push(temp);
               return;
           }
           if(Matrix[temp.i][temp.j].state==1&&recordMatrix[temp.i][temp.j].state==1)
           {
               recordMatrix[temp.i][temp.j].state=0;
               top.state=d;//记录当前尝试的路径方向（是左、下、右、上中的哪一个，下次回退时，不在从新开始，避免重复测试）
               PathStack.push(top);//将当前路径压入到路径栈中
               temp.state=0;
               PathStack.push(temp);
               break;
           }
           d++;
       }
   }
}
void maze::keyPressEvent(QKeyEvent *event)
{
    //qDebug()<<"Key is active!";
    if(!isPlay)
    {
        return;
    }
    //int step=0;
    //可以作为调节速度的接口
    //while(step<100000000)
    //while(step<1000)
    //{
    //   step++;
    //}
    switch(event->key())
    {
    case Qt::Key_W://上
        //上面为墙，则什么也不执行
        if(Matrix[x-1][y].state==0)
        {
            return;
        }
        x-=1;
        gLayout_Map->addWidget(People,x,y);
        People->setStyleSheet(MapStytle[StytleNum][3]);//显示向上移动图片
        break;
    case Qt::Key_S://下
        if(Matrix[x+1][y].state==0)
        {
           return;
        }
        x+=1;
        gLayout_Map->addWidget(People,x,y);
        People->setStyleSheet(MapStytle[StytleNum][0]);
        break;
    case Qt::Key_A://左
        if(Matrix[x][y-1].state==0)
        {
            return;
        }
        y-=1;
        gLayout_Map->addWidget(People,x,y);
        People->setStyleSheet(MapStytle[StytleNum][6]);
        //qDebug()<<"width "<<People->width()<<"height "<<People->height();
        break;
    case Qt::Key_D://右
        if(Matrix[x][y+1].state==0)
        {
            return;
        }
        y+=1;
        gLayout_Map->addWidget(People,x,y);
        People->setStyleSheet(MapStytle[StytleNum][9]);
        break;
    }
    if(x==exit_x&&y==exit_y)
    {
        QMessageBox message(QMessageBox::NoIcon, "恭喜你！", "进入迷宫下一层！");
        message.setIconPixmap(QPixmap(":/info/image/information/congratulation.png"));
        message.exec();//不加这个对话框会一闪而过
        CreateMaze_Layout();
        return;
    }
}
