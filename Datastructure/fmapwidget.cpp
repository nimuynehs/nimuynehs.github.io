#include "fmapwidget.h"
#include "ui_fmapwidget.h"
#include <QPainter>
#include <QPixmap>
#include <QMouseEvent>
#include <QDebug>
#include <QPoint>
#include<QString>
#include<QMovie>

FmapWidget::FmapWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FmapWidget)
{
    ui->setupUi(this);
    setWindowTitle(tr("Navigation"));// 设置窗体标题
    setWindowIcon(QIcon(":/new/SHU/SH.ico"));
}


int startpoint;
int destination;
int barometer;

int FmapWidget::on_BoxStart_currentIndexChanged(int index)
{
    index=ui->BoxStart->currentIndex();//索引号
    //qDebug("当前索引号是：%d",index);
    startpoint=index;
    this->hide();
    this->show();
    return startpoint;
}


int FmapWidget::on_BoxFinal_currentIndexChanged(int index)
{
    index=ui->BoxFinal->currentIndex();
    this->hide();
    this->show();
    destination=index;
    return destination;
}

int FmapWidget::on_comboBox_currentIndexChanged(int index)
{
    index=ui->comboBox->currentIndex();
    this->hide();
    this->show();
    barometer=index;
    return barometer;
}


void FmapWidget:: paintEvent(QPaintEvent*)
    {
        //导入地图图片
        QPainter painter(this);
        QPixmap pix;
        pix.load(":/new/SHU/Shumap2.png");
        painter.drawPixmap(340, 20, pix.width(), pix.height(), pix);
        painter.setBrush(QColor(255, 255, 255, 100));
        painter.drawRect(340,20, pix.width(), pix.height());

       //画线程序+Floy主程序
        int pathpointx[50];
        int pathpointy[50];
        int v, w, k,sum;
        MGraph MG;
        Path P;
        ShortDis D;
        CreateMGraph(&MG);
        ShortestPath_Floyd(MG, P, D);
        //ui->shuzi->setText("索引号："+QString::number(index));//输出
       // qDebug ()<< "各顶点间最短路径如下: " << endl;
        if(startpoint==0)
        {
            if(destination==0)
            {
                if(barometer==0)
                {
                    barometer=barometer+1;
                }
                destination=destination+1;
            }
            startpoint=startpoint+1;
        }
        v=startpoint;
        w=destination;
        cnt=0;
       //qDebug() << v << "--" << w << " weight: " << D[v][w]<< "m"<< " Path: " << v << ' ';

        if(barometer==2)
        {
            D[v][w]=D[v][w]*1.2;
        }

       ui->lbuxing->setText("Expected Walking Duration："+QString::number(D[v][w]/1)+"s");
       ui->lqixing->setText("Expected Cycling Duration："+QString::number(D[v][w]/4)+"s");

                //path
        sum=1;
        setStartStation(v);
        pathpoint[cnt]=v;
        pathpointx[cnt]=startX;
        pathpointy[cnt]=startY;
       // qDebug() << startX << ", " << startY;
        cnt++;
           k = P[v][w];
                while (k != w)
                  {
                     // qDebug() << "-> " << k << " ";
                     k = P[k][w];
                     setNextPos(k);
                     pathpoint[cnt]=k;
                     pathpointx[cnt]=endX;
                     pathpointy[cnt]=endY;
                   //  qDebug() << endX << ", " << endY;
                     sum++;
                     cnt++;
          //  qDebug()<< "-> " << w << endl;

        }
         setEndStation(w);
         pathpoint[cnt]=v;
         pathpointx[cnt]=endX;
         pathpointy[cnt]=endY;
       //   qDebug() << endX << ", " << endY;


        QPen pen;
        QBrush brush;
        pen.setColor(Qt::blue);
        pen.setWidth(2);
        painter.setPen(pen);
        brush.setStyle(Qt::SolidPattern);
        painter.setBrush(brush);
        if (cnt==1)
            painter.drawLine(pathpointx[0],pathpointy[0],pathpointx[1],pathpointy[1]);
        else
        {
            QPoint points[10];
            for(cnt=0;cnt<=sum;cnt++)
            {
                points[cnt].setX(pathpointx[cnt]);
                points[cnt].setY(pathpointy[cnt]);
             }
            painter.drawPolyline(points,sum);
        }

}

