#ifndef TCPSOCKET_H
#define TCPSOCKET_H
#include <QtNetwork>
#include <verification.h>
class tcpsocket : public QTcpSocket
{
    Q_OBJECT
public:
    tcpsocket(QObject *parent=0 ,int type = 0, QString s_id=QString(""));     //type 决定tcpsocket类型 是用于客户端还是服务端 还是普通 默认普通
    QString filefolder; //存放接收文件的文件夹
signals:
    void updateClients(QByteArray,tcpsocket *);
    void updateClients_file(qint64,qint64,QString,tcpsocket *); //通知文件接收进度
    void updateClients_newfile(qint64,qint64,QString,tcpsocket *); //通知文件接收进度
    void disconnected(tcpsocket *);
    void verificationpassed(QString,tcpsocket *);                   //告诉服务端，验证通过
    void deletetcpsocket(tcpsocket *);                              //发送释放这部分内存的信号
    void verifyserver(QByteArray,tcpsocket *);                                 //发送验证信号
    void sendid(QString);                                               //更新卫星窗口的id
    void sendclientverifyresult(QString);                                //发送客户端验证结果（取消）
protected slots:
    void dataReceived();    //接收数据
    void dataReceived_file();   //接收文件数据
    void slotDisconnected();
    void verifyidclient();                //验证身份 客户端
    void verifyidclient_file();                //验证身份 文件客户端
    void waitverification();              //等待服务器允许接入的响应
public slots:
    void verifyidserver();                //验证身份 服务端
private:
    QString id; //卫星id
    qint64 totalBytes;  //存放总大小信息
    qint64 bytesReceived;  //已收到数据的大小
    qint64 fileNameSize;  //文件名的大小信息
    QString fileName;   //存放文件名
    QFile *localFile;   //本地文件
    QByteArray inBlock;   //数据缓冲区
};

#endif // TCPSOCKET_H
