#include "sign.h"
#include <QDebug>
Sign::Sign(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("登录服务器"));
    initialSignWidget();
}
void Sign::initialSignWidget()
{
    registerBtn = new QPushButton("注册",this);
    LogInBtn = new QPushButton("登录",this);
    nameLabel = new QLabel("用户名:",this);
    passwordLabel = new QLabel("密码:",this);
    nameLineEdit = new QLineEdit(this);
    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    tcpSocket = new QTcpSocket(this);
    connect(registerBtn, SIGNAL(clicked()), this, SLOT(slotRegister()));
    connect(LogInBtn, SIGNAL(clicked()), this, SLOT(slotLogIn()));
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));

    QGridLayout* mainLayout = new QGridLayout(this);
    mainLayout->addWidget(nameLabel, 0, 0);
    mainLayout->addWidget(nameLineEdit, 0, 1, 1, 2);
    mainLayout->addWidget(passwordLabel, 1, 0);
    mainLayout->addWidget(passwordLineEdit, 1, 1, 1, 2);
    mainLayout->addWidget(registerBtn, 2, 1);
    mainLayout->addWidget(LogInBtn, 2, 2);

    this->setLayout(mainLayout);
}
void Sign::slotRegister()
{
    QString msg = REGISTER;
    //qDebug()<<"msg1: "<<msg;
    username = nameLineEdit->text();
    QString password = passwordLineEdit->text();
    msg += username + '\t' + password;
    //qDebug()<<"msg2: "<<msg;
    int length = tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());
    if(length != msg.toUtf8().length())
        return;
}
void Sign::slotLogIn()
{
    QHostAddress* serverIP = new QHostAddress();
    QString ip = SERVER_IP;
    serverIP->setAddress(ip);
    tcpSocket->connectToHost(*serverIP, SERVER_PORT);

    QString msg=LOGIN;
    //qDebug()<<"msg1: "<<msg;
    username = nameLineEdit->text();
    QString password = passwordLineEdit->text();
    msg += username + '\t' + password;
    //qDebug()<<"msg2: "<<msg;
    int length = tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());
    if(length != msg.toUtf8().length())
        return;
}
void Sign::dataReceived()
{
    QByteArray datagram;
    datagram.resize(tcpSocket->bytesAvailable());
    tcpSocket->read(datagram.data(), datagram.size());
    QString msg = datagram.data();
    //qDebug()<<"receive msg: "<<msg;
    if(msg=="LOG IN SUCCESS")
    {
        QMessageBox::information(this, "log in success", "log in success");
    }
    else if(msg=="password is wrong")
    {
        QMessageBox::information(this, "password is wrong", "password is wrong");
    }
    else if(msg == "REGISTER SUCCESS")
        QMessageBox::information(this, "register success, now you can log in", "register success, now you can log in");
    else if(msg=="You have to register first")
    {
        QMessageBox::information(this, "You have to register first", "You have to register first");
    }
    else if(msg=="The name has been registered")
    {
        QMessageBox::information(this, "The name has been registered", "The name has been registered");
    }

}

void Sign::showEvent(QShowEvent *)
{
    this->setFocus();
}
