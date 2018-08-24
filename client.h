#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <stable.h>
#include <winpcap.h>
#include <tcpserver.h>
#include <tcpsocket.h>
#include <QFileDialog>

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
};

#endif // CLIENT_H
