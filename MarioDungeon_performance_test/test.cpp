#include "test.h"

Test::Test(QWidget *parent):QWidget(parent)
{
    tcpSocket=new QTcpSocket();
    updatePosTimer=new QTimer();
    ConnectToHost(SERVER_IP,SERVER_PORT);
    connect(updatePosTimer,SIGNAL(timeout()),this,SLOT(udpkPosRequest()));
}
void Test::Run_Test()
{
    GenStringArray();
    Register();
    Login();
    CreateRoom();
    EnterRoom();
    ReadyToPlay();
    GameStart();
    MapRequest();
    //udpkPosRequest();
}
void Test::ConnectToHost(QString ServerIp,int ServerPort)
{
    QHostAddress* serverIP = new QHostAddress();
    QString ip = ServerIp;
    serverIP->setAddress(ip);
    tcpSocket->connectToHost(*serverIP, ServerPort);
}
void Test::GenStringArray()
{
    playerNum=Room_Num*2;
    Name_arr=new QString[playerNum];
    PassWord_arr=new QString[playerNum];
    RoomName_arr=new QString[Room_Num];
}
void Test::Register()
{
    for(int i=0;i<playerNum;++i)
    {
        Name_arr[i]=("测试"+QString::number(i));
        PassWord_arr[i]=("ceshi"+QString::number(i));
    }
    for(int i=0;i<playerNum;++i)
    {
        QString msg = REGISTER;
        msg += Name_arr[i] + '\t' + PassWord_arr[i];
        msg+=ENDFLAG;
        int length = tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());
        if(length != msg.toUtf8().length())
            return;
    }
}
void Test::Login()
{
    for(int i=0;i<playerNum;++i)
    {
        QString msg=LOGIN;
        msg += Name_arr[i] + '\t' + PassWord_arr[i];
        msg+=ENDFLAG;
        int length = tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());
        if(length != msg.toUtf8().length())
            return;
    }
}
void Test::CreateRoom()
{
    for(int i=0;i<Room_Num;++i)
        RoomName_arr[i]=("测试"+QString::number(i));
    for(int i=0;i<Room_Num;++i)
    {
        QString msg = CREATEROOM;
        msg+=RoomName_arr[i]+'\t'+Name_arr[2*i];
        msg+=ENDFLAG;
        int length = tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());
        if(length != msg.toUtf8().length())
            return;
    }
}
void Test::EnterRoom()
{
    for(int i=0;i<Room_Num;++i)
    {
        QString msg = ENTERROOM;
        msg+=RoomName_arr[i]+'\t'+Name_arr[2*i];
        msg+=ENDFLAG;
        msg+= ENTERROOM;
        msg+=RoomName_arr[i]+'\t'+Name_arr[2*i+1];
        msg+=ENDFLAG;
        int length = tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());
        if(length != msg.toUtf8().length())
            return;
    }
}
void Test::ReadyToPlay()
{
    for(int i=0;i<Room_Num;++i)
    {
        QString msg = READYRQ;
        msg+=Name_arr[2*i+1]+'\t'+RoomName_arr[i]+'\t'+"ready";
        msg+=ENDFLAG;
        int length = tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());
        if(length != msg.toUtf8().length())
            return;
    }
}
void Test::GameStart()
{
    for(int i=0;i<Room_Num;++i)
    {
        QString msg = STARTRQ;
        msg+=Name_arr[2*i]+'\t'+RoomName_arr[i];
        msg+=ENDFLAG;
        int length = tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());
        if(length != msg.toUtf8().length())
            return;
    }
}
void Test::MapRequest()
{
    for(int i=0;i<playerNum;++i)
    {
        QString msg = MAPRQ;
        msg+=Name_arr[i];
        msg+=ENDFLAG;
        int length = tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());
        if(length != msg.toUtf8().length())
            return;
    }
}
void Test::udpkPosRequest()
{
    qDebug()<<"this is run";
    for(int i=0;i<playerNum;++i)
    {
        QString msg = UDPKPOSRQ;
        msg+=Name_arr[i]+'\t'+QString::number(5)+'#'+QString::number(5)+'\t'+QString::number(2);
        msg+=ENDFLAG;
        int length = tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());
        if(length != msg.toUtf8().length())
            return;
    }
}
void Test::TruncateTable()
{
    QString msg = TRUNCATETABLERQ;
    msg+=ENDFLAG;
    int length = tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());
    if(length != msg.toUtf8().length())
        return;
}
