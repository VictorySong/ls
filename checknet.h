#ifndef CHECKNET_H
#define CHECKNET_H

#include <stable.h>

class arpactdev;
class checknet : public QThread
{
    Q_OBJECT
public:
    checknet();

    arpactdev *arpinf;
protected:
    void run();
signals:
    void updev_tip();
private:
    volatile bool stopped;          //线程是否结束的标志
    
};

#endif // CHECKNET_H
