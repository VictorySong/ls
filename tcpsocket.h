#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <stable.h>
#include <verification.h>
class tcpsocket : public QTcpSocket
{
    Q_OBJECT
public:
    tcpsocket(QObject *parent=0 ,int type = 0);     //type 决定tcpsocket类型 是用于客户端还是服务端 还是普通 默认普通
signals:
    void updateClients(QByteArray,tcpsocket *);
    void disconnected(tcpsocket *);
    void verificationpassed(QString,tcpsocket *);                   //告诉服务端，验证通过
    void deletetcpsocket(tcpsocket *);                              //发送释放这部分内存的信号
    void verifyserver(QByteArray,tcpsocket *);                                 //发送验证信号
    void sendid(QString);                                               //更新卫星窗口的id
    void sendclientverifyresult(QString);                                //发送客户端验证结果（取消）
protected slots:
    void dataReceived();
    void slotDisconnected();
    void verifyidclient();                //验证身份 客户端
    void waitverification();              //等待服务器允许接入的响应
public slots:
    void verifyidserver();                //验证身份 服务端


};

#endif // TCPSOCKET_H
