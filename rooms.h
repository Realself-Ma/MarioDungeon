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
    QString owner_;
    ChatRoom* chatRoom;
    QPushButton* startGameBtn;
    void initialPlayerRequest();
    void doOfflineRequest();
private:
    void initialmainWidget();
    void intialRoomPlayersWidget();
    void initialRoomsListBtn();
    void initialCreateroomDialog();
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent*);
    void flushRoomList();
    void doEnterRoom();
    void startRq();
    void beReady();
private:
    Factory* fac;
    QTcpSocket* tcpSocket;
    QLabel* playerLbl;
    QToolButton* emptyBtn;
    bool haveEmpty;
    bool haveinitial;
    bool newRoomCreated;
    bool LeaveDone;
    bool Ready;
    int initialSize;
    QToolButton* ownerTbt;
    QToolButton* playerTbt;
    QWidget* roomInfo;
    QWidget* roomPlayers;
    QWidget* roomsList;
    QString roomName;
    QTimer* flushRoomListTimer;
    QToolButton **roomsListButtons;
    QToolButton* newRoomBtn;
    QQueue<std::pair<QString,QString>> roomsListQueue;
    QVBoxLayout* ListLayout;
    QLineEdit* nameEdit;
    QDialog* CreateroomName;
private slots:
    void slotConnected();
    void dataReceived();
    void CreateRoom();
    void DeleteRoom();
    void doCreateRoom();
    void EnterRoom();
    void LeaveRoom();
    void returnRoomList();
    void GameStart();
    void CheckLeaveStatus();
};

#endif // ROOMS_H
