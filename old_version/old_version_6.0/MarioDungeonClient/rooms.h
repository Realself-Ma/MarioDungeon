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
class rooms: public QMainWindow
{
    Q_OBJECT
public:
    explicit rooms(QWidget *parent = 0);
    QString playerName;
private:
    void initialmainWidget();
    void initialRoomsListBtn();
    void showEvent(QShowEvent *);
    void flushRoomList();
private:
    Factory* fac;
    QTcpSocket* tcpSocket;
    QLabel* playerLbl;
    QToolButton* emptyBtn;
    bool haveEmpty;
    QToolButton **roomsListButtons;
    QQueue<std::pair<QString,QString>> roomsList;
    QVBoxLayout* ListLayout;
    QLineEdit* nameEdit;
    QDialog* CreateroomName;
private slots:
    void dataReceived();
    void CreateRoom();
    void doCreateRoom();
};

#endif // ROOMS_H
