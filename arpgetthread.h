#ifndef ARPGETTHREAD_H
#define ARPGETTHREAD_H


#include <stable.h>
class arpactdev;

class arpgetthread : public QThread
{
    Q_OBJECT
public:
    arpgetthread();
    arpactdev *arpinf;                  //存储本地ip信息
    void stop();
protected:
    void run();                     //运行线程
private:
    volatile bool stopped;          //线程是否结束的标志
public slots:
    void sendallarp();              //发送arp探测包完后会触发此槽函数，结束监听进程
signals:
    void sendactmac(unsigned long ip,QString mac); //发送更新局域网内活动主机mac地址信号
public:
    QString mactoqstring(char *mac);        //将字符串类型mac地址转化成Qstring

};

#endif // ARPGETTHREAD_H
