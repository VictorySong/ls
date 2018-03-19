#ifndef ARPSENDTHREAD_H
#define ARPSENDTHREAD_H


#include <stable.h>
class arpactdev;

class arpsendthread : public QThread
{
    Q_OBJECT
public:
    arpactdev *arpinf;                      //存储本地ip信息
    arpsendthread();
    void stop();
protected:
    void run();
private:
    volatile bool stopped;              //是否停止运行线程的控制信号
signals:
    void sendall();         //arp 包发送完 后发送信号
};

#endif // ARPSENDTHREAD_H
