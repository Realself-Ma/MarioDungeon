#ifndef HELP_H
#define HELP_H

#include <QWidget>
#include "factory.h"
class Help : public QWidget
{
    Q_OBJECT
public:
    explicit Help(QWidget *parent = 0);
    QPushButton *Return;
private:
    Factory* fac;
private slots:
};

#endif // HELP_H
