#include "Dungeon.h"

Dungeon::Dungeon()
{
    Total_Floor=7;
   //开辟堆上三维数组
   map=new int **[Total_Floor];
   for(int i=0;i<Total_Floor;i++)
   {
       map[i]=new int *[12];
       for(int j=0;j<12;j++)
       {
           map[i][j]=new int[16];
           for(int k=0;k<16;k++)
           {
               map[i][j][k]=0;
           }
       }
   }
   initialMap();
   initialMapString();
   initialMonsterPic();
   initialenemy();
}
void Dungeon::initialrole()
{
    role.name="Mario";
    role.level=1;
    role.exp=0;
    role.hp=100;
    role.mp=50;
    role.atk=5;
    role.def=3;
    role.scoreNum=0;
    role.yellowkey=0;
    role.purplekey=0;
    role.redkey=0;
}

void Dungeon::initialenemy()
{
    enemy[0].name="小星星";//3
    enemy[0].hp=50;
    enemy[0].atk=2;
    enemy[0].def=2;
    enemy[0].exp=4;
    enemy[0].gold=5;

    enemy[1].name="毒蘑菇";//4
    enemy[1].hp=100;
    enemy[1].atk=6;
    enemy[1].def=4;
    enemy[1].exp=8;
    enemy[1].gold=10;

    enemy[2].name="绿鸭子";//11
    enemy[2].hp=200;
    enemy[2].atk=10;
    enemy[2].def=8;
    enemy[2].exp=16;
    enemy[2].gold=20;

    enemy[3].name="红鸭子";//12
    enemy[3].hp=300;
    enemy[3].atk=20;
    enemy[3].def=16;
    enemy[3].exp=32;
    enemy[3].gold=30;

    enemy[4].name="食人花-绿";//25
    enemy[4].hp=600;
    enemy[4].atk=30;
    enemy[4].def=24;
    enemy[4].exp=42;
    enemy[4].gold=40;

    enemy[5].name="食人花-红";//26
    enemy[5].hp=900;
    enemy[5].atk=42;
    enemy[5].def=18;
    enemy[5].exp=52;
    enemy[5].gold=46;

    enemy[6].name="飞行鸭";//27
    enemy[6].hp=1200;
    enemy[6].atk=80;
    enemy[6].def=12;
    enemy[6].exp=64;
    enemy[6].gold=60;

}

