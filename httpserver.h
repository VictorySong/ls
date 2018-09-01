#ifndef HTTPSERVER_H
#define HTTPSERVER_H
#include <stable.h>
#include <httpsocket.h>
class httpserver : public QTcpServer
{
    Q_OBJECT
public:
    httpserver(QObject *parent = 0);
protected:
    void incomingConnection(qintptr handle);
public slots:
    void socketdestoryed(QObject *);
};

#endif // HTTPSERVER_H
