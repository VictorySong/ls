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
    buff = this->read(this->bytesAvailable());

    emit updateClients(buff,this);
}

void tcpsocket::slotDisconnected()
{
    disconnect(this,SIGNAL(readyRead()),this,SLOT(dataReceived()));
    connect(this,SIGNAL(readyRead()),this,SLOT(waitverification()));
    emit disconnected(this);
}

void tcpsocket::verifyidclient()
{
    verification tem;
    emit sendclientverifyresult("验证中..");
    if(tem.exec() == QDialog::Accepted){
        //发送位置信息
        QJsonObject json;
        json.insert("id",tem.id);
        json.insert("secret",tem.secret);
        QJsonDocument document;
        document.setObject(json);
        QByteArray datagram = document.toJson(QJsonDocument::Compact);  //将位置信息转化为json格式
        emit sendid(tem.id);                    //更新卫星窗口id
        if(this->isWritable())
            this->write(datagram);
    }else{
        emit sendclientverifyresult("fail");
        this->close();
    }
}

void tcpsocket::verifyidserver()
{
    qDebug()<<"有新数据";
    QByteArray buff;
    buff = this->read(this->bytesAvailable());          //把数据从缓存区取出
    qDebug()<<buff;
    emit verifyserver(buff,this);
}

void tcpsocket::waitverification()
{
    QByteArray buff;
    buff = this->read(this->bytesAvailable());
    if(QString("allow").toLatin1() == buff){
        //验证成功后
        disconnect(this,SIGNAL(readyRead()),this,SLOT(waitverification()));
        connect(this,SIGNAL(readyRead()),this,SLOT(dataReceived()));
        emit sendclientverifyresult("success");
    }else{
        QMessageBox::information(0, QString("验证出错！"), QString("id或密码错误"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        this->close();
    }
}
