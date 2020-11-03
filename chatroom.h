#ifndef CHATROOM_H
#define CHATROOM_H

#include <QWidget>
#include <QListWidget>
#include <QGridLayout>
#include <QTimer>
#include "request.h"
#include "factory.h"
extern int ***map;//三维数组
extern int Total_Floor;
extern QTimer *NetStartTimer;
extern QTimer *pkStartTimer;
extern QTimer *pkMapUdTimer;
extern QTimer *FightTimer;
extern QTimer* CmptorGameOverCallTimer;
class ChatRoom : public QWidget
{
    Q_OBJECT
public:
    explicit ChatRoom(QWidget *parent = 0);
    QString playerName;
    QString owner_;
    QListWidget *contentListWidget;
    void pkRequest();
    void udpkPosRequest(int x, int y, int dir);
    void udpkStatusRequest(int hp, int atk, int def);
    void FightRequest();
    void GameOverRequest();
    QString competitoName;
    int cur_x;
    int cur_y;
    int dir;
    int hp;
    int atk;
    int def;
private:
    Factory* fac;
    QLineEdit *sendLineEdit;
    QPushButton *sendBtn;
    QTcpSocket *tcpSocket;
    std::string recv;
    unsigned int msgSize;
    bool headGet;
    bool pkStartGet;
private:
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent*);
    void hideEvent(QHideEvent*);
    void initialChatRoomWidget();
    void mapRequest();
    void initialReadyRquest();
private slots:
    void slotConnected();
    void slotSend();
    void dataReceived();
};

#endif // CHATROOM_H
