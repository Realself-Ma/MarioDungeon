#include "rooms.h"
rooms::rooms(QWidget *parent) :
    QMainWindow(parent)
{
    fac=new Factory();
    haveEmpty=false;
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

    QWidget* roomInfo=new QWidget(mainWidget);
    roomInfo->setMinimumSize(640,640);
    roomInfo->hide();
    QGridLayout* g_roomInfoLayout=new QGridLayout();
    g_roomInfoLayout->setContentsMargins(0,0,0,0);//让布局紧贴主窗口，不留空白
    g_roomInfoLayout->setSpacing(0);//让两个控件之间的间隔为0
    roomInfo->setLayout(g_roomInfoLayout);
    h_roomWidgetLayout->addWidget(roomInfo);

    QWidget* roomsList=new QWidget(mainWidget);
    //roomsList->setStyleSheet("background-color:orangered");
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
    connect(playerBtn,SIGNAL(clicked()),this,SLOT(CreateRoom()));

    QHBoxLayout* h_mainLayout=new QHBoxLayout();
    h_mainLayout->setContentsMargins(0,0,0,0);//让布局紧贴主窗口，不留空白
    h_mainLayout->setSpacing(0);//让两个控件之间的间隔为0
    mainWidget->setLayout(h_mainLayout);
    h_mainLayout->addWidget(roomWidget);
    h_mainLayout->addWidget(infoWidget);

    this->setCentralWidget(mainWidget);
}
void rooms::initialRoomsListBtn()
{
    if(haveEmpty)
    {
        ListLayout->removeWidget(emptyBtn);
        haveEmpty=false;
    }
    if(!roomsList.isEmpty())
    {
        roomsListButtons=new QToolButton *[roomsList.size()];
    }
    int Num=0;
    while(!roomsList.isEmpty())
    {
        QString name=roomsList.front().first;
        QString owner=roomsList.front().second;
        roomsList.dequeue();
        QString text=name+" 房主: "+owner;
        roomsListButtons[Num]=fac->CreateQToolButton(text,640,40,":/info/image/information/房间.png",true);
        ListLayout->addWidget(roomsListButtons[Num]);
        //qDebug()<<"set done";
        Num++;
    }
}
void rooms::CreateRoom()
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
    CreateroomName->show();
    connect(EnterBtn,SIGNAL(clicked()),this,SLOT(doCreateRoom()));
}
void rooms::doCreateRoom()
{
    QString msg = CREATEROOM;
    msg+=nameEdit->text()+'\t'+playerName;
    int length = tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());
    if(length != msg.toUtf8().length())
        return;
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
        QMessageBox::information(this, "房间名已被占用", "房间名已被占用");
        nameEdit->clear();
    }
    else if(msg=="CreateRoom done!")
    {
        if(haveEmpty==true)
        {
            haveEmpty=false;
            ListLayout->removeWidget(emptyBtn);
            emptyBtn->hide();
        }
        QString text=nameEdit->text()+" 房主: "+playerName;
        QToolButton* roomBtn=fac->CreateQToolButton(text,640,40,":/info/image/information/房间.png",true);
        ListLayout->addWidget(roomBtn);
        CreateroomName->hide();
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
            roomsList.push_back(std::pair<QString,QString>(name,owner));
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
    qDebug()<<"flushRooms";
    flushRoomList();
}
