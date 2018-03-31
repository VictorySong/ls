#ifndef SERVER_H
#define SERVER_H

#include <stable.h>
#include <QWidget>
#include <winpcap.h>
#include <tcpserver.h>
#include <tcpsocket.h>
#include <udpbroad.h>

namespace Ui {
class server;
}

class server : public QWidget
{
    Q_OBJECT

public:
    explicit server(QWidget *parent = 0,winpcap *tem = NULL);
    ~server();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::server *ui;
    winpcap *arp;
    QUdpSocket *udpsender;              //udp发送
    tcpserver *tcpServer;              //tcp监听
    udpbroad *udpbro;                   //广播服务器ip 和端口的 线程

public slots:
    void updatetabelwidget(QByteArray,tcpsocket *);         //更新已有连接界面显示
    void updatenewclient(tcpsocket *);                      //有新连接时更新界面显示
    void disconnected(tcpsocket *);                         //有连接断开时更新视图

protected:
    void paintEvent(QPaintEvent *);

};

#endif // SERVER_H
