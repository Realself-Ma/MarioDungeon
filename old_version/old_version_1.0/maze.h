#ifndef MAZE_H
#define MAZE_H

#include <QStack>

class point
{
public:
    point();
    point(int _i,int _j,int _state);
    int i;
    int j;
    int state;
    bool operator==(const point &p);
};

class maze
{
private:
    point **recordMatrix;
    QStack<point> *MazeStack;
    point move[4];
public:
    int height;
    int width;
    int last_height;
    int x;//角色位置
    int y;
    int exit_x;//出口位置
    int exit_y;
    point **Matrix;
    QStack<point> PathStack;//存放自动寻路的过程(可能包括回头路)
public:
    QString MapStytle[4][22];//地图风格字符串
public:
    maze();
    void initialMaze(int h,int w);
    void initialMapStytle();//初始化地图风格字符串
    void CreateMaze();
    void autoFindPath();
    void setCharacterPos();//设置角色位置
    void setExitPos();
private:
    void setDirectionFalse(bool &up,bool &down,bool &left,bool &right);
};

#endif // MAZE_H
