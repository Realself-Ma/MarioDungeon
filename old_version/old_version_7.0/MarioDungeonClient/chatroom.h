#ifndef CHATROOM_H
#define CHATROOM_H

#include <QWidget>
#include <QListWidget>
#include <QGridLayout>
#include "request.h"
#include "factory.h"
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
private:
    void showEvent(QShowEvent *);
    void initialChatRoomWidget();
private slots:
    void slotConnected();
    void slotSend();
    void dataReceived();
};

#endif // CHATROOM_H
