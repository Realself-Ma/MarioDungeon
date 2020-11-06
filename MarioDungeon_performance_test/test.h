#ifndef TEST_H
#define TEST_H

#include <QHostAddress>
#include <QTcpSocket>
#include <QString>
#include <QTimer>
#include <QWidget>
#include <QDebug>
#include "request.h"
extern int Room_Num;
class Test: public QWidget
{
    Q_OBJECT
public:
    Test(QWidget *parent = 0);
    void Run_Test();
    void TruncateTable();
    QTimer* updatePosTimer;
private:
    QTcpSocket* tcpSocket;
    QString *Name_arr;
    QString *PassWord_arr;
    QString *RoomName_arr;
    int playerNum;
    void ConnectToHost(QString ServerIp,int ServerPort);
    void GenStringArray();
    void Register();
    void Login();
    void CreateRoom();
    void EnterRoom();
    void ReadyToPlay();
    void GameStart();
    void MapRequest();
private slots:
    void udpkPosRequest();
};

#endif // TEST_H
