#include <QDesktopWidget>
#include <QApplication>
#include "mainwindow.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.move((a.desktop()->width() - w.width()) / 2, (a.desktop()->height() - 702) / 2);//移动窗口到屏幕中央
    w.show();

    return a.exec();
}
