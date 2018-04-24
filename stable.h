#ifndef STABLE_H
#define STABLE_H
#include <QApplication>
#include <QDebug>
#define HAVE_REMOTE
#include "pcap.h"
#include "remote-ext.h"
#include <QTextCodec>
#include <QRadioButton>
#include <QVector>
#include <QStringListModel>
#include <iphlpapi.h>
#include <QProcess>
#include <QThread>
#include <QObject>
#include <QHash>
#include <QtNetwork>
#include <QMessageBox>
#include <QInputDialog>
#include <QDialog>
#include <QPainter>
#include "wlan.h"

#ifndef IPTOSBUFFERS
#define IPTOSBUFFERS    12
#endif
#ifndef ETH_ARP
#define ETH_ARP         0x0806  //以太网帧类型表示后面数据的类型，对于ARP请求或应答来说，该字段的值为x0806
#endif
#ifndef ARP_HARDWARE
#define ARP_HARDWARE    1  //硬件类型字段值为表示以太网地址
#endif
#ifndef ETH_IP
#define ETH_IP          0x0800  //协议类型字段表示要映射的协议地址类型值为x0800表示IP地址
#endif
#ifndef ARP_REQUEST
#define ARP_REQUEST     1   //ARP请求
#endif
#ifndef ARP_REPLY
#define ARP_REPLY       2      //ARP应答
#endif
#ifndef HOSTNUM
#define HOSTNUM         255   //主机数量
#endif

#pragma pack(1)
//帧头部结构体，共14字节
struct EthernetHeader
{
    u_char DestMAC[6];    //目的MAC地址 6字节
    u_char SourMAC[6];   //源MAC地址 6字节
    u_short EthType;         //上一层协议类型，如0x0800代表上一层是IP协议，0x0806为arp  2字节
};

//28字节ARP帧结构
struct Arpheader {
    unsigned short HardwareType; //硬件类型
    unsigned short ProtocolType; //协议类型
    unsigned char HardwareAddLen; //硬件地址长度
    unsigned char ProtocolAddLen; //协议地址长度
    unsigned short OperationField; //操作字段
    unsigned char SourceMacAdd[6]; //源mac地址
    unsigned long SourceIpAdd; //源ip地址
    unsigned char DestMacAdd[6]; //目的mac地址
    unsigned long DestIpAdd; //目的ip地址
};

//arp包结构
struct ArpPacket {
    EthernetHeader ed;
    Arpheader ah;
};

struct actdevinf{
    QString mac;            //mac 地址
    bool    tcpconsta;          //tcp连接状态
};

struct inf{
    float x;            //卫星横坐标
    float y;            //卫星纵坐标
};



#endif // STABLE_H




