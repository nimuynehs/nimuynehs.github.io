#ifndef FMAPWIDGET_H
#define FMAPWIDGET_H
#include <Qstring>
#include <QWidget>
#include <iostream>
#include <string>

using namespace std;

namespace Ui {
class FmapWidget;
}

extern int qsd;
extern int zzd;

class FmapWidget : public QWidget
{
    Q_OBJECT
    int k;
    int w;
    int v;
    //void  mousePressEvent(QMouseEvent* event);
    void  setStartStation (int v);
    void  setEndStation (int k);
    void  setNextPos (int w);
    void  paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent* event);
public:
    explicit FmapWidget(QWidget *parent = 0);
    ~FmapWidget();
    void FindPath ();
    int startX, startY, endX, endY;
    int i,cnt,cntx,cnty;

    #define MAXVEX 51
    #define INFINITY 65535
    typedef int Path[MAXVEX][MAXVEX];
    typedef int ShortDis[MAXVEX][MAXVEX];
    int pathpoint[50];
    int pathpointx[50];
    int pathpointy[50];
    typedef int sum;
    typedef struct
    {
    string a[MAXVEX];   //顶点集合
    int b[MAXVEX][MAXVEX];    //边距/权重
    int numPlots;
    } MGraph;
    void CreateMGraph(MGraph *G);
    void ShortestPath_Floyd(MGraph MG, Path P, ShortDis D);
    void mainFloyd();
    void setStart(int X, int Y);
    void setEnd(int X, int Y);

    static int cycling;
    static int walking;

private slots:
    void on_fsBtn_clicked();
    void receivemenu();
    void receivezoom();

    void on_remenu_clicked();

    void on_tcBtn_clicked();


    int on_BoxStart_currentIndexChanged(int index);

    int on_BoxFinal_currentIndexChanged(int index);

    int on_comboBox_currentIndexChanged(int index);

signals:
    void showzoom();
    void quit();
    void reshowmenu();

private:
    Ui::FmapWidget *ui;

};

#endif // FMAPWIDGET_H
