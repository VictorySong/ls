#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <stable.h>
#include <winpcap.h>
#include <tcpserver.h>
#include <tcpsocket.h>

namespace Ui {
class client;
}

class client : public QWidget
{
    Q_OBJECT

public:
    explicit client(QWidget *parent = 0,winpcap * tem = NULL);
    ~client();

private:
    Ui::client *ui;
    winpcap *arp;
    QUdpSocket *udpServer;              //udp监听
    tcpsocket *tcpsender;              //tcp发送
public slots:
    void udpget();                      //处理接收到的udp信息
    void tcpconnected();                //连接成功后
    void newdata(QByteArray,tcpsocket*);        //有新数据到达时
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
};

#endif // CLIENT_H
