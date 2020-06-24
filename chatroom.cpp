#include "chatroom.h"

ChatRoom::ChatRoom(QWidget *parent) : QWidget(parent)
{
    msgSize=0;
    headGet=false;
    pkStartGet=false;
    fac=new Factory();
    initialChatRoomWidget();
}
void ChatRoom::initialChatRoomWidget()
{
    contentListWidget = new QListWidget(this);
    sendLineEdit = new QLineEdit(this);
    sendBtn = new QPushButton("发送");
    QGridLayout* mainLayout = new QGridLayout(this);
    mainLayout->addWidget(contentListWidget,0,0,1,3);
    mainLayout->addWidget(sendLineEdit,1,0,1,2);
    mainLayout->addWidget(sendBtn,1,2);
    this->setLayout(mainLayout);

    connect(sendBtn,SIGNAL(clicked()),this,SLOT(slotSend()));
}
void ChatRoom::slotSend()
{
    if(sendLineEdit->text() == "")
    {
        return;
    }
    QString msg = MESSAGE;
    msg += playerName +'\t'+ ": " + sendLineEdit->text();
    msg+=ENDFLAG;
    //qDebug()<<msg;
    tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());//发送的数据转换为utf-8格式(网络字节序，中文也能识别)
    sendLineEdit->clear();
}
void ChatRoom::slotConnected()
{
    int length=0;
    QString msg = MESSAGE;
    msg += playerName +'\t'+ ": Enter ChatRoom";
    msg+=ENDFLAG;
    qDebug()<<msg;
    if((length = tcpSocket->write(msg.toUtf8(),msg.toUtf8(). length())) != msg.toUtf8().length())
    {
        return;
    }
}
void ChatRoom::mapRequest()
{
    QString msg = MAPRQ;
    msg+=playerName;
    msg+=ENDFLAG;
    int length = tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());
    if(length != msg.toUtf8().length())
        return;
}
void ChatRoom::initialReadyRquest()
{
    QString msg = INITREQDYRQ;
    msg+=playerName;
    msg+=ENDFLAG;
    int length = tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());
    if(length != msg.toUtf8().length())
        return;
}
void ChatRoom::pkRequest()
{
    pkStartGet=false;
    QString msg = PKRQ;
    msg+=playerName;
    msg+=ENDFLAG;
    int length = tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());
    if(length != msg.toUtf8().length())
        return;
}
void ChatRoom::udpkPosRequest(int x,int y,int dir)
{
    QString msg = UDPKPOSRQ;
    msg+=playerName+'\t'+QString::number(x)+'#'+QString::number(y)+'\t'+QString::number(dir);
    msg+=ENDFLAG;
    int length = tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());
    if(length != msg.toUtf8().length())
        return;
}
void ChatRoom::udpkStatusRequest(int hp, int atk, int def)
{
    QString msg = UDPKSTATUSRQ;
    msg+=playerName+'\t'+QString::number(hp)+'#'+QString::number(atk)+'\t'+QString::number(def);
    msg+=ENDFLAG;
    int length = tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());
    if(length != msg.toUtf8().length())
        return;
}
void ChatRoom::FightRequest()
{
   QString msg = FIGHTRQ;
   msg+=playerName;
   msg+=ENDFLAG;
   int length = tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());
   if(length != msg.toUtf8().length())
       return;
}
void ChatRoom::GameOverRequest()
{
    QString msg = GAMEOVERRQ;
    msg+=playerName;
    msg+=ENDFLAG;
    int length = tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());
    if(length != msg.toUtf8().length())
        return;
}
void ChatRoom::initialPlayerRequest()
{
    if(playerName!=owner_)
    {
        QString msg = INITPLAYERRQ;
        msg+=playerName;
        msg+=ENDFLAG;
        int length = tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());
        if(length != msg.toUtf8().length())
            return;
    }
}
void ChatRoom::dataReceived()
{
    QByteArray datagram;
    datagram.resize(tcpSocket->bytesAvailable());
    tcpSocket->read(datagram.data(),datagram.size());
    recv+=datagram.data();
    QString recvMessage=QString::fromStdString(recv);
    qDebug()<<"recvMessage:"<<recvMessage;
    if(!headGet)
    {
        int idx=recvMessage.indexOf('/',2);
        //qDebug()<<"recvMessage:"<<recvMessage<<" idx:"<<idx<<" recvMessageSize:"<<recv.size();
        if(idx!=-1)
        {
            msgSize=recvMessage.section('/',1,1).toInt();
            //qDebug()<<"msgSize:"<<msgSize;
            recv=recv.substr(idx+1);
        }
    }
    //qDebug()<<"recvSize:"<<recv.size();
    if(recv.size()<msgSize)
        return;
    recvMessage=QString::fromStdString(recv);
    if(recvMessage=="游戏开始!")
    {
        if(playerName==owner_)
            mapRequest();
    }
    else if(recvMessage.mid(0,9)=="mapStream")
    {
        //qDebug()<<"mapStream:"<<msg;
        recvMessage=recvMessage.mid(9);
        //qDebug()<<"recvMessage:"<<recvMessage;
        for(int floor=0;floor<Total_Floor;++floor)
        {
            QString floor_msg=recvMessage.section('#',floor,floor);
            //qDebug()<<"floor_msg:"<<floor_msg;
            for(int row=0;row<12;++row)
            {
                QString floor_row=floor_msg.section(':',row,row);
                //qDebug()<<"floor_row:"<<floor_row;
                for(int column=0;column<16;++column)
                {
                    map[floor][row][column]=floor_row.section(',',column,column).toInt();
                }
            }
        }
        headGet=false;
        recv.clear();
        NetStartTimer->start(100);
        return;
    }
    else if(recvMessage=="pkStart")
    {
        if(!pkStartGet)
            pkStartTimer->start(100);
        pkStartGet=true;
        headGet=false;
        recv.clear();
        return;
    }
    else if(recvMessage=="fightStart")
    {
        FightTimer->start(50);
        headGet=false;
        recv.clear();
        return;
    }
    else if(recvMessage.mid(0,12)=="gameoverCall")
    {
        QString Name=recvMessage.mid(12);
        if(Name!=playerName)
            CmptorGameOverCallTimer->start(1);
        headGet=false;
        recv.clear();
        return;
    }
    else if(recvMessage.mid(0,11)=="pkPosUpdate")
    {
        recvMessage=recvMessage.mid(11);
        //qDebug()<<"pkMapUpdate recvMessage:"<<recvMessage;
        QString Name=recvMessage.section('#',0,0);
        QString pos_str=recvMessage.section('#',1,1);
        //qDebug()<<"pkMapUpdate name:"<<competitoName;
        //qDebug()<<"pkMapUpdate pos_str:"<<pos_str;
        //qDebug()<<"pkMapUpdate status_str:"<<status_str;
        if(Name!=playerName)
        {
            competitoName=Name;
            cur_x=pos_str.section(',',0,0).toInt();
            cur_y=pos_str.section(',',1,1).toInt();
            dir=pos_str.section(',',2,2).toInt();
            pkMapUdTimer->start(1);
        }
        headGet=false;
        recv.clear();
        return;
    }
    else if(recvMessage.mid(0,11)=="pkStaUpdate")
    {
        recvMessage=recvMessage.mid(11);
        QString Name=recvMessage.section('#',0,0);
        QString status_str=recvMessage.section('#',1,1);
        if(Name!=playerName)
        {
            competitoName=Name;
            hp=status_str.section(',',0,0).toInt();
            atk=status_str.section(',',1,1).toInt();
            def=status_str.section(',',2,2).toInt();
        }
        headGet=false;
        recv.clear();
        return;
    }
    contentListWidget->addItem(recvMessage.left(datagram.size()));
    contentListWidget->scrollToBottom();
    headGet=false;
    recv.clear();

}
void ChatRoom::showEvent(QShowEvent *)
{
    tcpSocket=fac->CreateQTcpSocket(SERVER_IP,SERVER_PORT);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));
    connect(tcpSocket,SIGNAL(connected()),this,SLOT (slotConnected()));
    if(playerName!=""&&owner_!=""&&playerName!=owner_)
        initialReadyRquest();
}
