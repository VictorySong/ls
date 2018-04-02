#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <stable.h>
class tcpsocket;
class tcpserver : public QTcpServer
{
    Q_OBJECT
public:
    tcpserver(QObject *parent = 0);
    QHash<QString,tcpsocket*> tcpClientSocketList;
signals:
    void updateServer(QByteArray,tcpsocket *);
    void newclientsocket(tcpsocket *);
    void disconnected(tcpsocket *);
public slots:
    void updateClients(QByteArray,tcpsocket *);
    void slotDisconnected(tcpsocket *);
    void newverifiedclient(QString,tcpsocket *);
    void releasetcpsocket(tcpsocket *);                 //释放那部分没有通过验证的tcp连接
protected:
    void incomingConnection(int socketDescriptor);
};

#endif // TCPSERVER_H
