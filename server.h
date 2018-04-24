#ifndef SERVER_H
#define SERVER_H

#include <stable.h>
#include <QWidget>
#include <tcpserver.h>
#include <tcpsocket.h>
#include <udpbroad.h>

#include <QtGui>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QSize>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QPixmap>

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
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();


private:
    Ui::server *ui;
    QUdpSocket *udpsender;              //udp发送
    tcpserver *tcpServer;              //tcp监听
    udpbroad *udpbro;                   //广播服务器ip 和端口的 线程
    QHash<QString,inf> locationlist;            //记录卫星最新位置

    QPixmap pix;                            //图层

    QPoint lastpoint;                   //起点
    QPoint endpoint;                    //终点3
    QGraphicsScene scene;
    QGraphicsItem *lineItemPointer[12];
    QGraphicsItemGroup group;

    void socketinit();                  //初始化socket

public slots:
    void updatetabelwidget(QByteArray,tcpsocket *);         //更新已有连接界面显示
    void updatenewclient(tcpsocket *);                      //有新连接时更新界面显示
    void disconnected(tcpsocket *);                         //有连接断开时更新视

protected:
    void paintEvent(QPaintEvent *);

};

#endif // SERVER_H
