#include "mainwindow.h"
#include "menudialog.h"
#include"logindialog.h"
#include"zoomdiolag.h"
#include"teamdialog.h"
#include"fmapwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.processEvents ();           //使程序在显示启动动画的同时能能响应鼠标等其他事件

    MainWindow w;
    LoginDialog l;
    MenuDialog m;
    ZoomDiolag z;
    TeamDialog t;
    FmapWidget f;


    //f.show();
    l.show();//先展示登录平台

    //信号关联
    QObject::connect(&l,SIGNAL(showmenu()),&m,SLOT(receivelogin()));
    QObject::connect(&m,SIGNAL(showmainwindow()),&f,SLOT(receivemenu()));
    QObject::connect(&f,SIGNAL(reshowmenu()),&m,SLOT(receivemainwindow()));
    QObject::connect(&f,SIGNAL(showzoom()),&z,SLOT(receivemain()));
    QObject::connect(&z,SIGNAL(reshowmain()),&f,SLOT(receivezoom()));
    QObject::connect(&m,SIGNAL(showteam()),&t,SLOT(receivecaidan()));
    QObject::connect(&t,SIGNAL(backmenu()),&m,SLOT(receiveteam()));
    QObject::connect(&l,SIGNAL(quit()),&a,SLOT(quit()));
    QObject::connect(&m,SIGNAL(quit()),&a,SLOT(quit()));
    QObject::connect(&f,SIGNAL(quit()),&a,SLOT(quit()));

    return a.exec();
}
