#include "mainwindow.h"
#include <QMessageBox>
#include <QStyleFactory>
#include <QPalette>
#include <QPixmap>
#include <QTime>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉边框
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
    for(int i=0; i <60 ;i++)
    {
        for(int j=0; j<60; j++)
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
    surface->setMinimumSize(840,680);
    connect(surface->timer,SIGNAL(timeout()),this,SLOT(ShowWidget()));//显示控制界面()));//传递过来的参数有问题，会一直调用槽函数
    gLayout_Map->addWidget(surface,0,0);
    Mapwidget->setMinimumSize(840,680);
    Mapwidget->setLayout(gLayout_Map);

    BASIC_WIDTH=0;
    BASIC_HEIGHT=0;
    GetitemNum=0;//获得物品信息栏初始化
    itemNum=0;
    MonsterNum=0;
    fight_period_it = 0;//战斗状态信息栏初始化
    fight_end_it = 0;
    moveDirection=-1;
    moveNum=0;
    isFighting=false;
    iNum=0;
    lastheight=0;
    lastwidth=0;
    isPlay=false;
    isShow=false;
    issurface=true;
    isAIAnimationButton=false;
    isAutoMoveButton=false;

    display_it=0;

    initialControlWidget();
    initialinfoWidget();


    MainWidget=new QWidget(this);
    hLayout->addWidget(Mapwidget);
    hLayout->addWidget(Controlwidget);
    hLayout->addWidget(infoWidget);

    initialFightWin();//希望显示在MainWidget之上的窗口，要在初始化MainWidget后再初始化

    Store=new StoreWidget(this);//StoreWidget只是一个类，要想有主窗口，类中必须自己定义一个
    Store->setGeometry(292,192,256,352);
    Store->hide();

    initialSetNameWin();
    initialGetitemWin();

    MainWidget->setLayout(hLayout);
    MainWidget->setFocusPolicy(Qt::StrongFocus);


    //设置widget为Mainwindow的中心窗口
    this->setCentralWidget(MainWidget);
    group=new QSequentialAnimationGroup;
    timer=new QTimer(this);
    iNum=1;
    GetitemTimer=new QTimer(this);
    FightTimer=new QTimer(this);
    dynamicEffectTimer=new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(moveCharacter()));
    connect(timer,SIGNAL(timeout()),this,SLOT(ShowPath()));
    connect(GetitemTimer,SIGNAL(timeout()),this,SLOT(GetitemWinshow()));
    connect(FightTimer,SIGNAL(timeout()),this,SLOT(FightWinshow()));
    connect(Store->haveBuyTimer,SIGNAL(timeout()),this,SLOT(OpenStore()));
    connect(dynamicEffectTimer,SIGNAL(timeout()),this,SLOT(ShowdynamicEffect()));

    BGM=new QMediaPlayer(this);//背景音乐
    BGM->setMedia(QUrl("qrc:/music/music/main_theme.mp3"));//相对路径
    BGM->setVolume(50);

    soundlist=new QMediaPlaylist(this);//游戏音效
    soundlist->addMedia(QUrl("qrc:/music/music/coin.mp3"));//硬币音效
    soundlist->addMedia(QUrl("qrc:/music/music/powerup.mp3"));//升级音效
    soundlist->addMedia(QUrl("qrc:/music/music/flagpole.wav"));//碰到旗子音效
    soundlist->addMedia(QUrl("qrc:/music/music/stomp.mp3"));//捡到钥匙音效
    soundlist->addMedia(QUrl("qrc:/music/music/count_down.mp3"));//战斗计算音效
    soundlist->addMedia(QUrl("qrc:/music/music/one_up.mp3"));//吃到蘑菇音效
    soundlist->addMedia(QUrl("qrc:/music/music/pipe.mp3"));//开门音效
    sound=new QMediaPlayer(this);
    sound->setPlaylist(soundlist);
    sound->setVolume(80);

    BGMTimer=new QTimer(this);
    soundTimer=new QTimer(this);
    connect(soundTimer,SIGNAL(timeout()),this,SLOT(soundStop()));
    connect(BGMTimer,SIGNAL(timeout()),this,SLOT(CheckBGMstate()));
}

