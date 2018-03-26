#ifndef GETARP_H
#define GETARP_H

#include <stable.h>
class winpcap;
class getarp : public QThread
{
    Q_OBJECT
public:
    getarp(winpcap *tem);
public slots:
    void stop();                            //停止线程
private:
    volatile bool stopped;                  //停止标志 初始化为false
    winpcap *arp;
protected:
    void run();                     //运行线程

};

#endif // GETARP_H
