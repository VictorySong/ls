#include "udpbroad.h"

udpbroad::udpbroad(QUdpSocket *tem ,QByteArray datagram ,int port,QString multicastip)
{
    if(tem == NULL || 0==datagram.size())
        exit(1);
    udpsender = tem;
    stopped = false;
    block = datagram;
    toport = port;
    this->multicast = multicastip;
}

void udpbroad::run()
{
    while(!stopped){
        udpsender->writeDatagram(block,QHostAddress(multicast),toport);
        qDebug()<<"已广播ip和端口";
        msleep(2000);           //每一秒向局域网内广播一次ip
    }
}

void udpbroad::stop()
{
    stopped = true;
}
