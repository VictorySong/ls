#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <stable.h>

class tcpsocket : public QTcpSocket
{
    Q_OBJECT
public:
    tcpsocket(QObject *parent=0);
signals:
    void updateClients(QByteArray,tcpsocket *);
    void disconnected(tcpsocket *);
protected slots:
    void dataReceived();
    void slotDisconnected();

};

#endif // TCPSOCKET_H