void FmapWidget::mousePressEvent(QMouseEvent* event)
{
    //获得相对于屏幕的坐标
   QPoint sPoint1=event->pos();
   qDebug()<<"setStart"<<"("<<sPoint1.rx()<<","<<sPoint1.ry()<<")";
}//void

// Floyd算法，求网图G中各顶点v到其余顶点w的最短路径P[v][w]及带权长度D[v][w]。
void FmapWidget:: ShortestPath_Floyd(MGraph MG, Path P, ShortDis D)
{
    int v, w, k;
    for (v = 1; v < MG.numPlots; v++)/* 初始化D与P */
    {
        for (w = 1; w < MG.numPlots; w++)
        {
            D[v][w] = MG.b[v][w];/* D[v][w]值即为对应点间的权值 */
            P[v][w] = w;/* 初始化P */
        }
    }

    for (k = 1; k < MG.numPlots; k++)
    {
        for (v = 1; v < MG.numPlots; v++)
        {
            for (w = 1; w < MG.numPlots; w++)
            {
                /* 如果经过下标为k顶点路径比原两点间路径更短 */
                if (D[v][w] > D[v][k] + D[k][w])
                {
                    /* 将当前两点间权值设为更小的一个 */
                    D[v][w] = D[v][k] + D[k][w];
                    P[v][w] = P[v][k];/* 路径设置为经过下标为k的顶点 */
                }
            }
        }
    }

}

void FmapWidget::setStart(int X, int Y)
{
    startX = X; startY = Y;
  // qDebug() << X << ", " << Y;
}

void FmapWidget::setEnd(int X, int Y)
{
    endX = X; endY = Y;
}

void FmapWidget::setStartStation (int v)
{
    switch (v) {

    case 1:
        setStart ( 527 , 67 ); break;
    case 2:
        setStart ( 507 , 72 ); break;
    case 3:
        setStart ( 465 , 102 ); break;
    case 4:
        setStart ( 618 , 98 ); break;
    case 5:
        setStart ( 603 , 134 ); break;
    case 6:
        setStart ( 644 , 146 );break;
    case 7:
        setStart ( 772 , 250 ); break;
    case 8:
        setStart ( 484 , 183 ); break;
    case 9:
        setStart ( 766 , 194 ); break;
    case 10:
        setStart ( 450 , 166 ); break;
    case 11:
        setStart ( 854 , 204 ); break;
    case 12:
        setStart ( 845 , 281 ); break;
    case 13:
        setStart ( 1075 , 372 ); break;
    case 14:
        setStart ( 1009 , 373 ); break;
    case 15:
        setStart ( 848 , 378 ); break;
    case 16:
        setStart ( 1038 , 516 ); break;
    case 17:
        setStart ( 1008 , 484 ); break;
    case 18:
        setStart ( 1002 , 625 ); break;
    case 19:
        setStart ( 1056 , 231 ); break;
    case 20:
        setStart ( 910 , 367 ); break;
    case 21:
        setStart ( 733 , 649 ); break;
    case 22:
        setStart ( 849 , 649 ); break;
    case 23:
        setStart ( 913 , 613 ); break;
    case 24:
        setStart ( 831 , 532 ); break;
    case 25:
        setStart ( 819 , 537 ); break;
    case 26:
        setStart ( 759 , 481 ); break;
    case 27:
        setStart ( 726 , 424 ); break;
    case 28:
        setStart ( 688 , 399 ); break;
    case 29:
        setStart ( 616 , 395 ); break;
    case 30:
        setStart ( 668 , 348 ); break;
    case 31:
        setStart ( 467 , 704 ); break;
    case 32:
        setStart ( 486 , 678 ); break;
    case 33:
        setStart ( 483 , 646 ); break;
    case 34:
        setStart ( 539 , 592 ); break;
    case 35:
        setStart ( 640 , 604 ); break;
    case 36:
        setStart ( 448 , 415 ); break;
    case 37:
        setStart ( 470 , 545 ); break;
    case 38:
        setStart ( 421 , 383 ); break;
    case 39:
        setStart ( 445 , 320 ); break;
    case 40:
        setStart ( 412 , 359 ); break;
    case 41:
        setStart ( 345 , 563 ); break;
    case 42:
        setStart ( 321 , 392 ); break;
    case 43:
        setStart ( 380 , 390 ); break;
    case 44:
        setStart ( 423 , 441 ); break;
    case 45:
        setStart ( 444 , 361 ); break;
    case 46:
        setStart ( 504 , 385 ); break;
    case 47:
        setStart ( 440 , 262 ); break;
    case 48:
        setStart ( 428 , 352 ); break;
    case 49:
        setStart ( 651 , 492 ); break;
    case 50:
        setStart ( 271 , 390 ); break;

    default:
        break;
    }
}

