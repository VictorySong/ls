#ifndef SENDARP_H
#define SENDARP_H

#include <stable.h>
class winpcap;
class sendarp : public QThread
{
    Q_OBJECT
public:
    sendarp(winpcap *tem);
    void stop();
private:
    volatile bool stopped;                  //停止标志 初始化为false
    winpcap *arp;
protected:
    void run();                     //运行线程
signals:
    void setmaxprogressbar(unsigned long);         //发送进度条总长度
    void setprogressbar(unsigned long);             //当前进度

};

#endif // SENDARP_H
