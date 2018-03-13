#ifndef ARPACTDEV_H
#define ARPACTDEV_H

#include <QWidget>
#pragma pack(1)

#define IPTOSBUFFERS    12
#define ETH_ARP         0x0806  //以太网帧类型表示后面数据的类型，对于ARP请求或应答来说，该字段的值为x0806
#define ARP_HARDWARE    1  //硬件类型字段值为表示以太网地址
#define ETH_IP          0x0800  //协议类型字段表示要映射的协议地址类型值为x0800表示IP地址
#define ARP_REQUEST     1   //ARP请求
#define ARP_REPLY       2      //ARP应答
#define HOSTNUM         255   //主机数量


namespace Ui {
class arpactdev;
}

class arpactdev : public QWidget
{
    Q_OBJECT

public:
    explicit arpactdev(QWidget *parent = 0);
    ~arpactdev();

private:
    Ui::arpactdev *ui;
};

#endif // ARPACTDEV_H
