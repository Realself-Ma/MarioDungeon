#include "Dungeon.h"
#include <QDebug>
Dungeon::Dungeon(QWidget *parent): QWidget(parent)
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
    initialrole();
    initialenemy();

    GetitemNum=0;//获得物品信息栏初始化
    itemNum=0;
    MonsterNum=0;
    fight_period_it = 0;//战斗状态信息栏初始化
    fight_end_it = 0;
    moveDirection=-1;
    moveNum=0;
    isFighting=false;
    display_it=0;

    fac=new Factory();
    music=new Sound();
}
Dungeon::~Dungeon(){}
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
    for(int i=0;i<41;++i)
    {
        for(int j=0;j<4;++j)
        {
            if(isStatic(i))
            {
                DungeonStytle[i][j]=QString("border-image: url(:/DungeonNew/image/DungeonNew/")+
                        QString::number(i)+QString(".png);");
                break;
            }
            else
                DungeonStytle[i][j]=QString("border-image: url(:/DungeonNew/image/DungeonNew/")+
                        QString::number(i)+"/"+QString::number(i)+QString::number(j)+QString(".png);");
        }
    }
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
void Dungeon::setCharacterPos(int floor,int target)
{
    bool findflag=false;
    int i,j;
    for(i=0;i<12;i++)
    {
        for(j=0;j<16;j++)
        {
            if(map[floor][i][j]==target)
            {
                findflag=true;
                break;
            }
        }
        if(findflag)
            break;
    }
    if(i<11&&map[floor][i+1][j]==1)
    {

        x=i+1;
        y=j;
    }
    else if(i>0&&map[floor][i-1][j]==1)
    {
        x=i-1;
        y=j;
    }
    else if(j<15&&map[floor][i][j+1]==1)
    {

        x=i;
        y=j+1;
    }
    else if(j>0&&map[floor][i][j-1]==1)
    {
        x=i;
        y=j-1;
    }
    else
    {
        x=y=0;
    }
}

void Dungeon::initialCharacterPos()
{
    if(isPre)
    {
        setCharacterPos(floor-2,15);
        isPre=false;
    }
    if(isNext)
    {
        setCharacterPos(floor,14);
        isNext=false;
    }
}
void Dungeon::initialDugeon()
{
    Store=new StoreWidget(ptr_MianWindow);//StoreWidget只是一个类，要想有主窗口，类中必须自己定义一个
    Store->setGeometry(292,192,256,352);
    Store->hide();

    initialinfoWidget();
    initialFightWin();//希望显示在MainWidget之上的窗口，要在初始化MainWidget后再初始化
    initialSetNameWin();
    initialGetitemWin();

    GetitemTimer=new QTimer(this);
    FightTimer=new QTimer(this);
    dynamicEffectTimer=new QTimer(this);

    connect(GetitemTimer,SIGNAL(timeout()),this,SLOT(GetitemWinshow()));
    connect(FightTimer,SIGNAL(timeout()),this,SLOT(FightWinshow()));
    connect(Store->haveBuyTimer,SIGNAL(timeout()),this,SLOT(OpenStore()));
    connect(dynamicEffectTimer,SIGNAL(timeout()),this,SLOT(ShowdynamicEffect()));
}

