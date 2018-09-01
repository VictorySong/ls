#include "tcpserver.h"
#include "tcpsocket.h"
#include "server.h"

tcpserver::tcpserver(QObject *parent):
    QTcpServer(parent)
{
    //创建存储文件的文件夹
    filefolder=QString("C:/lsfile");
    QDir dir(filefolder);
    if(!dir.exists())
        dir.mkpath(filefolder);//创建多级目录
}

void tcpserver::incomingConnection(int socketDescriptor)
{
    qDebug()<<"有新连接请求";
    //创建一个新的TcpClientSocket与客户端通信
    tcpsocket *tcpClientSocket=new tcpsocket(this,2);           //服务端
    //将新创建的TcpClientSocket的套接字描述符指定为参数socketDescriptor
    tcpClientSocket->setSocketDescriptor(socketDescriptor);//(tcp连接建成)
    //将验证通过信号和验证通过处理程序关联
    connect(tcpClientSocket,SIGNAL(verificationpassed(QString,tcpsocket*)),this,SLOT(newverifiedclient(QString,tcpsocket*)));
    connect(tcpClientSocket,SIGNAL(deletetcpsocket(tcpsocket*)),this,SLOT(releasetcpsocket(tcpsocket*)));           //验证不通过后释放该连接内存
    connect(tcpClientSocket,SIGNAL(verifyserver(QByteArray,tcpsocket*)),this,SLOT(verifyserver(QByteArray,tcpsocket*)));    //验证程序
}

void tcpserver::updateClients(QByteArray mes, tcpsocket *clientsocket)
{
    //添加信息内容，标明来源
    QString ip = clientsocket->peerAddress().toString();
    quint16 port = clientsocket->peerPort();
    QJsonParseError error;
    QJsonDocument jsondoc = QJsonDocument::fromJson(mes,&error);       //转化成json对象
    if(error.error == 0){
        QJsonObject tem = jsondoc.object();
        tem.insert(QString("ip"),ip);
        tem.insert(QString("port"),port);

        if(tem.value(QString("type")).toString() == QString("location")){
            //插入卫星id
            QHashIterator <QString,tcpsocket *> h(tcpClientSocketList);
            while(h.hasNext()){
                h.next();
                if(h.value()->peerAddress().toString() == ip && h.value()->peerPort() == port){
                    tem.insert(QString("id"),h.key());
                    emit updateServer(mes,clientsocket,h.key());
                    break;
                }
            }

            QJsonDocument tem2;
            tem2.setObject(tem);
            QByteArray tem3 = tem2.toJson(QJsonDocument::Compact);

            //将位置信息转发到其他ip
            QHashIterator <QString,tcpsocket *> i(tcpClientSocketList);
            while(i.hasNext()){
                i.next();
                if(i.value()->peerAddress().toString() != ip){
                    i.value()->write(tem3);
                }else if(i.value()->peerPort() != port){
                    qDebug()<<"转发";
                    i.value()->write(tem3);
                }
            }

            inf teminf = server::locationlist.value(tem.value("id").toString());
            int r,g,b;
            //获取颜色 并转发到手机端
            teminf.color.getRgb(&r,&g,&b);
            tem.insert("R",r);
            tem.insert("G",g);
            tem.insert("B",b);
            tem2.setObject(tem);
            tem3 = tem2.toJson(QJsonDocument::Compact);
            QMutableListIterator<tcpsocket*> j(tcpphonesocket);
            while (j.hasNext()) {
                j.next();
                j.value()->write(tem3);
            }
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
            if(tcpFileSocketList.contains(i.key()))
                tcpFileSocketList.value(i.key())->abort();
            tcpClientSocketList.remove(i.key());
            emit disconnected(clientsocket);

            //通知手机端某卫星断开
            QMutableListIterator<tcpsocket*> k(tcpphonesocket);
            QJsonObject tem ;
            tem.insert("id",i.key());
            tem.insert("status",0);             //状态为0表示断开
            QJsonDocument tem2;
            tem2.setObject(tem);
            QByteArray tem3 = tem2.toJson(QJsonDocument::Compact);
            while (k.hasNext()) {
                k.next();
                k.value()->write(tem3);
            }

            break;
        }
    }

    QHashIterator <QString,tcpsocket *> k(tcpFileSocketList);
    while (k.hasNext()) {
        k.next();
        if(k.value()->socketDescriptor() ==t){
            tcpFileSocketList.remove(k.key());
            clientsocket->close();
            qDebug()<<"删除一个文件客户端";
            break;
        }
    }

    QMutableListIterator<tcpsocket*> j(tcpphonesocket);
    while (j.hasNext()) {
        j.next();
        if(j.value()->socketDescriptor() ==t){
            j.remove();
            clientsocket->close();
            qDebug()<<"删除一个手机客户端";
            break;
        }
    }
    clientsocket->deleteLater();    //释放内存
}

void tcpserver::newverifiedclient(QString id, tcpsocket *clientsocket)
{
    //连接TcpClientSocket的updateClients（）信号
    connect(clientsocket,SIGNAL(updateClients(QByteArray,tcpsocket *)),this,SLOT(updateClients(QByteArray,tcpsocket *)));
    //连接TcpClientSocket的disconnected（）信号
    connect(clientsocket,SIGNAL(disconnected(tcpsocket *)),this,SLOT(slotDisconnected(tcpsocket *)));
    //将tcpClientSocket加入客户端套接字列表以便管理
    tcpClientSocketList.insert(id,clientsocket);
    emit newclientsocket(id,clientsocket);

    //通知手机端某卫星连接
    QMutableListIterator<tcpsocket*> k(tcpphonesocket);
    QJsonObject tem ;
    tem.insert("id",id);
    tem.insert("status",1);             //状态为1表示连接
    QJsonDocument tem2;
    tem2.setObject(tem);
    QByteArray tem3 = tem2.toJson(QJsonDocument::Compact);
    while (k.hasNext()) {
        k.next();
        k.value()->write(tem3);
    }
}