void MainWindow::initialControlWidget()
{
    gLayout_Control=new QGridLayout;
    gLayout_Control->setSpacing(10);
    Controlwidget=new QWidget(this);
    Controlwidget->setStyleSheet("background-color:lightGray");
    QString button_style="QPushButton{background-color:white; color: black;border-radius:1px;border:2px groove gray;border-style:outset;}"
                         "QPushButton:hover{background-color:lightGray; color: black;}"
                         "QPushButton:pressed{background-color:gray;border-style:inset;}";

    QLabel *label_w=new QLabel;
    QLabel *label_h=new QLabel;
    label_w->setStyleSheet("background-color:transparent");
    label_h->setStyleSheet("background-color:transparent");
    QLabel *label_blank[10];
    for(int i=0;i<10;i++)
    {
        label_blank[i]=new QLabel(this);
        label_blank[i]->setStyleSheet("background-color:transparent");
    }

    label_Stytle=new QLabel;
    StytleNum=2;
    label_Stytle->setStyleSheet(m.MapStytle[StytleNum][15]);
    QLabel *label_select=new QLabel;
    label_w->setText("迷宫宽度");
    label_h->setText("迷宫高度");
    label_select->setText("选择地图风格");
    sp_w=new QSpinBox(this);
    sp_h=new QSpinBox(this);
    sp_w->setStyleSheet("background-color:transparent");
    sp_h->setStyleSheet("background-color:transparent");
    SelectMapStytle=new QComboBox(this);
    SelectMapStytle->addItem(tr("魔法少女"));//项目编号从0开始
    SelectMapStytle->addItem(tr("火影忍者"));
    SelectMapStytle->addItem(tr("超级玛丽"));
    SelectMapStytle->setCurrentIndex(2);
    connect(SelectMapStytle,SIGNAL(currentIndexChanged(int)),this,SLOT(MapStytleSet()));
    //currentIndexChanged(int) 中的int 去掉，就不能被识别为信号
    QPushButton *GenerateButton=new QPushButton(this);
    quitButton=new QPushButton(this);
    AIAnimationButton=new QPushButton(this);
    AutoMoveButton=new QPushButton(this);
    AIAnimationButton->setEnabled(false);
    AutoMoveButton->setEnabled(false);
    GenerateButton->setText("生成迷宫");
    AutoMoveButton->setText("自动寻路");
    AIAnimationButton->setText("AI操作");
    quitButton->setText("主菜单");
    quitButton->setEnabled(false);
    GenerateButton->setStyleSheet(button_style);
    AutoMoveButton->setStyleSheet(button_style);
    AIAnimationButton->setStyleSheet(button_style);
    quitButton->setStyleSheet(button_style);

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
void MainWindow::resetMaze()
{
    AIAnimationButton->setEnabled(false);
    AutoMoveButton->setEnabled(false);
    quitButton->setEnabled(false);
    sp_h->setValue(0);
    sp_w->setValue(0);
    SelectMapStytle->setCurrentIndex(2);
}

QString intToQString(int num)
{
    QString result="";
    result.sprintf("%d",num);
    return result;
}

void MainWindow::initialinfoWidget()
{
    infoWidget=new QWidget(this);
    QFont fontLabel("Microsoft YaHei" ,12, 75);
    QFont fontNum("Microsoft YaHei" ,10, 65);
    QFont fontName("Microsoft YaHei" ,10, 55);
    QString button_style="QPushButton{border-image:url(:/interface/image/interface/labelbg.png);color:white;border-radius:10px;}"
                         "QPushButton:hover{border-image:url(:/interface/image/interface/PushButtonhoverbg.png); color: black;}"
                         "QPushButton:pressed{border-image:url(:/interface/image/interface/PushButtonPressbg.png);}";


    QLabel *CharacterPic=new QLabel(infoWidget);
    QLabel *label_Name=new QLabel(infoWidget);
    CharacterName=new QLabel(infoWidget);
    QLabel *label_level=new QLabel(infoWidget);
    QLabel *label_hp=new QLabel(infoWidget);
    QLabel *label_mp=new QLabel(infoWidget);
    QLabel *label_atk=new QLabel(infoWidget);
    QLabel *label_def=new QLabel(infoWidget);
    QLabel *label_score=new QLabel(infoWidget);
    QLabel *label_exp=new QLabel(infoWidget);
    QLabel *label_yellowkey=new QLabel(infoWidget);
    QLabel *label_purplekey=new QLabel(infoWidget);
    QLabel *label_redkey=new QLabel(infoWidget);
    QLabel *label_floor1=new QLabel(infoWidget);
    QLabel *label_floor2=new QLabel(infoWidget);
    ReturnMainMenu =new QPushButton(infoWidget);
    Restart=new QPushButton(infoWidget);

    label_Name->setFont(fontLabel);
    label_Name->setStyleSheet("border-image: url(:/interface/image/interface/labelbg.png);");

    CharacterName->setFont(fontName);
    CharacterName->setStyleSheet("border-image: url(:/interface/image/interface/labelbg.png);");
    label_level->setFont(fontLabel);
    label_level->setStyleSheet("border-image: url(:/interface/image/interface/labelbg.png);");
    label_hp->setFont(fontLabel);
    label_hp->setStyleSheet("border-image: url(:/interface/image/interface/labelbg.png);");
    label_mp->setFont(fontLabel);
    label_mp->setStyleSheet("border-image: url(:/interface/image/interface/labelbg.png);");
    label_atk->setFont(fontLabel);
    label_atk->setStyleSheet("border-image: url(:/interface/image/interface/labelbg.png);");
    label_def->setFont(fontLabel);
    label_def->setStyleSheet("border-image: url(:/interface/image/interface/labelbg.png);");
    label_score->setFont(fontLabel);
    label_score->setStyleSheet("border-image: url(:/interface/image/interface/labelbg.png);");
    label_exp->setFont(fontLabel);
    label_exp->setStyleSheet("border-image: url(:/interface/image/interface/labelbg.png);");
    label_floor1->setFont(fontLabel);
    label_floor1->setStyleSheet("border-image: url(:/interface/image/interface/labelbg.png);");
    label_floor2->setFont(fontLabel);
    label_floor2->setStyleSheet("border-image: url(:/interface/image/interface/labelbg.png);");
    ReturnMainMenu->setFont(fontLabel);
    ReturnMainMenu->setStyleSheet(button_style);
    Restart->setFont(fontLabel);
    Restart->setStyleSheet(button_style);


    level=new QLabel(infoWidget);
    hp=new QLabel(infoWidget);
    mp=new QLabel(infoWidget);
    atk=new QLabel(infoWidget);
    def=new QLabel(infoWidget);
    score=new QLabel(infoWidget);
    exp=new QLabel(infoWidget);
    yellowkeyNum=new QLabel(infoWidget);
    purplekeyNum=new QLabel(infoWidget);
    redkeyNum=new QLabel(infoWidget);
    Floor=new QLabel(infoWidget);

    level->setFont(fontNum);
    level->setStyleSheet("border-image: url(:/interface/image/interface/labelbg.png);");
    hp->setFont(fontNum);
    hp->setStyleSheet("border-image: url(:/interface/image/interface/labelbg.png);");
    mp->setFont(fontNum);
    mp->setStyleSheet("border-image: url(:/interface/image/interface/labelbg.png);");
    atk->setFont(fontNum);
    atk->setStyleSheet("border-image: url(:/interface/image/interface/labelbg.png);");
    def->setFont(fontNum);
    def->setStyleSheet("border-image: url(:/interface/image/interface/labelbg.png);");
    score->setFont(fontNum);
    score->setStyleSheet("border-image: url(:/interface/image/interface/labelbg.png);");
    exp->setFont(fontNum);
    exp->setStyleSheet("border-image: url(:/interface/image/interface/labelbg.png);");
    yellowkeyNum->setFont(fontNum);
    yellowkeyNum->setStyleSheet("border-image: url(:/interface/image/interface/labelbg.png);");
    purplekeyNum->setFont(fontNum);
    purplekeyNum->setStyleSheet("border-image: url(:/interface/image/interface/labelbg.png);");
    redkeyNum->setFont(fontNum);
    redkeyNum->setStyleSheet("border-image: url(:/interface/image/interface/labelbg.png);");
    Floor->setFont(fontNum);
    Floor->setStyleSheet("border-image: url(:/interface/image/interface/labelbg.png);");

    CharacterPic->setStyleSheet("border-image: url(:/info/image/information/Character.png);");
    label_Name->setText("人们叫他");
    label_level->setText("级");
    label_hp->setText("生命");
    label_mp->setText("魔法");
    label_atk->setText("攻击");
    label_def->setText("防御");
    label_score->setText("金币");
    label_exp->setText("经验");
    label_yellowkey->setStyleSheet("border-image: url(:/info/image/information/yellowkey.png);");
    label_purplekey->setStyleSheet("border-image: url(:/info/image/information/purplekey.png);");
    label_redkey->setStyleSheet("border-image: url(:/info/image/information/redkey.png);");
    label_floor1->setText("第");
    label_floor2->setText("层");
    ReturnMainMenu->setText("返回主菜单");
    Restart->setText("重新开始");

    level->setText(intToQString(d.role.level));
    hp->setText(intToQString(d.role.hp));
    mp->setText(intToQString(d.role.mp));
    atk->setText(intToQString(d.role.atk));
    def->setText(intToQString(d.role.def));
    score->setText(intToQString(d.role.scoreNum));
    exp->setText(intToQString(d.role.exp));
    yellowkeyNum->setText(intToQString(d.role.yellowkey));
    purplekeyNum->setText(intToQString(d.role.purplekey));
    redkeyNum->setText(intToQString(d.role.redkey));
    Floor->setText(intToQString(d.floor));

    CharacterPic->setGeometry(20,20,40,40);
    label_level->setGeometry(140,20,40,40);
    level->setGeometry(80,20,40,40);
    label_Name->setGeometry(20,70,100,30);
    CharacterName->setGeometry(130,70,170,30);
    label_hp->setGeometry(20,110,65,30);
    hp->setGeometry(105,110,100,30);
    label_mp->setGeometry(20,150,65,30);
    mp->setGeometry(105,150,100,30);
    label_exp->setGeometry(20,190,65,30);
    exp->setGeometry(105,190,100,30);
    label_atk->setGeometry(20,230,65,30);
    atk->setGeometry(105,230,100,30);
    label_def->setGeometry(20,270,65,30);
    def->setGeometry(105,270,100,30);
    label_score->setGeometry(20,310,65,30);
    score->setGeometry(105,310,100,30);
    label_yellowkey->setGeometry(20,370,40,40);
    yellowkeyNum->setGeometry(105,370,100,40);
    label_purplekey->setGeometry(20,420,40,40);
    purplekeyNum->setGeometry(105,420,100,40);
    label_redkey->setGeometry(20,470,40,40);
    redkeyNum->setGeometry(105,470,100,40);
    label_floor1->setGeometry(20,520,40,40);
    Floor->setGeometry(80,520,40,40);
    label_floor2->setGeometry(140,520,40,40);
    ReturnMainMenu->setGeometry(140,580,120,30);
    Restart->setGeometry(140,620,100,30);

    connect(ReturnMainMenu,SIGNAL(clicked()),this,SLOT(ReturnMainMenuPlay()));
    connect(Restart,SIGNAL(clicked()),this,SLOT(RestartPlay()));

    infoWidget->setMinimumSize(300,680);
    infoWidget->setStyleSheet("color:white;border-image: url(:/interface/image/interface/infobg.png);");
    infoWidget->hide();
}
void MainWindow::initialGetitemWin()
{
    //显示获得物品信息
    GetitemsShow=new QLabel(this);
    GetitemsShow->setGeometry(220,340,400,50);//居中
    GetitemsShow->setStyleSheet("color:white;background-color:black;");
    GetitemsShow->setAlignment(Qt::AlignCenter);
    QFont font("Microsoft YaHei" ,12, 75);
    GetitemsShow->setFont(font);
    GetitemsShow->hide();
}
void MainWindow::initialFightWin()
{
    QFont font("Microsoft YaHei" ,12, 75);
    FightWidget=new QWidget(this);
    FightWidget->setStyleSheet("color:white;background-color:black;");
    FightWidget->setGeometry(160,160,520,320);

    Monster_pic=new QLabel(FightWidget);
    Monster_name=new QLabel(FightWidget);
    Monster_hpText=new QLabel(FightWidget);
    Monster_hp=new QLabel(FightWidget);
    Monster_atkText=new QLabel(FightWidget);
    Monster_atk=new QLabel(FightWidget);
    Monster_defText=new QLabel(FightWidget);
    Monster_def=new QLabel(FightWidget);
    Monster_name->setFont(font);
    Monster_hpText->setFont(font);
    Monster_hp->setFont(font);
    Monster_atkText->setFont(font);
    Monster_atk->setFont(font);
    Monster_defText->setFont(font);
    Monster_def->setFont(font);
    Monster_pic->setGeometry(25,60,40,40);
    Monster_name->setGeometry(10,160,100,40);
    Monster_hpText->setGeometry(105,20,100,40);
    Monster_hp->setGeometry(105,60,100,40);
    Monster_atkText->setGeometry(105,120,100,40);
    Monster_atk->setGeometry(105,160,100,40);
    Monster_defText->setGeometry(105,220,100,40);
    Monster_def->setGeometry(105,260,100,40);



    Monster_hpText->setText("生命值");
    Monster_atkText->setText("攻击力");
    Monster_defText->setText("防御力");

    Character_pic=new QLabel(FightWidget);
    Character_name=new QLabel(FightWidget);
    Character_hpText=new QLabel(FightWidget);
    Character_hp=new QLabel(FightWidget);
    Character_atkText=new QLabel(FightWidget);
    Character_atk=new QLabel(FightWidget);
    Character_defText=new QLabel(FightWidget);
    Character_def=new QLabel(FightWidget);

    QFont fontName("Microsoft YaHei" ,8, 55);
    Character_name->setFont(fontName);
    Character_name->setAlignment(Qt::AlignCenter);
    Character_hpText->setFont(font);
    Character_hp->setFont(font);
    Character_atkText->setFont(font);
    Character_atk->setFont(font);
    Character_defText->setFont(font);
    Character_def->setFont(font);
    Character_pic->setGeometry(445,60,40,40);
    Character_name->setGeometry(400,160,120,40);
    Character_hpText->setGeometry(335,20,100,40);
    Character_hp->setGeometry(335,60,100,40);
    Character_atkText->setGeometry(335,120,100,40);
    Character_atk->setGeometry(335,160,60,40);
    Character_defText->setGeometry(335,220,100,40);
    Character_def->setGeometry(335,260,100,40);


    Character_pic->setStyleSheet("border-image: url(:/info/image/information/Character.png);");
    Character_name->setText(d.role.name);
    Character_hpText->setText("生命值");
    Character_atkText->setText("攻击力");
    Character_defText->setText("防御力");

    QLabel *VS=new QLabel(FightWidget);
    VS->setText("VS");
    VS->setStyleSheet("color:red;background-color:black;");
    QFont font_VS("Microsoft YaHei" ,24, 75);
    VS->setFont(font_VS);
    VS->setGeometry(225,100,100,80);

    FightWidget->hide();
}

void MainWindow::initialSetNameWin()
{
    NameEdit=new QLineEdit(this);
    SetDone=new QPushButton(this);

    QString button_style="QPushButton{background-color:black;color:white;border-radius:10px;}"
                         "QPushButton:hover{background-color:black;color:springgreen;}"
                         "QPushButton:pressed{background-color:black;color:springgreen;}";
    QFont font("Microsoft YaHei" ,12, 35);
    NameEdit->setGeometry(320,260,200,40);
    NameEdit->setFont(font);
    NameEdit->setStyleSheet("color:black");
    SetDone->setGeometry(360,320,120,30);
    SetDone->setText("确定");
    SetDone->setFont(font);
    SetDone->setStyleSheet(button_style);
    connect(SetDone,SIGNAL(clicked()),this,SLOT(EnterDungeon()));
    NameEdit->hide();
    SetDone->hide();
}
bool isOdd(int num)
{
    return !(num%2==0);
}
void MainWindow::CreateMaze_Layout()
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
        m.height=sp_h->value();
        m.width=sp_w->value();
    }
    //当h和w都为偶数时，迷宫的出口（height-2，width-2）会被墙给封住
    if(!isOdd(m.height)&&!isOdd(m.width))
    {
        if(m.height>m.width)
        {
            m.height+=1;
            m.width=m.height;
        }
        else
        {
            m.width+=1;
            m.height=m.width;
        }
    }
    BASIC_WIDTH=840/m.width;//52
    BASIC_HEIGHT=680/m.height;//56

    if(issurface)
    {
        surface->hide();
        gLayout_Map->removeWidget(surface);
        issurface=false;
    }

    People->setMinimumSize(BASIC_WIDTH,BASIC_HEIGHT);

    int temph=m.height;
    int tempw=m.width;
    m.initialMaze(temph,tempw);
    m.CreateMaze();
    m.setCharacterPos();
    m.setExitPos();
    m.autoFindPath();
    StytleNum=SelectMapStytle->currentIndex();
    ShowMaze_Layout();
    lastheight=temph;
    lastwidth=tempw;
    m.last_height=m.height;
    AIAnimationButton->setEnabled(true);
    AutoMoveButton->setEnabled(true);
    quitButton->setEnabled(true);
    isPlay=true;
}

