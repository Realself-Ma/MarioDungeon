#include "chatroom.h"

ChatRoom::ChatRoom(QWidget *parent) : QWidget(parent)
{
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
void ChatRoom::dataReceived()
{
    while(tcpSocket->bytesAvailable() > 0)
    {
        QByteArray datagram;
        datagram.resize(tcpSocket->bytesAvailable());
        tcpSocket->read(datagram.data(),datagram.size());
        QString msg = datagram.data();
        contentListWidget->addItem(msg.left(datagram.size()));
        contentListWidget->scrollToBottom();
    }
}
void ChatRoom::showEvent(QShowEvent *)
{
    tcpSocket=fac->CreateQTcpSocket(SERVER_IP,SERVER_PORT);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));
    connect(tcpSocket,SIGNAL(connected()),this,SLOT (slotConnected()));
}
