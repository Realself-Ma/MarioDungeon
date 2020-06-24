#include "chatroom.h"

ChatRoom::ChatRoom(QWidget *parent) : QWidget(parent)
{
    msgSize=0;
    headGet=false;
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
    //qDebug()<<msg;
    tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());//发送的数据转换为utf-8格式(网络字节序，中文也能识别)
    sendLineEdit->clear();
}
void ChatRoom::slotConnected()
{
    int length=0;
    QString msg = MESSAGE;
    msg += playerName +'\t'+ ": Enter ChatRoom";
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
    int length = tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());
    if(length != msg.toUtf8().length())
        return;
}
void ChatRoom::dataReceived()
{
    QByteArray datagram;
    datagram.resize(tcpSocket->bytesAvailable());
    tcpSocket->read(datagram.data(),datagram.size());
    recv+=datagram.data();
    QString recvMessage=QString::fromStdString(recv);
    if(!headGet)
    {
        int idx=recvMessage.indexOf('/',2);
        qDebug()<<"recvMessage:"<<recvMessage<<" idx:"<<idx<<" recvMessageSize:"<<recv.size();
        if(idx!=-1)
        {
            msgSize=recvMessage.section('/',1,1).toInt();
            qDebug()<<"msgSize:"<<msgSize;
            recv=recv.substr(idx+1);
        }
    }
    qDebug()<<"recvSize:"<<recv.size();
    if(recv.size()<msgSize)
        return;
    recvMessage=QString::fromStdString(recv);
    if(recvMessage=="游戏开始!")
        mapRequest();
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
}
