#include "sound.h"
Sound::Sound(QWidget *parent) :
    QWidget(parent)
{
    fac=new Factory();
    InterfaceBGM=fac->CreateQMediaPlayer(this,QUrl("qrc:/music/music/interface_main_theme.mp3"),50);
    InterfaceBGM->play();
    DungeonBGM=fac->CreateQMediaPlayer(this,QUrl("qrc:/music/music/main_theme.mp3"),50);
    FightSound=fac->CreateQMediaPlayer(this,QUrl("qrc:/music/music/brick_smash.mp3"),80);
    DungeonSoundList=fac->CreateQMediaPlaylist(this);
    DungeonSound=fac->CreateQMediaPlayer(this,QUrl(""),80);
    DungeonSound->setPlaylist(DungeonSoundList);

    InterfaceBGMTimer=new QTimer(this);
    connect(InterfaceBGMTimer,SIGNAL(timeout()),this,SLOT(CheckInterfaceBGMstate()));
    InterfaceBGMTimer->start(10);

    DungeonBGMTimer=new QTimer(this);
    connect(DungeonBGMTimer,SIGNAL(timeout()),this,SLOT(CheckDungeonBGMstate()));

    FightSoundTimer=new QTimer(this);
    connect(FightSoundTimer,SIGNAL(timeout()),this,SLOT(CheckFightSoundstate()));

    DungeonSoundTimer=new QTimer(this);
    connect(DungeonSoundTimer,SIGNAL(timeout()),this,SLOT(SoundStop()));

    musicChange=true;
}
void Sound::CheckInterfaceBGMstate()
{
   if(InterfaceBGM->state()==0)//0代表停止 1代表播放 2代表暂停
   {
       InterfaceBGM->play();
   }
}
void Sound::CheckDungeonBGMstate()
{
   if(DungeonBGM->state()==0)//0代表停止 1代表播放 2代表暂停
   {
       DungeonBGM->play();
   }
}
void Sound::CheckFightSoundstate()
{
    if(FightSound->state()==0)
        FightSound->play();
}

void Sound::SoundStop()
{
    DungeonSound->stop();
    DungeonSoundTimer->stop();
}
void Sound::SoundPlay(int CurrentIndex)
{
    DungeonSoundList->setCurrentIndex(CurrentIndex);
    DungeonSound->play();
    if(CurrentIndex==3)
    {
        DungeonSoundTimer->start(400);
    }
    else if(CurrentIndex==5)
    {
        DungeonSoundTimer->start(3000);
    }
    else
    {
        DungeonSoundTimer->start(1000);
    }
}