void FmapWidget::setNextPos (int k)
{
    switch (k) {
    case 1:
        setEnd ( 527 , 67 ); break;
    case 2:
        setEnd ( 507 , 72 ); break;
    case 3:
        setEnd ( 465 , 102 ); break;
    case 4:
        setEnd ( 618 , 98 ); break;
    case 5:
        setEnd ( 603 , 134 ); break;
    case 6:
        setEnd ( 644 , 146 );break;
    case 7:
        setEnd ( 772 , 250 ); break;
    case 8:
        setEnd ( 484 , 183 ); break;
    case 9:
        setEnd ( 766 , 194 ); break;
    case 10:
        setEnd ( 450 , 166 ); break;
    case 11:
        setEnd ( 854 , 204 ); break;
    case 12:
        setEnd ( 845 , 281 ); break;
    case 13:
        setEnd ( 1075 , 372 ); break;
    case 14:
        setEnd ( 1009 , 373 ); break;
    case 15:
        setEnd ( 848 , 378 ); break;
    case 16:
        setEnd ( 1038 , 516 ); break;
    case 17:
        setEnd ( 1008 , 484 ); break;
    case 18:
        setEnd ( 1002 , 625 ); break;
    case 19:
        setEnd ( 1056 , 231 ); break;
    case 20:
        setEnd ( 910 , 367 ); break;
    case 21:
        setEnd ( 733 , 649 ); break;
    case 22:
        setEnd ( 849 , 649 ); break;
    case 23:
        setEnd ( 913 , 613 ); break;
    case 24:
        setEnd ( 831 , 532 ); break;
    case 25:
        setEnd ( 819 , 537 ); break;
    case 26:
        setEnd ( 759 , 481 ); break;
    case 27:
        setEnd ( 726 , 424 ); break;
    case 28:
        setEnd ( 688 , 399 ); break;
    case 29:
        setEnd ( 616 , 395 ); break;
    case 30:
        setEnd ( 668 , 348 ); break;
    case 31:
        setEnd ( 467 , 704 ); break;
    case 32:
        setEnd ( 486 , 678 ); break;
    case 33:
        setEnd ( 483 , 646 ); break;
    case 34:
        setEnd ( 539 , 592 ); break;
    case 35:
        setEnd ( 640 , 604 ); break;
    case 36:
        setEnd ( 448 , 415 ); break;
    case 37:
        setEnd ( 470 , 545 ); break;
    case 38:
        setEnd ( 421 , 383 ); break;
    case 39:
        setEnd ( 445 , 320 ); break;
    case 40:
        setEnd ( 412 , 359 ); break;
    case 41:
        setEnd ( 345 , 563 ); break;
    case 42:
        setEnd ( 321 , 392 ); break;
    case 43:
        setEnd ( 380 , 390 ); break;
    case 44:
        setEnd ( 423 , 441 ); break;
    case 45:
        setEnd ( 444 , 361 ); break;
    case 46:
        setEnd ( 504 , 385 ); break;
    case 47:
        setEnd ( 440 , 262 ); break;
    case 48:
        setEnd ( 428 , 352 ); break;
    case 49:
        setEnd ( 651 , 492 ); break;
    case 50:
        setEnd ( 271 , 390 ); break;
    default:
        break;
    }
}

