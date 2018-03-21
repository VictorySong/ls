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
    volatile bool sendthreadbegin;             //发送探测活动mac线程是否开始表示
public slots:
    void sendallarp();              //发送完mac探测包后会触发此槽函数
    void sendmacarpbegin();         //开始发送mac探测包后触发此槽函数
signals:
    void sendactmac(unsigned long ip,QString mac); //发送更新局域网内活动主机mac地址信号
public:
    QString mactoqstring(char *mac);        //将字符串类型mac地址转化成Qstring

};

#endif // ARPGETTHREAD_H
