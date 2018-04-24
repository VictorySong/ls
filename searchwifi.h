#ifndef SEARCHWIFI_H
#define SEARCHWIFI_H

#include <stable.h>
class searchwifi : public QThread
{
    Q_OBJECT
public:
    searchwifi();
    void stop();
private:
    volatile bool stopped;    //停止标志
protected:
    void run();                     //运行线程
signals:
    void wificonnected();               //wifi连接成功
};

#endif // SEARCHWIFI_H
