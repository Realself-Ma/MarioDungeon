#ifndef SIGN_H
#define SIGN_H

#include <QDialog>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QMessageBox>
#include <QString>
#include "request.h"
#include "factory.h"
#include "rooms.h"
class Sign : public QDialog
{
    Q_OBJECT
public:
    explicit Sign(QWidget *parent = 0);
    rooms *roomWidget;
private:
    void initialSignWidget();
    void showEvent(QShowEvent *);
private:
    Factory* fac;
    QTcpSocket* tcpSocket;
    QGridLayout *mainLayout;
    QLabel *nameLabel;
    QLabel *passwordLabel;
    QLineEdit *nameLineEdit;
    QLineEdit *passwordLineEdit;
    QPushButton *registerBtn;
    QPushButton *LogInBtn;
    QString username;
private slots:
    void slotRegister();
    void slotLogIn();
    void dataReceived();
};

#endif // SIGN_H
