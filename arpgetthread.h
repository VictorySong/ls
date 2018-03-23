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
protected:
    void run();                     //运行线程
private:
    volatile bool sendthreadstopped;          //发送探测活动mac线程是否结束标志
    volatile bool stopped;
public slots:
    void sendallarp();              //发送完mac探测包后会触发此槽函数
signals:
    void sendactmac(unsigned long ip,QString mac); //发送更新局域网内活动主机mac地址信号
    void sendnetstate(bool online);                 //当网络状态改变是发送此信号
    void upmactable();                              //收到探测活动mac的arp包发送完后 发送更新窗口的信号
    void upadhandle();                               //创建新的适配器描述字
public:
    QString mactoqstring(char *mac);        //将字符串类型mac地址转化成Qstring

};

#endif // ARPGETTHREAD_H
