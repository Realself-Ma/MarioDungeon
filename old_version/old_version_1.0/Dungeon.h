#ifndef DUNGEON_H
#define DUNGEON_H
#include <QString>
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

class Dungeon
{
public:
    Dungeon();
    void initialMap();
    void initialCharacterPos();
    int pk(struct Monster &m);
    int calc_damage(int MonsterNum);
private:
    void initialMapString();
    void initialMonsterPic();//初始化怪物图片（用于战斗界面）
    void initialrole();
    void initialenemy();
public:
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
};

#endif // DUNGEON_H
