#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QWidget>
#include <QSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QLineEdit>
#include <QKeyEvent>
#include <QResizeEvent>//QResizeEvent 是主窗口的变化事件
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>
#include <QTimer>//计时器   QTime 只是用来产生随机数的
#include <QMediaPlayer>//视频、音乐类
#include <QMediaPlaylist>
#include <QPoint>
#include <QProgressBar>
#include "maze.h"
#include "Dungeon.h"
#include "interface.h"
#include "storewidget.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
/**********************   主界面     ***********************/
private:
    int BASIC_WIDTH;//地图窗口宽度
    int BASIC_HEIGHT;//地图窗口高度
    QPoint windowPos;//窗口位置
    QPoint mousePos;//鼠标位置
    QPoint dPos;//鼠标移动后的位置
private:
    interface *surface;//开始界面
    QWidget *MainWidget;//中心窗口
    QWidget *MazeWidget[60][60];//地图窗口数组
    QWidget *People;//角色
    QString character[12];//角色图片字符串
    QGridLayout *gLayout_Map;//地图网格布局管理器
    QHBoxLayout *hLayout;//主界面水平布局管理器
private:
    void initialWindow_Layout();//初始化主窗口
    void keyPressEvent(QKeyEvent *event);//键盘事件（用于操作）
    void mousePressEvent(QMouseEvent *event);//鼠标按下事件（用于移动主界面）
    void mouseMoveEvent(QMouseEvent *event);//鼠标移动事件
private slots:
    void ShowWidget();//显示界面
/**********************   迷宫模式     ***********************/
private:
    int iNum;//自动寻路显示路径辅助变量
    int lastheight;//上一次设置的迷宫高度
    int lastwidth;//上一次设置的迷宫宫宽度
    int StytleNum;//地图风格数字
    bool isPlay;//键盘操作标识（用于限制操作时自动寻路和AI）
    bool isShow;//显示地图标识（用于刷新迷宫地图时销毁上次生成的窗口）
    bool issurface;//开始界面标识（用于标识开始界面已加载完成）
    bool isAIAnimationButton;//AI移动标识
    bool isAutoMoveButton;//自动寻路标识
private:
    maze m;//迷宫类
    QWidget *Controlwidget;//控制界面
    QGridLayout *gLayout_Control;//控制界面网格布局
    QSpinBox *sp_w;//迷宫大小输入
    QSpinBox *sp_h;
    QLabel *label_Stytle;//地图风格显示
    QComboBox *SelectMapStytle;//地图风格选择
    QPushButton *AIAnimationButton;//AI操作
    QPushButton *AutoMoveButton;//自动寻路
    QPushButton *quitButton;//返回主菜单按钮
    QSequentialAnimationGroup *group;//AI操作动画组
    QTimer *timer;//定时器
private:
    void initialControlWidget();//初始化控制界面
    void ShowMaze_Layout();//显示迷宫地图
    void hideMaze();//隐藏迷宫地图
    void resetMaze();//重置迷宫
private slots:
    void moveCharacter();//AI移动时人物图片切换
    void CreateMaze_Layout();//创建迷宫
    void ShowPath();//自动寻路显示路径
    void ShowAnimation();//显示动画
    void timeStart();//定时器开启
    void MapStytleSet();//设置地图风格
    void quit();//返回主菜单
/**********************   地牢模式     ***********************/
private:
    Dungeon d;//地牢类
    QWidget *infoWidget;//状态显示界面
    int moveDirection;//移动方向
    int moveNum;
    QLabel *Floor;//地牢层数
    QLabel *level;//人物状态信息
    QLabel *exp;
    QLabel *hp;
    QLabel *mp;
    QLabel *atk;
    QLabel *def;
    QLabel *score;//金币
    QLabel *yellowkeyNum;//钥匙信息
    QLabel *purplekeyNum;
    QLabel *redkeyNum;
    QLabel *CharacterName;
private:
    void initialinfoWidget();//初始化状态界面
    void initialGetitemWin();//初始化获得物品信息界面
    void initialFightWin();//初始化战斗界面
    void initialSetNameWin();//初始化设置角色名界面
    void ShowDungeon();//显示地牢
    void hideDungeon();//隐藏地牢
    void updateStatusData();//更新状态数据
    void Move();//人物移动
    void changeHP(int num);
    void QuestionBox(int num);//问号箱
    void items(int _itemNum, int _moveDirection);//物品
    void doors(int _doorNum, int _moveDirection);
    void Monsters(int _MonsterNum,int _moveDirection);
    void checkPrefloor();//上一层
    void checkNextfloor();//下一层
    bool isDynamic(int i,int j);
    void soundPlay(int CurrentIndex);
private slots:
    void GetitemWinshow();//获得物品信息界面显示
    void FightWinshow();//战斗界面显示
    void ShowdynamicEffect();//展示动态效果
    void OpenStore();//打开商店
    void ReturnMainMenuPlay();//返回主菜单
    void RestartPlay();//重新开始
    void soundStop();//音效停止
    void CheckBGMstate();//检查音乐状态
    void EnterDungeon();//角色名设置完成,进入地牢
private://获得物品信息栏
    QLabel *GetitemsShow;
    QTimer *GetitemTimer;
    int GetitemNum;
    int itemNum;
private://设置角色名界面
    QLineEdit *NameEdit;
    QPushButton *SetDone;
private://战斗界面
    QWidget *FightWidget;
    QTimer *FightTimer;
    bool isFighting;
    QLabel *Monster_pic;//怪物信息标签
    QLabel *Monster_name;
    QLabel *Monster_hpText;
    QLabel *Monster_hp;
    QLabel *Monster_atkText;
    QLabel *Monster_atk;
    QLabel *Monster_defText;
    QLabel *Monster_def;

    QLabel *Character_pic;//人物信息标签
    QLabel *Character_name;
    QLabel *Character_hpText;
    QLabel *Character_hp;
    QLabel *Character_atkText;
    QLabel *Character_atk;
    QLabel *Character_defText;
    QLabel *Character_def;

    int MonsterNum;//怪物编号
    int fight_period_it;//战斗次数
    int fight_end_it;//战斗结束标识
private://商店界面
    StoreWidget *Store;
private://按钮
    QPushButton *ReturnMainMenu;
    QPushButton *Restart;
private://动态效果
    QTimer *dynamicEffectTimer;//动态效果计时器
    int display_it;
private://音效
    QMediaPlayer *BGM;
    QTimer *BGMTimer;
    QMediaPlaylist *soundlist;
    QMediaPlayer *sound;
    QTimer *soundTimer;
};

#endif // MAINWINDOW_H
