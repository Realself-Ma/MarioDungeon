#include "rooms.h"
rooms::rooms(QWidget *parent) :
    QMainWindow(parent)
{
    fac=new Factory();
    haveEmpty=false;
    haveinitial=false;
    newRoomCreated=false;
    LeaveDone=false;
    Ready=false;
    initialSize=0;
    flushRoomListTimer=new QTimer(this);
    connect(flushRoomListTimer,SIGNAL(timeout()),this,SLOT(CheckLeaveStatus()));
    initialmainWidget();
}
void rooms::initialmainWidget()
{
    QWidget* mainWidget=new QWidget(this);
    QWidget* roomWidget=new QWidget(mainWidget);
    roomWidget->setMinimumSize(640,640);
    QHBoxLayout* h_roomWidgetLayout=new QHBoxLayout();
    h_roomWidgetLayout->setContentsMargins(0,0,0,0);//让布局紧贴主窗口，不留空白
    h_roomWidgetLayout->setSpacing(0);//让两个控件之间的间隔为0
    roomWidget->setLayout(h_roomWidgetLayout);

    roomInfo=new QWidget(mainWidget);
    roomInfo->setMinimumSize(640,640);
    roomInfo->hide();
    QVBoxLayout* vLayout=new QVBoxLayout();
    vLayout->setContentsMargins(0,0,0,0);//让布局紧贴主窗口，不留空白
    vLayout->setSpacing(0);//让两个控件之间的间隔为0
    roomInfo->setLayout(vLayout);
    roomPlayers=new QWidget(roomInfo);
    roomPlayers->setMinimumSize(640,340);
    intialRoomPlayersWidget();
    chatRoom=new ChatRoom(roomInfo);
    chatRoom->setMinimumSize(640,300);
    vLayout->addWidget(roomPlayers);
    vLayout->addWidget(chatRoom);
    h_roomWidgetLayout->addWidget(roomInfo);

    roomsList=new QWidget(mainWidget);
    roomsList->setMinimumSize(640,640);
    ListLayout=new QVBoxLayout();
    ListLayout->setContentsMargins(0,0,0,0);//让布局紧贴主窗口，不留空白
    ListLayout->setSpacing(0);//让两个控件之间的间隔为0
    ListLayout->setAlignment(Qt::AlignTop);//设置顶部对齐
    roomsList->setLayout(ListLayout);
    h_roomWidgetLayout->addWidget(roomsList,0,0);

    QWidget* infoWidget=new QWidget(mainWidget);
    infoWidget->setStyleSheet("background-color:papayawhip");
    infoWidget->setMinimumSize(200,640);
    QString infoWidget_style="color:black;background-color:white";
    playerLbl=fac->CreateQLabel(infoWidget,0,320,120,40,"",infoWidget_style);
    QPushButton* playerBtn=fac->CreateQPushButton(infoWidget,0,380,120,40,"创建房间",infoWidget_style);
    QPushButton* deleteRoomBtn=fac->CreateQPushButton(infoWidget,0,440,120,40,"删除房间",infoWidget_style);
    initialCreateroomDialog();
    connect(playerBtn,SIGNAL(clicked()),this,SLOT(CreateRoom()));
    connect(deleteRoomBtn,SIGNAL(clicked()),this,SLOT(DeleteRoom()));

    QHBoxLayout* h_mainLayout=new QHBoxLayout();
    h_mainLayout->setContentsMargins(0,0,0,0);//让布局紧贴主窗口，不留空白
    h_mainLayout->setSpacing(0);//让两个控件之间的间隔为0
    mainWidget->setLayout(h_mainLayout);
    h_mainLayout->addWidget(roomWidget);
    h_mainLayout->addWidget(infoWidget);

    this->setCentralWidget(mainWidget);
}
void rooms::intialRoomPlayersWidget()
{
    ownerTbt=new QToolButton(roomPlayers);
    ownerTbt->setGeometry(100,95,150,150);
    ownerTbt->setIcon(QPixmap(":/info/image/information/MarioLeft.png"));
    ownerTbt->setIconSize(QPixmap(":/info/image/information/MarioLeft.png").size());
    ownerTbt->setAutoRaise(true);
    ownerTbt->setEnabled(true);
    ownerTbt->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    playerTbt=new QToolButton(roomPlayers);
    playerTbt->setGeometry(390,95,150,150);
    playerTbt->setIcon(QPixmap(":/info/image/information/MarioRight.png"));
    playerTbt->setIconSize(QPixmap(":/info/image/information/MarioRight.png").size());
    playerTbt->setAutoRaise(true);
    playerTbt->setEnabled(true);
    playerTbt->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    QFont font_VS("Microsoft YaHei" ,24, 75);
    QString VS_style="color:orangered;background-color:transparent;";
    QLabel *VS=fac->CreateQLabel(roomPlayers,280,130,100,80,"VS",VS_style,font_VS);
    QString button_style="QPushButton{background-color:white;color:black;border-radius:5px;}"
                         "QPushButton:hover{background-color:palegreen; color: orangered;}"
                         "QPushButton:pressed{background-color:aquamarine;border-style:inset;}";
    QPushButton* returnRoomListBtn=fac->CreateQPushButton(roomPlayers,100,290,120,40,"房间列表",button_style);
    startGameBtn=fac->CreateQPushButton(roomPlayers,390,290,120,40,"",button_style);
    connect(returnRoomListBtn,SIGNAL(clicked()),this,SLOT(returnRoomList()));
    connect(startGameBtn,SIGNAL(clicked()),this,SLOT(GameStart()));

}
void rooms::initialRoomsListBtn()
{
    if(haveEmpty)
    {
        emptyBtn->hide();
        delete emptyBtn;
        haveEmpty=false;
    }
    if(haveinitial&&initialSize!=0)
    {
        for(int i=0;i<initialSize;++i)
        {
            delete roomsListButtons[i];
        }
        delete [] roomsListButtons;
        haveinitial=false;
        initialSize=0;
    }
    if(newRoomCreated)
    {
        newRoomBtn->hide();
        delete newRoomBtn;
        newRoomCreated=false;
    }
    if(!roomsListQueue.isEmpty())
    {
        roomsListButtons=new QToolButton *[roomsListQueue.size()];
        initialSize=roomsListQueue.size();
        haveinitial=true;
    }
    int Num=0;
    while(!roomsListQueue.isEmpty())
    {
        QString name=roomsListQueue.front().first;
        QString owner=roomsListQueue.front().second;
        roomsListQueue.dequeue();
        QString text=name+" 房主: "+owner;
        roomsListButtons[Num]=fac->CreateQToolButton(text,640,40,":/info/image/information/房间.png",true);
        connect(roomsListButtons[Num],SIGNAL(clicked()),this,SLOT(EnterRoom()));
        ListLayout->addWidget(roomsListButtons[Num]);
        //qDebug()<<"set done";
        Num++;
    }
}
void rooms::initialCreateroomDialog()
{
    CreateroomName=new QDialog();
    QLabel* nameLabel = new QLabel("房间名:",CreateroomName);
    nameEdit=new QLineEdit(CreateroomName);
    QPushButton* EnterBtn = new QPushButton("确定",CreateroomName);
    QGridLayout* gLayout=new QGridLayout();
    gLayout->addWidget(nameLabel,0,0);
    gLayout->addWidget(nameEdit,0,1);
    gLayout->addWidget(EnterBtn,1,1);
    CreateroomName->setLayout(gLayout);
    connect(EnterBtn,SIGNAL(clicked()),this,SLOT(doCreateRoom()));
}
void rooms::CreateRoom()
{
    CreateroomName->show();
}
void rooms::doCreateRoom()
{
    if(nameEdit->text()=="")
    {
        QMessageBox::information(this, "创建失败", "房间名不能为空");
        CreateroomName->raise();
        return;
    }
    if(nameEdit->text().contains('\t')||nameEdit->text().contains(':'))
    {
        QMessageBox::information(this, "无效的用户名", "用户名中包含非法字符！");
        return;
    }
    QString msg = CREATEROOM;
    msg+=nameEdit->text()+'\t'+playerName;
    int length = tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());
    if(length != msg.toUtf8().length())
        return;
}
void rooms::DeleteRoom()
{
    QString msg = DELETEROOM;
    msg+=playerName;
    int length = tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());
    if(length != msg.toUtf8().length())
        return;
}
void rooms::EnterRoom()
{
    QToolButton* curBtn=qobject_cast<QToolButton*>(sender());//通过sender()获取发送信号对象
    QString text=curBtn->text();
    QString str=" 房主: ";
    int index=text.indexOf(str,0);
    roomName=text.mid(0,index);
    owner_=text.mid(index+5);
    qDebug()<<"roomName:"<<roomName;
    QString msg = ENTERROOM;
    msg+=roomName+'\t'+playerName;
    int length = tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());
    if(length != msg.toUtf8().length())
        return;
}
void rooms::doEnterRoom()
{
    if(owner_==playerName)
    {
        startGameBtn->setText("开始");
        ownerTbt->setText(owner_);
        playerTbt->setText("");
    }
    else
    {
        startGameBtn->setText("准备");
        playerTbt->setText(playerName);
        ownerTbt->setText("");
    }

    roomInfo->show();
    roomsList->hide();
}
void rooms::LeaveRoom()
{
    QString msg = LEAVEROOM;
    msg+=playerName;
    int length = tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());
    if(length != msg.toUtf8().length())
        return;
}
void rooms::CheckLeaveStatus()
{
    if(LeaveDone)
    {
        flushRoomList();
        flushRoomListTimer->stop();
        LeaveDone=false;
    }
}
void rooms::returnRoomList()
{
    LeaveRoom();
    flushRoomListTimer->start(10);
    roomInfo->hide();
    chatRoom->contentListWidget->clear();
    roomsList->show();
}
void rooms::startRq()
{
    QString msg = STARTRQ;
    msg+=playerName+'\t'+roomName;
    int length = tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());
    if(length != msg.toUtf8().length())
        return;
}
void rooms::beReady()
{
    Ready=!Ready;
    QString ready_str;
    if(Ready)
    {
        startGameBtn->setText("取消准备");
        ready_str="ready";
    }
    else
    {
        startGameBtn->setText("准备");
        ready_str="unready";
    }
    QString msg = READYRQ;
    msg+=playerName+'\t'+roomName+'\t'+ready_str;
    qDebug()<<"ready msg:"<<msg;
    int length = tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());
    if(length != msg.toUtf8().length())
        return;
}
void rooms::GameStart()
{
    if(owner_==playerName)
        startRq();
    else
        beReady();
}
void rooms::dataReceived()
{
    QByteArray datagram;
    datagram.resize(tcpSocket->bytesAvailable());
    tcpSocket->read(datagram.data(), datagram.size());
    QString msg = datagram.data();
    qDebug()<<"rooms msg: "<<msg;
    if(msg=="rooms empty!")
    {
        emptyBtn=fac->CreateQToolButton("(空)",640,40,":/info/image/information/房间.png",false);
        ListLayout->addWidget(emptyBtn);
        haveEmpty=true;
        return;
    }
    else if(msg=="duplicated name!")
    {
        QMessageBox::information(this, "创建失败", "房间名已被占用");
        nameEdit->clear();
        CreateroomName->raise();
        return;
    }
    else if(msg=="you already have a room!")
    {
        QMessageBox::information(this, "创建失败", "你已经创建了一个房间了!");
        nameEdit->clear();
        CreateroomName->hide();
        return;
    }
    else if(msg=="CreateRoom done!")
    {
        if(haveEmpty==true)
        {
            haveEmpty=false;
            //ListLayout->removeWidget(emptyBtn);
            emptyBtn->hide();
            delete emptyBtn;
        }
        QString text=nameEdit->text()+" 房主: "+playerName;
        newRoomBtn=fac->CreateQToolButton(text,640,40,":/info/image/information/房间.png",true);
        connect(newRoomBtn,SIGNAL(clicked()),this,SLOT(EnterRoom()));
        ListLayout->addWidget(newRoomBtn);
        nameEdit->clear();
        CreateroomName->hide();
        newRoomCreated=true;
    }
    else if(msg=="No Room!")
    {
        QMessageBox::information(this, "删除房间失败", "你还没有创建房间");
        return;
    }
    else if(msg=="Enter Refused!")
    {
        QMessageBox::information(this, "进入房间失败", "房主不能进入他人房间,可以删除自己的房间后进入");
        return;
    }
    else if(msg.mid(0,9)=="EnterRoom")
    {
        doEnterRoom();
        QString text=msg.mid(9);
        int it=text.indexOf('\t');
        QString owner=text.mid(0,it);
        QString player=text.mid(it+1);
        qDebug()<<"owner:"<<owner<<" plyer:"<<player;
        ownerTbt->setText(owner);
        playerTbt->setText(player);
    }
    else if(msg.mid(0,9)=="LeaveRoom")
    {
        QString player=msg.mid(9);
        if(playerTbt->text()==player)
            playerTbt->setText("");
        else if(ownerTbt->text()==player)
            ownerTbt->setText("");
        LeaveDone=true;
    }
    else if(msg=="flush RoomList")
    {
        flushRoomList();
        if(roomsListQueue.size()==0)
            initialRoomsListBtn();
    }
    else
    {
        int count=msg.section('#',1,1).toInt();
        qDebug()<<"count:"<<count;
        QString str;
        for(int i=0;i<count;i++)
        {
            str=msg.section(':',i,i);
            //qDebug()<<"str:"<<str;
            QString name=str.section(',',0,0);
            //qDebug()<<"name:"<<name;
            QString owner=str.section(',',1,1);
            //qDebug()<<"owner:"<<owner;
            roomsListQueue.push_back(std::pair<QString,QString>(name,owner));
        }
        //qDebug()<<"get done";
        initialRoomsListBtn();
    }
}
void rooms::flushRoomList()
{
    QString msg = FLUSHROOMLIST;
    int length = tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());
    if(length != msg.toUtf8().length())
        return;
}
void rooms::showEvent(QShowEvent *)
{
    tcpSocket=fac->CreateQTcpSocket(SERVER_IP,SERVER_PORT);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));
    playerLbl->setText(playerName);
    chatRoom->playerName=playerName;
    //qDebug()<<"flushRooms";
    flushRoomList();
}
