#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include "test.h"
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
private:
    QLabel* m_label;
    QPushButton* Go_Test;
    QPushButton* Go_Pk;
    QPushButton* End_Pk;
    QPushButton* End_Test;
    QLineEdit* m_edit;
    QGridLayout* m_gLayout;
    Test* test;
private slots:
    void go_test();
    void end_test();
    void go_pk();
    void end_pk();
};

#endif // WIDGET_H
