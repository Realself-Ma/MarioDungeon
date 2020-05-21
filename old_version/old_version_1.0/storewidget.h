#ifndef STOREWIDGET_H
#define STOREWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QGraphicsView>
#include <QTimer>
#include <QKeyEvent>
#include <QShowEvent>
class StoreWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StoreWidget(QWidget *parent = 0);
    int chooseOption; //选择了第几项
    int BuyTimes;
    QTimer *haveBuyTimer;//购买计时器
    bool setChooseEnable;//是否可以购买
    bool StoreWinisShow;//商店窗口是否显示
    int Currstore_price;//当前价格
    int Nextstore_price;//下一次价格
private:
    int OptionNum; //有多少个选项
    int border_color_it;
    QString OptionBoxColor[6]= {"FFFFFF", "CCCCCC", "999999", "666666", "999999", "CCCCCC"};
    QTimer *OptionBoxTimer;
    QGraphicsView *OptionBox;
    QWidget *StoreWin;
    QLabel *text1;
    QLabel *text2;
    QLabel *hpOption;
    QLabel *mpOption;
    QLabel *atkOption;
    QLabel *defOption;
    QLabel *exitOption;
private:
    void initialStoreWin();//初始化商店界面
    void keyPressEvent(QKeyEvent *event);//当前类中的键盘事件与mainwindow中的键盘事件互不影响
    void showEvent(QShowEvent *);//重写窗口显示事件,为了使当前窗口获得焦点
public slots:
    void OptionBoxborderChanged();
};

#endif // STOREWIDGET_H
