#include "factory.h"
//静态变量的初始化应该放在cpp文件中，放在头文件中会被重复包含导致重定义
QString Factory::DefaultStyleSheet="color:white;background-color:black";
QFont Factory::DefaultFont=QFont("Microsoft YaHei" ,12, 30);
QLabel* Factory::CreateQLabel(QWidget*pos, int x, int y, int w, int h,
                  QString Text,QString StyleSheet,QFont Font,Qt::Alignment s)
{
    QLabel* p=new QLabel(pos);
    p->setGeometry(x,y,w,h);
    p->setText(Text);
    p->setStyleSheet(StyleSheet);
    p->setFont(Font);
    p->setAlignment(s);
    return p;
}
QLabel* Factory::CreateQLabel(QMainWindow*pos, int x, int y, int w, int h,
                  QString Text,QString StyleSheet,QFont Font,Qt::Alignment s)
{
    QLabel* p=new QLabel(pos);
    p->setGeometry(x,y,w,h);
    p->setText(Text);
    p->setStyleSheet(StyleSheet);
    p->setFont(Font);
    p->setAlignment(s);
    return p;
}
QLabel* Factory::CreateQLabel(QWidget *pos, QString Text, QString StyleSheet, QFont Font)
{
    QLabel* p=new QLabel(pos);
    p->setText(Text);
    p->setStyleSheet(StyleSheet);
    p->setFont(Font);
    return p;
}

QPushButton* Factory::CreateQPushButton(QWidget*pos,int x, int y, int w, int h,
                                        QString Text,QString StyleSheet,QFont Font)
{
    QPushButton* p=new QPushButton(pos);
    p->setGeometry(x,y,w,h);
    p->setText(Text);
    p->setStyleSheet(StyleSheet);
    p->setFont(Font);
    return p;
}
QPushButton* Factory::CreateQPushButton(QMainWindow*pos,int x, int y, int w, int h,
                               QString Text,QString StyleSheet,QFont Font)
{
    QPushButton* p=new QPushButton(pos);
    p->setGeometry(x,y,w,h);
    p->setText(Text);
    p->setStyleSheet(StyleSheet);
    p->setFont(Font);
    return p;
}

QPushButton* Factory::CreateQPushButton(QWidget* pos,QString Text,QString StyleSheet)
{
    QPushButton* p=new QPushButton(pos);
    p->setText(Text);
    p->setStyleSheet(StyleSheet);
    return p;
}

QProgressBar* Factory::CreateQProgressBar(QWidget*pos,int x, int y, int w, int h,bool TextVisible)
{
    QProgressBar* p=new QProgressBar(pos);
    p->setGeometry(x,y,w,h);
    p->setStyle(QStyleFactory::create("fusion"));
    p->setTextVisible(TextVisible);
    return p;
}
QMediaPlayer* Factory::CreateQMediaPlayer(QWidget*pos, QUrl url, int v)
{
    QMediaPlayer* p=new QMediaPlayer(pos);
    p->setMedia(url);//相对路径
    p->setVolume(v);
    return p;
}
QMediaPlaylist* Factory::CreateQMediaPlaylist(QWidget* pos)
{
    QMediaPlaylist* p=new QMediaPlaylist(pos);//游戏音效
    p->addMedia(QUrl("qrc:/music/music/coin.mp3"));//硬币音效
    p->addMedia(QUrl("qrc:/music/music/powerup.mp3"));//升级音效
    p->addMedia(QUrl("qrc:/music/music/flagpole.mp3"));//碰到旗子音效
    p->addMedia(QUrl("qrc:/music/music/stomp.mp3"));//捡到钥匙音效
    p->addMedia(QUrl("qrc:/music/music/one_up.mp3"));//吃到蘑菇音效
    p->addMedia(QUrl("qrc:/music/music/death.mp3"));//死亡音效
    return p;
}

QSpinBox* Factory::CreateQSpinBox(QWidget*pos,QString StyleSheet)
{
    QSpinBox* p=new QSpinBox(pos);
    p->setStyleSheet(StyleSheet);
    return p;
}
QComboBox* Factory::CreateQComboBox(QWidget* pos,int index)
{
    QComboBox* p=new QComboBox(pos);
    p->addItem(QObject::tr("魔法少女"));//项目编号从0开始
    p->addItem(QObject::tr("火影忍者"));
    p->addItem(QObject::tr("超级玛丽"));
    p->setCurrentIndex(index);
    return p;
}
QLineEdit* Factory::CreateQLineEdit(QMainWindow*pos,int x, int y, int w,int h,QString StyleSheet,QFont Font)
{
    QLineEdit* p=new QLineEdit(pos);
    p->setGeometry(x,y,w,h);
    p->setStyleSheet(StyleSheet);
    p->setFont(Font);
    return p;
}
QTcpSocket* Factory::CreateQTcpSocket(QString ServerIp,int ServerPort)
{
    QTcpSocket* tcpSocket = new QTcpSocket();
    QHostAddress* serverIP = new QHostAddress();
    QString ip = ServerIp;
    serverIP->setAddress(ip);
    tcpSocket->connectToHost(*serverIP, ServerPort);
    return tcpSocket;
}
QToolButton* Factory::CreateQToolButton(QString text,int w,int h,QString url,bool Enable)
{
    QToolButton* p=new QToolButton();
    p->setText(text);
    p->setMinimumSize(w,h);
    p->setIcon(QPixmap(url));
    p->setIconSize(QPixmap(url).size());
    p->setAutoRaise(true);
    p->setEnabled(Enable);
    p->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    return p;
}
