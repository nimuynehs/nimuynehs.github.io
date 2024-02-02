#ifndef ZOOMDIOLAG_H
#define ZOOMDIOLAG_H

#include <QWidget>
#include <QGraphicsView>    //视图类
#include <QGraphicsScene>   //场景类
#include <QGraphicsItem>    //图元类
#include<QDialog>

#include "pixitem.h"
#include <math.h>

namespace Ui {
class ZoomDiolag;
}

class ZoomDiolag : public QWidget
{
    Q_OBJECT

public:
    explicit ZoomDiolag(QWidget *parent = nullptr);
    ~ZoomDiolag();

private slots:
    void on_pbt_zoomIn_clicked();

    void on_pbt_zoomOut_clicked();

    void on_pbt_reset_clicked();

    void on_pbt_changeImage_clicked();

    void on_back_clicked();

    void on_tuichu_clicked();

    void receivemain();

signals:
    void reshowmain();
    void quit();

private:
    Ui::ZoomDiolag *ui;
    PixItem *pixItem;       //自定义的图元类
    QGraphicsScene *m_graphicsScene;  //场景
    QGraphicsView *m_graphicsView;
};

#endif // ZOOMDIOLAG_H
