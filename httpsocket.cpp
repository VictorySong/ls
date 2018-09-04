#include "httpsocket.h"

httpsocket::httpsocket(QObject *parent ):
    QTcpSocket(parent)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(httpreadyread()));
    connect(this,SIGNAL(disconnected()),this,SLOT(deleteLater()));
}

void httpsocket::httpreadyread()
{
    QByteArray buff;
    buff = this->read(this->bytesAvailable());
    qDebug()<<buff;

    QString http,response;
    QByteArray headData, data;
    QFile file("C:/lsfile/ls.apk");
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug("http file open failed!");
        response = QString("\"C:/lsfile/ls.apk\"文件不存在,请确认将软件复制到该路径");
        data.append(response);
        http = "HTTP/1.1 200 OK\r\n";
        http += "Server: nginx\r\n";
        http += "Content-Type: text/html;charset=utf-8\r\n";
        http += QString("Content-Length: %1\r\n\r\n").arg(QString::number(data.size()));
        headData.append(http);
        this->write(headData);
        this->write(data);
        return;
    }
    http = "HTTP/1.1 200 OK\r\n";
    http += "Server: nginx\r\n";
    http += "Content-Type: application/vnd.android.package-archive;charset=utf-8\r\n";
    http += "Connection: keep-alive\r\n";
    http += "Content-Disposition: attachment; filename=微小卫星.apk\r\n";
    http += QString("Content-Length: %1\r\n\r\n").arg(QString::number(file.size()));
    headData.append(http);
    this->write(headData);

    while(!file.atEnd()) {
        if(this->isWritable()){
            data = file.read(10240);        //每次读取10k的数据，并发送
            this->write(data);
        }
    }

    file.close();
    qDebug("http发送文件成功");
}