void FmapWidget::setEndStation (int w)
{
    switch (w) {
    case 1:
        setEnd ( 527 , 67 ); break;
    case 2:
        setEnd ( 507 , 72 ); break;
    case 3:
        setEnd ( 465 , 102 ); break;
    case 4:
        setEnd ( 618 , 98 ); break;
    case 5:
        setEnd ( 603 , 134 ); break;
    case 6:
        setEnd ( 644 , 146 ); break;
    case 7:
        setEnd ( 772 , 250 ); break;
    case 8:
        setEnd ( 484 , 183 ); break;
    case 9:
        setEnd ( 766 , 194 ); break;
    case 10:
        setEnd ( 450 , 166 ); break;
    case 11:
        setEnd ( 854 , 204 ); break;
    case 12:
        setEnd ( 845, 281 ); break;
    case 13:
        setEnd ( 1076 , 372 ); break;
    case 14:
        setEnd ( 1009 , 373 ); break;
    case 15:
        setEnd ( 848 , 378 ); break;
    case 16:
        setEnd ( 1038 , 516 ); break;
    case 17:
        setEnd ( 1008 , 484 ); break;
    case 18:
        setEnd ( 1002 , 625 ); break;
    case 19:
        setEnd ( 1056 , 231 ); break;
    case 20:
        setEnd ( 910 , 367 ); break;
    case 21:
        setEnd ( 733 , 649 ); break;
    case 22:
        setEnd ( 849 , 649 ); break;
    case 23:
        setEnd ( 913 , 613 ); break;
    case 24:
        setEnd ( 831 , 532 ); break;
    case 25:
        setEnd ( 819 , 537 ); break;
    case 26:
        setEnd ( 759 , 481 ); break;
    case 27:
        setEnd ( 726 , 424 ); break;
    case 28:
        setEnd ( 688 , 399 ); break;
    case 29:
        setEnd ( 616 , 395 ); break;
    case 30:
        setEnd ( 668 , 348 ); break;
    case 31:
        setEnd ( 467 , 704 ); break;
    case 32:
        setEnd ( 486 , 678 ); break;
    case 33:
        setEnd ( 483 , 646 ); break;
    case 34:
        setEnd ( 539 , 592 ); break;
    case 35:
        setEnd ( 640 , 604 ); break;
    case 36:
        setEnd ( 448 , 415 ); break;
    case 37:
        setEnd ( 470 , 545 ); break;
    case 38:
        setEnd ( 421 , 383 ); break;
    case 39:
        setEnd ( 445 , 320 ); break;
    case 40:
        setEnd ( 412 , 359 ); break;
    case 41:
        setEnd ( 345 , 563 ); break;
    case 42:
        setEnd ( 321 , 392 ); break;
    case 43:
        setEnd ( 380 , 390 ); break;
    case 44:
        setEnd ( 423 , 441 ); break;
    case 45:
        setEnd ( 444 , 361 ); break;
    case 46:
        setEnd ( 504 , 385 ); break;
    case 47:
        setEnd ( 440 , 262 ); break;
    case 48:
        setEnd ( 428 , 352 ); break;
    case 49:
        setEnd ( 651 , 492 ); break;
    case 50:
        setEnd ( 271 , 390 ); break;

    default:
        break;

    }
}