//布局管理器方式，大小可以随着窗口改变
void MainWindow::ShowMaze_Layout()
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
    gLayout_Map->addWidget(People,m.x,m.y);
    People->setStyleSheet(m.MapStytle[StytleNum][0]);
    People->show();//用就显示
    for(int i=0;i<m.height;i++)
    {
        for(int j=0;j<m.width;j++)
        {
            MazeWidget[i][j]->setMinimumSize(BASIC_WIDTH,BASIC_HEIGHT);
            MazeWidget[i][j]->show();//要用的窗口必须要显示出来
            if(m.Matrix[i][j].state == 0)//状态0 代表 墙 障碍
                MazeWidget[i][j]->setStyleSheet(m.MapStytle[StytleNum][12]);
            if(m.Matrix[i][j].state == 1)//状态1 代表 草地 非障碍
                MazeWidget[i][j]->setStyleSheet(m.MapStytle[StytleNum][13]);
        }
    }
    MazeWidget[m.exit_x][m.exit_y]->setStyleSheet(m.MapStytle[StytleNum][14]);
    isShow=true;
}
void MainWindow::hideMaze()
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
void MainWindow::quit()
{
    Controlwidget->hide();
    hideMaze();
    resetMaze();
    surface->isok=false;

    surface->show();//开始界面显示后，主窗口大小还是不变
    this->resize(840,680);//需要重新设置主窗口大小

    surface->showMianMenu();
    issurface=true;//标识主界面显示过
}
void MainWindow::ShowDungeon()
{
    BASIC_WIDTH=840/16;
    BASIC_HEIGHT=680/12;
    if(issurface)
    {
        surface->hide();
        gLayout_Map->removeWidget(surface);
        issurface=false;
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
    gLayout_Map->addWidget(People,d.x,d.y);
    People->setStyleSheet(d.DungeonStytle[38][0]);
    People->show();//用就显示

    for(int i=0;i<12;i++)
    {
        for(int j=0;j<16;j++)
        {
            MazeWidget[i][j]->setMinimumSize(BASIC_WIDTH,BASIC_HEIGHT);
            MazeWidget[i][j]->show();//要用的窗口必须要显示出来
            MazeWidget[i][j]->setStyleSheet(d.DungeonStytle[d.map[d.floor-1][i][j]][0]);
        }
    }
    isShow=true;
    dynamicEffectTimer->start(500);

    BGM->play();
    BGMTimer->start(10);
}
void MainWindow::hideDungeon()
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
    BGM->stop();
    BGMTimer->stop();
}

