#include "maze.h"
#include <QTime>
#include <QDebug>
point::point(){}

point::point(int _i,int _j,int _state):i(_i),j(_j),state(_state)
{

}
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

maze::maze()
{
   recordMatrix=nullptr;
   Matrix=nullptr;
   height=0;
   width=0;
   x=y=0;
   exit_x=exit_y=0;
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

void maze::initialMaze(int h, int w)
{
    MazeStack->clear();
    height=h;
    width=w;
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
    {
        for(int j=0;j<width;j++)
        {
            Matrix[i][j].state=0;
            recordMatrix[i][j].state=1;
        }
    }
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
void maze::setDirectionFalse(bool &up,bool &down,bool &left,bool &right)
{
    up=down=left=right=false;
}

void maze::CreateMaze()
{
    int i=3,j=3;
    Matrix[i][j].state=1;
    point temp;
    temp.state=1;
    bool up=false,down=false,left=false,right=false;
    while(true)
    {
        temp.i=i;
        temp.j=j;
        //构造函数中生成的变量，在类中其他函数中也可以调用
        int randNum=qrand()%4;//randNum是随机的，所以每次生成的迷宫也是随机的
        switch(randNum)
        {
        case 0://up
            if(!up&&i>2&&Matrix[i-2][j].state==0)
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
void maze::setCharacterPos()
{
    //随机设定方式
//   point temp;
//   QStack<point> s;
//   for(int i=0;i<height;i++)
//   {
//       for(int j=0;j<width;j++)
//       {
//           if(Matrix[i][j].state==1)
//           {
//               temp.i=i;
//               temp.j=j;
//               s.push(temp);
//           }
//       }
//   }
//   int randNum=qrand()%s.size();
//   x=s[randNum].i;
//   y=s[randNum].j;
   x=3;
   y=3;
}
void maze::setExitPos()
{
    //随机设定方式
//    point temp;
//    QStack<point> s;
//    for(int i=0;i<height;i++)
//    {
//        for(int j=0;j<width;j++)
//        {
//            if(Matrix[i][j].state==1)
//            {
//                temp.i=i;
//                temp.j=j;
//                s.push(temp);
//            }
//        }
//    }
//    while(true)
//    {
//        int randNum=qrand()%s.size();
//        exit_x=s[randNum].i;
//        exit_y=s[randNum].j;
//        if(exit_x!=x||exit_y!=y)
//        {
//            break;
//        }
//    }
    exit_x=height-2;
    exit_y=width-2;
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
               top.state=d;
               PathStack.push(top);
               temp.state=0;
               PathStack.push(temp);
               break;
           }
           d++;
       }
   }
}
