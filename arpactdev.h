#ifndef ARPACTDEV_H
#define ARPACTDEV_H


#include <stable.h>
#include <QWidget>
#include "arpsendthread.h"
#include "arpgetthread.h"

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
    bool flag;                      //ip地址是否可用的标志
    char *ip_addr;                                    //IP地址
    char *ip_netmask;                             //子网掩码
    unsigned char *ip_mac;          //本机MAC地址
    pcap_if_t *alldevs;               //所有适配器
    pcap_if_t *d;                       //当前适配器
    pcap_t *adhandle;           //捕捉实例,是pcap_open返回的对象

public:
    friend class arpgetthread;
    friend class arpsendthread;
    friend class checknet;
    QHash<QString,actdevinf> actmac;      //活动设备的mac地址
    char errbuf[PCAP_ERRBUF_SIZE];
    int dev_num;                            //适配器计数变量
    QVector <pcap_if_t *> usabledev;

                   //
    int init();
    QString iptos(u_long in);       //转换ip地址为qstring 格式
    QString mactos(unsigned char* mac);  //转换mac地址为qstring 格式
    void ifget(pcap_if_t *d, char *ip_addr, char *ip_netmask ,unsigned int &unetmask);
    int GetSelfMac(pcap_t *adhandle, const char *ip_addr);
    QString actmackey(QString t);//获取actmac 中mac值为t的 key 无则返回空字符串
    int pcap_get(pcap_t* thandle,pcap_pkthdr ** pcap_h,const u_char **pcap_d);    //为pcap_next_ex 加线程锁
    int pcap_send(pcap_t* a,const u_char* b,int c);                 //为pcap_sendpacket加锁
    void changealldev(pcap_if_t * dev ,QVector <pcap_if_t *> udev,QList <QString> combox);                    //当前网络状态发生变化是重新获取适配器列表

private slots:
    void on_ipcombox_currentIndexChanged(const QString &arg1);
    void on_getmacbutton_clicked();
    void getactmac(unsigned long ip,QString mac);  //接收局域网内活动主机mac信号，更新actmac
    void updatedev_tip(bool online);       //更新网络状态显示窗口
    void upmactab();            //更新活动主机ip及mac显示
    bool newadhandle();
    void on_pushButton_clicked();
    void on_listentcpport_clicked();

    void udpget();                  //接受udp 槽函数
    void tcpserversend();               //tcp 服务端发送函数
    void tcpclientread();
    void displayerror(QAbstractSocket::SocketError);

    void on_newtcpconnect_clicked();

    void on_pushButton_2_clicked();

private:
    arpsendthread arpth;                //发送arp探测包的线程
    arpgetthread arpgth;                //获取arp探测包的线程
    QTcpServer *tcpServer;              //tcp监听
    QTcpSocket *tcpsender;              //tcp发送
    QUdpSocket *udpsender;              //udp发送
    QUdpSocket *udpServer;              //udp监听

    QString tcpmessage;  //存放从服务器接收到的字符串
    quint16 blockSize; //存放文件的大小信息

    mutable QMutex mutex;               //锁保证线程安全

public:
    QString tcpserver_ip;
    int tcpserver_port;

};
#endif // ARPACTDEV_H