void MainWindow::Move()
{
    switch(moveDirection)
    {
    case 0://up
    {
        d.x-=1;
        d.map[d.floor-1][d.x][d.y]=1;
        MazeWidget[d.x][d.y]->setStyleSheet(d.DungeonStytle[1][0]);
        gLayout_Map->addWidget(People,d.x,d.y);
        People->setStyleSheet(d.DungeonStytle[37][0]);
    }
        break;
    case 1://down
    {
        d.x+=1;
        d.map[d.floor-1][d.x][d.y]=1;
        MazeWidget[d.x][d.y]->setStyleSheet(d.DungeonStytle[1][0]);
        gLayout_Map->addWidget(People,d.x,d.y);
        People->setStyleSheet(d.DungeonStytle[38][0]);
    }
        break;
    case 2://left
    {
        d.y-=1;
        d.map[d.floor-1][d.x][d.y]=1;
        MazeWidget[d.x][d.y]->setStyleSheet(d.DungeonStytle[1][0]);
        gLayout_Map->addWidget(People,d.x,d.y);
        People->setStyleSheet(d.DungeonStytle[39][moveNum%4]);
    }
        break;
    case 3://right
    {
        d.y+=1;
        d.map[d.floor-1][d.x][d.y]=1;
        MazeWidget[d.x][d.y]->setStyleSheet(d.DungeonStytle[1][0]);
        gLayout_Map->addWidget(People,d.x,d.y);
        People->setStyleSheet(d.DungeonStytle[40][moveNum%4]);
    }
        break;
    }
    moveNum++;
}
void MainWindow::changeHP(int num)
{
    if(d.role.hp+num>=d.MAX_HP)
    {
        d.role.hp=d.MAX_HP;
    }
    else
    {
        d.role.hp+=num;
    }
}
void MainWindow::QuestionBox(int num)
{
    int temp1=0;
    int temp2=0;
    if(num==0)
    {
        temp1=d.x-1;
        temp2=d.y;
    }
    else if(num==1)
    {
        temp1=d.x+1;
        temp2=d.y;
    }
    else if(num==2)
    {
        temp1=d.x;
        temp2=d.y-1;
    }
    else if(num==3)
    {
        temp1=d.x;
        temp2=d.y+1;

    }
    QMessageBox message(QMessageBox::Information,"发现箱子！","箱子上写着问号，不知道里面装着什么，是否打开？",QMessageBox::Yes|QMessageBox::No,this);
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
                d.map[d.floor-1][temp1][temp2]=1;
                MazeWidget[temp1][temp2]->setStyleSheet(d.DungeonStytle[1][0]);
                return;
            }
               break;
            case 9:
            {
                itemNum=9;
                GetitemTimer->start(100);
                d.role.mp+=50;
                updateStatusData();
                d.map[d.floor-1][temp1][temp2]=1;
                MazeWidget[temp1][temp2]->setStyleSheet(d.DungeonStytle[1][0]);
                return;
            }
                break;
            case 10:
            {
                itemNum=10;
                GetitemTimer->start(100);
                d.MAX_HP+=100;
                changeHP(100);
                updateStatusData();
                d.map[d.floor-1][temp1][temp2]=1;
                MazeWidget[temp1][temp2]->setStyleSheet(d.DungeonStytle[1][0]);
                return;
            }
                break;
            case 11:
            {
                QMessageBox mes(QMessageBox::NoIcon, "你打开了箱子", "突然从箱子里跳出一只绿鸭子并冲向你，你不得不与之战斗！！！");
                mes.setWindowIcon(QIcon(":/info/image/information/提示.ico"));
                mes.setIconPixmap(QPixmap(":/info/image/information/greenDuck.png"));
                mes.exec();

                if(d.pk(d.enemy[2])==-1||d.pk(d.enemy[2])==-2||d.pk(d.enemy[2])==1)
                {
                    itemNum=0;
                    GetitemTimer->start(100);
                    d.map[d.floor-1][temp1][temp2]=11;
                    MazeWidget[temp1][temp2]->setStyleSheet(d.DungeonStytle[11][0]);
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

                if(d.pk(d.enemy[3])==-1||d.pk(d.enemy[3])==-2||d.pk(d.enemy[3])==1)
                {
                    itemNum=0;
                    GetitemTimer->start(100);
                    d.map[d.floor-1][temp1][temp2]=12;
                    MazeWidget[temp1][temp2]->setStyleSheet(d.DungeonStytle[12][0]);
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
void MainWindow::items(int _itemNum, int _moveDirection)
{
    QLabel *target=new QLabel;
    int *tempNum=nullptr;
    switch(_itemNum)
    {
    case 8:
    {
      changeHP(100);
      hp->setText(QString::number(d.role.hp));
      soundPlay(5);
    }
        break;
    case 9:
    {
       d.role.mp+=50;
       mp->setText(QString::number(d.role.mp));
       soundPlay(5);
    }
        break;
    case 10:
    {
       d.MAX_HP+=100;
       changeHP(100);
       hp->setText(QString::number(d.role.hp));
       soundPlay(5);
    }
        break;
    case 19:
    {
        target=redkeyNum;
        tempNum=&(d.role.redkey);
        soundPlay(3);
    }
        break;
    case 20:
    {
        target=yellowkeyNum;
        tempNum=&(d.role.yellowkey);
        soundPlay(3);
    }
        break;
    case 21:
    {
        target=purplekeyNum;
        tempNum=&(d.role.purplekey);
        soundPlay(3);
    }
        break;
    case 22:
    {
        d.role.def+=3;
        def->setText(QString::number(d.role.def));
        soundPlay(3);
    }
        break;
    case 23:
    {
        d.role.atk+=2;
        atk->setText(QString::number(d.role.atk));
        soundPlay(3);
    }
        break;
    case 24:
    {
        d.role.atk+=10;
        atk->setText(QString::number(d.role.atk));
        soundPlay(1);
    }
        break;
    case 28:
    {
        d.role.atk+=25;
        atk->setText(QString::number(d.role.atk));
        soundPlay(1);
    }
        break;
    case 29:
    {
        d.role.atk+=36;
        atk->setText(QString::number(d.role.atk));
        soundPlay(1);
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
        target->setText(intToQString(*tempNum));
    }
}
void MainWindow::Monsters(int _MonsterNum,int _moveDirection)
{
    if(d.pk(d.enemy[_MonsterNum])==-1)
    {
        itemNum=-1;
        GetitemTimer->start(100);
    }
    else if(d.pk(d.enemy[_MonsterNum])==-2)
    {
        itemNum=-2;
        GetitemTimer->start(100);
    }
    else if(d.pk(d.enemy[_MonsterNum])==1)
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
void MainWindow::doors(int _doorNum, int _moveDirection)
{

    QLabel *target=new QLabel;
    int *tempNum=nullptr;
    switch(_doorNum)
    {
    case 16:
    {
        target=redkeyNum;
        tempNum=&(d.role.redkey);
    }
        break;
    case 17:
    {
        target=yellowkeyNum;
        tempNum=&(d.role.yellowkey);
    }
        break;
    case 18:
    {
        target=purplekeyNum;
        tempNum=&(d.role.purplekey);
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
        target->setText(intToQString(*tempNum));
        soundPlay(6);
    }
}

void MainWindow::checkPrefloor()
{
    if(d.floor==1)
    {
        QMessageBox message(QMessageBox::NoIcon, "对不起！", "你不能走出地牢！");
        message.setIconPixmap(QPixmap(":/info/image/information/sorry.png"));
        message.exec();
        return;
    }
    d.isPre=true;
    d.initialCharacterPos();
    People->hide();
    d.floor-=1;
    Floor->setText(intToQString(d.floor));
    ShowDungeon();
    soundPlay(2);//放在尾部才会音效持久？？？
}

void MainWindow::checkNextfloor()
{
    if(d.floor==d.Total_Floor)
    {
        QMessageBox message(QMessageBox::NoIcon, "你想干啥？", "这是地牢最后一层了！");
        message.setIconPixmap(QPixmap(":/info/image/information/question.png"));
        message.exec();
        return;
    }
    d.isNext=true;
    d.initialCharacterPos();
    People->hide();
    d.floor+=1;
    Floor->setText(intToQString(d.floor));
    ShowDungeon();
    soundPlay(2);
}
void MainWindow::OpenStore()
{
    if(d.role.scoreNum-Store->Currstore_price<0)//金币不够，不能购买
    {
        itemNum=35;//提示玩家
        GetitemTimer->start(100);
        Store->haveBuyTimer->stop();
    }
    else
    {
        d.role.scoreNum-=Store->Currstore_price;
        if(Store->chooseOption==0)
        {
            d.MAX_HP+=100 * (Store->BuyTimes-1);
            d.role.hp+=100 * (Store->BuyTimes-1);
            hp->setText(QString::number(d.role.hp));
            score->setText(QString::number(d.role.scoreNum));
            if(d.role.scoreNum-Store->Nextstore_price<0)//判断下一次是否可以购买
            {
                Store->setChooseEnable=false;
            }
            Store->haveBuyTimer->stop();//购买完成
        }
        else if(Store->chooseOption==1)
        {
            d.role.mp+=20 * (Store->BuyTimes-1);
            mp->setText(QString::number(d.role.mp));
            score->setText(QString::number(d.role.scoreNum));

            if(d.role.scoreNum-Store->Nextstore_price<0)
            {
                Store->setChooseEnable=false;
            }
            Store->haveBuyTimer->stop();
        }
        else if(Store->chooseOption==2)
        {
            d.role.atk+=2;
            atk->setText(QString::number(d.role.atk));
            score->setText(QString::number(d.role.scoreNum));

            if(d.role.scoreNum-Store->Nextstore_price<0)
            {
                Store->setChooseEnable=false;
            }
            Store->haveBuyTimer->stop();
        }
        else if(Store->chooseOption==3)
        {
            d.role.def+=4;
            def->setText(QString::number(d.role.def));
            score->setText(QString::number(d.role.scoreNum));

            if(d.role.scoreNum-Store->Nextstore_price<0)
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
void MainWindow::ReturnMainMenuPlay()//返回主菜单
{
    infoWidget->hide();//隐藏状态界面
    hideDungeon();//隐藏地牢界面
    surface->isok=false;
    surface->BGM->play();
    surface->BGMTimer->start(10);
    NameEdit->clear();
    surface->show();//开始界面显示后，主窗口大小还是不变
    this->resize(840,680);//需要重新设置主窗口大小

    surface->showMianMenu();
    issurface=true;//标识主界面显示过
    d.initialMap();
    updateStatusData();
}
void MainWindow::RestartPlay()//重新开始
{
    d.initialMap();
    ShowDungeon();
    updateStatusData();
}

//角色跳跃方式移动(结合布局管理器)
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(surface->classicalisok)//经典模式
    {
        //qDebug()<<"Key is active!";
        if(!isPlay)
        {
            return;
        }
        int step=0;
        //可以作为调节速度的接口
        //while(step<100000000)
        while(step<1000)
        {
           step++;
        }
        switch(event->key())
        {
        case Qt::Key_W://上
            //上面为墙，则什么也不执行
            if(m.Matrix[m.x-1][m.y].state==0)
            {
                return;
            }
            m.x-=1;
            gLayout_Map->addWidget(People,m.x,m.y);
            People->setStyleSheet(m.MapStytle[StytleNum][3]);//显示向上移动图片
            break;
        case Qt::Key_S://下
            if(m.Matrix[m.x+1][m.y].state==0)
            {
               return;
            }
            m.x+=1;
            gLayout_Map->addWidget(People,m.x,m.y);
            People->setStyleSheet(m.MapStytle[StytleNum][0]);
            break;
        case Qt::Key_A://左
            if(m.Matrix[m.x][m.y-1].state==0)
            {
                return;
            }
            m.y-=1;
            gLayout_Map->addWidget(People,m.x,m.y);
            People->setStyleSheet(m.MapStytle[StytleNum][6]);
            //qDebug()<<"width "<<People->width()<<"height "<<People->height();
            break;
        case Qt::Key_D://右
            if(m.Matrix[m.x][m.y+1].state==0)
            {
                return;
            }
            m.y+=1;
            gLayout_Map->addWidget(People,m.x,m.y);
            People->setStyleSheet(m.MapStytle[StytleNum][9]);
            break;
        }
        if(m.x==m.exit_x&&m.y==m.exit_y)
        {
            QMessageBox message(QMessageBox::NoIcon, "恭喜你！", "进入迷宫下一层！");
            message.setIconPixmap(QPixmap(":/info/image/information/congratulation.png"));
            message.exec();//不加这个对话框会一闪而过
            CreateMaze_Layout();
            return;
        }
    }
    else if(surface->Dungeonisok)//地牢模式
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
        int step=0;
        while(step<1000)
        {
            step++;
        }
        switch(event->key())
        {
/********************************************************************************************************/
/***************************************    上     ******************************************************/
/********************************************************************************************************/
          case Qt::Key_W://上
            if(d.x>0&&d.map[d.floor-1][d.x-1][d.y]==0)
            {
                return;
            }
            else if(d.x>0&&d.map[d.floor-1][d.x-1][d.y]==1)
            {
                moveDirection=0;
                Move();
            }
            else if(d.x>0&&d.map[d.floor-1][d.x-1][d.y]==2)
            {
                moveDirection=0;
                Move();
                d.role.scoreNum+=10;
                score->setText(intToQString(d.role.scoreNum));
                soundPlay(0);
            }
            else if(d.x>0&&d.map[d.floor-1][d.x-1][d.y]==3)
            {
                Monsters(0,0);
            }
            else if(d.x>0&&d.map[d.floor-1][d.x-1][d.y]==4)
            {
                Monsters(1,0);
            }
            else if(d.x>0&&d.map[d.floor-1][d.x-1][d.y]==5)
            {
               QuestionBox(0);
            }
            else if(d.x>0&&d.map[d.floor-1][d.x-1][d.y]==8)
            {
                items(8,0);
            }
            else if(d.x>0&&d.map[d.floor-1][d.x-1][d.y]==9)
            {
                items(9,0);
            }
            else if(d.x>0&&d.map[d.floor-1][d.x-1][d.y]==10)
            {
                items(10,0);
            }
            else if(d.x>0&&d.map[d.floor-1][d.x-1][d.y]==11)
            {
                Monsters(2,0);
            }
            else if(d.x>0&&d.map[d.floor-1][d.x-1][d.y]==12)
            {
                Monsters(3,0);
            }
            else if(d.x>0&&d.map[d.floor-1][d.x-1][d.y]==14)
            {
                checkPrefloor();
            }
            else if(d.x>0&&d.map[d.floor-1][d.x-1][d.y]==15)
            {
                checkNextfloor();
            }
            else if(d.x>0&&d.map[d.floor-1][d.x-1][d.y]==16)
            {
                doors(16,0);
            }
            else if(d.x>0&&d.map[d.floor-1][d.x-1][d.y]==17)
            {
                doors(17,0);
            }
            else if(d.x>0&&d.map[d.floor-1][d.x-1][d.y]==18)
            {
                doors(18,0);
            }
            else if(d.x>0&&d.map[d.floor-1][d.x-1][d.y]==19)
            {
                items(19,0);
            }
            else if(d.x>0&&d.map[d.floor-1][d.x-1][d.y]==20)
            {
                items(20,0);
            }
            else if(d.x>0&&d.map[d.floor-1][d.x-1][d.y]==21)
            {
                items(21,0);
            }
            else if(d.x>0&&d.map[d.floor-1][d.x-1][d.y]==22)
            {
                items(22,0);
            }
            else if(d.x>0&&d.map[d.floor-1][d.x-1][d.y]==23)
            {
                items(23,0);
            }
            else if(d.x>0&&d.map[d.floor-1][d.x-1][d.y]==25)
            {
                Monsters(4,0);
            }
            else if(d.x>0&&d.map[d.floor-1][d.x-1][d.y]==26)
            {
                Monsters(5,0);
            }
            else if(d.x>0&&d.map[d.floor-1][d.x-1][d.y]==27)
            {
                Monsters(6,0);
            }
            else if(d.x>0&&d.map[d.floor-1][d.x-1][d.y]==28)
            {
                items(28,0);
            }
            else if(d.x>0&&d.map[d.floor-1][d.x-1][d.y]==29)
            {
                items(29,0);
            }
            else if(d.x>0&&d.map[d.floor-1][d.x-1][d.y]==35)
            {
                Store->show();
                if(d.role.scoreNum-Store->Currstore_price>=0)
                {
                    Store->setChooseEnable=true;
                }
                Store->StoreWinisShow=true;
            }
            else
            {
                return;
            }
            break;
/********************************************************************************************************/
/***************************************    下     ******************************************************/
/********************************************************************************************************/
        case Qt::Key_S://下
            if(d.x<11&&d.map[d.floor-1][d.x+1][d.y]==0)
            {
                return;
            }
            else if(d.x<11&&d.map[d.floor-1][d.x+1][d.y]==1)
            {
                moveDirection=1;
                Move();
            }
            else if(d.x<11&&d.map[d.floor-1][d.x+1][d.y]==2)
            {
                moveDirection=1;
                Move();
                d.role.scoreNum+=10;
                score->setText(intToQString(d.role.scoreNum));
                soundPlay(0);
            }
            else if(d.x<11&&d.map[d.floor-1][d.x+1][d.y]==3)
            {
                Monsters(0,1);
            }
            else if(d.x<11&&d.map[d.floor-1][d.x+1][d.y]==4)
            {
                Monsters(1,1);
            }
            else if(d.x<11&&d.map[d.floor-1][d.x+1][d.y]==5)
            {
               QuestionBox(1);
            }
            else if(d.x<11&&d.map[d.floor-1][d.x+1][d.y]==8)
            {
                items(8,1);
            }
            else if(d.x<11&&d.map[d.floor-1][d.x+1][d.y]==9)
            {
                items(9,1);
            }
            else if(d.x<11&&d.map[d.floor-1][d.x+1][d.y]==10)
            {
                items(10,1);
            }
            else if(d.x<11&&d.map[d.floor-1][d.x+1][d.y]==11)
            {
                Monsters(2,1);
            }
            else if(d.x<11&&d.map[d.floor-1][d.x+1][d.y]==12)
            {
                Monsters(3,1);
            }
            else if(d.x<11&&d.map[d.floor-1][d.x+1][d.y]==14)
            {

                checkPrefloor();
            }
            else if(d.x<11&&d.map[d.floor-1][d.x+1][d.y]==15)
            {
                checkNextfloor();
            }
            else if(d.x<11&&d.map[d.floor-1][d.x+1][d.y]==16)
            {
                doors(16,1);
            }
            else if(d.x<11&&d.map[d.floor-1][d.x+1][d.y]==17)
            {
               doors(17,1);
            }
            else if(d.x<11&&d.map[d.floor-1][d.x+1][d.y]==18)
            {
                doors(18,1);
            }
            else if(d.x<11&&d.map[d.floor-1][d.x+1][d.y]==19)
            {
                items(19,1);
            }
            else if(d.x<11&&d.map[d.floor-1][d.x+1][d.y]==20)
            {
                items(20,1);
            }
            else if(d.x<11&&d.map[d.floor-1][d.x+1][d.y]==21)
            {
                items(21,1);
            }
            else if(d.x<11&&d.map[d.floor-1][d.x+1][d.y]==22)
            {
                items(22,1);
            }
            else if(d.x<11&&d.map[d.floor-1][d.x+1][d.y]==23)
            {
                items(23,1);
            }
            else if(d.x<11&&d.map[d.floor-1][d.x+1][d.y]==24)
            {
                items(24,1);
            }
            else if(d.x<11&&d.map[d.floor-1][d.x+1][d.y]==25)
            {
                Monsters(4,1);
            }
            else if(d.x<11&&d.map[d.floor-1][d.x+1][d.y]==26)
            {
                Monsters(5,1);
            }
            else if(d.x<11&&d.map[d.floor-1][d.x+1][d.y]==27)
            {
                Monsters(6,1);
            }
            else if(d.x<11&&d.map[d.floor-1][d.x+1][d.y]==28)
            {
                items(28,1);
            }
            else if(d.x<11&&d.map[d.floor-1][d.x+1][d.y]==29)
            {
                items(29,1);
            }
            else
            {
                return;
            }
            break;
/********************************************************************************************************/
/***************************************    左     ******************************************************/
/********************************************************************************************************/
        case Qt::Key_A://左
            if(d.y>0&&d.map[d.floor-1][d.x][d.y-1]==0)
            {
                return;
            }
            else if(d.y>0&&d.map[d.floor-1][d.x][d.y-1]==1)
            {
                moveDirection=2;
                Move();
            }
            else if(d.y>0&&d.map[d.floor-1][d.x][d.y-1]==2)
            {
                moveDirection=2;
                Move();
                d.role.scoreNum+=10;
                score->setText(intToQString(d.role.scoreNum));
                soundPlay(0);
            }
            else if(d.y>0&&d.map[d.floor-1][d.x][d.y-1]==3)
            {
                Monsters(0,2);
            }
            else if(d.y>0&&d.map[d.floor-1][d.x][d.y-1]==4)
            {
                Monsters(1,2);
            }
            else if(d.y>0&&d.map[d.floor-1][d.x][d.y-1]==5)
            {
               QuestionBox(2);
            }
            else if(d.y>0&&d.map[d.floor-1][d.x][d.y-1]==8)
            {
                items(8,2);
            }
            else if(d.y>0&&d.map[d.floor-1][d.x][d.y-1]==9)
            {
                items(9,2);
            }
            else if(d.y>0&&d.map[d.floor-1][d.x][d.y-1]==10)
            {
                items(10,2);
            }
            else if(d.y>0&&d.map[d.floor-1][d.x][d.y-1]==11)
            {
                Monsters(2,2);
            }
            else if(d.y>0&&d.map[d.floor-1][d.x][d.y-1]==12)
            {
                Monsters(3,2);
            }
            else if(d.y>0&&d.map[d.floor-1][d.x][d.y-1]==14)
            {
               checkPrefloor();
            }
            else if(d.y>0&&d.map[d.floor-1][d.x][d.y-1]==15)
            {
               checkNextfloor();
            }
            else if(d.y>0&&d.map[d.floor-1][d.x][d.y-1]==16)
            {
                doors(16,2);
            }
            else if(d.y>0&&d.map[d.floor-1][d.x][d.y-1]==17)
            {
                doors(17,2);
            }
            else if(d.y>0&&d.map[d.floor-1][d.x][d.y-1]==18)
            {
                doors(18,2);
            }
            else if(d.y>0&&d.map[d.floor-1][d.x][d.y-1]==19)
            {
                items(19,2);

            }
            else if(d.y>0&&d.map[d.floor-1][d.x][d.y-1]==20)
            {
                items(20,2);
            }
            else if(d.y>0&&d.map[d.floor-1][d.x][d.y-1]==21)
            {
                items(20,2);
            }
            else if(d.y>0&&d.map[d.floor-1][d.x][d.y-1]==22)
            {
                items(22,2);
            }
            else if(d.y>0&&d.map[d.floor-1][d.x][d.y-1]==23)
            {
                items(23,2);
            }
            else if(d.y>0&&d.map[d.floor-1][d.x][d.y-1]==25)
            {
                Monsters(4,2);
            }
            else if(d.y>0&&d.map[d.floor-1][d.x][d.y-1]==26)
            {
                Monsters(5,2);
            }
            else if(d.y>0&&d.map[d.floor-1][d.x][d.y-1]==27)
            {
                Monsters(6,2);
            }
            else if(d.y>0&&d.map[d.floor-1][d.x][d.y-1]==28)
            {
                items(28,2);
            }
            else if(d.y>0&&d.map[d.floor-1][d.x][d.y-1]==29)
            {
                items(29,2);
            }
            else
            {
                return;
            }
            break;
/********************************************************************************************************/
/***************************************    右     ******************************************************/
/********************************************************************************************************/
        case Qt::Key_D:
            if(d.y<15&&d.map[d.floor-1][d.x][d.y+1]==0)
            {
                return;
            }
            else if(d.y<15&&d.map[d.floor-1][d.x][d.y+1]==1)
            {
                moveDirection=3;
                Move();
            }
            else if(d.y<15&&d.map[d.floor-1][d.x][d.y+1]==2)
            {
                moveDirection=3;
                Move();
                d.role.scoreNum+=10;
                score->setText(intToQString(d.role.scoreNum));
                soundPlay(0);
            }
            else if(d.y<15&&d.map[d.floor-1][d.x][d.y+1]==3)
            {
                Monsters(0,3);
            }
            else if(d.y<15&&d.map[d.floor-1][d.x][d.y+1]==4)
            {
                Monsters(1,3);
            }
            else if(d.y<15&&d.map[d.floor-1][d.x][d.y+1]==5)
            {
               QuestionBox(3);
            }
            else if(d.y<15&&d.map[d.floor-1][d.x][d.y+1]==8)
            {
                items(8,3);
            }
            else if(d.y<15&&d.map[d.floor-1][d.x][d.y+1]==9)
            {
                items(9,3);
            }
            else if(d.y<15&&d.map[d.floor-1][d.x][d.y+1]==10)
            {
                items(10,3);
            }
            else if(d.y<15&&d.map[d.floor-1][d.x][d.y+1]==11)
            {
                Monsters(2,3);
            }
            else if(d.y<15&&d.map[d.floor-1][d.x][d.y+1]==12)
            {
                Monsters(3,3);
            }
            else if(d.y<15&&d.map[d.floor-1][d.x][d.y+1]==14)
            {
               checkPrefloor();
            }
            else if(d.y<15&&d.map[d.floor-1][d.x][d.y+1]==15)
            {
                checkNextfloor();
            }
            else if(d.y<15&&d.map[d.floor-1][d.x][d.y+1]==16)
            {
                doors(16,3);
            }
            else if(d.y<15&&d.map[d.floor-1][d.x][d.y+1]==17)
            {
                doors(17,3);
            }
            else if(d.y<15&&d.map[d.floor-1][d.x][d.y+1]==18)
            {
                doors(18,3);
            }
            else if(d.y<15&&d.map[d.floor-1][d.x][d.y+1]==19)
            {
                items(19,3);
            }
            else if(d.y<15&&d.map[d.floor-1][d.x][d.y+1]==20)
            {
                items(20,3);
            }
            else if(d.y<15&&d.map[d.floor-1][d.x][d.y+1]==21)
            {
                items(21,3);
            }
            else if(d.y<15&&d.map[d.floor-1][d.x][d.y+1]==22)
            {
                items(22,3);
            }
            else if(d.y<15&&d.map[d.floor-1][d.x][d.y+1]==23)
            {
                items(23,3);
            }
            else if(d.y<15&&d.map[d.floor-1][d.x][d.y+1]==24)
            {
                items(24,3);
            }
            else if(d.y<15&&d.map[d.floor-1][d.x][d.y+1]==25)
            {
                Monsters(4,3);
            }
            else if(d.y<15&&d.map[d.floor-1][d.x][d.y+1]==26)
            {
                Monsters(5,3);
            }
            else if(d.y<15&&d.map[d.floor-1][d.x][d.y+1]==27)
            {
                Monsters(6,3);
            }
            else if(d.y<15&&d.map[d.floor-1][d.x][d.y+1]==28)
            {
                items(28,3);
            }
            else if(d.y<15&&d.map[d.floor-1][d.x][d.y+1]==29)
            {
                items(29,3);
            }
            else
            {
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

void MainWindow::ShowAnimation()
{
    group->clear();//动画组清空
    isAIAnimationButton=true;
    isAutoMoveButton=false;
    AutoMoveButton->setEnabled(false);
    for(int i=0; i<m.PathStack.size()-1;i++)
    {
        QPropertyAnimation* animation = new QPropertyAnimation(People, "pos");//动作初始化,People是操作对象
        animation->setDuration(200);//设置动作间隔                              //"pos"是操作属性，与QPoint 对应
                                                                              //"geometry" 与QRect/QRectF 对应
        animation->setStartValue(QPoint(m.PathStack[i].j*BASIC_WIDTH,m.PathStack[i].i*BASIC_HEIGHT));
        animation->setEndValue(QPoint(m.PathStack[i+1].j*BASIC_WIDTH,m.PathStack[i+1].i*BASIC_HEIGHT));
        animation->setEasingCurve(QEasingCurve::Linear);
        group->addAnimation(animation);//向动画组中添加动作
    }
    timer->start(200);//设置计时间隔，必须与动作间隔大小一样，才会同步
    group->start();//动画组启用
}

void MainWindow::timeStart()
{
    isAutoMoveButton=true;
    isAIAnimationButton=false;
    AIAnimationButton->setEnabled(false);
    timer->start(50);
}
void MainWindow::MapStytleSet()
{
   label_Stytle->setStyleSheet(m.MapStytle[SelectMapStytle->currentIndex()][15]);

}
void MainWindow::ShowWidget()
{
    if(surface->isok)
    {
        if(surface->classicalisok)
        {
            Controlwidget->show();
        }
        else if(surface->Dungeonisok)
        {
            NameEdit->show();
            SetDone->show();
        }
    }
}
void MainWindow::GetitemWinshow()
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
bool MainWindow::isDynamic(int i,int j)
{
    if(d.map[d.floor-1][i][j]==2||d.map[d.floor-1][i][j]==3||d.map[d.floor-1][i][j]==4||
            d.map[d.floor-1][i][j]==5||d.map[d.floor-1][i][j]==11||d.map[d.floor-1][i][j]==12||
            d.map[d.floor-1][i][j]==25||d.map[d.floor-1][i][j]==26||d.map[d.floor-1][i][j]==27)
        return true;
    else
        return false;
}
void MainWindow::ShowdynamicEffect()
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
             MazeWidget[i][j]->setStyleSheet(d.DungeonStytle[d.map[d.floor-1][i][j]][display_it]);
            }
            else
            {
              MazeWidget[i][j]->setStyleSheet(d.DungeonStytle[d.map[d.floor-1][i][j]][0]);
            }
        }
    }

}
void MainWindow::CheckBGMstate()
{
   if(BGM->state()==0)//0代表停止 1代表播放 2代表暂停
   {
       BGM->play();
   }
}
void MainWindow::EnterDungeon()
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
        surface->BGM->stop();
        surface->BGMTimer->stop();
    }
}
void MainWindow::soundPlay(int CurrentIndex)
{
    soundlist->setCurrentIndex(CurrentIndex);
    sound->play();
    if(CurrentIndex==3)
    {
       soundTimer->start(400);
    }
    else
    {
        soundTimer->start(1000);
    }
}
void MainWindow::soundStop()
{
    if(soundlist->currentIndex()!=4)
    {
        sound->stop();
    }
    soundTimer->stop();
}
void MainWindow::updateStatusData()
{
    if(d.role.exp>=100*d.role.level)
    {
        d.MAX_HP+=d.role.level*50;
        d.role.hp+=d.role.level*50;
        d.role.mp+=d.role.level*5;
        d.role.atk+=3;
        d.role.def+=2;
        d.role.level+=1;
        d.role.exp=0;
        itemNum=-3;
        GetitemTimer->start(100);
        soundPlay(1);
    }
    score->setText(intToQString(d.role.scoreNum));
    level->setText(intToQString(d.role.level));
    exp->setText(intToQString(d.role.exp));
    hp->setText(intToQString(d.role.hp));
    mp->setText(intToQString(d.role.mp));
    atk->setText(intToQString(d.role.atk));
    def->setText(intToQString(d.role.def));
    yellowkeyNum->setText(intToQString(d.role.yellowkey));
    purplekeyNum->setText(intToQString(d.role.purplekey));
    redkeyNum->setText(intToQString(d.role.redkey));
    Floor->setText(intToQString(d.floor));
}
void MainWindow::FightWinshow()//战斗界面
{
    if (fight_period_it == 0)
    {
        isFighting=true;
        //准备战斗界面
        Monster_hp->setText(QString::number(d.enemy[MonsterNum].hp));
        Monster_atk->setText(QString::number(d.enemy[MonsterNum].atk));
        Monster_def->setText(QString::number(d.enemy[MonsterNum].def));
        Character_hp->setText(QString::number(d.role.hp));
        Character_atk->setText(QString::number(d.role.atk));
        Character_def->setText(QString::number(d.role.def));
        FightWidget->show();
        Monster_pic->setStyleSheet(d.MonsterPic[MonsterNum]);
        Monster_name->setText(d.enemy[MonsterNum].name);
        fight_period_it = 1;

        soundlist->setCurrentIndex(4);
        sound->play();
    }//fight_period_it 代表对战次数，奇数次计算任务对怪物造成的伤害，偶数次计算怪物对人造成的伤害
    else if (fight_period_it % 2 == 1 && fight_end_it == 0)
    {
        if (d.enemy[MonsterNum].hp - (d.role.atk - d.enemy[MonsterNum].def) * (fight_period_it / 2 + 1) <= 0)
        {
            Monster_hp->setText(QString::number(0));//fight_period_it / 2 + 1 代表人物对怪物攻击的累计次数
            fight_end_it = 1;
        }
        else
        {
            Monster_hp->setText(QString::number(d.enemy[MonsterNum].hp - (d.role.atk - d.enemy[MonsterNum].def) * (fight_period_it / 2 + 1)));
            fight_period_it++;
        }
    }
    else if (fight_period_it % 2 == 0 && fight_end_it == 0)
    {
        if (d.enemy[MonsterNum].atk > d.role.def)
        {
            Character_hp->setText(QString::number(d.role.hp - (d.enemy[MonsterNum].atk - d.role.def) * (fight_period_it / 2)));
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
            int damage = d.calc_damage(MonsterNum);
            d.role.hp -= damage;
            d.role.scoreNum += d.enemy[MonsterNum].gold;
            d.role.exp += d.enemy[MonsterNum].exp;
            //隐藏战斗界面
            FightWidget->hide();
            GetitemsShow->setText(QString::fromWCharArray(L"获得经验值 ") + QString::number(d.enemy[MonsterNum].exp) + QString::fromWCharArray(L" 金币 ") + QString::number(d.enemy[MonsterNum].gold));
            GetitemsShow->show();
            sound->stop();
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
void MainWindow::ShowPath()
{
    if(!isAIAnimationButton)
    {
        point temp=m.PathStack[iNum];
        MazeWidget[temp.i][temp.j]->setStyleSheet(m.MapStytle[StytleNum][0]);
        iNum++;
        if(iNum == m.PathStack.size()-1)
        {
            iNum = 0;
            for(int i=0;i<m.PathStack.size();i++)
            {
                point temp=m.PathStack[i];
                MazeWidget[temp.i][temp.j]->setStyleSheet(m.MapStytle[StytleNum][13]);
                MazeWidget[m.height-2][m.width-2]->setStyleSheet(m.MapStytle[StytleNum][14]);
            }
            timer->stop();
            AIAnimationButton->setEnabled(true);
            isAutoMoveButton=false;
            isAIAnimationButton=true;
            return;
        }

    }
}
void MainWindow::moveCharacter()//设置移动时的图片
{
    if(!isAutoMoveButton)
    {
        if(m.PathStack[iNum+1].i < m.PathStack[iNum].i)//up
        {
            People->setStyleSheet(m.MapStytle[StytleNum][3+iNum%3]);
        }
        if(m.PathStack[iNum+1].i > m.PathStack[iNum].i)//down
        {
            People->setStyleSheet(m.MapStytle[StytleNum][0+iNum%3]);
        }
        if(m.PathStack[iNum+1].j > m.PathStack[iNum].j)//right
        {
            People->setStyleSheet(m.MapStytle[StytleNum][9+iNum%3]);
        }
        if(m.PathStack[iNum+1].j < m.PathStack[iNum].j)//left
        {
            People->setStyleSheet(m.MapStytle[StytleNum][6+iNum%3]);
        }
        iNum++;
        if(iNum == m.PathStack.size()-1)
        {
            timer->stop();
            iNum = 0;
            AutoMoveButton->setEnabled(true);
            return;
        }
    }

}
