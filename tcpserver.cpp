#include "tcpserver.h"
#include "tcpsocket.h"

tcpserver::tcpserver(QObject *parent):
    QTcpServer(parent)
{

}

void tcpserver::incomingConnection(int socketDescriptor)
{
    //创建一个新的TcpClientSocket与客户端通信
    tcpsocket *tcpClientSocket=new tcpsocket(this);
    //连接TcpClientSocket的updateClients（）信号
    connect(tcpClientSocket,SIGNAL(updateClients(QByteArray,tcpsocket *)),
            this,SLOT(updateClients(QByteArray,tcpsocket *)));

    //连接TcpClientSocket的disconnected（）信号
    connect(tcpClientSocket,SIGNAL(disconnected(tcpsocket *)),
            this,SLOT(slotDisconnected(tcpsocket *)));
    //将新创建的TcpClientSocket的套接字描述符指定为参数socketDescriptor
    tcpClientSocket->setSocketDescriptor(socketDescriptor);
    //将tcpClientSocket加入客户端套接字列表以便管理
    tcpClientSocketList.append(tcpClientSocket);
    qDebug()<<tcpClientSocketList;
}

void tcpserver::updateClients(QByteArray mes, tcpsocket *clientsocket)
{
    qDebug()<<mes;
    emit updateServer(mes,clientsocket);
}

void tcpserver::slotDisconnected(tcpsocket *clientsocket)
{
    qintptr t = clientsocket->socketDescriptor();
    for(int i=0;i<tcpClientSocketList.count();i++)
    {
        QTcpSocket *item = tcpClientSocketList.at(i);
        if(item->socketDescriptor()== t)
        {
            tcpClientSocketList.removeAt(i);
            qDebug()<<"断开连接";
            return;
        }
    }
    return;
}
