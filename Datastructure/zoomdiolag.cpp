#include "zoomdiolag.h"
#include "ui_zoomdiolag.h"
#include"pixitem.h"
#include"mainwindow.h"
#include<QFileDialog>
#include<QMessageBox>

ZoomDiolag::ZoomDiolag(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ZoomDiolag)
{
    ui->setupUi(this);
    setWindowTitle(tr("Zooming"));// 设置窗体标题
    setWindowIcon(QIcon(":/new/SHU/SH.ico"));//窗体图标

    setMinimumSize(740, 520);
    m_graphicsView = new QGraphicsView();
    ui->gridLayout->addWidget(m_graphicsView);//将自定义的组件添加到布局中
    m_graphicsScene = new QGraphicsScene;  //new 一个新的场景对象
    m_graphicsScene->setSceneRect(-370, -250, 740, 500);     //限定场景对象的显示区域
    m_graphicsView->setScene(m_graphicsScene);          //将视图对象于场景相连

    pixItem = new PixItem(new QPixmap(":/new/SHU/shumap.png"));//打开图片
    //将该图元对象添加到场景中，并设置此图元在场景中的位置为中心（0，0）
    m_graphicsScene->addItem(pixItem);
    pixItem->setPos(0,0);
}

ZoomDiolag::~ZoomDiolag()
{
    delete ui;
}

//放大
void ZoomDiolag::on_pbt_zoomIn_clicked()
{
    pixItem->setScaleValue(pixItem->getScaleValue() + 1);
    pixItem->setZoomState(NO_STATE);
}

//缩小
void ZoomDiolag::on_pbt_zoomOut_clicked()
{
    pixItem->setScaleValue(pixItem->getScaleValue() - 1);
    pixItem->setZoomState(NO_STATE);
}

//复位
void ZoomDiolag::on_pbt_reset_clicked()
{
    pixItem->setScaleValue(0);
    pixItem->setZoomState(RESET);
    pixItem->setPos(0, 0);
    m_graphicsView->resetMatrix(); //重置图片
}

//更换图片
void ZoomDiolag::on_pbt_changeImage_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
                this, tr("open image file"),
                "./", tr("Image files(*.bmp *.jpg *.pbm *.pgm *.png *.ppm *.xbm *.xpm);;All files (*.*)"));

    m_graphicsScene->removeItem(pixItem);   //将上一个图元从场景中移除,重新添加新的图元
    QPixmap *pixmap = new QPixmap(fileName);
    pixItem = new PixItem(pixmap);          //实例化自定义类的一个对象,传入一个图片用于显示
    m_graphicsScene->addItem(pixItem);      //将该图元对象添加到场景中，并设置此图元在场景中的位置为中心(0,0)
    pixItem->setPos(0, 0);
    on_pbt_reset_clicked();
}

//返回
void ZoomDiolag::on_back_clicked()
{
    this->hide();
    emit reshowmain();
}
//退出
void ZoomDiolag::on_tuichu_clicked()
{
    emit quit();
}

void ZoomDiolag::receivemain()
{
    this->show();
}
