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
    emit newclientsocket(tcpClientSocket);
    qDebug()<<tcpClientSocketList;
}

void tcpserver::updateClients(QByteArray mes, tcpsocket *clientsocket)
{
    emit updateServer(mes,clientsocket);
    //添加信息内容，标明来源
    QString ip = clientsocket->peerAddress().toString();
    quint16 port = clientsocket->peerPort();
    QJsonParseError error;
    QJsonDocument jsondoc = QJsonDocument::fromJson(mes,&error);       //转化成json对象
    QJsonObject tem = jsondoc.object();
    tem.insert(QString("ip"),ip);
    tem.insert(QString("port"),port);
    QJsonDocument tem2;
    tem2.setObject(tem);
    QByteArray tem3 = tem2.toJson(QJsonDocument::Compact);

    QByteArray block;
    //使用数据流写入数据
    QDataStream out(&block,QIODevice::WriteOnly);
    //设置数据流的版本，客户端和服务器端使用的版本要相同
    out.setVersion(QDataStream::Qt_4_6);
    out<<tem3;          //不用数据流也可以，此处使用方便以后拓展



    //将位置信息转发到其他ip
    for(int i=0;i<tcpClientSocketList.count();i++)
    {
        QTcpSocket *item = tcpClientSocketList.at(i);
        if(item->peerAddress().toString() != ip){
            item->write(block);
        }else if(item->peerPort() != port){
            item->write(block);
        }
    }

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
            emit disconnected(clientsocket);
            qDebug()<<"断开连接";
            return;
        }
    }
    return;
}