/* 构建图 */
void FmapWidget:: CreateMGraph(MGraph *G)
{
    int i,j;
    G->numPlots = 51;
    //定义每个点的名称
    G->a[1]="地铁站";
    G->a[2]="北门";
    G->a[3]="校医院";
    G->a[4]="乐乎新楼";
    G->a[5]="尔美食堂";
    G->a[6]="风雨棚操场";
    G->a[7]="音乐学院";
    G->a[8]="网球馆";
    G->a[9]="游泳馆";
    G->a[10]="玄陵网球馆";
    G->a[11]="体育馆";
    G->a[12]="伟长楼";
    G->a[13]="通信楼";
    G->a[14]="通信实验楼";
    G->a[15]="水秀楼";
    G->a[16]="钱伟长图书馆";
    G->a[17]="东区食堂";
    G->a[18]="经管楼";
    G->a[19]="生科楼";
    G->a[20]="H楼";
    G->a[21]="美术学院";
    G->a[22]="南门";
    G->a[23]="行政楼";
    G->a[24]="B504";
    G->a[25]="B楼";
    G->a[26]="D楼";
    G->a[27]="J楼";
    G->a[28]="音乐广场";
    G->a[29]="下沉式广场";
    G->a[30]="图书馆";
    G->a[31]="南区食堂";
    G->a[32]="小南门";
    G->a[33]="后勤基地";
    G->a[34]="I区篮球场";
    G->a[35]="中心花园";
    G->a[36]="1号楼";
    G->a[37]="I区足球场";
    G->a[38]="教育超市";
    G->a[39]="S1楼";
    G->a[40]="快递点";
    G->a[41]="社区学院";
    G->a[42]="新世纪门口";
    G->a[43]="西门";
    G->a[44]="西门球场";
    G->a[45]="上大浴室";
    G->a[46]="益新食堂";
    G->a[47]="山明食堂";
    G->a[48]="打印店(浴室对面)";
    G->a[49]="动物园";
    G->a[50]="鸿基广场";

    for (i = 1; i < G->numPlots; i++)/* 初始化图 */
    {
        for ( j = 1; j < G->numPlots; j++)
        {
            if (i == j)
                G->b[i][j] = 0;
            else
                G->b[i][j] = G->b[j][i] = INFINITY;
        }
    }
    //定义权重的值
    G->b[1][2]=20;
    G->b[2][3]=70; G->b[2][4]=190; G->b[2][5]=310;G->b[2][8]=290;
    G->b[3][4]=206; G->b[3][5]=340; G->b[3][8]=350;
    G->b[4][5]=357;
    G->b[5][6]=80; G->b[5][8]=320;
    G->b[6][7]=340;
    G->b[7][9]=40; G->b[7][11]=223; G->b[7][12]=90; G->b[7][15]=140; G->b[7][19]=647;
    G->b[8][10]=35; G->b[8][47]=257;
    G->b[9][11]=220;
    G->b[11][12]=142; G->b[11][19]=507;
    G->b[12][15]=100; G->b[12][30]=366;
    G->b[13][14]=65; G->b[13][16]=160; G->b[13][17]=232;G->b[13][19]=157;
    G->b[14][17]=133;
    G->b[15][20]=60; G->b[15][23]=273;G->b[15][25]=232;G->b[15][27]=162;
    G->b[16][17]=60; G->b[16][18]=90; G->b[16][19]=300;
    G->b[17][18]=137; G->b[17][19]=180;
    G->b[21][22]=105; G->b[21][35]=97;
    G->b[22][23]=60; G->b[22][24]=125; G->b[22][26]=160;
    G->b[23][24]=110; G->b[23][25]=115;
    G->b[24][25]=6; G->b[24][26]=125;
    G->b[26][27]=63;
    G->b[27][28]=40; G->b[27][49]=135;
    G->b[28][29]=80; G->b[28][30]=70;
    G->b[29][30]=85; G->b[29][46]=107;
    G->b[31][32]=20;
    G->b[32][33]=60;
    G->b[33][34]=65; G->b[33][37]=73;
    G->b[34][35]=115; G->b[34][36]=220; G->b[34][37]=72;
    G->b[36][37]=140; G->b[36][38]=43; G->b[36][39]=90; G->b[36][44]=50; G->b[36][45]=55; G->b[36][46]=75; G->b[36][48]=50;
    G->b[37][44]=135;
    G->b[38][40]=20; G->b[38][43]=20; G->b[38][44]=50; G->b[38][45]=45; G->b[38][46]=100; G->b[38][48]=42;
    G->b[39][45]=20; G->b[39][47]=67; G->b[39][48]=23;
    G->b[40][45]=30; G->b[40][48]=28;
    G->b[41][42]=282;
    G->b[42][50]=41;
    G->b[43][44]=150; G->b[43][50]=134;

    for(i = 1; i < G->numPlots; i++)    //只保存右上矩阵
    {
        for(j = i; j < G->numPlots; j++)
        {
            G->b[j][i] = G->b[i][j];
        }
    }

}



FmapWidget::~FmapWidget()
{
    delete ui;
}


//打开放缩
void FmapWidget::on_fsBtn_clicked()
{
    emit showzoom();
}

//返回菜单
void FmapWidget::on_remenu_clicked()
{
    this->hide();
    emit reshowmenu();
}

//退出程序
void FmapWidget::on_tcBtn_clicked()
{
    emit quit();
}

void FmapWidget::receivemenu()
{
    this->show();
}

void FmapWidget::receivezoom()
{
    this->show();
}