void Dungeon::initialinfoWidget()
{
    infoWidget=new QWidget(ptr_MianWindow);
    QFont fontLabel("Microsoft YaHei" ,12, 75);
    QFont fontNum("Microsoft YaHei" ,10, 65);
    QFont fontName("Microsoft YaHei" ,10, 55);
    QString labelbg_style="border-image: url(:/interface/image/interface/labelbg.png);";
    QString CharacterPic_style="border-image: url(:/info/image/information/Character.png);";
    QString yellowkey_style="border-image: url(:/info/image/information/yellowkey.png);";
    QString purplekey_style="border-image: url(:/info/image/information/purplekey.png);";
    QString redkey_style="border-image: url(:/info/image/information/redkey.png);";
    QString button_style="QPushButton{border-image:url(:/interface/image/interface/labelbg.png);color:white;border-radius:10px;}"
                         "QPushButton:hover{border-image:url(:/interface/image/interface/PushButtonhoverbg.png); color: black;}"
                         "QPushButton:pressed{border-image:url(:/interface/image/interface/PushButtonPressbg.png);}";


    QLabel *CharacterPic=fac->CreateQLabel(infoWidget,20,20,40,40,"",CharacterPic_style);
    QLabel *label_Name=fac->CreateQLabel(infoWidget,20,70,100,30,"人们叫他",labelbg_style,fontLabel,Qt::AlignLeft);
    CharacterName=fac->CreateQLabel(infoWidget,130,70,170,30,"",labelbg_style,fontName,Qt::AlignLeft);
    QLabel *label_level=fac->CreateQLabel(infoWidget,140,20,40,40,"级",labelbg_style,fontLabel);
    QLabel *label_hp=fac->CreateQLabel(infoWidget,20,110,65,30,"生命",labelbg_style,fontLabel,Qt::AlignLeft);
    QLabel *label_mp=fac->CreateQLabel(infoWidget,20,150,65,30,"魔法",labelbg_style,fontLabel,Qt::AlignLeft);
    QLabel *label_atk=fac->CreateQLabel(infoWidget,20,230,65,30,"攻击",labelbg_style,fontLabel,Qt::AlignLeft);
    QLabel *label_def=fac->CreateQLabel(infoWidget,20,270,65,30,"防御",labelbg_style,fontLabel,Qt::AlignLeft);
    QLabel *label_score=fac->CreateQLabel(infoWidget,20,310,65,30,"金币",labelbg_style,fontLabel,Qt::AlignLeft);
    QLabel *label_exp=fac->CreateQLabel(infoWidget,20,190,65,30,"经验",labelbg_style,fontLabel,Qt::AlignLeft);
    QLabel *label_yellowkey=fac->CreateQLabel(infoWidget,20,370,40,40,"",yellowkey_style);
    QLabel *label_purplekey=fac->CreateQLabel(infoWidget,20,420,40,40,"",purplekey_style);
    QLabel *label_redkey=fac->CreateQLabel(infoWidget,20,470,40,40,"",redkey_style);
    QLabel *label_floor1=fac->CreateQLabel(infoWidget,20,520,40,40,"第",labelbg_style,fontLabel);
    QLabel *label_floor2=fac->CreateQLabel(infoWidget,140,520,40,40,"层",labelbg_style,fontLabel);

    ReturnMainMenu=fac->CreateQPushButton(infoWidget,140,580,120,30,"返回主菜单",button_style,fontLabel);
    Restart=fac->CreateQPushButton(infoWidget,140,620,100,30,"重新开始",button_style,fontLabel);

    level=fac->CreateQLabel(infoWidget,80,20,40,40,QString::number(role.level),labelbg_style,fontNum);
    hp=fac->CreateQLabel(infoWidget,105,110,100,30,QString::number(role.hp),labelbg_style,fontNum);
    mp=fac->CreateQLabel(infoWidget,105,150,100,30,QString::number(role.mp),labelbg_style,fontNum);
    atk=fac->CreateQLabel(infoWidget,105,230,100,30,QString::number(role.atk),labelbg_style,fontNum);
    def=fac->CreateQLabel(infoWidget,105,270,100,30,QString::number(role.def),labelbg_style,fontNum);
    score=fac->CreateQLabel(infoWidget,105,310,100,30,QString::number(role.scoreNum),labelbg_style,fontNum);
    exp=fac->CreateQLabel(infoWidget,105,190,100,30,QString::number(role.exp),labelbg_style,fontNum);
    yellowkeyNum=fac->CreateQLabel(infoWidget,105,370,100,30,QString::number(role.yellowkey),labelbg_style,fontNum);
    purplekeyNum=fac->CreateQLabel(infoWidget,105,420,100,30,QString::number(role.purplekey),labelbg_style,fontNum);
    redkeyNum=fac->CreateQLabel(infoWidget,105,470,100,30,QString::number(role.redkey),labelbg_style,fontNum);
    Floor=fac->CreateQLabel(infoWidget,80,520,40,40,QString::number(floor),labelbg_style,fontNum);

    connect(ReturnMainMenu,SIGNAL(clicked()),this,SLOT(ReturnMainMenuPlay()));
    connect(Restart,SIGNAL(clicked()),this,SLOT(RestartPlay()));

    infoWidget->setMinimumSize(300,680);
    infoWidget->setStyleSheet("color:white;border-image: url(:/interface/image/interface/infobg.png);");
    infoWidget->hide();
}
void Dungeon::ReturnMainMenuPlay()//返回主菜单
{
    infoWidget->hide();//隐藏状态界面
    hideDungeon();//隐藏地牢界面
    surface->isok=false;
    music->InterfaceBGM->play();
    music->InterfaceBGMTimer->start(10);
    NameEdit->clear();
    surface->show();//开始界面显示后，主窗口大小还是不变
    ptr_MianWindow->resize(840,680);//需要重新设置主窗口大小

    surface->showMianMenu();
    initialMap();
    updateStatusData();
}
void Dungeon::RestartPlay()//重新开始
{
    initialMap();
    ShowDungeon();
    updateStatusData();
}
void Dungeon::ShowDungeon()
{
    int BASIC_WIDTH=840/16;
    int BASIC_HEIGHT=680/12;
    if(surface->surfaceShow)
    {
        surface->hide();
        gLayout_Map->removeWidget(surface);
        surface->surfaceShow=false;
    }

    if(isShow)
    {
        for(int i=0;i<12;i++)
        {
            for(int j=0;j<16;j++)
            {
                MazeWidget[i][j]->hide();
            }
        }
    }

    People->setMinimumSize(BASIC_WIDTH,BASIC_HEIGHT);
    gLayout_Map->addWidget(People,x,y);
    People->setStyleSheet(DungeonStytle[38][0]);
    People->show();//用就显示

    for(int i=0;i<12;i++)
    {
        for(int j=0;j<16;j++)
        {
            MazeWidget[i][j]->setMinimumSize(BASIC_WIDTH,BASIC_HEIGHT);
            MazeWidget[i][j]->show();//要用的窗口必须要显示出来
            MazeWidget[i][j]->setStyleSheet(DungeonStytle[map[floor-1][i][j]][0]);
        }
    }
    isShow=true;
    dynamicEffectTimer->start(500);

    music->DungeonBGM->play();
    music->DungeonBGMTimer->start(10);
}
void Dungeon::hideDungeon()
{
    for(int i=0;i<12;i++)
    {
        for(int j=0;j<16;j++)
        {
            MazeWidget[i][j]->hide();
        }
    }
    People->hide();
    isShow=false;
    surface->Dungeonisok=false;
    dynamicEffectTimer->stop();
    music->DungeonBGM->stop();
    music->DungeonBGMTimer->stop();
}
void Dungeon::initialSetNameWin()
{
    QString button_style="QPushButton{background-color:black;color:white;border-radius:10px;}"
                         "QPushButton:hover{background-color:black;color:springgreen;}"
                         "QPushButton:pressed{background-color:black;color:springgreen;}";
    QFont font("Microsoft YaHei" ,12, 35);

    NameEdit=fac->CreateQLineEdit(ptr_MianWindow,320,260,200,40,"color:black",font);
    SetDone=fac->CreateQPushButton(ptr_MianWindow,360,320,120,30,"确定",button_style,font);
    connect(SetDone,SIGNAL(clicked()),this,SLOT(EnterDungeon()));
    NameEdit->hide();
    SetDone->hide();
}
void Dungeon::EnterDungeon()
{
    if(NameEdit->text()=="")
    {
        return;
    }
    else
    {
        NameEdit->hide();
        SetDone->hide();
        Character_name->setText(NameEdit->text());
        CharacterName->setText(NameEdit->text());
        infoWidget->show();
        ShowDungeon();
        music->InterfaceBGM->stop();
        music->InterfaceBGMTimer->stop();
    }
}
void Dungeon::initialGetitemWin()
{
    //显示获得物品信息
    QFont font("Microsoft YaHei" ,12, 75);
    GetitemsShow=fac->CreateQLabel(ptr_MianWindow,220,340,400,50,"","color:white;background-color:black;",font);
    GetitemsShow->hide();
}
void Dungeon::initialFightWin()
{
    QFont font("Microsoft YaHei" ,12, 75);
    QFont fontName("Microsoft YaHei" ,8, 55);
    QFont font_VS("Microsoft YaHei" ,24, 75);
    QString FightWidget_style="color:white;background-color:black;";
    QString Character_pic_style="border-image: url(:/info/image/information/Character.png);";
    FightWidget=new QWidget(ptr_MianWindow);
    FightWidget->setStyleSheet(FightWidget_style);
    FightWidget->setGeometry(160,160,520,320);

    Monster_pic=fac->CreateQLabel(FightWidget,25,60,40,40,"",FightWidget_style,font);
    Monster_name=fac->CreateQLabel(FightWidget,10,160,100,40,"",FightWidget_style,font);
    Monster_hpText=fac->CreateQLabel(FightWidget,105,20,100,40,"生命值",FightWidget_style,font);
    Monster_hp=fac->CreateQLabel(FightWidget,105,60,100,40,"",FightWidget_style,font);
    Monster_atkText=fac->CreateQLabel(FightWidget,105,120,100,40,"攻击力",FightWidget_style,font);
    Monster_atk=fac->CreateQLabel(FightWidget,105,160,100,400,"",FightWidget_style,font);
    Monster_defText=fac->CreateQLabel(FightWidget,105,220,100,40,"防御力",FightWidget_style,font);
    Monster_def=fac->CreateQLabel(FightWidget,105,260,100,40,"",FightWidget_style,font);


    Character_pic=fac->CreateQLabel(FightWidget,445,60,40,40,"",Character_pic_style,fontName);
    Character_name=fac->CreateQLabel(FightWidget,400,160,120,40,role.name,FightWidget_style,font);
    Character_hpText=fac->CreateQLabel(FightWidget,335,20,100,40,"生命值",FightWidget_style,font);
    Character_hp=fac->CreateQLabel(FightWidget,335,60,100,40,"",FightWidget_style,font);
    Character_atkText=fac->CreateQLabel(FightWidget,335,120,100,40,"攻击力",FightWidget_style,font);
    Character_atk=fac->CreateQLabel(FightWidget,335,160,60,40,"",FightWidget_style,font);
    Character_defText=fac->CreateQLabel(FightWidget,335,220,100,40,"防御力",FightWidget_style,font);
    Character_def=fac->CreateQLabel(FightWidget,335,260,100,40,"",FightWidget_style,font);

    QLabel *VS=fac->CreateQLabel(FightWidget,225,100,100,80,"VS",FightWidget_style,font_VS);

    FightWidget->hide();
}
void Dungeon::updateStatusData()
{
    if(role.exp>=100*role.level)
    {
        MAX_HP+=role.level*50;
        role.hp+=role.level*50;
        role.mp+=role.level*5;
        role.atk+=3;
        role.def+=2;
        role.level+=1;
        role.exp=0;
        itemNum=-3;
        GetitemTimer->start(100);
        music->SoundPlay(1);
    }
    score->setText(QString::number(role.scoreNum));
    level->setText(QString::number(role.level));
    exp->setText(QString::number(role.exp));
    hp->setText(QString::number(role.hp));
    mp->setText(QString::number(role.mp));
    atk->setText(QString::number(role.atk));
    def->setText(QString::number(role.def));
    yellowkeyNum->setText(QString::number(role.yellowkey));
    purplekeyNum->setText(QString::number(role.purplekey));
    redkeyNum->setText(QString::number(role.redkey));
    Floor->setText(QString::number(floor));
}
void Dungeon::GetitemWinshow()
{
    if(GetitemNum==0)
    {
        switch(itemNum)
        {
        case -1:
        {
            GetitemsShow->setText("打不过");
            GetitemsShow->show();
        }
            break;
        case -2:
        {
            GetitemsShow->setText("怪物防御太高，你需要提高攻击力");
            GetitemsShow->show();
        }
            break;
        case -3:
        {
            GetitemsShow->setText("Level Up!!!");
            GetitemsShow->show();
        }
            break;
        case 0:
        {
            GetitemsShow->setText("你打不过这只怪物只好遁地逃走");
            GetitemsShow->show();
        }
            break;
        case 1:
        {
            GetitemsShow->setText("你的生命值太低，需要提高才能击败目标！");
            GetitemsShow->show();
        }
            break;
        case 8:
        {
            GetitemsShow->setText("获得小回血蘑菇!生命值加 100");
            GetitemsShow->show();
        }
            break;
        case 9:
        {
            GetitemsShow->setText("获得回蓝蘑菇!魔法值加 50");
            GetitemsShow->show();
        }
            break;
        case 10:
        {
            GetitemsShow->setText("获得大回血蘑菇！增加100点生命值上限！");
            GetitemsShow->show();
        }
            break;
        case 11:
        {
            GetitemsShow->setText("突然从箱子里跳出一只绿鸭子并冲向你，你不得不与之战斗！！！");
            GetitemsShow->show();
        }
            break;
        case 12:
        {
            GetitemsShow->setText("突然从箱子里跳出一只红鸭子并飞向你，你不得不与之战斗！！！");
            GetitemsShow->show();
        }
        case 16:
        {
            GetitemsShow->setText("打不开门, 缺少红钥匙！");
            GetitemsShow->show();
        }
            break;
        case 17:
        {

            GetitemsShow->setText("打不开门, 缺少黄钥匙！");
            GetitemsShow->show();

        }
            break;
        case 18:
        {
            GetitemsShow->setText("打不开门, 缺少紫钥匙！");
            GetitemsShow->show();
        }
            break;
        case 19:
        {
            GetitemsShow->setText("获得一把红钥匙");
            GetitemsShow->show();
        }
            break;
        case 20:
        {
            GetitemsShow->setText("获得一把黄钥匙");
            GetitemsShow->show();
        }
            break;
        case 21:
        {
            GetitemsShow->setText("获得一把紫钥匙");
            GetitemsShow->show();
        }
            break;
        case 22:
        {
            GetitemsShow->setText("获得绿龟壳，防御力加 3");
            GetitemsShow->show();
        }
            break;
        case 23:
        {
            GetitemsShow->setText("获得红龟壳，攻击力加 2");
            GetitemsShow->show();
        }
            break;
        case 24:
        {
            GetitemsShow->setText("获得铁剑，攻击力加 10");
            GetitemsShow->show();
        }
            break;//一定要break;不然会接着执行后面的语句
        case 28:
        {
            GetitemsShow->setText("获得花剑，攻击力加 25");
            GetitemsShow->show();
        }
            break;
        case 29:
        {
            GetitemsShow->setText("获得金剑，攻击力加 36");
            GetitemsShow->show();
        }
            break;
        case 35:
        {
            GetitemsShow->setText("你没有足够的金币");
            GetitemsShow->show();
        }
            break;
        }
        GetitemNum++;
    }
    else if(GetitemNum<=4)//控制显示时间
    {
        GetitemNum++;
    }
    else
    {
        GetitemNum=0;
        GetitemTimer->stop();
        GetitemsShow->hide();
    }
}
void Dungeon::FightWinshow()//战斗界面
{
    if (fight_period_it == 0)
    {
        isFighting=true;
        //准备战斗界面
        Monster_hp->setText(QString::number(enemy[MonsterNum].hp));
        Monster_atk->setText(QString::number(enemy[MonsterNum].atk));
        Monster_def->setText(QString::number(enemy[MonsterNum].def));
        Character_hp->setText(QString::number(role.hp));
        Character_atk->setText(QString::number(role.atk));
        Character_def->setText(QString::number(role.def));
        FightWidget->show();
        Monster_pic->setStyleSheet(MonsterPic[MonsterNum]);
        Monster_name->setText(enemy[MonsterNum].name);
        fight_period_it = 1;

        music->DungeonSoundList->setCurrentIndex(4);
        music->DungeonSound->play();
    }//fight_period_it 代表对战次数，奇数次计算任务对怪物造成的伤害，偶数次计算怪物对人造成的伤害
    else if (fight_period_it % 2 == 1 && fight_end_it == 0)
    {
        if (enemy[MonsterNum].hp - (role.atk - enemy[MonsterNum].def) * (fight_period_it / 2 + 1) <= 0)
        {
            Monster_hp->setText(QString::number(0));//fight_period_it / 2 + 1 代表人物对怪物攻击的累计次数
            fight_end_it = 1;
        }
        else
        {
            Monster_hp->setText(QString::number(enemy[MonsterNum].hp - (role.atk - enemy[MonsterNum].def) * (fight_period_it / 2 + 1)));
            fight_period_it++;
        }
    }
    else if (fight_period_it % 2 == 0 && fight_end_it == 0)
    {
        if (enemy[MonsterNum].atk > role.def)
        {
            Character_hp->setText(QString::number(role.hp - (enemy[MonsterNum].atk - role.def) * (fight_period_it / 2)));
            //fight_period_it / 2 代表怪物对人物的攻击累计次数
        }
        fight_period_it++;
    }
    else
    {
        if (fight_end_it <= 2)//可以控制界面存在时长
        {
            fight_end_it++;
        }
        else if(fight_end_it == 3)
        {
            //结算战斗结果
            int damage = calc_damage(MonsterNum);
            role.hp -= damage;
            role.scoreNum += enemy[MonsterNum].gold;
            role.exp += enemy[MonsterNum].exp;
            //隐藏战斗界面
            FightWidget->hide();
            GetitemsShow->setText(QString::fromWCharArray(L"获得经验值 ") + QString::number(enemy[MonsterNum].exp) + QString::fromWCharArray(L" 金币 ") + QString::number(enemy[MonsterNum].gold));
            GetitemsShow->show();
            music->DungeonSound->stop();
            updateStatusData();//更新战斗结果
            fight_end_it++;
        }
        else if (fight_end_it <= 5)//可以控制界面存在时长
        {
            fight_end_it++;
        }
        else //fight_end_it =6 的时候,战斗状态信息栏才隐藏
        {
            FightTimer->stop();
            GetitemsShow->hide();
            Move();
            isFighting=false;
            fight_end_it = 0;
            fight_period_it = 0;
            MonsterNum = 0;
        }
    }
}
bool Dungeon::isStatic(int num)//静态贴图
{
    return num==0||num==1||(num>=6&&num<=10)||(num>=14&&num<=24)||(num>=28&&num<=38);
}

