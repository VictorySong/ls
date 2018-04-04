#include "tcpserver.h"
#include "tcpsocket.h"

tcpserver::tcpserver(QObject *parent):
    QTcpServer(parent)
{

}

void tcpserver::incomingConnection(int socketDescriptor)
{
    qDebug()<<"有新连接请求";
    //创建一个新的TcpClientSocket与客户端通信
    tcpsocket *tcpClientSocket=new tcpsocket(this,2);           //服务端

    //将新创建的TcpClientSocket的套接字描述符指定为参数socketDescriptor
    tcpClientSocket->setSocketDescriptor(socketDescriptor);
    //将验证通过信号和验证通过处理程序关联
    connect(tcpClientSocket,SIGNAL(verificationpassed(QString,tcpsocket*)),this,SLOT(newverifiedclient(QString,tcpsocket*)));
    connect(tcpClientSocket,SIGNAL(deletetcpsocket(tcpsocket*)),this,SLOT(releasetcpsocket(tcpsocket*)));           //验证不通过后释放该连接内存
    connect(tcpClientSocket,SIGNAL(verifyserver(QByteArray,tcpsocket*)),this,SLOT(verifyserver(QByteArray,tcpsocket*)));    //验证程序
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
    QHashIterator <QString,tcpsocket *> i(tcpClientSocketList);
    while(i.hasNext()){
        i.next();
        if(i.value()->peerAddress().toString() != ip){
            i.value()->write(block);
        }else if(i.value()->peerPort() != port){
            qDebug()<<"转发";
            i.value()->write(block);
        }
    }
}

void tcpserver::slotDisconnected(tcpsocket *clientsocket)
{
    qintptr t = clientsocket->socketDescriptor();
    QHashIterator <QString,tcpsocket *> i(tcpClientSocketList);
    while(i.hasNext()){
        i.next();
        if(i.value()->socketDescriptor() == t){
            tcpClientSocketList.remove(i.key());
            emit disconnected(clientsocket);
            return;
        }
    }

    return;
}

void tcpserver::newverifiedclient(QString id, tcpsocket *clientsocket)
{
    //连接TcpClientSocket的updateClients（）信号
    connect(clientsocket,SIGNAL(updateClients(QByteArray,tcpsocket *)),
            this,SLOT(updateClients(QByteArray,tcpsocket *)));

    //连接TcpClientSocket的disconnected（）信号
    connect(clientsocket,SIGNAL(disconnected(tcpsocket *)),
            this,SLOT(slotDisconnected(tcpsocket *)));

    //将tcpClientSocket加入客户端套接字列表以便管理
    tcpClientSocketList.insert(id,clientsocket);
    emit newclientsocket(clientsocket);
    qDebug()<<tcpClientSocketList;

}

void tcpserver::releasetcpsocket(tcpsocket *clientsocket)
{
    delete clientsocket;
}

void tcpserver::verifyserver(QByteArray buff,tcpsocket *clientsocket)
{
    qDebug()<<buff;

    //验证客户端   这是在服务器类中， 与tcpsocket类中的verifyidserver 的注释部分相似
    QJsonParseError error;
    QJsonDocument jsondoc = QJsonDocument::fromJson(buff,&error);       //转化成json对象
    QVariantMap result = jsondoc.toVariant().toMap();
    if(!tcpClientSocketList.contains(result["id"].toString())){
        if(result["secret"].toString() == QString("123")){
            disconnect(clientsocket,SIGNAL(readyRead()),clientsocket,SLOT(verifyidserver()));
            connect(clientsocket,SIGNAL(readyRead()),clientsocket,SLOT(dataReceived()));
            emit clientsocket->verificationpassed(result["id"].toString(),clientsocket);

            //回复同意连接
            QByteArray datagram = "allow";

            if(clientsocket->isWritable())
                clientsocket->write(datagram);

            qDebug()<<"同意连接";
        }else{
            //请求是否同意连接
            QString tem = QString("有新的连接请求，IP：%1  端口：%2 卫星id：%3 卫星接入密码：%4 是否同意连接").arg(clientsocket->peerAddress().toString()).arg(clientsocket->peerPort()).arg(result["id"].toString()).arg(result["secret"].toString());
            if(QMessageBox::No == QMessageBox::question(0,
                                                         tr("Question"),
                                                         tem,
                                                         QMessageBox::Yes | QMessageBox::No,
                                                        QMessageBox::No)){
                qDebug()<<clientsocket;
                clientsocket->abort();
                emit clientsocket->deletetcpsocket(clientsocket);                     //发送释放这个内存的信号
                return;
            }
            //回复同意连接
            QByteArray datagram = "allow";

            if(clientsocket->isWritable())
                clientsocket->write(datagram);

            //验证通过
            disconnect(clientsocket,SIGNAL(readyRead()),clientsocket,SLOT(verifyidserver()));
            connect(clientsocket,SIGNAL(readyRead()),clientsocket,SLOT(dataReceived()));
            emit clientsocket->verificationpassed(result["id"].toString(),clientsocket);
        }
    }else{
        //列表中已经有该id的连接
        clientsocket->abort();
        emit clientsocket->deletetcpsocket(clientsocket);                     //发送释放这个内存的信号
        return;
    }
}
