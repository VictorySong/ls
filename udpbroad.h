#ifndef UDPBROAD_H
#define UDPBROAD_H

#include <stable.h>

class udpbroad : public QThread
{
    Q_OBJECT
public:
    udpbroad(QUdpSocket *tem = NULL ,QByteArray datagram = "",int port = 0,QString multicastip = "255.255.255.255");
    void stop();
private:
    QUdpSocket * udpsender;
    volatile bool stopped;                  //停止标志 初始化为false
    QByteArray block;                       //udp广播数据 包含服务器ip 和端口
    int toport;                             //广播的端口
    QString multicast;                      //多播地址
protected:
    void run();                     //运行线程
};

#endif // UDPBROAD_H