//动态贴图方式
void Dungeon::initialMapString()
{
    DungeonStytle[0][0]="border-image: url(:/DungeonNew/image/DungeonNew/0.png);";
    DungeonStytle[1][0]="border-image: url(:/DungeonNew/image/DungeonNew/1.png);";
    DungeonStytle[2][0]="border-image: url(:/DungeonNew/image/DungeonNew/2/20.png);";
    DungeonStytle[2][1]="border-image: url(:/DungeonNew/image/DungeonNew/2/21.png);";
    DungeonStytle[2][2]="border-image: url(:/DungeonNew/image/DungeonNew/2/22.png);";
    DungeonStytle[2][3]="border-image: url(:/DungeonNew/image/DungeonNew/2/23.png);";
    DungeonStytle[3][0]="border-image: url(:/DungeonNew/image/DungeonNew/3/30.png);";
    DungeonStytle[3][1]="border-image: url(:/DungeonNew/image/DungeonNew/3/31.png);";
    DungeonStytle[3][2]="border-image: url(:/DungeonNew/image/DungeonNew/3/32.png);";
    DungeonStytle[3][3]="border-image: url(:/DungeonNew/image/DungeonNew/3/33.png);";
    DungeonStytle[4][0]="border-image: url(:/DungeonNew/image/DungeonNew/4/40.png);";
    DungeonStytle[4][1]="border-image: url(:/DungeonNew/image/DungeonNew/4/41.png);";
    DungeonStytle[4][2]="border-image: url(:/DungeonNew/image/DungeonNew/4/42.png);";
    DungeonStytle[4][3]="border-image: url(:/DungeonNew/image/DungeonNew/4/43.png);";
    DungeonStytle[5][0]="border-image: url(:/DungeonNew/image/DungeonNew/5/50.png);";
    DungeonStytle[5][1]="border-image: url(:/DungeonNew/image/DungeonNew/5/51.png);";
    DungeonStytle[5][2]="border-image: url(:/DungeonNew/image/DungeonNew/5/52.png);";
    DungeonStytle[5][3]="border-image: url(:/DungeonNew/image/DungeonNew/5/53.png);";
    DungeonStytle[6][0]="border-image: url(:/DungeonNew/image/DungeonNew/6.png);";
    DungeonStytle[7][0]="border-image: url(:/DungeonNew/image/DungeonNew/7.png);";
    DungeonStytle[8][0]="border-image: url(:/DungeonNew/image/DungeonNew/8.png);";
    DungeonStytle[9][0]="border-image: url(:/DungeonNew/image/DungeonNew/9.png);";
    DungeonStytle[10][0]="border-image: url(:/DungeonNew/image/DungeonNew/10.png);";
    DungeonStytle[11][0]="border-image: url(:/DungeonNew/image/DungeonNew/11/110.png);";
    DungeonStytle[11][1]="border-image: url(:/DungeonNew/image/DungeonNew/11/111.png);";
    DungeonStytle[11][2]="border-image: url(:/DungeonNew/image/DungeonNew/11/112.png);";
    DungeonStytle[11][3]="border-image: url(:/DungeonNew/image/DungeonNew/11/113.png);";
    DungeonStytle[12][0]="border-image: url(:/DungeonNew/image/DungeonNew/12/120.png);";
    DungeonStytle[12][1]="border-image: url(:/DungeonNew/image/DungeonNew/12/121.png);";
    DungeonStytle[12][2]="border-image: url(:/DungeonNew/image/DungeonNew/12/122.png);";
    DungeonStytle[12][3]="border-image: url(:/DungeonNew/image/DungeonNew/12/123.png);";
    DungeonStytle[14][0]="border-image: url(:/DungeonNew/image/DungeonNew/14.png);";
    DungeonStytle[15][0]="border-image: url(:/DungeonNew/image/DungeonNew/15.png);";
    DungeonStytle[16][0]="border-image: url(:/DungeonNew/image/DungeonNew/16.png);";
    DungeonStytle[17][0]="border-image: url(:/DungeonNew/image/DungeonNew/17.png);";
    DungeonStytle[18][0]="border-image: url(:/DungeonNew/image/DungeonNew/18.png);";
    DungeonStytle[19][0]="border-image: url(:/DungeonNew/image/DungeonNew/19.png);";
    DungeonStytle[20][0]="border-image: url(:/DungeonNew/image/DungeonNew/20.png);";
    DungeonStytle[21][0]="border-image: url(:/DungeonNew/image/DungeonNew/21.png);";
    DungeonStytle[22][0]="border-image: url(:/DungeonNew/image/DungeonNew/22.png);";
    DungeonStytle[23][0]="border-image: url(:/DungeonNew/image/DungeonNew/23.png);";
    DungeonStytle[24][0]="border-image: url(:/DungeonNew/image/DungeonNew/24.png);";
    DungeonStytle[25][0]="border-image: url(:/DungeonNew/image/DungeonNew/25/250.png);";
    DungeonStytle[25][1]="border-image: url(:/DungeonNew/image/DungeonNew/25/251.png);";
    DungeonStytle[25][2]="border-image: url(:/DungeonNew/image/DungeonNew/25/252.png);";
    DungeonStytle[25][3]="border-image: url(:/DungeonNew/image/DungeonNew/25/253.png);";
    DungeonStytle[26][0]="border-image: url(:/DungeonNew/image/DungeonNew/26/260.png);";
    DungeonStytle[26][1]="border-image: url(:/DungeonNew/image/DungeonNew/26/261.png);";
    DungeonStytle[26][2]="border-image: url(:/DungeonNew/image/DungeonNew/26/262.png);";
    DungeonStytle[26][3]="border-image: url(:/DungeonNew/image/DungeonNew/26/263.png);";
    DungeonStytle[27][0]="border-image: url(:/DungeonNew/image/DungeonNew/27/270.png);";
    DungeonStytle[27][1]="border-image: url(:/DungeonNew/image/DungeonNew/27/271.png);";
    DungeonStytle[27][2]="border-image: url(:/DungeonNew/image/DungeonNew/27/272.png);";
    DungeonStytle[27][3]="border-image: url(:/DungeonNew/image/DungeonNew/27/273.png);";
    DungeonStytle[28][0]="border-image: url(:/DungeonNew/image/DungeonNew/28.png);";
    DungeonStytle[29][0]="border-image: url(:/DungeonNew/image/DungeonNew/29.png);";
    DungeonStytle[31][0]="border-image: url(:/DungeonNew/image/DungeonNew/31.png);";
    DungeonStytle[32][0]="border-image: url(:/DungeonNew/image/DungeonNew/32.png);";
    DungeonStytle[33][0]="border-image: url(:/DungeonNew/image/DungeonNew/33.png);";
    DungeonStytle[34][0]="border-image: url(:/DungeonNew/image/DungeonNew/34.png);";
    DungeonStytle[35][0]="border-image: url(:/DungeonNew/image/DungeonNew/35.png);";
    DungeonStytle[36][0]="border-image: url(:/DungeonNew/image/DungeonNew/36.png);";
    DungeonStytle[37][0]="border-image: url(:/DungeonNew/image/DungeonNew/Mario/up00.png);";
    DungeonStytle[38][0]="border-image: url(:/DungeonNew/image/DungeonNew/Mario/down00.png);";
    DungeonStytle[39][0]="border-image: url(:/DungeonNew/image/DungeonNew/Mario/left00.png);";
    DungeonStytle[39][1]="border-image: url(:/DungeonNew/image/DungeonNew/Mario/left01.png);";
    DungeonStytle[39][2]="border-image: url(:/DungeonNew/image/DungeonNew/Mario/left02.png);";
    DungeonStytle[39][3]="border-image: url(:/DungeonNew/image/DungeonNew/Mario/left03.png);";
    DungeonStytle[40][0]="border-image: url(:/DungeonNew/image/DungeonNew/Mario/right00.png);";
    DungeonStytle[40][1]="border-image: url(:/DungeonNew/image/DungeonNew/Mario/right01.png);";
    DungeonStytle[40][2]="border-image: url(:/DungeonNew/image/DungeonNew/Mario/right02.png);";
    DungeonStytle[40][3]="border-image: url(:/DungeonNew/image/DungeonNew/Mario/right03.png);";
}


