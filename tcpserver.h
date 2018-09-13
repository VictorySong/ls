#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <stable.h>


class tcpsocket;
class tcpserver : public QTcpServer
{
    Q_OBJECT
public:
    tcpserver(QObject *parent = 0);
    QHash<QString,tcpsocket*> tcpClientSocketList;              //卫星
    QList<tcpsocket *> tcpphonesocket;                      //手机客户端
    QHash<QString,tcpsocket*> tcpFileSocketList;    //文件
    QString filefolder; //接收文件保存的文件夹
signals:
    void updateServer(QByteArray,tcpsocket *,QString);
    void updateServer_file(qint64,qint64,QString,tcpsocket *,QString);
    void updateServer_newfile(qint64,qint64,QString,tcpsocket *,QString);   //有新文件传入
    void newclientsocket(QString,tcpsocket *);
    void disconnected(tcpsocket *,QString);
public slots:
    void updateClients(QByteArray,tcpsocket *);
    void updateClients_file(qint64,qint64,QString,tcpsocket *);
    void updateClients_newfile(qint64,qint64,QString,tcpsocket *);
    void slotDisconnected(tcpsocket *);
    void newverifiedclient(QString,tcpsocket *);
    void releasetcpsocket(tcpsocket *);                 //释放那部分没有通过验证的tcp连接
    void verifyserver(QByteArray,tcpsocket *);                                //验证程序
    void newphone(tcpsocket*);                      //有新的手机加入时
protected:
    void incomingConnection(int socketDescriptor);
};

#endif // TCPSERVER_H
