#include "sound.h"
Sound::Sound(QWidget *parent) :
    QWidget(parent)
{
    fac=new Factory();
    InterfaceBGM=fac->CreateQMediaPlayer(this,QUrl("qrc:/music/music/interface_main_theme.mp3"),50);
    InterfaceBGM->play();
    DungeonBGM=fac->CreateQMediaPlayer(this,QUrl("qrc:/music/music/main_theme.mp3"),50);
    DungeonSoundList=fac->CreateQMediaPlaylist(this);
    DungeonSound=fac->CreateQMediaPlayer(this,QUrl(""),80);
    DungeonSound->setPlaylist(DungeonSoundList);

    InterfaceBGMTimer=new QTimer(this);
    connect(InterfaceBGMTimer,SIGNAL(timeout()),this,SLOT(CheckInterfaceBGMstate()));
    InterfaceBGMTimer->start(10);

    DungeonBGMTimer=new QTimer(this);
    connect(DungeonBGMTimer,SIGNAL(timeout()),this,SLOT(CheckDungeonBGMstate()));

    DungeonSoundTimer=new QTimer(this);
    connect(DungeonSoundTimer,SIGNAL(timeout()),this,SLOT(SoundStop()));

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
void Sound::SoundStop()
{
    if(DungeonSoundList->currentIndex()!=4)
    {
        DungeonSound->stop();
    }
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
    else
    {
        DungeonSoundTimer->start(1000);
    }
}
