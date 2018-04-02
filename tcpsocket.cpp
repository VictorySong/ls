#include "tcpsocket.h"

tcpsocket::tcpsocket(QObject *parent ,int type):
    QTcpSocket(parent)
{
    switch(type){
    case 1:                     //客户端
        //disconnected()信号在断开连接时发出
        connect(this,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
        //tcp连接后弹出对话框输入 卫星id和接入密钥
        connect(this,SIGNAL(connected()),this,SLOT(verifyidclient()));
        //等待服务器允许接入的回复
        connect(this,SIGNAL(readyRead()),this,SLOT(waitverification()));
        break;
    case 2:                     //服务端
        connect(this,SIGNAL(readyRead()),this,SLOT(verifyidserver()));
        //disconnected()信号在断开连接时发出
        connect(this,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
        break;
    default:
        connect(this,SIGNAL(readyRead()),this,SLOT(dataReceived()));
        //disconnected()信号在断开连接时发出
        connect(this,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
        break;

    }




}

void tcpsocket::dataReceived()
{
    QByteArray buff;
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_6);
    //设置数据流版本，这里要和服务器端相同
    in>>buff;

    emit updateClients(buff,this);
}

void tcpsocket::slotDisconnected()
{
    emit disconnected(this);
}

void tcpsocket::verifyidclient()
{
    verification tem;
    if(tem.exec() == QDialog::Accepted){
        //发送位置信息
        QJsonObject json;
        json.insert("id",tem.id);
        json.insert("secret",tem.secret);
        QJsonDocument document;
        document.setObject(json);
        QByteArray datagram = document.toJson(QJsonDocument::Compact);  //将位置信息转化为json格式
        QByteArray block;
        //使用数据流写入数据
        QDataStream out(&block,QIODevice::WriteOnly);
        //设置数据流的版本，客户端和服务器端使用的版本要相同
        out.setVersion(QDataStream::Qt_4_6);
        out<<datagram;
        if(this->isWritable())
            this->write(block);
    }
}

void tcpsocket::verifyidserver()
{
    //验证客户端
    QByteArray buff;
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_6);
    //设置数据流版本，这里要和服务器端相同
    in>>buff;
    QJsonParseError error;
    QJsonDocument jsondoc = QJsonDocument::fromJson(buff,&error);       //转化成json对象
    QVariantMap result = jsondoc.toVariant().toMap();
    if(result["secret"].toString() == QString("123")){
        disconnect(this,SIGNAL(readyRead()),this,SLOT(verifyidserver()));
        connect(this,SIGNAL(readyRead()),this,SLOT(dataReceived()));
        emit verificationpassed(result["id"].toString(),this);

        //回复同意连接
        QByteArray datagram = "allow";
        QByteArray block;
        //使用数据流写入数据
        QDataStream out(&block,QIODevice::WriteOnly);
        //设置数据流的版本，客户端和服务器端使用的版本要相同
        out.setVersion(QDataStream::Qt_4_6);
        out<<datagram;
        if(this->isWritable())
            this->write(block);

        qDebug()<<"同意连接";
    }else{
        //请求是否同意连接
        QString tem = QString("有新的连接请求，IP：%1  端口：%2 卫星id：%3 卫星接入密码：%4 是否同意连接").arg(this->peerAddress().toString()).arg(this->peerPort()).arg(result["id"].toString()).arg(result["secret"].toString());
        if(QMessageBox::No == QMessageBox::question(0,
                                                     tr("Question"),
                                                     tem,
                                                     QMessageBox::Yes | QMessageBox::No,
                                                    QMessageBox::No)){
            qDebug()<<this;
            this->abort();
            emit deletetcpsocket(this);                     //发送释放这个内存的信号
            return;
        }
        //回复同意连接
        QByteArray datagram = "allow";
        QByteArray block;
        //使用数据流写入数据
        QDataStream out(&block,QIODevice::WriteOnly);
        //设置数据流的版本，客户端和服务器端使用的版本要相同
        out.setVersion(QDataStream::Qt_4_6);
        out<<datagram;
        if(this->isWritable())
            this->write(block);

        //验证通过
        disconnect(this,SIGNAL(readyRead()),this,SLOT(verifyidserver()));
        connect(this,SIGNAL(readyRead()),this,SLOT(dataReceived()));
        emit verificationpassed(result["id"].toString(),this);
    }
}

void tcpsocket::waitverification()
{
    QByteArray buff;
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_6);
    //设置数据流版本，这里要和服务器端相同
    in>>buff;
    if(QString("allow").toLatin1() == buff){
        //验证成功后
        disconnect(this,SIGNAL(readyRead()),this,SLOT(waitverification()));
        connect(this,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    }else{
        QMessageBox::information(0, QString("验证出错！"), QString("id或密码错误"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        this->close();
    }
}
