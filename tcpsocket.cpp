#include "tcpsocket.h"

tcpsocket::tcpsocket(QObject *parent):
    QTcpSocket(parent)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    //disconnected()信号在断开连接时发出
    connect(this,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
}

void tcpsocket::dataReceived()
{
    QByteArray buff;
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_6);
    //设置数据流版本，这里要和服务器端相同
    in>>buff;
//    buff = this->read(100);
    emit updateClients(buff,this);
}

void tcpsocket::slotDisconnected()
{
    emit disconnected(this);
}
