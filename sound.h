#ifndef SOUND_H
#define SOUND_H
#include "factory.h"

class Sound: public QWidget
{
    Q_OBJECT
public:
    explicit Sound(QWidget *parent = 0);
    QMediaPlayer *InterfaceBGM;
    QMediaPlayer *DungeonBGM;
    QMediaPlayer *DungeonSound;
    QMediaPlayer *FightSound;
    QMediaPlaylist *DungeonSoundList;
    QTimer *FightSoundTimer;
    QTimer *DungeonSoundTimer;
    QTimer *InterfaceBGMTimer;
    QTimer *DungeonBGMTimer;
    bool musicChange;
public:
    void SoundPlay(int CurrentIndex);
private:
    Factory* fac;
private slots:
    void CheckInterfaceBGMstate();
    void CheckDungeonBGMstate();
    void CheckFightSoundstate();
    void SoundStop();
};

#endif // SOUND_H
