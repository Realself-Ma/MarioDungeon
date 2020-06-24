#ifndef FACTORY_H
#define FACTORY_H
#include <QWidget>
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QProgressBar>
#include <QSpinBox>
#include <QComboBox>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTimer>
#include <QStyleFactory>
#include <QString>
class Factory
{
public:
    QLabel* CreateQLabel(QWidget *pos, int x, int y, int w, int h,
                 QString Text="", QString StyleSheet=DefaultStyleSheet, QFont Font=DefaultFont, Qt::Alignment s=Qt::AlignCenter);
    QLabel* CreateQLabel(QMainWindow*pos,int x, int y, int w, int h,
                 QString Text="",QString StyleSheet=DefaultStyleSheet,QFont Font=DefaultFont,Qt::Alignment s=Qt::AlignCenter);
    QLabel* CreateQLabel(QWidget* pos,QString Text="",QString StyleSheet=DefaultStyleSheet,QFont Font=DefaultFont);
    QPushButton* CreateQPushButton(QWidget*pos,int x, int y, int w, int h,
                                   QString Text="",QString StyleSheet=DefaultStyleSheet,QFont Font=DefaultFont);
    QPushButton* CreateQPushButton(QMainWindow*pos,int x, int y, int w, int h,
                                   QString Text="",QString StyleSheet=DefaultStyleSheet,QFont Font=DefaultFont);
    QPushButton* CreateQPushButton(QWidget* pos,QString Text="",QString StyleSheet=DefaultStyleSheet);
    QProgressBar* CreateQProgressBar(QWidget*pos,int x, int y, int w, int h,bool TextVisible);
    QMediaPlayer *CreateQMediaPlayer(QWidget*pos,QUrl url,int v);
    QMediaPlaylist* CreateQMediaPlaylist(QWidget* pos);
    QSpinBox* CreateQSpinBox(QWidget*pos,QString StyleSheet);
    QComboBox* CreateQComboBox(QWidget* pos,int index);
    QLineEdit* CreateQLineEdit(QMainWindow*pos,int x, int y, int w,int h,QString StyleSheet,QFont Font);
private:
    static QString DefaultStyleSheet;
    static QFont DefaultFont;
};
#endif // FACTORY_H
