#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <stable.h>
#include <winpcap.h>
#include <tcpserver.h>
#include <tcpsocket.h>
#include <QFileDialog>
#include <cameraview.h>

namespace Ui {
class client;
}

class client : public QWidget
{
    Q_OBJECT

public:
    explicit client(QWidget *parent = 0);
    ~client();
    void wificonnected();                       //wifi连接

private:
    Ui::client *ui;
    QUdpSocket *udpServer;              //udp监听
    tcpsocket *tcpsender;              //tcp发送
    tcpsocket *tcpsender_file;     //tcp文件发送

    QFile *localFile;  //要发送的文件
    qint64 totalBytes;  //数据总大小
    qint64 bytesWritten;  //已经发送数据大小
    qint64 bytesToWrite;   //剩余数据大小
    qint64 loadSize;   //每次发送数据的大小
    QString fileName;  //保存文件路径
    QByteArray outBlock;  //数据缓冲区，即存放每次要发送的数据

    cameraview *cview;  //显示相机画面

    void socketinit();                  //初始化socket
public slots:
    void udpget();                      //处理接收到的udp信息
    void tcpconnected();                //连接成功后
    void newdata(QByteArray,tcpsocket*);        //有新数据到达时
    void tcpdisconnect();               //tcp连接断开时
    void verifyresult(QString);             //验证结果
    void verifyresult_file(QString);        //文件客户端连接验证结果

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void getid(QString);                    //更新窗口id
    void on_pushButton_5_clicked();
    void updateClientProgress(qint64); //发送数据，更新进度条
    void displayError(QAbstractSocket::SocketError); //显示错误
};

#endif // CLIENT_H
