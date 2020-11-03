#include "sign.h"
#include <QDebug>
//QTcpSocket* tcpSocket;//放在头文件中会被重复包含，导致重定义错误，在cpp文件只在本cpp文件有效，外部引用加extern
Sign::Sign(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("登录服务器"));
    initialSignWidget();
    fac=new Factory();
    roomWidget=new rooms();
    roomWidget->resize(840,680);
    roomWidget->hide();
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

    connect(registerBtn, SIGNAL(clicked()), this, SLOT(slotRegister()));
    connect(LogInBtn, SIGNAL(clicked()), this, SLOT(slotLogIn()));

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
    username = nameLineEdit->text();
    if(username=="")
    {
        QMessageBox::information(this, "无效的用户名", "用户名不能为空！");
        return;
    }
    if(username.contains('\t')||username.contains(':'))
    {
        QMessageBox::information(this, "无效的用户名", "用户名中包含非法字符！");
        return;
    }
    QString msg = REGISTER;
    QString password = passwordLineEdit->text();
    msg += username + '\t' + password;
    msg+=ENDFLAG;
    //qDebug()<<"msg2: "<<msg;
    int length = tcpSocket->write(msg.toUtf8(), msg.toUtf8().length());
    if(length != msg.toUtf8().length())
        return;
}
void Sign::slotLogIn()
{    
    if(nameLineEdit->text()=="")
    {
        QMessageBox::information(this, "无效的用户名", "用户名不能为空！");
        return;
    }
    if(passwordLineEdit->text()=="")
    {
        QMessageBox::information(this, "无效的密码", "密码不能为空！");
        return;
    }
//    if(nameLineEdit->text().indexOf("\t")!=-1||nameLineEdit->text().indexOf(':')!=-1)
//    {
//        QMessageBox::information(this, "无效的用户名", "用户名中包含非法字符！");
//        return;
//    }

    QString msg=LOGIN;
    username = nameLineEdit->text();
    QString password = passwordLineEdit->text();
    msg += username + '\t' + password;
    msg+=ENDFLAG;
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
    qDebug()<<"sign msg: "<<msg;
    if(msg=="LOG IN SUCCESS")
    {
        QMessageBox::information(this, "log in success", "log in success");
        roomWidget->playerName=username;
        roomWidget->show();
        this->hide();
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
    else if(msg=="another client have Login this acount")
    {
        QMessageBox::information(this, "another client have Login this acount", "another client have Login this acount");
    }
    else
        return;

}

void Sign::showEvent(QShowEvent *)
{
    this->setFocus();
    tcpSocket=fac->CreateQTcpSocket(SERVER_IP,SERVER_PORT);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(dataReceived()));
}
