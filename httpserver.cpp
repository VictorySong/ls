#include "httpserver.h"

httpserver::httpserver(QObject *parent):
    QTcpServer(parent)
{

}

void httpserver::incomingConnection(qintptr handle)
{
    httpsocket *clientsocket = new httpsocket(this);
    clientsocket->setSocketDescriptor(handle);
    connect(clientsocket,SIGNAL(destroyed(QObject*)),this,SLOT(socketdestoryed(QObject*)));
}

void httpserver::socketdestoryed(QObject *tme)
{
    qDebug()<<"连接断开";
}
