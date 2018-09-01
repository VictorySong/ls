#ifndef HTTPSOCKET_H
#define HTTPSOCKET_H

#include <stable.h>

class httpsocket : public QTcpSocket
{
    Q_OBJECT
public:
    httpsocket(QObject *parent = 0);
private slots:
    void httpreadyread();
};

#endif // HTTPSOCKET_H
