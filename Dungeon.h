#ifndef DUNGEON_H
#define DUNGEON_H
#include <QGridLayout>
#include <QMediaPlaylist>
#include <QKeyEvent>
#include <QString>
#include "interface.h"
#include "factory.h"
#include "storewidget.h"
#include "sound.h"
extern QMainWindow *ptr_MianWindow;
extern interface* surface;
extern QGridLayout*gLayout_Map;
extern QWidget *MazeWidget[60][60];
extern QWidget *People;
extern bool isShow;
struct character
{
    QString name;
    int level;
    int exp;
    int hp;
    int mp;
    int atk;
    int def;
    int scoreNum;
    int yellowkey;
    int purplekey;
    int redkey;
};
struct Monster
{
    QString name;
    int hp;
    int atk;
    int def;
    int exp;
    int gold;
};

class Dungeon: public QWidget
{
    Q_OBJECT
public:
    explicit Dungeon(QWidget *parent = 0);
    ~Dungeon();
    QWidget *infoWidget;//状态显示界面
    void initialDugeon();
    void keyPressEvent(QKeyEvent *event);
private:
    void initialMap();
    void initialCharacterPos();
    int pk(Monster &m);
    int calc_damage(int MonsterNum);
    void initialinfoWidget();//初始化状态界面
    void setCharacterPos(int floor,int target);
    void initialMapString();
    void initialMonsterPic();//初始化怪物图片（用于战斗界面）
    void initialrole();
    void initialenemy();
    void ShowDungeon();//显示地牢
    void hideDungeon();
    void initialSetNameWin();
    void initialGetitemWin();//初始化获得物品信息界面
    void initialFightWin();//初始化战斗界面
    void updateStatusData();//更新状态数据
    bool isDynamic(int i,int j);
    bool isStatic(int num);
    void Move();//人物移动
    void Operation(int num);//移动操作
    void changeHP(int num);
    void QuestionBox(int num);//问号箱
    void items(int _itemNum, int _moveDirection);//物品
    void doors(int _doorNum, int _moveDirection);
    void Monsters(int _MonsterNum,int _moveDirection);
    void checkPrefloor();//上一层
    void checkNextfloor();//下一层
private://获得物品信息栏
    QLabel *GetitemsShow;
    QTimer *GetitemTimer;
    int GetitemNum;
    int itemNum;
public://设置角色名界面
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
private://按钮
    QPushButton *ReturnMainMenu;
    QPushButton *Restart;
private://动态效果
    QTimer *dynamicEffectTimer;//动态效果计时器
    int display_it;
private://音效
    Sound* music;
private slots:
    void GetitemWinshow();//获得物品信息界面显示
    void FightWinshow();//战斗界面显示
    void ShowdynamicEffect();//展示动态效果
    void OpenStore();//打开商店

    void ReturnMainMenuPlay();//返回主菜单
    void RestartPlay();//重新开始
    void EnterDungeon();//角色名设置完成,进入地牢
private:
    character role;
    Monster enemy[10];
    QString DungeonStytle[41][4];
    QString MonsterPic[20];//怪物图片字符串
    int floor;
    int Total_Floor;
    int ***map;//三维数组
    int x;//角色位置
    int y;
    int MAX_HP;
    bool isPre;
    bool isNext;
private:
    int moveDirection;//移动方向
    int moveNum;
private:
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
    StoreWidget *Store;
    Factory* fac;
};

#endif // DUNGEON_H
