#ifndef MAZE_H
#define MAZE_H

#include <QStack>
#include <QGridLayout>
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>
#include <QMainWindow>
#include <QKeyEvent>
#include "factory.h"
#include "interface.h"
extern QMainWindow *ptr_MianWindow;
extern interface* surface;
extern QGridLayout*gLayout_Map;
extern QWidget *MazeWidget[60][60];
extern QWidget *People;
extern bool isShow;
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

class maze : public QWidget
{
    Q_OBJECT
public:
    explicit maze(QWidget *parent = 0);
    QWidget *Controlwidget;//控制界面
private:
    int x;//角色位置
    int y;
    int exit_x;//出口位置
    int exit_y;
    int height;
    int width;
    int last_height;
    bool isPlay;//键盘操作标识（用于限制操作时自动寻路和AI）
    point **Matrix;
    QStack<point> PathStack;//存放自动寻路的过程(可能包括回头路)
    point **recordMatrix;
    QStack<point> *MazeStack;
    point move[4];
    QString MapStytle[4][22];//地图风格字符串
    int StytleNum;//地图风格数字
    int BASIC_WIDTH;//地图窗口宽度
    int BASIC_HEIGHT;//地图窗口高度
    int iNum;//自动寻路显示路径辅助变量
    int lastheight;//上一次设置的迷宫高度
    int lastwidth;//上一次设置的迷宫宫宽度
    bool isAIAnimationButton;//AI移动标识
    bool isAutoMoveButton;//自动寻路标识
    QGridLayout *gLayout_Control;//控制界面网格布局
    QSpinBox *sp_w;//迷宫大小输入
    QSpinBox *sp_h;
    QLabel *label_Stytle;//地图风格显示
    QComboBox *SelectMapStytle;//地图风格选择
    QPushButton *AIAnimationButton;//AI操作
    QPushButton *AutoMoveButton;//自动寻路
    QPushButton *quitButton;//返回主菜单按钮
    QSequentialAnimationGroup *group;//AI操作动画组
    Factory* fac;
    QTimer *timer;//定时器
public:
    ~maze();
    void initialControlWidget();
    void keyPressEvent(QKeyEvent *event);
private:
    void initialMaze();
    void initialMapStytle();//初始化地图风格字符串
    void CreateMaze();
    void autoFindPath();
    void hideMaze();//隐藏迷宫地图
    void resetMaze();//重置迷宫
    void setDirectionFalse(bool &up,bool &down,bool &left,bool &right);
    bool isOdd(int num);
private slots:
    void CreateMaze_Layout();
    void ShowMaze_Layout();
    void timeStart();
    void ShowAnimation();
    void moveCharacter();//AI移动时人物图片切换
    void ShowPath();
    void quit();
    void MapStytleSet();//设置地图风格
};

#endif // MAZE_H
