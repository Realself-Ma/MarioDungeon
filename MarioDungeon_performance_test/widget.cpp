#include "widget.h"
#include "ui_widget.h"

int Room_Num=0;
Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    this->resize(400,200);
    m_gLayout=new QGridLayout(this);
    m_label=new QLabel("房间数：");
    Go_Test=new QPushButton("开始测试");
    End_Test=new QPushButton("结束测试");
    Go_Pk=new QPushButton("开始PK");
    End_Pk=new QPushButton("结束PK");
    m_edit=new QLineEdit(this);
    m_gLayout->addWidget(m_label,0,0);
    m_gLayout->addWidget(m_edit,0,1,1,3);
    m_gLayout->addWidget(Go_Test,1,0);
    m_gLayout->addWidget(Go_Pk,1,1);
    m_gLayout->addWidget(End_Pk,1,2);
    m_gLayout->addWidget(End_Test,1,3);
    this->setLayout(m_gLayout);
    connect(Go_Test,SIGNAL(clicked(bool)),this,SLOT(go_test()));
    connect(End_Test,SIGNAL(clicked(bool)),this,SLOT(end_test()));
    connect(Go_Pk,SIGNAL(clicked(bool)),this,SLOT(go_pk()));
    connect(End_Pk,SIGNAL(clicked(bool)),this,SLOT(end_pk()));
    test=new Test();
}

Widget::~Widget()
{

}
void Widget::go_test()
{
    Room_Num=m_edit->text().toInt();
    test->Run_Test();
}
void Widget::go_pk()
{
    test->updatePosTimer->start(1000);
}
void Widget::end_pk()
{
    test->updatePosTimer->stop();
}
void Widget::end_test()
{
    test->TruncateTable();
}
