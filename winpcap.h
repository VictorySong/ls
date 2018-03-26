#ifndef WINPCAP_H
#define WINPCAP_H

#include <stable.h>
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


class winpcap :public QObject
{
    Q_OBJECT
public:
    winpcap();
    ~winpcap();
private:
    bool flag;                      //ip地址是否可用的标志
    char *ip_addr;                                    //IP地址
    char *ip_netmask;                             //子网掩码
    unsigned char *ip_mac;          //本机MAC地址
    pcap_if_t *alldevs;               //所有适配器
    pcap_if_t *d;                       //当前适配器
    pcap_t *adhandle;           //捕捉实例,是pcap_open返回的对象
    char errbuf[PCAP_ERRBUF_SIZE];
    int dev_num;                            //适配器计数变量
    QMap<QString,pcap_if_t *> usabledev;       //ip可用的适配器
    QMutex mutex;               //锁保证线程安全
public:
    QHash<QString,actdevinf> actmac;      //活动设备的mac地址
    QList<QString> getusableip();

public:
    void init();
    QString iptos(u_long in);       //转换ip地址为qstring 格式
    bool newhandle(QString ip);         //新的适配器描述子
    void ifget(unsigned int &unetmask);      //获取网卡上的ip 子网掩码
    void GetSelfMac();      //发送获取自身mac地址的数据包
    bool getsendarp();          //捕获探测自身mac地址的arp包   确定有网络连接
    int pcap_get(pcap_pkthdr ** pcap_h,const u_char **pcap_d);    //为pcap_next_ex 加线程锁
    QString getselfmac();      //获取当前选定网卡mac信息
    QString macos();             //将char* 类型的mac 转化为qstring
    QString getnetmask();               //获取子网掩码


};

#endif // WINPCAP_H
