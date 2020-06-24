#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QSpinBox>
#include <QComboBox>
#include <QLineEdit>
#include <QKeyEvent>
#include <QResizeEvent>//QResizeEvent 是主窗口的变化事件
#include <QSequentialAnimationGroup>
#include <QPropertyAnimation>
#include <QTimer>//计时器   QTime 只是用来产生随机数的
#include <QMediaPlaylist>
#include <QPoint>
#include "Dungeon.h"
#include "interface.h"
#include "storewidget.h"
#include "factory.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    QPoint windowPos;//窗口位置
    QPoint mousePos;//鼠标位置
    QPoint dPos;//鼠标移动后的位置
private:
    QWidget *MainWidget;//中心窗口
    QString character[12];//角色图片字符串
    QHBoxLayout *hLayout;//主界面水平布局管理器
private:
    void initialWindow_Layout();//初始化主窗口d
    void keyPressEvent(QKeyEvent *event);//键盘事件（用于操作）
    void mousePressEvent(QMouseEvent *event);//鼠标按下事件（用于移动主界面）
    void mouseMoveEvent(QMouseEvent *event);//鼠标移动事件
private slots:
    void ShowWidget();//显示界面
private:
    Dungeon d;//地牢类
};

#endif // MAINWINDOW_H
