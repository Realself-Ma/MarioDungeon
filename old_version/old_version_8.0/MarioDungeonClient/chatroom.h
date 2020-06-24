#ifndef CHATROOM_H
#define CHATROOM_H

#include <QWidget>
#include <QListWidget>
#include <QGridLayout>
#include <QTimer>
#include "request.h"
#include "factory.h"
extern int ***map;//三维数组
extern int Total_Floor;
extern QTimer *NetStartTimer;
class ChatRoom : public QWidget
{
    Q_OBJECT
public:
    explicit ChatRoom(QWidget *parent = 0);
    QString playerName;
    QListWidget *contentListWidget;
private:
    Factory* fac;
    QLineEdit *sendLineEdit;
    QPushButton *sendBtn;
    QTcpSocket *tcpSocket;
    std::string recv;
    unsigned int msgSize;
    bool headGet;
private:
    void showEvent(QShowEvent *);
    void initialChatRoomWidget();
    void mapRequest();
private slots:
    void slotConnected();
    void slotSend();
    void dataReceived();
};

#endif // CHATROOM_H
