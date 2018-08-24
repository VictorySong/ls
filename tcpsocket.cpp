#include "tcpsocket.h"

tcpsocket::tcpsocket(QObject *parent ,int type,QString s_id):
    QTcpSocket(parent)
{
    this->id = s_id;
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
    {
        totalBytes = 0;
        bytesReceived = 0;
        fileNameSize = 0;
        connect(this,SIGNAL(readyRead()),this,SLOT(verifyidserver()));
        //disconnected()信号在断开连接时发出
        connect(this,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
        break;
    }
    case 3:                     //客户端 传输文件
        //disconnected()信号在断开连接时发出
        connect(this,SIGNAL(disconnected()),this,SLOT(slotDisconnected()));
        //tcp连接发送验证信息
        connect(this,SIGNAL(connected()),this,SLOT(verifyidclient_file()));
        //等待服务器允许接入的回复
        connect(this,SIGNAL(readyRead()),this,SLOT(waitverification()));
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
        //发送验证信息
        QJsonObject json;
        json.insert("id",tem.id);
        json.insert("secret",tem.secret);
        json.insert("type",QString("client"));
        QJsonDocument document;
        document.setObject(json);
        QByteArray datagram = document.toJson(QJsonDocument::Compact);  //将信息转化为json格式
        emit sendid(tem.id);                    //更新卫星窗口id
        if(this->isWritable())
            this->write(datagram);
    }else{
        emit sendclientverifyresult("abort");
        this->close();
    }
}

void tcpsocket::verifyidserver()
{
    qDebug()<<"服务端有新数据验证";
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
        emit sendclientverifyresult("fail");
        this->close();
    }
}

void tcpsocket::verifyidclient_file()
{
    qDebug()<<"客户端发起文件传输连接";
    QJsonObject json;
    json.insert("id",this->id);
    json.insert("type",QString("clientfile"));
    QJsonDocument document;
    document.setObject(json);
    QByteArray datagram = document.toJson(QJsonDocument::Compact);  //将信息转化为json格式
    if(this->isWritable())
        this->write(datagram);
}

void tcpsocket::dataReceived_file()
{
    QDataStream in(this);
    in.setVersion(QDataStream::Qt_4_6);
    if(bytesReceived <= sizeof(qint64)*2){
       //如果接收到的数据小于16个字节，那么是刚开始接收数据，我们保存到//来的头文件信息
       if((this->bytesAvailable() >= sizeof(qint64)*2) && (fileNameSize == 0)){
           //接收数据总大小信息和文件名大小信息
           in >> totalBytes >> fileNameSize;
           bytesReceived += sizeof(qint64) * 2;
       }
       if((this->bytesAvailable() >= fileNameSize) && (fileNameSize != 0)){
           //接收文件名，并建立文件
           in >> fileName;
           qDebug()<<"接收文件"<<fileName;
           bytesReceived += fileNameSize;
           //检查文件夹是否存在
           QDir dir(filefolder);
           if(!dir.exists())
               dir.mkpath(filefolder);//创建多级目录
           localFile= new QFile(this->filefolder+QString("/%1").arg(fileName));
           if(!localFile->open(QFile::WriteOnly))
           {
                qDebug() << "open file error!";
                return;
           }
           emit updateClients_newfile(bytesReceived,totalBytes,fileName,this);  //发送有新文件的信号
       }
       else return;
    }
    if(bytesReceived < totalBytes){
      //如果接收的数据小于总数据，那么写入文件
      bytesReceived += this->bytesAvailable();
      inBlock= this->readAll();
      localFile->write(inBlock);
      emit updateClients_file(bytesReceived,totalBytes,fileName,this);  //更新文件接收的大小
      inBlock.resize(0);
    }
    if(bytesReceived == totalBytes){
        totalBytes = 0;
        bytesReceived = 0;
        fileNameSize = 0;
        localFile->close();
    }
}
