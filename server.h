#ifndef SERVER_H
#define SERVER_H

#include <stable.h>
#include <QWidget>
#include <tcpserver.h>
#include <tcpsocket.h>
#include <udpbroad.h>
#include <instruction.h>
#include <QToolBar>
#include <QProgressBar>
#include <cameraview.h>

#include <QtGui>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QSize>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QGraphicsRectItem>
#include <QAbstractGraphicsShapeItem>

namespace Ui {
class server;
}

class server : public QWidget
{
    Q_OBJECT

public:
    explicit server(QWidget *parent = 0);
    ~server();
    void wificonnected();                       //wifi连接成功后
    static QHash<QString,inf> locationlist;            //记录卫星最新位置
    QHash<QString,QProgressBar *> filelist;            //记录已传输的文件

private slots:
    void tablewidget_clicked(int,int);

private:
    Ui::server *ui;
    QUdpSocket *udpsender;              //udp发送
    tcpserver *tcpServer;              //tcp监听
    udpbroad *udpbro;                   //广播服务器ip 和端口的 线程
    cameraview *cview;
    QCamera *camera;

    QPixmap pix;                            //图层
    QPoint lastpoint;                   //起点
    QPoint endpoint;                    //终点3
    QGraphicsScene scene;               //用于绘制轨迹
    QGraphicsLineItem **lineItemPointer;
    //二级指针，用于指向各个客户端记录轨迹段数据的lineItemPoninter指针数组，便于绘制轨迹
    int lineItemNum=0;
    QPen pen;
    QBrush brush;

    void socketinit();                  //初始化socket

public slots:
    void updatetabelwidget(QByteArray,tcpsocket *,QString);         //更新已有连接界面显示
    void updatenewclient(QString,tcpsocket *);                      //有新连接时更新界面显示
    void disconnected(tcpsocket *);                         //有连接断开时更新视
    void toolbar_actiontriggered(QAction *);       //工具栏action被点击
    void updatefileview(qint64,qint64,QString,tcpsocket*,QString);  //有文件传入时更新界面
    void updatefileview_new(qint64,qint64,QString,tcpsocket*,QString);  //有新文件传入时更新界面

};

#endif // SERVER_H