bool Dungeon::isDynamic(int i,int j)
{
    if(map[floor-1][i][j]==2||map[floor-1][i][j]==3||map[floor-1][i][j]==4||
            map[floor-1][i][j]==5||map[floor-1][i][j]==11||map[floor-1][i][j]==12||
            map[floor-1][i][j]==25||map[floor-1][i][j]==26||map[floor-1][i][j]==27)
        return true;
    else
        return false;
}
void Dungeon::Move()
{
    switch(moveDirection)
    {
    case 0://up
    {
        x-=1;
        map[floor-1][x][y]=1;
        MazeWidget[x][y]->setStyleSheet(DungeonStytle[1][0]);
        gLayout_Map->addWidget(People,x,y);
        People->setStyleSheet(DungeonStytle[37][0]);
    }
        break;
    case 1://down
    {
        x+=1;
        map[floor-1][x][y]=1;
        MazeWidget[x][y]->setStyleSheet(DungeonStytle[1][0]);
        gLayout_Map->addWidget(People,x,y);
        People->setStyleSheet(DungeonStytle[38][0]);
    }
        break;
    case 2://left
    {
        y-=1;
        map[floor-1][x][y]=1;
        MazeWidget[x][y]->setStyleSheet(DungeonStytle[1][0]);
        gLayout_Map->addWidget(People,x,y);
        People->setStyleSheet(DungeonStytle[39][moveNum%4]);
    }
        break;
    case 3://right
    {
        y+=1;
        map[floor-1][x][y]=1;
        MazeWidget[x][y]->setStyleSheet(DungeonStytle[1][0]);
        gLayout_Map->addWidget(People,x,y);
        People->setStyleSheet(DungeonStytle[40][moveNum%4]);
    }
        break;
    }
    moveNum++;
}
void Dungeon::ShowdynamicEffect()
{
    if (display_it <= 2)
        display_it += 1;
    else
        display_it = 0;
    for(int i=0;i<12;i++)
    {
        for(int j=0;j<16;j++)
        {
            if(isDynamic(i,j))
            {
                MazeWidget[i][j]->setStyleSheet(DungeonStytle[map[floor-1][i][j]][display_it]);
            }
            else
            {
                MazeWidget[i][j]->setStyleSheet(DungeonStytle[map[floor-1][i][j]][0]);
            }
        }
    }

}
void Dungeon::OpenStore()
{
    if(role.scoreNum-Store->Currstore_price<0)//金币不够，不能购买
    {
        itemNum=35;//提示玩家
        GetitemTimer->start(100);
        Store->haveBuyTimer->stop();
    }
    else
    {
        role.scoreNum-=Store->Currstore_price;
        if(Store->chooseOption==0)
        {
            MAX_HP+=100 * (Store->BuyTimes-1);
            role.hp+=100 * (Store->BuyTimes-1);
            hp->setText(QString::number(role.hp));
            score->setText(QString::number(role.scoreNum));
            if(role.scoreNum-Store->Nextstore_price<0)//判断下一次是否可以购买
            {
                Store->setChooseEnable=false;
            }
            Store->haveBuyTimer->stop();//购买完成
        }
        else if(Store->chooseOption==1)
        {
            role.mp+=20 * (Store->BuyTimes-1);
            mp->setText(QString::number(role.mp));
            score->setText(QString::number(role.scoreNum));

            if(role.scoreNum-Store->Nextstore_price<0)
            {
                Store->setChooseEnable=false;
            }
            Store->haveBuyTimer->stop();
        }
        else if(Store->chooseOption==2)
        {
            role.atk+=2;
            atk->setText(QString::number(role.atk));
            score->setText(QString::number(role.scoreNum));

            if(role.scoreNum-Store->Nextstore_price<0)
            {
                Store->setChooseEnable=false;
            }
            Store->haveBuyTimer->stop();
        }
        else if(Store->chooseOption==3)
        {
            role.def+=4;
            def->setText(QString::number(role.def));
            score->setText(QString::number(role.scoreNum));

            if(role.scoreNum-Store->Nextstore_price<0)
            {
                Store->setChooseEnable=false;
            }
            Store->haveBuyTimer->stop();
        }
        else
        {
            return;
        }

    }
    return;
}
void Dungeon::changeHP(int num)
{
    if(role.hp+num>=MAX_HP)
    {
        role.hp=MAX_HP;
    }
    else
    {
        role.hp+=num;
    }
}
void Dungeon::QuestionBox(int num)
{
    int temp1=0;
    int temp2=0;
    if(num==0)
    {
        temp1=x-1;
        temp2=y;
    }
    else if(num==1)
    {
        temp1=x+1;
        temp2=y;
    }
    else if(num==2)
    {
        temp1=x;
        temp2=y-1;
    }
    else if(num==3)
    {
        temp1=x;
        temp2=y+1;

    }
    QMessageBox message(QMessageBox::Information,"发现箱子！","箱子上写着问号，不知道里面装着什么，是否打开？",QMessageBox::Yes|QMessageBox::No,ptr_MianWindow);
    message.setIconPixmap(QPixmap(":/info/image/information/box.png"));
    message.setWindowIcon(QIcon(":/info/image/information/提示.ico"));
    if (message.exec()==QMessageBox::Yes)
    {
        int Num=rand()%5+8;
        switch(Num)
        {
        case 8:
        {
            itemNum=8;
            GetitemTimer->start(100);
            changeHP(100);
            updateStatusData();
            map[floor-1][temp1][temp2]=1;
            MazeWidget[temp1][temp2]->setStyleSheet(DungeonStytle[1][0]);
            return;
        }
            break;
        case 9:
        {
            itemNum=9;
            GetitemTimer->start(100);
            role.mp+=50;
            updateStatusData();
            map[floor-1][temp1][temp2]=1;
            MazeWidget[temp1][temp2]->setStyleSheet(DungeonStytle[1][0]);
            return;
        }
            break;
        case 10:
        {
            itemNum=10;
            GetitemTimer->start(100);
            MAX_HP+=100;
            changeHP(100);
            updateStatusData();
            map[floor-1][temp1][temp2]=1;
            MazeWidget[temp1][temp2]->setStyleSheet(DungeonStytle[1][0]);
            return;
        }
            break;
        case 11:
        {
            QMessageBox mes(QMessageBox::NoIcon, "你打开了箱子", "突然从箱子里跳出一只绿鸭子并冲向你，你不得不与之战斗！！！");
            mes.setWindowIcon(QIcon(":/info/image/information/提示.ico"));
            mes.setIconPixmap(QPixmap(":/info/image/information/greenDuck.png"));
            mes.exec();

            if(pk(enemy[2])==-1||pk(enemy[2])==-2||pk(enemy[2])==1)
            {
                itemNum=0;
                GetitemTimer->start(100);
                map[floor-1][temp1][temp2]=11;
                MazeWidget[temp1][temp2]->setStyleSheet(DungeonStytle[11][0]);
            }
            else
            {
                MonsterNum=2;
                FightTimer->start(100);
                moveDirection=num;
            }
            return;
        }
            break;
        case 12:
        {
            QMessageBox mes(QMessageBox::NoIcon, "你打开了箱子", "突然从箱子里跳出一只红鸭子并飞向你，你不得不与之战斗！！！");
            mes.setWindowIcon(QIcon(":/info/image/information/提示.ico"));
            mes.setIconPixmap(QPixmap(":/info/image/information/flyDuck.png"));
            mes.exec();

            if(pk(enemy[3])==-1||pk(enemy[3])==-2||pk(enemy[3])==1)
            {
                itemNum=0;
                GetitemTimer->start(100);
                map[floor-1][temp1][temp2]=12;
                MazeWidget[temp1][temp2]->setStyleSheet(DungeonStytle[12][0]);
            }
            else
            {
                MonsterNum=3;
                FightTimer->start(100);
                moveDirection=num;
            }
            return;
        }
            break;
        }
    }
    else
    {
        return;
    }

}
void Dungeon::items(int _itemNum, int _moveDirection)
{
    QLabel *target=new QLabel;
    int *tempNum=nullptr;
    switch(_itemNum)
    {
    case 8:
    {
        changeHP(100);
        hp->setText(QString::number(role.hp));
        music->SoundPlay(5);
    }
        break;
    case 9:
    {
        role.mp+=50;
        mp->setText(QString::number(role.mp));
        music->SoundPlay(5);
    }
        break;
    case 10:
    {
        MAX_HP+=100;
        changeHP(100);
        hp->setText(QString::number(role.hp));
        music->SoundPlay(5);
    }
        break;
    case 19:
    {
        target=redkeyNum;
        tempNum=&(role.redkey);
        music->SoundPlay(3);
    }
        break;
    case 20:
    {
        target=yellowkeyNum;
        tempNum=&(role.yellowkey);
        music->SoundPlay(3);
    }
        break;
    case 21:
    {
        target=purplekeyNum;
        tempNum=&(role.purplekey);
        music->SoundPlay(3);
    }
        break;
    case 22:
    {
        role.def+=3;
        def->setText(QString::number(role.def));
        music->SoundPlay(3);
    }
        break;
    case 23:
    {
        role.atk+=2;
        atk->setText(QString::number(role.atk));
        music->SoundPlay(3);
    }
        break;
    case 24:
    {
        role.atk+=10;
        atk->setText(QString::number(role.atk));
        music->SoundPlay(1);
    }
        break;
    case 28:
    {
        role.atk+=25;
        atk->setText(QString::number(role.atk));
        music->SoundPlay(1);
    }
        break;
    case 29:
    {
        role.atk+=36;
        atk->setText(QString::number(role.atk));
        music->SoundPlay(1);
    }
        break;
    }
    itemNum=_itemNum;
    GetitemTimer->start(100);
    moveDirection=_moveDirection;
    Move();
    if(itemNum==19||itemNum==20||itemNum==21)
    {
        (*tempNum)+=1;
        target->setText(QString::number(*tempNum));
    }
}
void Dungeon::Monsters(int _MonsterNum,int _moveDirection)
{
    if(pk(enemy[_MonsterNum])==-1)
    {
        itemNum=-1;
        GetitemTimer->start(100);
    }
    else if(pk(enemy[_MonsterNum])==-2)
    {
        itemNum=-2;
        GetitemTimer->start(100);
    }
    else if(pk(enemy[_MonsterNum])==1)
    {
        itemNum=1;
        GetitemTimer->start(100);
    }
    else
    {
        MonsterNum=_MonsterNum;
        FightTimer->start(50);
        moveDirection=_moveDirection;
    }
}
void Dungeon::doors(int _doorNum, int _moveDirection)
{

    QLabel *target=new QLabel;
    int *tempNum=nullptr;
    switch(_doorNum)
    {
    case 16:
    {
        target=redkeyNum;
        tempNum=&(role.redkey);
    }
        break;
    case 17:
    {
        target=yellowkeyNum;
        tempNum=&(role.yellowkey);
    }
        break;
    case 18:
    {
        target=purplekeyNum;
        tempNum=&(role.purplekey);
    }
        break;

    }
    if(*tempNum==0)
    {
        itemNum=_doorNum;
        GetitemTimer->start(100);
        return;
    }
    else
    {
        moveDirection=_moveDirection;
        Move();
        (*tempNum)-=1;
        target->setText(QString::number(*tempNum));
        music->SoundPlay(6);
    }
}
void Dungeon::checkPrefloor()
{
    if(floor==1)
    {
        QMessageBox message(QMessageBox::NoIcon, "对不起！", "你不能走出地牢！");
        message.setIconPixmap(QPixmap(":/info/image/information/sorry.png"));
        message.exec();
        return;
    }
    isPre=true;
    initialCharacterPos();
    People->hide();
    floor-=1;
    Floor->setText(QString::number(floor));
    ShowDungeon();
    music->SoundPlay(2);//放在尾部才会音效持久
}

