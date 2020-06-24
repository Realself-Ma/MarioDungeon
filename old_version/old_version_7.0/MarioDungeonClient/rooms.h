#ifndef ROOMS_H
#define ROOMS_H

#include <QMainWindow>
#include <QToolButton>
#include <QDialog>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QQueue>
#include "request.h"
#include "factory.h"
#include "chatroom.h"
class rooms: public QMainWindow
{
    Q_OBJECT
public:
    explicit rooms(QWidget *parent = 0);
    QString playerName;
private:
    void initialmainWidget();
    void intialRoomPlayersWidget();
    void initialRoomsListBtn();
    void initialCreateroomDialog();
    void showEvent(QShowEvent *);
    void flushRoomList();
    void doEnterRoom();
private:
    Factory* fac;
    QTcpSocket* tcpSocket;
    QLabel* playerLbl;
    QToolButton* emptyBtn;
    bool haveEmpty;
    bool haveinitial;
    bool newRoomCreated;
    bool LeaveDone;
    int initialSize;
    QToolButton* ownerTbt;
    QToolButton* playerTbt;
    QPushButton* startGameBtn;
    QWidget* roomInfo;
    QWidget* roomPlayers;
    ChatRoom* chatRoom;
    QWidget* roomsList;
    QString owner_;
    QTimer* flushRoomListTimer;
    QToolButton **roomsListButtons;
    QToolButton* newRoomBtn;
    QQueue<std::pair<QString,QString>> roomsListQueue;
    QVBoxLayout* ListLayout;
    QLineEdit* nameEdit;
    QDialog* CreateroomName;
private slots:
    void dataReceived();
    void CreateRoom();
    void DeleteRoom();
    void doCreateRoom();
    void EnterRoom();
    void LeaveRoom();
    void returnRoomList();
    void CheckLeaveStatus();
};

#endif // ROOMS_H
