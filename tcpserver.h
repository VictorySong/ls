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

signals:
    void updateServer(QByteArray,tcpsocket *,QString);
    void updateServer_file(QByteArray,tcpsocket *,QString);
    void newclientsocket(QString,tcpsocket *);
    void disconnected(tcpsocket *);
public slots:
    void updateClients(QByteArray,tcpsocket *);
    void updateClients_file(QByteArray,tcpsocket *);
    void slotDisconnected(tcpsocket *);
    void newverifiedclient(QString,tcpsocket *);
    void releasetcpsocket(tcpsocket *);                 //释放那部分没有通过验证的tcp连接
    void verifyserver(QByteArray,tcpsocket *);                                //验证程序
    void newphone(tcpsocket*);                      //有新的手机加入时
protected:
    void incomingConnection(int socketDescriptor);
};

#endif // TCPSERVER_H