void Dungeon::initialMonsterPic()
{
    MonsterPic[0]="border-image: url(:/info/image/information/start.png);";
    MonsterPic[1]="border-image: url(:/info/image/information/badmushroom.png);";
    MonsterPic[2]="border-image: url(:/info/image/information/greenDuck.png);";
    MonsterPic[3]="border-image: url(:/info/image/information/flyDuck.png);";
    MonsterPic[4]="border-image: url(:/info/image/information/绿色食人花.png);";
    MonsterPic[5]="border-image: url(:/info/image/information/红色食人花.png);";
    MonsterPic[6]="border-image: url(:/info/image/information/飞行鸭.png);";
}

int Dungeon::pk(struct Monster & m)
{
    //计算伤害值，-1表示打不动对方防御，-2表示双方都打不动对方防御
    if (role.atk <= m.def)
    {
        if (m.atk > role.def)
            return -1;//打不过
        else
            return -2;//怪物防御太高，先增加你的攻击力吧
    }
    else//人物的攻击力大于怪物的防御，但是依然打不过的情况
    {
        if(m.atk>role.def)
        {
            int i=0;
            int role_hp=role.hp;
            int enemy_hp=m.hp;
            while(role_hp>0)
            {
               role_hp=role.hp - (m.atk - role.def)*i;
               enemy_hp=m.hp - (role.atk - m.def) * i;
               i++;
            }
            if(enemy_hp>=0)
            {
                return 1;
            }
        }
    }
    return 0;
}
int Dungeon::calc_damage(int MonsterNum)
{
    if (enemy[MonsterNum].atk > role.def)
    {
        int atk_times = (enemy[MonsterNum].hp - 1) / (role.atk - enemy[MonsterNum].def);//怪物攻击的次数
        return atk_times * (enemy[MonsterNum].atk - role.def);//造成的伤害
    }
    else
    {
        return 0;//怪物破不了我们的防，造成的伤害为0
    }

}
void Dungeon::initialMap()
{
    floor=1;
    MAX_HP=100;
    isPre=false;
    isNext=false;
    x=11;
    y=8;
    initialrole();
    int num=1;
    while(1)
    {

        if(num==1)
        {
            int tempmap[12][16]={
                15,1,1,3,3,3,1,1,1,1,1,1,1,1,1,1,
                0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
                0,8,1,1,1,3,17,1,17,1,1,22,2,5,0,1,
                0,1,1,11,1,1,0,1,0,1,1,23,2,20,0,1,
                0,0,0,17,0,0,0,1,0,0,0,0,17,0,0,1,
                0,20,1,1,1,1,0,1,0,1,3,2,4,1,0,1,
                0,20,1,11,1,1,0,1,17,3,2,4,10,1,0,1,
                0,0,0,17,0,0,0,1,0,0,0,0,0,0,0,1,
                0,1,1,2,1,1,0,1,1,1,1,1,1,1,1,1,
                0,8,2,2,2,5,0,0,17,0,0,0,17,0,0,0,
                0,8,2,2,2,5,0,20,2,2,0,1,1,4,1,0,
                0,2,2,2,2,2,0,2,1,2,0,1,3,10,3,0

            };
            for(int i=0;i<12;i++)
            {
                for(int j=0;j<16;j++)
                {
                    map[num-1][i][j]=tempmap[i][j];
                }
            }
        }
       if(num==2)
        {
           int tempmap[12][16]={
               14,1,18,1,1,1,1,1,1,1,1,1,1,1,1,0,
               1,1,0,0,0,0,19,4,20,4,21,0,0,0,0,0,
               1,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,
               1,0,20,20,5,5,0,1,1,1,0,12,5,2,2,0,
               1,0,19,20,5,5,16,1,1,1,16,12,5,2,19,0,
               1,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,
               1,16,5,5,12,5,0,1,1,1,0,12,8,19,2,0,
               1,0,5,5,12,19,0,1,1,1,0,12,9,20,22,0,
               1,0,0,0,0,0,0,1,1,1,16,12,10,21,23,0,
               1,0,19,2,2,2,0,1,1,1,0,0,0,0,0,0,
               1,0,5,5,10,10,0,1,1,1,0,2,2,2,2,0,
               15,0,5,5,10,12,16,1,1,1,16,2,2,2,2,0
           };
           for(int i=0;i<12;i++)
           {
               for(int j=0;j<16;j++)
               {
                   map[num-1][i][j]=tempmap[i][j];
               }
           }
        }
       if(num==3)
       {
           int tempmap[12][16]={
               20,1,1,22,0,20,1,1,20,0,1,0,1,1,2,1,
               1,1,2,1,0,1,20,20,1,0,1,0,1,2,8,2,
               1,2,8,2,0,1,1,21,1,0,1,17,4,4,2,1,
               4,1,4,1,0,20,1,1,20,0,1,0,0,0,0,0,
               0,0,17,0,0,0,1,1,0,0,1,0,1,1,1,1,
               1,1,1,1,1,3,1,1,1,1,3,1,1,1,1,1,
               0,0,17,0,0,1,1,1,1,0,1,0,0,0,0,0,
               1,1,11,1,0,0,1,1,0,0,1,0,2,2,2,20,
               1,4,3,20,0,1,1,1,1,0,1,17,4,4,3,8,
               1,1,3,23,0,1,1,1,1,0,1,0,0,0,0,0,
               0,0,0,0,0,0,1,1,0,0,3,0,1,1,1,1,
               14,1,1,1,1,1,1,1,1,0,1,17,1,1,1,15,
           };

           for(int i=0;i<12;i++)
           {
               for(int j=0;j<16;j++)
               {
                   map[num-1][i][j]=tempmap[i][j];
               }
           }
       }
       if(num==4)
       {
           int tempmap[12][16]={
               1,21,1,1,0,1,31,32,33,1,0,1,1,1,1,1,
               8,2,20,1,0,1,34,35,36,1,0,1,1,1,1,1,
               1,3,1,1,0,1,1,1,1,1,0,1,20,1,10,1,
               1,1,1,1,0,1,1,1,1,1,0,1,1,11,1,1,
               0,0,17,0,0,0,0,18,0,0,0,0,17,0,0,0,
               1,1,4,1,17,1,1,3,1,1,1,11,1,1,1,1,
               1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,
               3,1,1,1,3,1,1,1,1,1,1,1,1,1,1,1,
               17,0,0,0,17,0,0,0,0,17,0,0,0,0,0,17,
               1,0,1,20,4,1,0,1,1,4,1,1,1,1,0,1,
               1,0,1,3,1,3,0,1,23,2,8,1,1,1,0,1,
               15,0,20,1,20,1,0,1,1,3,1,1,1,1,0,14,
           };
           for(int i=0;i<12;i++)
           {
               for(int j=0;j<16;j++)
               {
                   map[num-1][i][j]=tempmap[i][j];
               }
           }
       }
       if(num==5)
       {
           int tempmap[12][16]={
               15,0,1,1,3,17,1,0,1,4,1,17,1,1,17,1,
               1,0,1,1,1,0,20,0,4,2,4,0,1,1,0,1,
               1,17,4,1,1,0,1,0,2,4,2,0,3,3,0,3,
               0,0,0,0,17,0,3,0,2,2,2,0,20,20,0,1,
               20,1,4,1,1,0,1,0,22,23,19,0,20,20,0,1,
               20,1,1,1,4,0,4,0,0,0,0,0,0,0,0,1,
               0,0,12,0,0,0,1,1,3,1,1,1,1,1,1,1,
               1,1,12,1,1,0,3,0,0,0,0,0,0,0,0,3,
               1,1,4,1,1,0,1,0,1,1,1,1,1,1,1,1,
               22,23,20,8,10,0,1,0,1,1,1,0,0,0,0,0,
               0,0,0,0,0,0,1,0,1,1,1,0,1,1,1,1,
               14,1,1,1,1,1,1,0,1,1,1,16,1,1,1,24,
           };
           for(int i=0;i<12;i++)
           {
               for(int j=0;j<16;j++)
               {
                   map[num-1][i][j]=tempmap[i][j];
               }
           }
       }
       if(num==6)
       {
           int tempmap[12][16]={
               14,0,20,20,2,0,1,1,1,1,1,1,1,1,1,1,
               1,0,20,20,2,0,1,0,0,0,0,0,0,0,0,17,
               1,0,0,0,4,0,1,0,25,8,2,1,2,25,1,25,
               1,17,17,1,1,17,1,0,5,1,2,25,1,2,1,1,
               1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,
               1,1,1,11,4,1,20,1,1,1,25,25,1,1,1,1,
               0,0,0,0,0,0,1,0,0,0,0,0,0,17,0,1,
               2,1,1,4,8,0,1,0,10,2,0,0,1,4,0,1,
               2,1,11,23,12,0,1,0,9,2,0,0,5,1,0,1,
               4,2,1,4,1,0,1,0,8,2,26,2,4,2,0,1,
               17,0,0,0,0,0,1,0,0,0,0,1,2,1,0,1,
               1,1,3,1,12,1,1,0,1,1,1,19,1,1,0,15,
           };
           for(int i=0;i<12;i++)
           {
               for(int j=0;j<16;j++)
               {
                   map[num-1][i][j]=tempmap[i][j];
               }
           }
       }
       if(num==7)
       {
           int tempmap[12][16]={
               15,0,27,29,0,22,0,1,1,1,0,10,0,20,0,3,
               1,0,8,27,0,23,0,1,1,1,0,4,0,20,0,3,
               1,0,8,27,0,4,0,3,0,26,0,3,0,8,0,3,
               1,0,2,8,0,2,0,1,0,1,0,1,0,2,0,1,
               1,0,2,10,0,1,0,1,0,1,0,1,0,1,0,1,
               17,0,25,25,0,17,0,18,0,17,0,17,0,25,0,17,
               1,25,1,1,12,1,1,1,1,1,1,1,1,1,1,1,
               17,0,25,25,0,17,0,17,0,17,0,17,0,25,0,17,
               1,0,2,2,0,1,0,1,0,1,0,1,0,1,0,1,
               3,0,8,10,0,2,0,12,0,4,0,3,0,10,0,1,
               3,0,8,27,0,2,0,22,0,20,0,4,0,20,0,1,
               3,0,8,28,0,2,0,23,0,20,0,10,0,20,0,14,
           };
           for(int i=0;i<12;i++)
           {
               for(int j=0;j<16;j++)
               {
                   map[num-1][i][j]=tempmap[i][j];
               }
           }
       }
       num++;
       if(num>Total_Floor)
       {
           break;
       }
    }
}
void Dungeon::initialCharacterPos()
{
    int tempx=0;
    int tempy=0;
    //floor-1 代表当前层

    if(isPre)
    {

        for(int i=0;i<12;i++)
        {
            for(int j=0;j<16;j++)
            {
                if(map[floor-2][i][j]==15)
                {
                    tempx=i;
                    tempy=j;
                    break;//一次break跳一层循环
                }
            }
        }
        if(tempx<11&&map[floor-2][tempx+1][tempy]==1)
        {

            x=tempx+1;
            y=tempy;
        }
        else if(tempx>0&&map[floor-2][tempx-1][tempy]==1)
        {
            x=tempx-1;
            y=tempy;
        }
        else if(tempy<15&&map[floor-2][tempx][tempy+1]==1)
        {

            x=tempx;
            y=tempy+1;
        }
        else if(tempy>0&&map[floor-2][tempx][tempy-1]==1)
        {
            x=tempx;
            y=tempy-1;
        }
        else
        {
            x=y=0;
        }

        isPre=false;
    }
    if(isNext)
    {
        for(int i=0;i<12;i++)
        {
            for(int j=0;j<16;j++)
            {
                if(map[floor][i][j]==14)
                {
                    tempx=i;
                    tempy=j;
                    break;
                }
            }
        }
        if(tempx<11&&map[floor][tempx+1][tempy]==1)
        {

            x=tempx+1;
            y=tempy;
        }
        else if(tempx>0&&map[floor][tempx-1][tempy]==1)
        {
            x=tempx-1;
            y=tempy;
        }
        else if(tempy<15&&map[floor][tempx][tempy+1]==1)
        {
            x=tempx;
            y=tempy+1;
        }
        else if(tempy>0&&map[floor][tempx][tempy-1]==1)
        {
            x=tempx;
            y=tempy-1;
        }
        else
        {
            x=y=0;
        }
        isNext=false;
    }
}