void Dungeon::checkNextfloor()
{
    if(floor==Total_Floor)
    {
        QMessageBox message(QMessageBox::NoIcon, "你想干啥？", "这是地牢最后一层了！");
        message.setIconPixmap(QPixmap(":/info/image/information/question.png"));
        message.exec();
        return;
    }
    isNext=true;
    initialCharacterPos();
    People->hide();
    floor+=1;
    Floor->setText(QString::number(floor));
    ShowDungeon();
    music->SoundPlay(2);
}
void Dungeon::Operation(int num)
{
    bool flag;
    int _x,_y;
    if(num==0)
    {
        flag=x>0;
        _x=x-1;
        _y=y;
    }
    else if(num==1)
    {
        flag=x<11;
        _x=x+1;
        _y=y;
    }
    else if(num==2)
    {
        flag=y>0;
        _x=x;
        _y=y-1;
    }
    else if(num==3)
    {
        flag=y<15;
        _x=x;
        _y=y+1;
    }
    else
        return;

    if(flag&&map[floor-1][_x][_y]==0)
    {
        return;
    }
    else if(flag&&map[floor-1][_x][_y]==1)
    {
        moveDirection=num;
        Move();
    }
    else if(flag&&map[floor-1][_x][_y]==2)
    {
        moveDirection=num;
        Move();
        role.scoreNum+=10;
        score->setText(QString::number(role.scoreNum));
        music->SoundPlay(0);
    }
    else if(flag&&map[floor-1][_x][_y]==3)
    {
        Monsters(0,num);
    }
    else if(flag&&map[floor-1][_x][_y]==4)
    {
        Monsters(1,num);
    }
    else if(flag&&map[floor-1][_x][_y]==5)
    {
        QuestionBox(num);
    }
    else if(flag&&map[floor-1][_x][_y]==8)
    {
        items(8,num);
    }
    else if(flag&&map[floor-1][_x][_y]==9)
    {
        items(9,num);
    }
    else if(flag&&map[floor-1][_x][_y]==10)
    {
        items(10,num);
    }
    else if(flag&&map[floor-1][_x][_y]==11)
    {
        Monsters(2,num);
    }
    else if(flag&&map[floor-1][_x][_y]==12)
    {
        Monsters(3,num);
    }
    else if(flag&&map[floor-1][_x][_y]==14)
    {
        checkPrefloor();
    }
    else if(flag&&map[floor-1][_x][_y]==15)
    {
        checkNextfloor();
    }
    else if(flag&&map[floor-1][_x][_y]==16)
    {
        doors(16,num);
    }
    else if(flag&&map[floor-1][_x][_y]==17)
    {
        doors(17,num);
    }
    else if(flag&&map[floor-1][_x][_y]==18)
    {
        doors(18,num);
    }
    else if(flag&&map[floor-1][_x][_y]==19)
    {
        items(19,num);
    }
    else if(flag&&map[floor-1][_x][_y]==20)
    {
        items(20,num);
    }
    else if(flag&&map[floor-1][_x][_y]==21)
    {
        items(21,num);
    }
    else if(flag&&map[floor-1][_x][_y]==22)
    {
        items(22,num);
    }
    else if(flag&&map[floor-1][_x][_y]==23)
    {
        items(23,num);
    }
    else if(flag&&map[floor-1][_x][_y]==24)
    {
        items(24,num);
    }
    else if(flag&&map[floor-1][_x][_y]==25)
    {
        Monsters(4,num);
    }
    else if(flag&&map[floor-1][_x][_y]==26)
    {
        Monsters(5,num);
    }
    else if(flag&&map[floor-1][_x][_y]==27)
    {
        Monsters(6,num);
    }
    else if(flag&&map[floor-1][_x][_y]==28)
    {
        items(28,num);
    }
    else if(flag&&map[floor-1][_x][_y]==29)
    {
        items(29,num);
    }
    else if(flag&&map[floor-1][_x][_y]==35)
    {
        Store->show();
        if(role.scoreNum-Store->Currstore_price>=0)
        {
            Store->setChooseEnable=true;
        }
        Store->StoreWinisShow=true;
    }
    else
    {
        return;
    }
}

void Dungeon::keyPressEvent(QKeyEvent *event)
{
    if(!isShow)
    {
        return;
    }
    else if(isFighting)//keyevent 是实时检测的，只要在战斗结束后再设置isFighting为false，就可以了
    {
        return;
    }
    else if(Store->StoreWinisShow)
    {
        return;
    }

    switch(event->key())
    {

    case Qt::Key_W://上
        Operation(0);
        break;
    case Qt::Key_S://下
        Operation(1);
        break;
    case Qt::Key_A://左
        Operation(2);
        break;
    case Qt::Key_D://右
        Operation(3);
        break;
    }
}

