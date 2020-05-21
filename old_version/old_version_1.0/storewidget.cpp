#include "storewidget.h"

StoreWidget::StoreWidget(QWidget *parent) : QWidget(parent)
{
    initialStoreWin();
    //StoreWin->setFocus();//构造函数中设置焦点为当前窗口无用，因为窗口还没显示出来
}
void StoreWidget::initialStoreWin()
{
    StoreWin=new QWidget(this);//类中不定义一个主窗口，那么新建的类对象也会没有主窗口
    QFont font("Microsoft YaHei" ,12, 30);
    this->setFont(font);
    StoreWin->setStyleSheet("color:white;background-color:black");
    QLabel *Store_pic=new QLabel(StoreWin);
    text1=new QLabel(StoreWin);
    text2=new QLabel(StoreWin);
    hpOption=new QLabel(StoreWin);
    mpOption=new QLabel(StoreWin);
    atkOption=new QLabel(StoreWin);
    defOption=new QLabel(StoreWin);
    exitOption=new QLabel(StoreWin);
    OptionBox=new QGraphicsView(StoreWin);//最后构造的控件会在最上面
    OptionBox->setStyleSheet("border:3px solid #FFFFFF;");

    Store_pic->setStyleSheet("border-image: url(:/info/image/information/Store.png);");
    Store_pic->setGeometry(10,10,30,30);
    text1->setGeometry(50,10,192,30);
    text2->setGeometry(50,50,192,30);
    hpOption->setGeometry(32,112,192,32);
    mpOption->setGeometry(32,160,192,32);
    atkOption->setGeometry(32,208,192,32);
    defOption->setGeometry(32,256,192,32);
    exitOption->setGeometry(32,304,192,32);

    text1->setAlignment(Qt::AlignCenter);
    text2->setAlignment(Qt::AlignCenter);
    hpOption->setAlignment(Qt::AlignCenter);
    mpOption->setAlignment(Qt::AlignCenter);
    atkOption->setAlignment(Qt::AlignCenter);
    defOption->setAlignment(Qt::AlignCenter);
    exitOption->setAlignment(Qt::AlignCenter);

    text1->setText("你若给我 20 个金币");
    text2->setText("我就可以帮你");
    hpOption->setText("提升 100 点生命");
    mpOption->setText("提升 20 点魔法");
    atkOption->setText("提升 2 点攻击力");
    defOption->setText("提升 4 点防御力");
    exitOption->setText("离开商店");


    OptionBox->setGeometry(32,112,192,32);

    OptionBoxTimer = new QTimer;
    border_color_it=0;
    OptionNum = 5;//选项数
    chooseOption = 0;//选择的选项
    BuyTimes=1;//购买次数
    Currstore_price=0;
    haveBuyTimer=new QTimer(this);
    StoreWinisShow=false;
    setChooseEnable=true;

    connect(OptionBoxTimer, SIGNAL(timeout()), this, SLOT(OptionBoxborderChanged()));
    OptionBoxTimer->start(100);
}
void StoreWidget::OptionBoxborderChanged()
{
    if (border_color_it <= 4)//设置为从0-5，会崩溃，因为当border_color_it=5时，border_color_it++就越界了
        border_color_it++;
    else
        border_color_it = 0;
    //实现选项框边缘颜色的变化（闪烁效果）

    //样式表 setStyleSheet 的组合用法  QString重载了 '+' 号运算符
    OptionBox->setStyleSheet(QString("border:3px solid #") +
                             QString(OptionBoxColor[border_color_it]) + QString(";background-color:transparent;"));
    //OptionBox的背景也为黑色，会遮挡住下面的控件，所以要设置背景为透明
}

void StoreWidget::keyPressEvent(QKeyEvent *event)
{   
    if(event->key() == Qt::Key_W)
    { //向上
        if (chooseOption > 0)
        {
            chooseOption -= 1;
            OptionBox->setGeometry(32, 112 + 48 * chooseOption, 192, 32);
        }
        else
            chooseOption = 0;
    }
    else if (event->key() == Qt::Key_S)
    { //向下
        if (chooseOption <= OptionNum - 2)
        {
            chooseOption += 1;
            OptionBox->setGeometry(32, 112 + 48 * chooseOption, 192, 32);
        }
        else
            chooseOption = OptionNum - 1;
    }
    else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter || event->key() == Qt::Key_Space)
    {
        if (chooseOption != OptionNum - 1)//不是选择的离开商店选项
        {
            Currstore_price=((BuyTimes) * (BuyTimes) - (BuyTimes) + 2) * 10;
            Nextstore_price = ((BuyTimes +1 ) * (BuyTimes +1 ) - (BuyTimes +1 ) + 2) * 10;

            haveBuyTimer->start(100);//开启购买（计时器开启后不是马上执行，而是在当前函数执行完毕后才开始执行,
                                              //所以，如果在执行计时器后才改变setChooseEnable的值，那么在
                                              //当前函数体内，setChooseEnable的值就还不会改变
            if(!setChooseEnable)//金币不够，则不能购买
            {
                return;
            }

            text1->setText(QString::fromStdWString(L"你若给我 ") +
                           QString::number(Nextstore_price) + QString::fromStdWString(L" 个金币"));
            text2->setText(QString::fromStdWString(L"我就可以帮你"));
            hpOption->setText(QString::fromStdWString(L"提升 ") +
                              QString::number(100 * (BuyTimes +1 )) + QString::fromStdWString(L" 点生命"));
            mpOption->setText(QString::fromStdWString(L"提升 ") +
                              QString::number(20 * (BuyTimes +1 )) + QString::fromStdWString(L" 点魔法"));


            BuyTimes++;

            return;
        }
        else
        {
            this->hide();
            StoreWinisShow=false;
            chooseOption=0;//选项框回到起始位置
            OptionBox->setGeometry(32,112,192,32);
            return;
        }
    }
}
void StoreWidget::showEvent(QShowEvent *)
{
    this->setFocus();//设置焦点在当前窗口
}