void tcpserver::releasetcpsocket(tcpsocket *clientsocket)
{
    clientsocket->close();
    clientsocket->deleteLater();    //释放内存
}

void tcpserver::verifyserver(QByteArray buff,tcpsocket *clientsocket)
{
    QByteArray tem = "phone";
    //验证客户端   这是在服务器类中， 与tcpsocket类中的verifyidserver 的注释部分相似
    QJsonParseError error;
    QJsonDocument jsondoc = QJsonDocument::fromJson(buff,&error);       //转化成json对象
    QVariantMap result = jsondoc.toVariant().toMap();
    if(error.error == 0 && result.contains("id") && result["id"].toString()!=QString("")){
        if(result["type"].toString()==QString("client")){
            //如果是文本客户端
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
                    emit clientsocket->verificationpassed(result["id"].toString(),clientsocket);        //触发验证通过的信号
                }
            }else{
                //列表中已经有该id的连接
                clientsocket->abort();
                emit clientsocket->deletetcpsocket(clientsocket);                     //发送释放这个内存的信号
                return;
            }
        }else if(result.value("type").toString()==QString("clientfile")){
            //如果是文件客户端
            qDebug()<<tcpClientSocketList;
            qDebug()<<tcpFileSocketList;
            if(tcpClientSocketList.contains(result["id"].toString())){
                if(!tcpFileSocketList.contains(result["id"].toString())){
                    disconnect(clientsocket,SIGNAL(readyRead()),clientsocket,SLOT(verifyidserver()));
                    connect(clientsocket,SIGNAL(readyRead()),clientsocket,SLOT(dataReceived_file()));
                    //连接TcpClientSocket的updateClients（）信号
                    connect(clientsocket,SIGNAL(updateClients_file(qint64,qint64,QString,tcpsocket *)),this,SLOT(updateClients_file(qint64,qint64,QString,tcpsocket *)));
                    //连接TcpClientSocket的disconnected（）信号
                    connect(clientsocket,SIGNAL(disconnected(tcpsocket *)),this,SLOT(slotDisconnected(tcpsocket *)));
                    //连接新传入文件信号
                    connect(clientsocket,SIGNAL(updateClients_newfile(qint64,qint64,QString,tcpsocket*)),this,SLOT(updateClients_newfile(qint64,qint64,QString,tcpsocket*)));
                    tcpFileSocketList.insert(result["id"].toString(),clientsocket);
                    //创建文件夹
                    QString folder = filefolder+QString("/%1").arg(result["id"].toString());
                    QDir dir(folder);
                    if(!dir.exists())
                        dir.mkpath(folder);//创建多级目录
                    clientsocket->filefolder = folder;

                    //回复同意连接
                    QByteArray datagram = "allow";
                    if(clientsocket->isWritable())
                        clientsocket->write(datagram);
                    qDebug()<<"有文件客户端接入";
                }
            }
        }
    }else if(buff == tem){
        //如果来自手机客户端的连接
        newphone(clientsocket);
        disconnect(clientsocket,SIGNAL(readyRead()),clientsocket,SLOT(verifyidserver()));
        connect(clientsocket,SIGNAL(readyRead()),clientsocket,SLOT(dataReceived()));
    }else{
        clientsocket->abort();
        emit clientsocket->deletetcpsocket(clientsocket);                     //发送释放这个内存的信号
        return;
    }
}

void tcpserver::newphone(tcpsocket *clientsocket)
{
    tcpphonesocket.append(clientsocket);
    //连接TcpClientSocket的updateClients（）信号
    connect(clientsocket,SIGNAL(updateClients(QByteArray,tcpsocket *)),
            this,SLOT(updateClients(QByteArray,tcpsocket *)));

    //连接TcpClientSocket的disconnected（）信号
    connect(clientsocket,SIGNAL(disconnected(tcpsocket *)),
            this,SLOT(slotDisconnected(tcpsocket *)));
}

void tcpserver::updateClients_file(qint64 bytesreceived,qint64 totalbytes,QString filename,tcpsocket *clientsocket)
{
    //有文件数据传入时
    QString ip = clientsocket->peerAddress().toString();
    quint16 port = clientsocket->peerPort();
    QHashIterator <QString,tcpsocket *> h(tcpFileSocketList);
    while(h.hasNext()){
        h.next();
        if(h.value()->peerAddress().toString() == ip && h.value()->peerPort() == port){
            emit updateServer_file(bytesreceived,totalbytes,filename,clientsocket,h.key());
            break;
        }
    }
}
void tcpserver::updateClients_newfile(qint64 bytesreceived, qint64 totalbytes, QString filename, tcpsocket *clientsocket)
{
    QString ip = clientsocket->peerAddress().toString();
    quint16 port = clientsocket->peerPort();
    QHashIterator <QString,tcpsocket *> h(tcpFileSocketList);
    while(h.hasNext()){
        h.next();
        if(h.value()->peerAddress().toString() == ip && h.value()->peerPort() == port){
            emit updateServer_newfile(bytesreceived,totalbytes,filename,clientsocket,h.key());
            break;
        }
    }
}
